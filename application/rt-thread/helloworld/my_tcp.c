#include "my_tcp.h"
#include "lwip/tcpip.h"

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
static rt_bool_t need_close_link = RT_FALSE;

// ===================== 线程安全异步发包封装 =====================
typedef struct
{
    struct tcp_pcb *pcb;
    uint8_t *data;
    uint16_t len;
} tcp_send_param_t;

static void tcp_send_async_cb(void *arg)
{
    tcp_send_param_t *p = (tcp_send_param_t *)arg;
    if (p->pcb == NULL)
    {
        rt_free(p);
        return;
    }
    tcp_write(p->pcb, p->data, p->len, TCP_WRITE_FLAG_COPY);
    tcp_output(p->pcb);
    rt_free(p);
}

static err_t tcp_safe_send(struct tcp_pcb *pcb, uint8_t *buf, uint16_t len)
{
    if (pcb == NULL || buf == NULL || len == 0)
        return ERR_ARG;
    tcp_send_param_t *param = rt_malloc(sizeof(tcp_send_param_t));
    if (param == NULL)
        return ERR_MEM;
    param->pcb = pcb;
    param->data = buf;
    param->len = len;
    return tcpip_callback(tcp_send_async_cb, param);
}

// TCP异常断开统一回调【核心：强制置空PCB，保证能重连】
static void tcp_common_err_cb(void *arg, err_t err)
{
    if (tcp_data_pcb != arg)
    {
        return;
    }
    rt_kprintf("[TCP ERROR] 链路异常断开！err=%d 网线拔除/对端强制下线\n", err);
    need_close_link = RT_TRUE;
    tcp_data_pcb = NULL;
    is_tcp_connected = RT_FALSE;
}

static err_t tcp_common_sent_cb(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    return ERR_OK;
}

// 接收回调：遍历全pbuf链 + 客户端收到心跳自动回复应答（双向保活）
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
        rt_kprintf("[TCP RECV] 对端主动发送FIN正常断开连接\n");
        need_close_link = RT_TRUE;
        return ERR_CLSD;
    }

    struct pbuf *q = p;
    while (q != NULL)
    {
        uint16_t recv_len = q->len;
        uint8_t *recv_buf = (uint8_t *)q->payload;

        // 心跳包处理 + 客户端回包应答
        if (recv_len == sizeof(tcp_heart_data) && memcmp(recv_buf, tcp_heart_data, sizeof(tcp_heart_data)) == 0)
        {
            last_recv_tick = rt_tick_get();
            // if (MY_SET_DHCP.host_state == RT_TRUE && is_tcp_connected && tcp_data_pcb != NULL)
            // {
            //     tcp_send_raw((uint8_t *)tcp_heart_data, sizeof(tcp_heart_data));
            // }
            q = q->next;
            continue;
        }

        parse_frame_and_tcp_send(recv_buf, recv_len);
        q = q->next;
    }

    pbuf_free(p);
    tcp_recved(pcb, p->tot_len);
    return ERR_OK;
}

// 客户端连接成功，强制刷新接收计时
static err_t tcp_client_connected_cb(void *arg, struct tcp_pcb *pcb, err_t err)
{
    if (is_tcp_connected)
        return ERR_OK;

    if (err != ERR_OK)
    {
        rt_kprintf("[TCP CLIENT] 连接失败\n");
        need_close_link = RT_TRUE;
        return err;
    }
    rt_kprintf("[TCP CLIENT] ✅ 成功连接对端服务端\n");
    last_recv_tick = rt_tick_get();
    is_tcp_connected = true;
    need_close_link = RT_FALSE;

    pcb->so_options |= SOF_REUSEADDR | SOF_KEEPALIVE;
    pcb->keep_idle = 4000;
    pcb->keep_intvl = 1500;
    pcb->keep_cnt = 3;

    return ERR_OK;
}

err_t tcp_send_raw(uint8_t *buf, uint16_t len)
{
    if (!is_tcp_connected || tcp_data_pcb == NULL || need_close_link)
        return -1;

    // 加互斥锁，同一时刻只能有一个发送任务进入
    if (rt_sem_take(tcp_send_sem, rt_tick_from_millisecond(1000)) != RT_EOK)
    {
        return -2; // 拿锁超时，直接返回失败
    }

    err_t ret = tcp_safe_send(tcp_data_pcb, buf, len);

    rt_sem_release(tcp_send_sem);
    return ret;
}

static void tcp_abort_old_pcb_cb(void *arg)
{
    struct tcp_pcb *pcb = (struct tcp_pcb *)arg;
    tcp_recv(pcb, NULL);
    tcp_sent(pcb, NULL);
    tcp_err(pcb, NULL);
    tcp_abort(pcb);
}

static err_t tcp_server_accept_cb(void *arg, struct tcp_pcb *new_pcb, err_t err)
{
    if (err != ERR_OK || new_pcb == NULL)
    {
        return ERR_CLSD;
    }

    struct tcp_pcb *old_pcb = tcp_data_pcb;

    tcp_data_pcb = new_pcb;
    tcp_data_pcb->so_options |= SOF_REUSEADDR;
    tcp_err(tcp_data_pcb, tcp_common_err_cb);
    tcp_recv(tcp_data_pcb, tcp_common_recv_cb);
    tcp_sent(tcp_data_pcb, tcp_common_sent_cb);

    last_recv_tick = rt_tick_get();
    is_tcp_connected = RT_TRUE;
    need_close_link = RT_FALSE;
    server_heart_tick = rt_tick_get();
    rt_kprintf("[TCP SERVER] ✅ 新连接接入，替换历史旧链路\n");

    if (old_pcb != NULL)
    {
        rt_kprintf("[TCP SERVER] 强制销毁上一条旧TCP连接\n");
        tcpip_callback(tcp_abort_old_pcb_cb, old_pcb);
    }

    return ERR_OK;
}

// 无环形缓冲区，单包即时解析，无脏数据残留
void parse_frame_and_tcp_send(uint8_t *recv_buf, uint16_t recv_len)
{

    if(recv_buf == NULL || recv_len == 0)
        return;
    if(g_tcp_recv_sem == RT_NULL)
    {
        return;
    }
    if(recv_len > TCP_RECV_BUF_MAX)
    {
        return;
    }

    rt_enter_critical();
    memcpy(g_tcp_recv_buf, recv_buf, recv_len);
    g_tcp_recv_len = recv_len;
    rt_exit_critical();

    // 释放信号量，唤醒deal_thread解析线程
    rt_sem_release(g_tcp_recv_sem);


}

// 关闭函数强制置空PCB，防止信号量拿锁失败残留指针
static void tcp_full_close(void)
{
    if (rt_sem_take(tcp_send_sem, rt_tick_from_millisecond(1000)) == RT_EOK)
    {
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

        need_close_link = RT_FALSE;
        rt_thread_mdelay(200);
        is_tcp_connected = false;

        rt_sem_release(tcp_send_sem);
    }
    // 兜底：无论是否拿到信号量，强制清空关键指针
    tcp_data_pcb = NULL;
    tcp_listen_pcb = NULL;
    is_tcp_connected = false;
    need_close_link = RT_FALSE;
    // rt_kprintf("[TCP SWITCH] 旧模式TCP资源全部释放完毕\n");
}

void send_light(bool energy, uint8_t light_val)
{
    char light_str[10] = {0};
    if(light_val >= 100)
        light_val = 95;
    if(light_val <= 5)
        light_val = 5;
    if(energy)
        sprintf(light_str, "li_en:%d", light_val);
    else
        sprintf(light_str, "light:%d", light_val);

    // 自动计算字符串实际长度发送，不用手动判断位数
    int send_len = rt_strlen(light_str);
    if(send_len > 0)
    {
        tcp_send_raw((uint8_t *)light_str, send_len);
    }
    rt_kprintf("[TCP LIGHT] 发送背光指令：%s\n", light_str);
}

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
    rt_kprintf("[TCP THREAD] TCP双板互测线程启动，初始模式:%s\n", last_host_state ? "TCP客户端(主动连另一块板)" : "TCP服务端(等待另一块板接入)");

    rt_thread_mdelay(2000); // 等待LWIP标志

    while (1)
    {
        if (!get_lwip_flag || !get_lwip_flag)
        {
            rt_thread_mdelay(TCP_CONN_DELAY_MS);
            // rt_kprintf("[TCP THREAD] 等待LWIP标志...\n");
            continue;
        }

        // 状态不一致兜底：已断开标记但PCB残留，强制回收
        if (!is_tcp_connected && tcp_data_pcb != NULL)
        {
            rt_kprintf("[TCP WARN] 连接状态与PCB不一致，强制清理连接\n");
            need_close_link = RT_TRUE;
        }

        if (need_close_link)
        {
            rt_thread_mdelay(300);
            tcp_full_close();
            rt_thread_mdelay(500);
            continue;
        }

        rt_tick_t now_tick = rt_tick_get();

        // 客户端9s超时断连
        if (MY_SET_DHCP.host_state == RT_TRUE && tcp_data_pcb != NULL && is_tcp_connected)
        {
            if ((now_tick - last_recv_tick) > CLIENT_TIMEOUT_TICK)
            {
                rt_kprintf("[TCP CLIENT] ❌ 超过9s无数据，标记断开重连\n");
                need_close_link = RT_TRUE;
                rt_thread_mdelay(200);
                continue;
            }
        }

        // 服务端定时发心跳
        if (MY_SET_DHCP.host_state == RT_FALSE && tcp_data_pcb != NULL && is_tcp_connected && need_close_link == RT_FALSE)
        {
            if ((now_tick - server_heart_tick) > SERVER_HEART_TICK)
            {
                tcp_send_raw((uint8_t *)tcp_heart_data, sizeof(tcp_heart_data));
                server_heart_tick = now_tick;
            }
        }

        // 模式热切换
        if (MY_SET_DHCP.host_state != last_host_state)
        {
            tcp_full_close();
            last_host_state = MY_SET_DHCP.host_state;
            rt_kprintf("[TCP SWITCH] 切换模式：%s\n", last_host_state ? "客户端主动连接" : "服务端监听接入");
        }

        // 客户端主动连接逻辑
        if (MY_SET_DHCP.host_state == RT_TRUE)
        {
            if (tcp_data_pcb != NULL)
            {
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }

            rt_thread_mdelay(TCP_CONN_DELAY_MS);
            char IP_temp[32] = {0};
            sprintf(IP_temp, "%d.%d.%d.%d",
                    MY_SET_DHCP.dns[0],
                    MY_SET_DHCP.dns[1],
                    MY_SET_DHCP.dns[2],
                    MY_SET_DHCP.dns[3]);
            ipaddr_aton(IP_temp, &target_ip);
            // rt_kprintf("[TCP CLIENT] 目标IP:%s\n", IP_temp);

            tcp_data_pcb = tcp_new();
            if (tcp_data_pcb == NULL)
            {
                rt_kprintf("[TCP CLIENT] tcp_new失败\n");
                continue;
            }

            tcp_err(tcp_data_pcb, tcp_common_err_cb);
            tcp_recv(tcp_data_pcb, tcp_common_recv_cb);
            tcp_sent(tcp_data_pcb, tcp_common_sent_cb);

            err = tcp_connect(tcp_data_pcb, &target_ip, TARGET_TCP_PORT, tcp_client_connected_cb);
            if (err != ERR_OK)
            {
                rt_kprintf("[TCP CLIENT] connect失败 err=%d\n", err);
                tcp_close(tcp_data_pcb);
                tcp_data_pcb = NULL;
                continue;
            }
            rt_kprintf("[TCP CLIENT] 正在连接 %s:%d\n", IP_temp, TARGET_TCP_PORT);
        }
        // 服务端监听逻辑
        else
        {
            if (tcp_listen_pcb != NULL)
            {
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }

            tcp_listen_pcb = tcp_new();
            if (tcp_listen_pcb == NULL)
            {
                rt_kprintf("[TCP SERVER] 监听PCB创建失败\n");
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }

            err = tcp_bind(tcp_listen_pcb, IP_ADDR_ANY, TCP_LISTEN_PORT);
            if (err != ERR_OK)
            {
                rt_kprintf("[TCP SERVER] 端口%d绑定失败 err=%d\n", TCP_LISTEN_PORT, err);
                tcp_accept(tcp_listen_pcb, NULL);
                tcp_close(tcp_listen_pcb);
                tcp_listen_pcb = NULL;
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }

            tcp_listen_pcb = tcp_listen_with_backlog(tcp_listen_pcb, TCP_ACCEPT_BACKLOG);
            tcp_accept(tcp_listen_pcb, tcp_server_accept_cb);
            rt_kprintf("[TCP SERVER] 端口%d开启监听\n", TCP_LISTEN_PORT);
        }

        rt_thread_mdelay(TCP_CONN_DELAY_MS);
    }
}

void tcp_deinit(void)
{
    tcp_full_close();
    if (tcp_send_sem != RT_NULL)
    {
        rt_sem_delete(tcp_send_sem);
        tcp_send_sem = RT_NULL;
    }
    rt_kprintf("[TCP DEINIT] TCP模块注销完毕\n");
}
