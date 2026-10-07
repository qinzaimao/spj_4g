#include "my_tcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

/**
 * MY_SET_DHCP.host_state 动态切换模式
 * true  = TCP客户端：板子主动连接另一块板子TCP服务器
 * false = TCP服务端：板子本机监听1346，等待另一块板子TCP客户端接入
 */

// 服务端监听端口
#define TCP_LISTEN_PORT 1346
#define TCP_ACCEPT_BACKLOG 2
// TCP客户端目标端口
#define TARGET_TCP_PORT 1346
// 公共基础配置
#define TCP_SEND_BUF_LEN 1024
#define TCP_CONN_DELAY_MS 1000
// 配置：服务端2.5s发一次心跳；客户端9秒无数据超时断开
#define SERVER_HEART_MS 2500
#define CLIENT_TIMEOUT_MS 9000
#define CLIENT_CONNECT_MAX_WAIT_MS 5000

#define SERVER_HEART_TICK rt_tick_from_millisecond(SERVER_HEART_MS)
#define CLIENT_TIMEOUT_TICK rt_tick_from_millisecond(CLIENT_TIMEOUT_MS)

// 统一心跳包内容
static const uint8_t tcp_heart_data[] = {0xAB, 0xCD, 0xEF, 0x01};

static struct tcp_pcb *tcp_listen_pcb = NULL;
static struct tcp_pcb *tcp_data_pcb = NULL;
static rt_sem_t tcp_send_sem = RT_NULL;
static ip_addr_t target_ip;
static rt_bool_t last_host_state = RT_FALSE;
rt_tick_t last_recv_tick = 0;
rt_tick_t server_heart_tick = 0;
static rt_bool_t is_connecting = RT_FALSE;
static rt_tick_t client_conn_success_tick = 0;
static rt_tick_t server_first_accept_tick = 0;
static rt_bool_t server_have_valid_conn = RT_FALSE;
static rt_tick_t client_connect_start_tick = 0;
static uint8_t bind_retry_cnt = 0;

// 前向声明
void parse_frame_and_tcp_send(uint8_t *recv_buf, uint16_t recv_len);

// ===================== TCP连接清理函数 =====================

/**
 * @brief 关闭当前数据连接（释放tcp_data_pcb）
 *        注意：服务端监听PCB (tcp_listen_pcb) 保持不变，
 *        避免网线断开重连时重复 bind 导致 ERR_USE (-8)
 */
static void tcp_close_conn(void)
{
    LOCK_TCPIP_CORE();
    if (tcp_data_pcb != NULL)
    {
        tcp_recv(tcp_data_pcb, NULL);
        tcp_sent(tcp_data_pcb, NULL);
        tcp_err(tcp_data_pcb, NULL);
        tcp_abort(tcp_data_pcb);
        tcp_data_pcb = NULL;
    }
    UNLOCK_TCPIP_CORE();

    need_close_link = RT_FALSE;
    is_tcp_connected = RT_FALSE;
    is_connecting = RT_FALSE;
    client_conn_success_tick = 0;
    client_connect_start_tick = 0;
    server_have_valid_conn = RT_FALSE;
    server_first_accept_tick = 0;
}

/**
 * @brief 完全关闭所有TCP资源（包括服务端监听PCB）
 *        仅在模式热切换（主从切换）或模块注销 (deinit) 时调用
 */
static void tcp_full_close(void)
{
    LOCK_TCPIP_CORE();
    if (tcp_data_pcb != NULL)
    {
        tcp_recv(tcp_data_pcb, NULL);
        tcp_sent(tcp_data_pcb, NULL);
        tcp_err(tcp_data_pcb, NULL);
        tcp_abort(tcp_data_pcb);
        tcp_data_pcb = NULL;
    }
    if (tcp_listen_pcb != NULL)
    {
        tcp_accept(tcp_listen_pcb, NULL);
        tcp_close(tcp_listen_pcb);
        tcp_listen_pcb = NULL;
    }
    UNLOCK_TCPIP_CORE();

    need_close_link = RT_FALSE;
    is_tcp_connected = RT_FALSE;
    is_connecting = RT_FALSE;
    client_conn_success_tick = 0;
    client_connect_start_tick = 0;
    server_have_valid_conn = RT_FALSE;
    server_first_accept_tick = 0;
    bind_retry_cnt = 0;
}

// ===================== 回调函数 =====================

// TCP异常断开统一回调（lwIP在调用err_cb前已将PCB释放，严禁在此调用tcp_abort）
static void tcp_common_err_cb(void *arg, err_t err)
{
    struct tcp_pcb *err_pcb = (struct tcp_pcb *)arg;
    if (arg != NULL && err_pcb != tcp_data_pcb)
    {
        return;
    }
    // rt_kprintf("[TCP ERROR] 链路异常断开！err=%d\n", err);
    tcp_data_pcb = NULL;
    is_tcp_connected = RT_FALSE;
    is_connecting = RT_FALSE;
    client_connect_start_tick = 0;
    need_close_link = RT_TRUE;
}

static err_t tcp_common_sent_cb(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    return ERR_OK;
}

// 接收回调
static err_t tcp_common_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    if (tcp_data_pcb != pcb)
    {
        if (p)
            pbuf_free(p);
        return ERR_OK;
    }
    last_recv_tick = rt_tick_get();
    if (p == NULL || err != ERR_OK)
    {
        rt_kprintf("[TCP RECV] 当前链路对端主动FIN断开\n");
        need_close_link = RT_TRUE;
        return ERR_OK;
    }
    struct pbuf *q = p;
    while (q != NULL)
    {
        uint16_t recv_len = q->len;
        uint8_t *recv_buf = (uint8_t *)q->payload;
        if (recv_len == sizeof(tcp_heart_data) && memcmp(recv_buf, tcp_heart_data, sizeof(tcp_heart_data)) == 0)
        {
            q = q->next;
            continue;
        }
        parse_frame_and_tcp_send(recv_buf, recv_len);
        q = q->next;
    }
    u16_t tot_len = p->tot_len;
    pbuf_free(p);
    tcp_recved(pcb, tot_len);
    return ERR_OK;
}

// 客户端连接成功回调
static err_t tcp_client_connected_cb(void *arg, struct tcp_pcb *pcb, err_t err)
{
    is_connecting = RT_FALSE;
    client_connect_start_tick = 0;
    if (pcb != tcp_data_pcb)
    {
        rt_kprintf("[TCP CLIENT] 迟到的connect成功回调，丢弃\n");
        return ERR_OK;
    }
    if (is_tcp_connected)
    {
        rt_kprintf("[TCP CLIENT] 连接回调重复触发，忽略\n");
        return ERR_OK;
    }
    if (err != ERR_OK)
    {
        rt_kprintf("[TCP CLIENT] 连接失败 err=%d\n", err);
        need_close_link = RT_TRUE;
        return err;
    }
    rt_kprintf("[TCP CLIENT] ✅ 成功连接对端服务端\n");
    tcp_arg(pcb, pcb);
    tcp_recv(pcb, tcp_common_recv_cb);
    tcp_sent(pcb, tcp_common_sent_cb);
    tcp_err(pcb, tcp_common_err_cb);
    last_recv_tick = rt_tick_get();
    client_conn_success_tick = rt_tick_get();
    is_tcp_connected = RT_TRUE;
    need_close_link = RT_FALSE;
    pcb->so_options |= SOF_REUSEADDR;
    pcb->keep_idle = 2000;
    pcb->keep_intvl = 1000;
    pcb->keep_cnt = 3;
    return ERR_OK;
}

// 服务端accept回调（运行于tcpip_thread上下文，已持有CORE锁）
static err_t tcp_server_accept_cb(void *arg, struct tcp_pcb *new_pcb, err_t err)
{
    if (err != ERR_OK || new_pcb == NULL)
    {
        return ERR_CLSD;
    }

    // 若已有旧数据连接（必须确保不是同一个PCB，防止内存池复用同地址时误杀新连接）
    if (tcp_data_pcb != NULL && tcp_data_pcb != new_pcb)
    {
        rt_kprintf("[TCP SERVER] 替换并销毁旧TCP连接\n");
        tcp_recv(tcp_data_pcb, NULL);
        tcp_sent(tcp_data_pcb, NULL);
        tcp_err(tcp_data_pcb, NULL);
        tcp_abort(tcp_data_pcb);
        tcp_data_pcb = NULL;
    }

    tcp_data_pcb = new_pcb;
    tcp_data_pcb->so_options |= SOF_REUSEADDR;
    tcp_arg(tcp_data_pcb, tcp_data_pcb);
    tcp_recv(tcp_data_pcb, tcp_common_recv_cb);
    tcp_sent(tcp_data_pcb, tcp_common_sent_cb);
    tcp_err(tcp_data_pcb, tcp_common_err_cb);
    last_recv_tick = rt_tick_get();
    is_tcp_connected = RT_TRUE;
    need_close_link = RT_FALSE;
    server_heart_tick = rt_tick_get();
    server_have_valid_conn = RT_TRUE;
    server_first_accept_tick = rt_tick_get();
    rt_kprintf("[TCP SERVER] ✅ 新连接接入\n");
    return ERR_OK;
}

// ===================== 业务处理与发送 =====================

// 收到TCP数据，抛给上层业务
void parse_frame_and_tcp_send(uint8_t *recv_buf, uint16_t recv_len)
{
    if (recv_buf == NULL || recv_len == 0)
        return;
    if (g_tcp_recv_sem == RT_NULL)
        return;
    if (recv_len > TCP_RECV_BUF_MAX)
    {
        return;
    }
    rt_enter_critical();
    memcpy(g_tcp_recv_buf, recv_buf, recv_len);
    g_tcp_recv_len = recv_len;
    rt_exit_critical();
    rt_sem_release(g_tcp_recv_sem);
}

// 发送对外接口（加CORE锁直接同步写入，杜绝栈变量生命周期跑飞的致命隐患）
err_t tcp_send_raw(uint8_t *buf, uint16_t len)
{
    if (!is_tcp_connected || tcp_data_pcb == NULL || need_close_link)
        return ERR_CONN;
    if (buf == NULL || len == 0)
        return ERR_ARG;

    if (rt_sem_take(tcp_send_sem, rt_tick_from_millisecond(200)) != RT_EOK)
    {
        rt_kprintf("[TCP WARN] 发包繁忙跳过\n");
        return ERR_TIMEOUT;
    }

    err_t ret = ERR_CONN;
    LOCK_TCPIP_CORE();
    if (is_tcp_connected && tcp_data_pcb != NULL)
    {
        ret = tcp_write(tcp_data_pcb, buf, len, TCP_WRITE_FLAG_COPY);
        if (ret == ERR_OK)
        {
            tcp_output(tcp_data_pcb);
        }
    }
    UNLOCK_TCPIP_CORE();

    rt_sem_release(tcp_send_sem);
    return ret;
}

// 发送背光指令
void send_light(bool energy, uint8_t light_val)
{
    char light_str[16] = {0};
    if (light_val >= 100)
        light_val = 95;
    if (light_val <= 5)
        light_val = 5;
    if (energy)
        sprintf(light_str, "li_en:%d", light_val);
    else
        sprintf(light_str, "light:%d", light_val);
    int send_len = rt_strlen(light_str);
    if (send_len > 0)
    {
        tcp_send_raw((uint8_t *)light_str, send_len);
    }
    rt_kprintf("[TCP LIGHT] 发送背光指令：%s\n", light_str);
}

// ===================== 主线程入口 =====================

void tcp_info_thread_entry(void *parameter)
{
    while (!init_set_img_ok) rt_thread_mdelay(200);
    err_t err;
    tcp_send_sem = rt_sem_create("tcp_sem", 1, RT_IPC_FLAG_FIFO);
    if (tcp_send_sem == RT_NULL)
    {
        rt_kprintf("[TCP THREAD] 发送互斥信号量创建失败，线程直接退出\n");
        return;
    }
    last_host_state = MY_SET_DHCP.host_state;
    last_recv_tick = rt_tick_get();
    is_tcp_connected = false;
    need_close_link = RT_FALSE;
    server_heart_tick = rt_tick_get();
    bind_retry_cnt = 0;
    rt_kprintf("[TCP THREAD] TCP双板互测线程启动，初始模式:%s\n", last_host_state ? "TCP客户端(主动连另一块板)" : "TCP服务端(等待另一块板接入)");
    rt_thread_mdelay(3000);

    while (1)
    {
        // 状态不一致，强制清理数据连接
        if (!is_connecting && !is_tcp_connected && tcp_data_pcb != NULL)
        {
            rt_kprintf("[TCP WARN] state inconsistent, force clean\n");
            need_close_link = RT_TRUE;
        }

        // 仅关闭数据链路，服务端监听保持活跃
        if (need_close_link)
        {
            tcp_close_conn();
            rt_thread_mdelay(100);
            continue;
        }

        rt_tick_t now_tick = rt_tick_get();

        // 客户端保活参数
        if (MY_SET_DHCP.host_state == RT_TRUE && is_tcp_connected && tcp_data_pcb != NULL)
        {
            if (client_conn_success_tick != 0 && ((now_tick - client_conn_success_tick) > rt_tick_from_millisecond(5000)))
            {
                tcp_data_pcb->keep_idle = 4000;
                tcp_data_pcb->keep_intvl = 1500;
                tcp_data_pcb->keep_cnt = 3;
            }
        }

        // 客户端9s无数据超时
        if (MY_SET_DHCP.host_state == RT_TRUE && tcp_data_pcb != NULL && is_tcp_connected)
        {
            if ((now_tick - last_recv_tick) > CLIENT_TIMEOUT_TICK)
            {
                rt_kprintf("[TCP CLIENT] timeout >9s,reset\n");
                need_close_link = RT_TRUE;
                rt_thread_mdelay(200);
                continue;
            }
        }

        // 服务端定时发送心跳包
        if (MY_SET_DHCP.host_state == RT_FALSE && tcp_data_pcb != NULL && is_tcp_connected && need_close_link == RT_FALSE)
        {
            if ((now_tick - server_heart_tick) > SERVER_HEART_TICK)
            {
                tcp_send_raw((uint8_t *)tcp_heart_data, sizeof(tcp_heart_data));
                server_heart_tick = now_tick;
            }
        }

        // 模式切换，彻底重建TCP资源
        if (MY_SET_DHCP.host_state != last_host_state)
        {
            tcp_full_close();
            server_have_valid_conn = RT_FALSE;
            server_first_accept_tick = 0;
            last_host_state = MY_SET_DHCP.host_state;
            rt_kprintf("[TCP SWITCH] mode change to %s\n", last_host_state ? "CLIENT" : "SERVER");
        }

        if (MY_SET_DHCP.host_state == RT_TRUE)
        {
            // ========= TCP 客户端逻辑 =========
            if (tcp_data_pcb != NULL)
            {
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }
            if (is_connecting)
            {
                if ((rt_tick_get() - client_connect_start_tick) > rt_tick_from_millisecond(CLIENT_CONNECT_MAX_WAIT_MS))
                {
                    rt_kprintf("[TCP CLIENT] connect timeout 5s! abort current connect\n");
                    need_close_link = RT_TRUE;
                    rt_thread_mdelay(300);
                    continue;
                }
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }
            rt_thread_mdelay(TCP_CONN_DELAY_MS);
            char ip_buf[32] = {0};
            rt_sprintf(ip_buf, "%d.%d.%d.%d",
                    MY_SET_DHCP.dns[0],
                    MY_SET_DHCP.dns[1],
                    MY_SET_DHCP.dns[2],
                    MY_SET_DHCP.dns[3]);
            ipaddr_aton(ip_buf, &target_ip);
            rt_thread_mdelay(800);

            LOCK_TCPIP_CORE();
            tcp_data_pcb = tcp_new();
            if (tcp_data_pcb == NULL)
            {
                UNLOCK_TCPIP_CORE();
                rt_kprintf("[TCP CLIENT] tcp_new fail\n");
                continue;
            }
            tcp_arg(tcp_data_pcb, tcp_data_pcb);
            tcp_err(tcp_data_pcb, tcp_common_err_cb);
            err = tcp_connect(tcp_data_pcb, &target_ip, TARGET_TCP_PORT, tcp_client_connected_cb);
            if (err != ERR_OK)
            {
                // rt_kprintf("[TCP CLIENT] tcp_connect fail err=%d\n", err);
                tcp_abort(tcp_data_pcb);
                tcp_data_pcb = NULL;
                UNLOCK_TCPIP_CORE();
                is_connecting = RT_FALSE;
                client_connect_start_tick = 0;
                rt_thread_mdelay(1500);
                continue;
            }
            UNLOCK_TCPIP_CORE();
            is_connecting = RT_TRUE;
            client_connect_start_tick = rt_tick_get();
            rt_kprintf("[TCP CLIENT] connecting %s:%d\n", ip_buf, TARGET_TCP_PORT);
        }
        else
        {
            // ========= TCP 服务端逻辑 =========
            // 服务端监听PCB已存在且正常监听，无需重复bind
            if (tcp_listen_pcb != NULL)
            {
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }
            LOCK_TCPIP_CORE();
            tcp_listen_pcb = tcp_new();
            if (tcp_listen_pcb == NULL)
            {
                UNLOCK_TCPIP_CORE();
                rt_kprintf("[TCP SERVER] listen pcb new fail\n");
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }
            err = tcp_bind(tcp_listen_pcb, IP_ADDR_ANY, TCP_LISTEN_PORT);
            if (err != ERR_OK)
            {
                rt_kprintf("[TCP SERVER] bind port %d fail err=%d, retry:%d\n", TCP_LISTEN_PORT, err, bind_retry_cnt);
                tcp_close(tcp_listen_pcb);
                tcp_listen_pcb = NULL;
                UNLOCK_TCPIP_CORE();
                bind_retry_cnt++;
                if (bind_retry_cnt == 1)
                {
                    rt_thread_mdelay(1000);
                }
                else if (bind_retry_cnt == 2)
                {
                    rt_thread_mdelay(2000);
                }
                else
                {
                    bind_retry_cnt = 0;
                    rt_thread_mdelay(3000);
                }
                continue;
            }
            bind_retry_cnt = 0;
            tcp_listen_pcb = tcp_listen_with_backlog(tcp_listen_pcb, TCP_ACCEPT_BACKLOG);
            tcp_accept(tcp_listen_pcb, tcp_server_accept_cb);
            UNLOCK_TCPIP_CORE();
            rt_kprintf("[TCP SERVER] port %d listen start\n", TCP_LISTEN_PORT);
        }
        rt_thread_mdelay(TCP_CONN_DELAY_MS);
    }
}

// TCP资源释放
void tcp_deinit(void)
{
    tcp_full_close();
    server_have_valid_conn = RT_FALSE;
    server_first_accept_tick = 0;
    if (tcp_send_sem != RT_NULL)
    {
        rt_sem_delete(tcp_send_sem);
        tcp_send_sem = RT_NULL;
    }
    rt_kprintf("[TCP DEINIT] done\n");
}
