#include "my_tcp.h"


/**
 * MY_SET_DHCP.host_state 动态切换模式
 *
 * true  = TCP客户端：板子主动连接另一块板子TCP服务器
 * false = TCP服务端：板子本机监听1346，等待另一块板子TCP客户端接入
*/

// 服务端监听端口
#define TCP_LISTEN_PORT     1346
#define TCP_ACCEPT_BACKLOG  2
// TCP客户端目标端口
#define TARGET_TCP_PORT     1346

// 公共基础配置
#define TCP_SEND_BUF_LEN    4096
#define TCP_CONN_DELAY_MS   1000
#define UART_8BYTE           8
#define UART_9BYTE           9

// 配置：服务端4秒发一次心跳；客户端9秒无数据超时断开
#define SERVER_HEART_MS     2500
#define CLIENT_TIMEOUT_MS   9000
#define SERVER_HEART_TICK   rt_tick_from_millisecond(SERVER_HEART_MS)
#define CLIENT_TIMEOUT_TICK rt_tick_from_millisecond(CLIENT_TIMEOUT_MS)

// 统一心跳包内容
static const uint8_t tcp_heart_data[] = {0xAB,0xCD,0xEF,0x01};

static struct tcp_pcb *tcp_listen_pcb = NULL;
static struct tcp_pcb *tcp_data_pcb = NULL;
static rt_sem_t tcp_send_sem = RT_NULL;
static ip_addr_t target_ip;
static rt_bool_t last_host_state = RT_FALSE;
rt_tick_t last_recv_tick = 0;
// 标记TCP握手完成、链路正式连通
rt_bool_t is_tcp_connected = RT_FALSE;
// 新增：超时断开标记，不立刻销毁资源
static rt_bool_t need_close_link = RT_FALSE;

// 环形缓冲区
typedef struct {
    uint8_t buf[TCP_SEND_BUF_LEN];
    uint32_t r_ptr;
    uint32_t w_ptr;
    uint32_t len;
} ring_buffer_t;
static ring_buffer_t data_ring_buf;

static void ring_buffer_init(ring_buffer_t *rb)
{
    memset(rb->buf, 0, TCP_SEND_BUF_LEN);
    rb->r_ptr = 0;
    rb->w_ptr = 0;
    rb->len = 0;
}

static int ring_buffer_write_byte(ring_buffer_t *rb, uint8_t byte)
{
    if (rb->len >= TCP_SEND_BUF_LEN)
    {
        rb->r_ptr = (rb->r_ptr + 1) % TCP_SEND_BUF_LEN;
        rb->len--;
        static uint32_t overflow_count = 0;
        overflow_count++;
        if (overflow_count % 100 == 0)
        {
            rt_kprintf("⚠️ 数据环形缓冲区溢出，累计次数：%d\n", overflow_count);
        }
    }
    rb->buf[rb->w_ptr] = byte;
    rb->w_ptr = (rb->w_ptr + 1) % TCP_SEND_BUF_LEN;
    rb->len++;
    return 0;
}

static void ring_buffer_read_at(ring_buffer_t *rb, uint8_t *dst, uint32_t pos, uint32_t len)
{
    uint32_t start_idx = (rb->r_ptr + pos) % TCP_SEND_BUF_LEN;
    uint32_t first_part_len = TCP_SEND_BUF_LEN - start_idx;
    if (len <= first_part_len)
    {
        memcpy(dst, &rb->buf[start_idx], len);
    }
    else
    {
        memcpy(dst, &rb->buf[start_idx], first_part_len);
        memcpy(dst + first_part_len, rb->buf, len - first_part_len);
    }
}

static void ring_buffer_skip(ring_buffer_t *rb, uint32_t len)
{
    if (len > rb->len)
        len = rb->len;
    rb->r_ptr = (rb->r_ptr + len) % TCP_SEND_BUF_LEN;
    rb->len -= len;
}

static void ring_buffer_clear(ring_buffer_t *rb)
{
    rb->r_ptr = 0;
    rb->w_ptr = 0;
    rb->len = 0;
    memset(rb->buf, 0, TCP_SEND_BUF_LEN);
}

// TCP异常断开统一回调
static void tcp_common_err_cb(void *arg, err_t err)
{
    rt_kprintf("[TCP ERROR] 链路异常断开！err=%d 网线拔除/对端强制下线\n", err);
    need_close_link = RT_TRUE;
}

// TCP发送完成应答回调
static err_t tcp_common_sent_cb(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    // rt_kprintf("[TCP SENT] 发送应答确认字节：%d\n", len);
    return ERR_OK;
}

// TCP数据接收回调
static err_t tcp_common_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    last_recv_tick = rt_tick_get();
    if (p == NULL || err != ERR_OK)
    {
        rt_kprintf("[TCP RECV] 对端主动发送FIN正常断开连接\n");
        need_close_link = RT_TRUE;
        return ERR_CLSD;
    }

    uint16_t recv_len = p->tot_len;
    uint8_t tcp_down_buf[TCP_SEND_BUF_LEN] = {0};
    pbuf_copy_partial(p, tcp_down_buf, recv_len, 0);

    // rt_kprintf("[TCP RECV] 远端下发数据 len=%d raw:", recv_len);
    // for (uint16_t i = 0; i < recv_len; i++)
    //     rt_kprintf(" %02X", tcp_down_buf[i]);
    // rt_kprintf("\n");



    // 判断是不是心跳包，是心跳直接丢弃不解析
    if (recv_len == sizeof(tcp_heart_data) && memcmp(tcp_down_buf, tcp_heart_data, sizeof(tcp_heart_data)) == 0)
    {
        pbuf_free(p);
        tcp_recved(pcb, p->tot_len);
        return ERR_OK;
    }

    // 非心跳才进入协议解析
    parse_frame_and_tcp_send(tcp_down_buf, recv_len);

    pbuf_free(p);
    tcp_recved(pcb, p->tot_len);
    return ERR_OK;
}

// TCP客户端连接成功回调
static err_t tcp_client_connected_cb(void *arg, struct tcp_pcb *pcb, err_t err)
{
    // 已经连上直接退出，禁止重复执行
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
    is_tcp_connected = RT_TRUE;
    need_close_link = RT_FALSE;

    return ERR_OK;
}

err_t tcp_send_raw(uint8_t *buf, uint16_t len)
{
    if(!is_tcp_connected || tcp_data_pcb == NULL || need_close_link)
        return -1;
    return tcp_write(tcp_data_pcb, buf, len, TCP_WRITE_FLAG_COPY);
}
// TCP服务端接入新客户端回调
static err_t tcp_server_accept_cb(void *arg, struct tcp_pcb *new_pcb, err_t err)
{
    if (err != ERR_OK || new_pcb == NULL)
        return err;

    // 如果已有旧客户端连接，直接关闭旧PCB
    if (tcp_data_pcb != NULL)
    {
        rt_kprintf("[TCP SERVER] 已有旧客户端连接，主动断开旧链路，接入新客户端\n");
        tcp_recv(tcp_data_pcb, NULL);
        tcp_sent(tcp_data_pcb, NULL);
        tcp_err(tcp_data_pcb, NULL);
        tcp_close(tcp_data_pcb);
        tcp_data_pcb = NULL;
        is_tcp_connected = RT_FALSE;
    }

    tcp_data_pcb = new_pcb;
    tcp_err(tcp_data_pcb, tcp_common_err_cb);
    tcp_recv(tcp_data_pcb, tcp_common_recv_cb);
    tcp_sent(tcp_data_pcb, tcp_common_sent_cb);
    last_recv_tick = rt_tick_get();
    is_tcp_connected = RT_TRUE;
    need_close_link = RT_FALSE;
    rt_kprintf("[TCP SERVER] ✅ 对端客户端板子成功接入，链路正式建立\n");
    return ERR_OK;
}

// 对外TCP发送接口
err_t tcp_send_data(uint8_t *data, uint16_t len)
{
    if (tcp_data_pcb == NULL || data == NULL || len == 0 || need_close_link || !is_tcp_connected)
    {
        rt_kprintf("[TCP SEND] 无有效TCP链路/标记待断开,发送数据丢弃\n");
        return ERR_CONN;
    }

    if (rt_sem_take(tcp_send_sem, rt_tick_from_millisecond(500)) != RT_EOK)
    {
        rt_kprintf("[TCP SEND] 获取发送信号量超时，丢弃发送\n");
        return ERR_CONN;
    }

    rt_kprintf("[TCP SEND] 上行发送帧 len=%d data:", len);
    for (uint16_t i = 0; i < len; i++)
        rt_kprintf(" %02X", data[i]);
    rt_kprintf("\n");

    err_t ret = tcp_write(tcp_data_pcb, data, len, TCP_WRITE_FLAG_COPY);
    if (ret == ERR_OK)
        tcp_output(tcp_data_pcb);
    else
        rt_kprintf("[TCP SEND] 发送缓冲区溢出失败 err:%d\n", ret);

    rt_sem_release(tcp_send_sem);
    // 本机主动发数据，同样刷新超时计时
    if(is_tcp_connected)
        last_recv_tick = rt_tick_get();
    return ret;
}

// 协议解析入口：串口数据 / TCP下行数据统一解析
void parse_frame_and_tcp_send(uint8_t *recv_buf, uint16_t recv_len)
{
    for (uint16_t i = 0; i < recv_len; i++)
    {
        ring_buffer_write_byte(&data_ring_buf, recv_buf[i]);

        // 9字节电梯协议帧 0x00起始 0xE6结尾
        if (data_ring_buf.len >= UART_9BYTE)
        {
            uint8_t valid_frame_buf[UART_9BYTE] = {0};
            ring_buffer_read_at(&data_ring_buf, valid_frame_buf, data_ring_buf.len - UART_9BYTE, UART_9BYTE);

            if (valid_frame_buf[0] == 0x00 && valid_frame_buf[8] == 0xE6)
            {
                uint16_t recv_crc = (valid_frame_buf[8] << 8) | valid_frame_buf[7];
                uint16_t calc_crc = crc_chk_value(valid_frame_buf, 7);

                if (calc_crc == recv_crc)
                {
                    // rt_kprintf("[TCP PARSE] 9字节帧CRC校验通过 calc=%04X recv=%04X\n", calc_crc, recv_crc);
                    Cmdparsing(valid_frame_buf);
                    // tcp_send_data(valid_frame_buf, UART_9BYTE);

                    if (video_udp_state == 1)
                    {
                        rt_kprintf("[TCP CTRL] 执行初始化播放器\n");
                        video_init(true, 1);
                    }
                    else if (video_udp_state == 2)
                    {
                        rt_kprintf("[TCP CTRL] 执行销毁播放器\n");
                        destroy_player(PRINTF_ELEVTOR);
                    }
                    else if (video_udp_state == 3)
                    {
                        if (my_lvgl_player_ctx.player != NULL)
                        {
                            rt_kprintf("[TCP CTRL] 视频从头播放\n");
                            seek_to_start_play_video();
                        }
                        else
                            rt_kprintf("[TCP CTRL] 播放器句柄为空，无法跳转\n");
                    }
                }
                else
                {
                    // rt_kprintf("[TCP PARSE] 9字节帧CRC校验失败 calc=%04X recv=%04X\n", calc_crc, recv_crc);
                }
                ring_buffer_skip(&data_ring_buf, data_ring_buf.len);
                continue;
            }
        }

        // 8字节版本协议帧 AA 56 帧头
        if (data_ring_buf.len >= UART_8BYTE)
        {
            uint8_t valid_frame_buf[UART_8BYTE] = {0};
            ring_buffer_read_at(&data_ring_buf, valid_frame_buf, data_ring_buf.len - UART_8BYTE, UART_8BYTE);

            if (valid_frame_buf[0] == 0xAA && valid_frame_buf[1] == 0x56)
            {
                // rt_kprintf("[TCP PARSE] 解析到8字节版本协议帧\n");
                process_version_packet(valid_frame_buf);
                // tcp_send_data(valid_frame_buf, UART_8BYTE);
                ring_buffer_skip(&data_ring_buf, data_ring_buf.len);
                continue;
            }
        }
    }
}

// 彻底关闭所有TCP资源，重置连接标记
static void tcp_full_close(void)
{
    rt_sem_take(tcp_send_sem, RT_WAITING_FOREVER);

    // 1 先解绑回调、关闭PCB
    if (tcp_data_pcb != NULL)
    {
        tcp_recv(tcp_data_pcb, NULL);
        tcp_sent(tcp_data_pcb, NULL);
        tcp_err(tcp_data_pcb, NULL);
        tcp_close(tcp_data_pcb);
        // 【关键】关闭后立刻置空全局指针，杜绝野指针二次操作
        tcp_data_pcb = NULL;
    }
    if (tcp_listen_pcb != NULL)
    {
        tcp_close(tcp_listen_pcb);
        tcp_listen_pcb = NULL;
    }

    ring_buffer_clear(&data_ring_buf);
    need_close_link = RT_FALSE;
    // 标志位最后再改，防止上层业务瞬间误判
    rt_thread_mdelay(500);
    is_tcp_connected = RT_FALSE;

    rt_sem_release(tcp_send_sem);
    rt_kprintf("[TCP SWITCH] 旧模式TCP资源全部释放完毕\n");
}

// TCP主业务线程
void tcp_info_thread_entry(void *parameter)
{
    err_t err;
    ring_buffer_init(&data_ring_buf);

    tcp_send_sem = rt_sem_create("tcp_sem", 1, RT_IPC_FLAG_FIFO);
    if (tcp_send_sem == RT_NULL)
    {
        rt_kprintf("[TCP THREAD] 发送互斥信号量创建失败，线程直接退出\n");
        return;
    }
    last_host_state = MY_SET_DHCP.host_state;
    last_recv_tick = rt_tick_get();
    is_tcp_connected = RT_FALSE;
    need_close_link = RT_FALSE;
    rt_kprintf("[TCP THREAD] TCP双板互测线程启动，初始模式:%s\n", last_host_state ? "TCP客户端(主动连另一块板)" : "TCP服务端(等待另一块板接入)");

    while (1)
    {
        // 等待LWIP协议栈初始化完成
        if (!get_lwip_flag)
        {
            rt_kprintf("[TCP THREAD] 等待LWIP网络协议栈就绪...\n");
            rt_thread_mdelay(TCP_CONN_DELAY_MS);
            continue;
        }

        // 优先处理标记位：需要关闭链路则统一销毁，先长时间让出CPU
        if (need_close_link)
        {
            rt_kprintf("[TCP WARN] 检测到链路需断开，执行资源回收\n");
            rt_thread_mdelay(300);
            tcp_full_close();
            rt_thread_mdelay(500);
            continue;
        }

        rt_tick_t now_tick = rt_tick_get();

        // ========== 客户端：超时先让出CPU再标记断开，禁止连续循环抢占 ==========
        if (MY_SET_DHCP.host_state == RT_TRUE && tcp_data_pcb != NULL && is_tcp_connected == RT_TRUE)
        {
            if ((now_tick - last_recv_tick) > CLIENT_TIMEOUT_TICK)
            {
                rt_kprintf("[TCP CLIENT] ❌ 已建立连接后超过9秒未收到服务端下发数据，标记链路待断开\n");
                need_close_link = RT_TRUE;
                // 超时后强制休眠，释放调度，防止死循环抢占CPU
                rt_thread_mdelay(200);
                continue;
            }
        }

        // ========== 服务端：接入后每4秒定时发送心跳包 ==========
        if (MY_SET_DHCP.host_state == RT_FALSE && tcp_data_pcb != NULL && is_tcp_connected == RT_TRUE)
        {
            if ((now_tick - last_recv_tick) > SERVER_HEART_TICK)
            {
                rt_sem_take(tcp_send_sem, RT_WAITING_FOREVER);
                err_t ret = tcp_write(tcp_data_pcb, tcp_heart_data, sizeof(tcp_heart_data), TCP_WRITE_FLAG_COPY);
                if (ret == ERR_OK)
                {
                    tcp_output(tcp_data_pcb);
                    rt_kprintf("[TCP SERVER] 定时2.5s心跳包发送: ");
                    for (int i = 0; i < sizeof(tcp_heart_data); i++)
                        rt_kprintf("%02X ", tcp_heart_data[i]);
                    rt_kprintf("\n");
                }
                rt_sem_release(tcp_send_sem);
                last_recv_tick = now_tick;
            }
        }

        // 检测主从模式变量修改，热切换TCP工作模式
        if (MY_SET_DHCP.host_state != last_host_state)
        {
            tcp_full_close();
            last_host_state = MY_SET_DHCP.host_state;
            rt_kprintf("[TCP SWITCH] 检测到host_state变更，切换为：%s\n",
                        last_host_state ? "TCP客户端(主动连接对端板子)" : "TCP服务端(等待对端板子接入)");
        }

        // ===================== TCP客户端逻辑 =====================
        if (MY_SET_DHCP.host_state == RT_TRUE)
        {
            if (tcp_data_pcb != NULL)
            {
                // 若PCB存在，但长时间没有任何收发，兜底强制断连重连
                if ((rt_tick_get() - last_recv_tick) > CLIENT_TIMEOUT_TICK)
                {
                    rt_kprintf("[TCP CLIENT] 连接握手无应答，超时强制断开重连\n");
                    need_close_link = RT_TRUE;
                    rt_thread_mdelay(200);
                    continue;
                }
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }

            // 固定间隔1秒尝试一次连接
            rt_thread_mdelay(TCP_CONN_DELAY_MS);
            // 从MY_SET_DHCP.dns数组动态读取目标服务端板子IP
            char IP_temp[30] = {0};
            sprintf(IP_temp, "%d.%d.%d.%d",
                    MY_SET_DHCP.dns[0],
                    MY_SET_DHCP.dns[1],
                    MY_SET_DHCP.dns[2],
                    MY_SET_DHCP.dns[3]);
            ipaddr_aton(IP_temp, &target_ip);
            rt_kprintf("[TCP CLIENT] 读取目标服务端IP:%s\n", IP_temp);

            tcp_data_pcb = tcp_new();
            if (tcp_data_pcb == NULL)
            {
                rt_kprintf("[TCP CLIENT] tcp_new 创建TCP PCB失败\n");
                continue;
            }

            tcp_err(tcp_data_pcb, tcp_common_err_cb);
            tcp_recv(tcp_data_pcb, tcp_common_recv_cb);
            tcp_sent(tcp_data_pcb, tcp_common_sent_cb);

            err = tcp_connect(tcp_data_pcb, &target_ip, TARGET_TCP_PORT, tcp_client_connected_cb);
            if (err != ERR_OK)
            {
                rt_kprintf("[TCP CLIENT] 发起TCP连接失败 err=%d,1秒后重试\n", err);
                tcp_close(tcp_data_pcb);
                tcp_data_pcb = NULL;
                continue;
            }
            rt_kprintf("[TCP CLIENT] 正在尝试连接 %s:%d\n", IP_temp, TARGET_TCP_PORT);
        }
        // ===================== TCP服务端逻辑 =====================
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
                rt_kprintf("[TCP SERVER] tcp_new 创建监听PCB失败\n");
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }

            err = tcp_bind(tcp_listen_pcb, IP_ADDR_ANY, TCP_LISTEN_PORT);
            if (err != ERR_OK)
            {
                rt_kprintf("[TCP SERVER] 端口%d绑定失败 err=%d\n", TCP_LISTEN_PORT, err);
                tcp_close(tcp_listen_pcb);
                tcp_listen_pcb = NULL;
                rt_thread_mdelay(TCP_CONN_DELAY_MS);
                continue;
            }

            tcp_listen_pcb = tcp_listen_with_backlog(tcp_listen_pcb, TCP_ACCEPT_BACKLOG);
            tcp_accept(tcp_listen_pcb, tcp_server_accept_cb);
            rt_kprintf("[TCP SERVER] 本机%d端口开启监听，等待另一块板子客户端接入\n", TCP_LISTEN_PORT);
        }

        rt_thread_mdelay(TCP_CONN_DELAY_MS);
    }
}

// 模块整体反初始化释放资源
void tcp_deinit(void)
{
    tcp_full_close();
    if (tcp_send_sem != RT_NULL)
    {
        rt_sem_delete(tcp_send_sem);
        tcp_send_sem = RT_NULL;
    }
    rt_kprintf("[TCP DEINIT] TCP通信模块全部注销完成\n");
}
