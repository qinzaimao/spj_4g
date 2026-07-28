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
#define UART_8BYTE 8


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
    if (recv_buf == NULL || recv_len == 0)
        return;

    uint16_t try_pos;
    uint16_t pos;
    uint16_t offset = 0;

    if(MY_SET_DHCP.host_state == true)
    {
        if (recv_len >= 5)
        {
            for(pos = 0; pos <= recv_len - 5; pos++)
            {
                uint8_t *tmp = recv_buf + pos;
                if(memcmp(tmp, "video", 5) == 0)
                {
                    seek_to_start_play_video();
                    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                    need_to_play_video_flag = false;
                    rt_mutex_release(elevtor_mutex);
                    // // 识别到video，标记初始化视频
                    // signed long long video_time = 0;
                    // // 当前播放时间(微秒)
                    // video_time = aic_player_get_play_time(my_lvgl_player_ctx.player);
                    // // 视频总时长(微秒)
                    // signed long long total_us = my_lvgl_player_ctx.media_info.duration;

                    // rt_kprintf("media_info.duration = %ld us\n", total_us);
                    // rt_kprintf("video_time = %ld us\n", video_time);

                    // // 计算播放进度百分比，防止总时长为0除零崩溃
                    // int play_progress = 0;
                    // if (total_us > 0)
                    // {
                    //     // 放大100倍算百分比，保留整数进度 0~100
                    //     play_progress = (video_time * 100) / total_us;
                    // }

                    // rt_kprintf("播放进度：%d %% \n", play_progress);
                    // if(!video_in_updating)
                    // {
                    //     char send_buf[32] = {0};
                    //     uint16_t video_sec  = video_time / 1000000;
                    //     if(video_sec % 10 == 0)
                    //     {
                    //         tcp_wait_video_time_flag = true;
                    //         video_sec += 1;
                    //     }
                    //     // 组装指令：vdes:时间秒数
                    //     rt_sprintf(send_buf, "time:%d", video_sec);
                    //     // 发送整条字符串
                    //     tcp_send_raw(send_buf, rt_strlen(send_buf));
                    //     last_recv_tick = rt_tick_get();
                    //     rt_kprintf("send time progress, sec = %d\n", video_sec);
                    // }

                    break;
                }
            }
        }
    }

    if(MY_SET_DHCP.host_state == false)
    {

        // ========== 第一层：6字节二进制短指令（最高优先级） ==========
        if (recv_len == 7)
        {
            // 增加首字节校验：必须第一个字节是0x00才是合法指令
            if(recv_buf[0] == 0x00)
            {
                uint8_t pkg_9[9] = {0x00};
                // 原始有效6个字节在recv_buf[1]~recv_buf[6]，拷贝到pkg9[1~6]
                memcpy(pkg_9 + 1, recv_buf + 1, 6);
                pkg_9[7] = 0x00;
                pkg_9[8] = 0xE6;
                Cmdparsing(pkg_9);
                return;
            }
            // 长度7但首字节不是0x00，属于杂包，不return，继续后续所有指令解析
        }

        // ========== 第二层：4字节视频/图片控制指令（次高频） ==========

        if(my_page == PAGE_HOME || my_page == PAGE_HOME_HOR)
        {
            while (offset + 4 <= recv_len)
            {
                uint8_t cmd_buf[4];
                memcpy(cmd_buf, recv_buf + offset, 4);

                if (memcmp(cmd_buf, "v1mp", 4) == 0) tcp_video_num = 1;
                else if (memcmp(cmd_buf, "v1av", 4) == 0) tcp_video_num = 2;
                else if (memcmp(cmd_buf, "v2mp", 4) == 0) tcp_video_num = 3;
                else if (memcmp(cmd_buf, "v2av", 4) == 0) tcp_video_num = 4;
                else if (memcmp(cmd_buf, "v3mp", 4) == 0) tcp_video_num = 5;
                else if (memcmp(cmd_buf, "v3av", 4) == 0) tcp_video_num = 6;

                if (memcmp(cmd_buf, "v1mp", 4) == 0 || memcmp(cmd_buf, "v1av", 4) == 0 ||
                    memcmp(cmd_buf, "v2mp", 4) == 0 || memcmp(cmd_buf, "v2av", 4) == 0 ||
                    memcmp(cmd_buf, "v3mp", 4) == 0 || memcmp(cmd_buf, "v3av", 4) == 0)
                {
                    if (video_in_updating)
                    {
                        tcp_video_num = 0;
                    }
                    offset += 4;
                    continue;
                }

                if (memcmp(cmd_buf, "vdes", 4) == 0)
                {
                    tcp_video_des = true;
                    offset += 4;
                    continue;
                }

                if (memcmp(cmd_buf, "vin1", 4) == 0) tcp_video_num = 1;
                else if (memcmp(cmd_buf, "vin2", 4) == 0) tcp_video_num = 2;
                else if (memcmp(cmd_buf, "vin3", 4) == 0) tcp_video_num = 3;
                else if (memcmp(cmd_buf, "vin4", 4) == 0) tcp_video_num = 4;
                else if (memcmp(cmd_buf, "vin5", 4) == 0) tcp_video_num = 5;
                else if (memcmp(cmd_buf, "vin6", 4) == 0) tcp_video_num = 6;
                else if (memcmp(cmd_buf, "vint", 4) == 0) tcp_video_num = 0;

                if (memcmp(cmd_buf, "vin1", 4) == 0 || memcmp(cmd_buf, "vin2", 4) == 0 ||
                memcmp(cmd_buf, "vin3", 4) == 0 || memcmp(cmd_buf, "vin4", 4) == 0 ||
                memcmp(cmd_buf, "vin5", 4) == 0 || memcmp(cmd_buf, "vin6", 4) == 0 ||
                memcmp(cmd_buf, "vint", 4) == 0)
                {
                    tcp_video_init = true;
                    if (video_in_updating)
                    {
                        tcp_video_num = 0;
                        tcp_video_init = false;
                    }
                    offset += 4;
                    continue;
                }

                if (memcmp(cmd_buf, "seek", 4) == 0)
                {
                    if(tcp_return_home_flag)
                    {
                        tcp_return_home_flag = false;
                        create_player_flag = true;

                        if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                            MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
                            video_init(true, 1);
                        else
                            video_init(true, 0);
                        rt_kprintf("从机设置退出等待主机信号\n");
                    }
                    if (my_lvgl_player_ctx.player != NULL)
                        seek_to_start_play_video();
                    else{
                        create_player_flag = true;
                        if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                            MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
                            video_init(true, 1);
                        else
                            video_init(true, 0);
                        rt_kprintf("seek 时没有播放器，创建\n");
                    }
                    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                    need_to_play_video_flag = false;
                    rt_mutex_release(elevtor_mutex);
                    offset += 4;
                    continue;
                }

                if (memcmp(cmd_buf, "img", 3) == 0)
                {
                    if (cmd_buf[3] >= '0' && cmd_buf[3] <= '9')
                    tcp_img_num = cmd_buf[3] - '0' + 1;
                    offset += 4;
                    continue;
                }

                if (memcmp(cmd_buf, "ida", 3) == 0)
                {
                    if (cmd_buf[3] >= '0' && cmd_buf[3] <= '9')
                    tcp_img_num = cmd_buf[3] - '0' + 1;
                    offset += 4;
                    continue;
                }

                offset++;
            }
        }

        // ========== time进度指令：挪到全局，不在首页大括号内；内部判断是否在主页 ==========
        // if (recv_len >= 5)
        // {
        //     for(int i= 0; i < recv_len; i++)
        //         rt_kprintf("%c", recv_buf[i]);
        //     rt_kprintf("\n");
        //     for (try_pos = 0; try_pos <= recv_len - 5; try_pos++)
        //     {
        //         uint8_t *p = recv_buf + try_pos;
        //         if(memcmp(p, "time:", 5) != 0)
        //             continue;

        //         // 匹配到time后，判断必须是主页，非主页直接跳过
        //         if(!(my_page == PAGE_HOME || my_page == PAGE_HOME_HOR))
        //             continue;

        //         // 已删除：time前不能是字母数字的判断逻辑

        //         // 校验：冒号后第一位必须是数字
        //         if (!(p[5] >= '0' && p[5] <= '9'))
        //             continue;

        //         // 校验：数据包包含中文/高位乱码直接跳过
        //         uint8_t has_zh = 0;
        //         for(uint16_t cc=0; cc < recv_len; cc++)
        //         {
        //             if(recv_buf[cc] > 127)
        //             {
        //                 has_zh = 1;
        //                 break;
        //             }
        //         }
        //         if(has_zh)
        //             continue;

        //         // 提取数字
        //         char num_buf[32] = {0};
        //         uint16_t num_idx = 0;
        //         uint16_t data_len = recv_len - try_pos;
        //         for(uint16_t i = 5; i < data_len; i++)
        //         {
        //             if(p[i] >= '0' && p[i] <= '9')
        //                 num_buf[num_idx++] = p[i];
        //             else
        //                 break;
        //         }
        //         num_buf[num_idx] = '\0';

        //         // 剔除末尾连续0
        //         while(num_idx > 0 && num_buf[num_idx - 1] == '0')
        //             num_idx--;
        //         num_buf[num_idx] = '\0';

        //         uint64_t sec_val = 0;
        //         for(uint16_t i=0; i<num_idx; i++)
        //             sec_val = sec_val * 10 + (num_buf[i] - '0');

        //         // 跳转时间逻辑
        //         if(tcp_wait_video_time_flag)
        //         {
        //             tcp_wait_video_time_flag = false;
        //             set_play_time(&my_lvgl_player_ctx, sec_val - 1);
        //         }
        //         else
        //             set_play_time(&my_lvgl_player_ctx, sec_val);

        //         rt_kprintf("[TCP TIME] 原始串:%s 去尾0:%s 有效秒数=%lu\n", num_buf, num_buf, sec_val);
        //         // 无return，继续解析其他指令
        //     }
        // }

        // ========== 第三层：8字节版本升级帧（0xAA 0x56） ==========
        if (recv_len >= UART_8BYTE)
        {
            for (uint16_t i = 0; i <= recv_len - UART_8BYTE; i++)
            {
                if (recv_buf[i] == 0xAA && recv_buf[i + 1] == 0x56)
                {
                    process_version_packet(recv_buf + i);
                    break;
                }
            }
        }

        // ========== 第四层：低频文本指令（背光 + 时间，放最后） ==========
        // 背光匹配
        if (recv_len >= 7)
        {
            for (try_pos = 0; try_pos <= 10 && try_pos <= recv_len - 6; try_pos++)
            {
                uint8_t *p = recv_buf + try_pos;
                if (memcmp(p, "light:", 6) == 0 || memcmp(p, "li_en:", 6) == 0)
                {
                    int val = 0;
                    for(uint16_t i = 6; i < (recv_len - try_pos); i++)
                    {
                        if(p[i] >= '0' && p[i] <= '9')
                            val = val * 10 + (p[i] - '0');
                        else break;
                    }
                    if(memcmp(p, "light:", 6) == 0)
                    {
                        MY_SET.backlight = val;
                        backlight_set(MY_SET.backlight);
                        rt_kprintf("[TCP BL] 收到背光设置：%d\n", MY_SET.backlight);
                    }else if(memcmp(p, "li_en:", 6) == 0)
                    {
                        MY_SET.e_con_backlight = val;
                        backlight_set(MY_SET.e_con_backlight);
                        rt_kprintf("[TCP BL] 收到节能背光设置：%d\n", MY_SET.e_con_backlight);
                    }
                    // rt_kprintf("[TCP BL] 收到背光设置：%d\n", MY_SET.backlight);
                    return;
                }
            }

        }
        // 时间字符串匹配
        if (recv_len >= 16)
        {
            // for(int i =0; i < 16; i++)
            // {
            //     rt_kprintf("%c", recv_buf[i]);
            // }
            // rt_kprintf("\n");
            for (try_pos = 0; try_pos <= 10 && try_pos <= recv_len - 16; try_pos++)
            {
                uint8_t *p = recv_buf + try_pos;
                if(p[4] == '/' && p[7] == '/' && p[10] == ' ')
                {
                    char date_str[11] = {0};
                    char time_str[6]  = {0};
                    memcpy(date_str, p, 10);
                    memcpy(time_str, p + 11, 5);

                    // rt_kprintf("[TCP TIME SET] 原始接收：%s %s\n", date_str, time_str);
                    int year, mon, day, hour, min;
                    sscanf(date_str, "%d/%d/%d", &year, &mon, &day);
                    sscanf(time_str, "%d:%d", &hour, &min);

                    // rt_kprintf("[TCP TIME SET] 解析结果 年:%d 月:%d 日:%d 时:%d 分:%d\n", year, mon, day, hour, min);
                    rt_err_t ret = RT_EOK;
                    ret = set_date(year, mon, day);
                    if (ret != RT_EOK)
                        rt_kprintf("set RTC date failed, ret:%d\n", ret);
                    rt_thread_mdelay(1);
                    ret = set_time(hour, min, 1);
                    if (ret != RT_EOK)
                        rt_kprintf("set RTC time failed, ret:%d\n", ret);
                    tcp_set_time_flag = true;
                    return;
                }
            }
        }
    }
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
