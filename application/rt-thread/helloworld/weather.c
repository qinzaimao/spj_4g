#include "weather.h"
#include "../../packages\third-party\cJSON-1.7.16\cJSON.c"
#include "../../kernel/rt-thread/components/net/lwip/lwip-2.0.3/src/include/lwip/api.h"
#include "../../kernel/rt-thread/components/net/lwip/lwip-2.0.3/src/include/lwip/dns.h"

extern void lwip_test_example_main_loop(void *data);

// 配置参数（可根据实际需求调整）
#define MAX_RESPONSE_SIZE 4096  // 响应缓冲区大小
#define CONNECT_TIMEOUT_MS 5000 // 连接超时时间（5秒）
#define RECV_TIMEOUT_MS 10000   // 接收超时时间（10秒）
#define RETRY_DELAY_MS 5000     // 异常重试延迟（5秒）

//网络时间获取
#define SYNC_FAIL_COOL     (10 * RT_TICK_PER_SECOND)      /* 失败冷却：10秒，可改成5/15秒 */
#define TIME_SYNC_OFFSET_SEC  1    /* 时间补偿（秒），按需调整 */

#define TIME_SERVER_CNT (sizeof(time_server_list)/sizeof(time_server_list[0]))
#define TIME_SYNC_INTERVAL (60 * 60 * RT_TICK_PER_SECOND) /* 1小时同步一次 */

/* 时间同步服务器配置（免费无限制，可随时换） */
#define TIME_SYNC_HOST     "www.baidu.com"
#define TIME_SYNC_PORT     80

typedef struct {
    const char *host;
    uint16_t port;
} time_server_t;

static const time_server_t time_server_list[] = {
    {"www.baidu.com", 80},
    {"www.qq.com",    80},
    {"www.taobao.com",80},
    {"www.163.com",   80},
};
/* 时间同步间隔控制 */
static rt_tick_t last_time_sync_tick = 0;
static rt_tick_t sync_fail_cool_tick = 0;  // 失败冷却时间点
/* 多节点时间服务器列表，HTTP HEAD 获取Date头 */


char response_buffer[MAX_RESPONSE_SIZE];

// 城市编码列表
char city[][10] = {
    "110000",  // 北京
    "440300",  // 深圳
    "440100",  // 广州
    "420100",  // 武汉
    "440400",  // 珠海
    "310000"   // 上海
};

#include <time.h>

// 月份英文缩写映射，RFC1123 Date头
static const char *month_str[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

/**
 * @brief 解析RFC‑1123 GMT时间字符串 "Wed, 27 Aug 2026 08:20:00 GMT" → 返回UTC时间戳(秒)
 * @param date_str 输入Date头内容，例如 "Wed, 27 Aug 2026 08:20:00 GMT"
 * @return unix utc timestamp(秒), 失败返回0
 */
static time_t parse_rfc1123_gmt(const char *date_str)
{
    int day, year, hour, min, sec;
    char mon_buf[4];
    int mon_idx = -1;
    // 格式样例：Wed, 27 Aug 2026 08:20:00 GMT
    int ret = sscanf(date_str, "%*[^,], %d %3s %d %d:%d:%d", &day, mon_buf, &year, &hour, &min, &sec);
    if(ret != 6)
    {
        return 0;
    }
    // 匹配月份缩写
    for(int i=0;i<12;i++)
    {
        if(strcmp(mon_buf, month_str[i]) == 0)
        {
            mon_idx = i;
            break;
        }
    }
    if(mon_idx < 0) return 0;

    struct tm tm_gmt;
    rt_memset(&tm_gmt,0,sizeof(tm_gmt));
    tm_gmt.tm_mday = day;
    tm_gmt.tm_mon  = mon_idx;
    tm_gmt.tm_year = year - 1900;
    tm_gmt.tm_hour = hour;
    tm_gmt.tm_min  = min;
    tm_gmt.tm_sec  = sec;
    tm_gmt.tm_isdst = 0;

    rt_kprintf("[HTTP‑Date parse] year:%d, mon:%d, day:%d, hour:%d, min:%d, sec:%d\n",
            tm_gmt.tm_year, tm_gmt.tm_mon, tm_gmt.tm_mday,
            tm_gmt.tm_hour, tm_gmt.tm_min, tm_gmt.tm_sec);

    time_t ts_utc = timegm(&tm_gmt);
    rt_kprintf("[HTTP‑Date] parsed UTC ts: %u\n", (unsigned int)ts_utc);

    return ts_utc;
}



/**
 * @brief 独立时间同步：通过免费HTTP服务器的Date头同步系统时间
 * @note  不限制界面，不占用天气接口次数
 */
/**
 * @brief 通过指定HTTP服务器HEAD请求获取Date头，解析系统时间
 * @param host 域名
 * @param port 端口
 * @return true:同步成功 false:失败
 */
static bool sync_system_time_by_host(const char *host, uint16_t port)
{
    struct netconn *conn = NULL;
    struct netbuf  *buf  = NULL;
    err_t err;
    rt_tick_t start_tick;
    bool ret = false;
    char *date_ptr = NULL;
    static char time_resp_buf[512];  /* 只收响应头，512字节足够 */
    int resp_index = 0;
    do
    {
        /* 1. DNS解析域名 */
        ip_addr_t server_ip;
        rt_kprintf("[TIME-SYNC] 正在解析域名 %s ...\n", host);
        err = netconn_gethostbyname(host, &server_ip);
        if (err != ERR_OK)
        {
            rt_kprintf("[TIME-SYNC] DNS解析失败 %s，错误码: %d\n", host, err);
            break;
        }
        rt_kprintf("[TIME-SYNC] DNS解析成功: %d.%d.%d.%d\n",
                ip4_addr1(&server_ip), ip4_addr2(&server_ip),
                ip4_addr3(&server_ip), ip4_addr4(&server_ip));
        /* 2. 创建TCP连接 */
        conn = netconn_new(NETCONN_TCP);
        if (conn == NULL)
        {
            rt_kprintf("[TIME-SYNC] 创建TCP连接失败\n");
            break;
        }
        /* 3. 连接服务器（带ERR_RTE内部重试） */
        rt_kprintf("[TIME-SYNC] 正在连接 %s:%d ...\n", host, port);
        start_tick = rt_tick_get();
        err = ERR_INPROGRESS;
        uint8_t rte_retry = 0;
        while (1)
        {
            err = netconn_connect(conn, &server_ip, port);
            if (err == ERR_RTE && rte_retry < 5)
            {
                rte_retry++;
                rt_kprintf("[TIME-SYNC] 路由暂不可用，第%u次重试...\n", rte_retry);
                rt_thread_mdelay(200);
                continue;
            }
            if (err != ERR_INPROGRESS) break;
            if (rt_tick_get() - start_tick > rt_tick_from_millisecond(CONNECT_TIMEOUT_MS))
            {
                err = ERR_TIMEOUT;
                break;
            }
            rt_thread_mdelay(10);
        }
        if (err != ERR_OK)
        {
            rt_kprintf("[TIME-SYNC] 连接服务器%s失败，错误码: %d\n", host, err);
            break;
        }
        /* 4. 发送HEAD请求（只拿响应头，最省流量） */
        char http_req[128] = {0};
        rt_snprintf(http_req, sizeof(http_req),
                "HEAD / HTTP/1.1\r\n"
                "Host: %s\r\n"
                "Connection: close\r\n"
                "\r\n",
                host);
        err = netconn_write(conn, http_req, strlen(http_req), NETCONN_COPY);
        if (err != ERR_OK)
        {
            rt_kprintf("[TIME-SYNC] 发送请求失败 %s，错误码: %d\n", host, err);
            break;
        }
        /* 5. 接收响应头，拿到Date就断开 */
        rt_memset(time_resp_buf, 0, sizeof(time_resp_buf));
        resp_index = 0;
        start_tick = rt_tick_get();
        bool got_date = false;
        while (1)
        {
            if (rt_tick_get() - start_tick > rt_tick_from_millisecond(RECV_TIMEOUT_MS))
            {
                rt_kprintf("[TIME-SYNC] 接收超时 %s\n", host);
                break;
            }
            err = netconn_recv(conn, &buf);
            if (err == ERR_OK)
            {
                start_tick = rt_tick_get();
                if (buf != NULL && buf->p != NULL)
                {
                    struct pbuf *q;
                    for (q = buf->p; q != NULL; q = q->next)
                    {
                        int copy_size = q->len;
                        if (resp_index + copy_size >= (int)sizeof(time_resp_buf))
                            copy_size = sizeof(time_resp_buf) - resp_index - 1;
                        memcpy(time_resp_buf + resp_index, q->payload, copy_size);
                        resp_index += copy_size;
                    }
                    netbuf_delete(buf);
                    buf = NULL;
                    time_resp_buf[resp_index] = '\0';
                    if (strstr(time_resp_buf, "\r\n\r\n") != NULL)
                    {
                        date_ptr = strstr(time_resp_buf, "Date:");
                        if (date_ptr != NULL)
                        {
                            got_date = true;
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
            else if (err == ERR_CLSD)
            {
                time_resp_buf[resp_index] = '\0';
                date_ptr = strstr(time_resp_buf, "Date:");
                if (date_ptr != NULL) got_date = true;
                break;
            }
            else if (err != ERR_WOULDBLOCK)
            {
                rt_kprintf("[TIME-SYNC] 接收错误 %d, host:%s\n", err, host);
                break;
            }
            if (got_date) break;
            rt_thread_mdelay(10);
        }
        if (!got_date || date_ptr == NULL)
        {
            rt_kprintf("[TIME-SYNC] %s 未找到Date头\n", host);
            break;
        }
        /* 6. 解析Date头 → 设置系统时间 */
        date_ptr += strlen("Date:");
        while (*date_ptr == ' ') date_ptr++;
        time_t ts_gmt = parse_rfc1123_gmt(date_ptr);
        if (ts_gmt > 0)
        {
            ts_gmt += TIME_SYNC_OFFSET_SEC;
            time_t ts_beijing = ts_gmt + 8 * 3600;
            struct tm tm_gmt = *gmtime(&ts_gmt);
            rt_kprintf("[TIME-SYNC] GMT: %04d-%02d-%02d %02d:%02d:%02d  ts:%u\n",
                    tm_gmt.tm_year + 1900, tm_gmt.tm_mon + 1, tm_gmt.tm_mday,
                    tm_gmt.tm_hour, tm_gmt.tm_min, tm_gmt.tm_sec,
                    (unsigned int)ts_gmt);
            struct tm tm_bj = *gmtime(&ts_beijing);
            rt_kprintf("[TIME-SYNC] BJ : %04d-%02d-%02d %02d:%02d:%02d  ts:%u\n",
                    tm_bj.tm_year + 1900, tm_bj.tm_mon + 1, tm_bj.tm_mday,
                    tm_bj.tm_hour, tm_bj.tm_min, tm_bj.tm_sec,
                    (unsigned int)ts_beijing);

            rt_kprintf("[TIME-SYNC] ✅时间同步成功（服务器: %s）\n", host);
            // rt_thread_mdelay(500);
            MY_SET_TIME.year = tm_bj.tm_year + 1900;
            MY_SET_TIME.month = tm_bj.tm_mon + 1;
            MY_SET_TIME.day = tm_bj.tm_mday;
            MY_SET_TIME.hour = tm_bj.tm_hour;
            MY_SET_TIME.minute = tm_bj.tm_min;
            MY_SET_TIME.second = tm_bj.tm_sec;

            int set_time_ret;
            set_time_ret = set_date(tm_bj.tm_year + 1900, tm_bj.tm_mon + 1, tm_bj.tm_mday);
            if (set_time_ret != RT_EOK)
                rt_kprintf("set RTC date failed");
            rt_thread_mdelay(1);
            set_time_ret = set_time(tm_bj.tm_hour,  tm_bj.tm_min, tm_bj.tm_sec);
            if (set_time_ret != RT_EOK)
                rt_kprintf("set RTC time failed");
            if (is_tcp_connected && MY_SET_DHCP.host_state == true)
            {
                char time_str[20] = {0};
                sprintf(time_str, "%04d/%02d/%02d %02d:%02d",
                MY_SET_TIME.year, MY_SET_TIME.month, MY_SET_TIME.day,
                MY_SET_TIME.hour, MY_SET_TIME.minute);
                // rt_kprintf("time_str:%s\n", time_str);
                tcp_send_raw(time_str, 16);
            }
            ret = true;
        }
        else
        {
            rt_kprintf("[TIME-SYNC] %s 解析Date头失败\n", host);
        }
    } while (0);
    /* 释放资源 */
    if (buf  != NULL) { netbuf_delete(buf);  buf  = NULL; }
    if (conn != NULL) { netconn_close(conn); netconn_delete(conn); conn = NULL; }
    return ret;
}

/**
 * @brief 多节点版本：依次尝试各个HTTP服务器，任意一个成功就返回true
 */
static bool sync_system_time_via_http(void)
{
    for(int i = 0; i < TIME_SERVER_CNT; i++)
    {
        const time_server_t *srv = &time_server_list[i];
        if(sync_system_time_by_host(srv->host, srv->port))
        {
            return true;
        }
        rt_kprintf("[TIME-SYNC] 节点 %s 失败，准备切换下一个节点\n", srv->host);
        rt_thread_mdelay(200); // 节点之间短暂间隔，防止连续冲击网络
    }
    rt_kprintf("[TIME-SYNC] ❌全部时间服务器节点尝试完毕，同步失败\n");
    return false;
}


void weather_thread_entry(void *parameter)
{
    while (!init_set_img_ok) rt_thread_mdelay(200);
    static uint8_t time_cnt = 0;
    static bool get_city_flag = false;
    struct netconn *conn = NULL;
    while (1)
    {
        /* ================================================================
         * 【时间同步】不限制界面，只要网络就绪且到1小时间隔就执行
         * ================================================================ */
        if (get_lwip_flag && (my_page == PAGE_HOME || my_page == PAGE_HOME_HOR)
            && ((rt_tick_get() - last_time_sync_tick) > TIME_SYNC_INTERVAL || !set_sync_cnt)
            && (rt_tick_get() > sync_fail_cool_tick)) // 冷却时间没到，直接跳过
        {
            rt_thread_mdelay(500);
            set_sync_cnt ++;
            rt_kprintf("\n--- 开始时间同步 ---\n");
            if (sync_system_time_via_http())
            {
                last_time_sync_tick = rt_tick_get();
                sync_fail_cool_tick = 0; // 成功，清除冷却标记
            }
            else
            {
                // 设置冷却，SYNC_FAIL_COOL秒之后才允许再次尝试
                sync_fail_cool_tick = rt_tick_get() + SYNC_FAIL_COOL;
                rt_kprintf("[TIME-SYNC] 同步失败，%d秒后重试\n", SYNC_FAIL_COOL / RT_TICK_PER_SECOND);
            }
        }

        /* ================================================================
         * 【天气获取】限制C404界面（原有逻辑，去掉了内嵌的时间同步代码）
         * ================================================================ */
        if (MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
        {
            if (get_city_flag)
            {
                if (++time_cnt > 60 * 60)
                {
                    start_set_lwip_flag = true;
                    get_city_flag = false;
                    time_cnt = 0;
                }
            }

            if (start_set_lwip_flag && get_lwip_flag &&
                (my_page == PAGE_HOME || my_page == PAGE_HOME_HOR))
            {
                rt_thread_mdelay(3000);
                time_cnt = 0;
                start_set_lwip_flag = false;

                struct netbuf *buf = NULL;
                err_t err;
                cJSON *root = NULL;
                rt_tick_t start_tick;

                memset(response_buffer, 0, MAX_RESPONSE_SIZE);
                int response_index = 0;
                rt_kprintf("\n\n--- 开始新的天气请求 --- \n");

                char http_request[2048] = {0};
                rt_snprintf(http_request, sizeof(http_request),
                            "GET /weather/get?areacode=%s HTTP/1.1\r\n"
                            "Host: iremote.isurpass.com.cn\r\n"
                            "token: W03c6KzHyzGf3SCf5WFGrltlYcg9+SPfbXQkpCNC/M4=\r\n"
                            "accept: */*\r\n"
                            "Connection: close\r\n"
                            "\r\n",
                            city_code[my_city_id.city_value + my_city_id.city_2][my_city_id.city_3]);
                printf("城市码:%s\n", city_code[my_city_id.city_value + my_city_id.city_2][my_city_id.city_3]);

                ip_addr_t server_ip;
                IP4_ADDR(&server_ip, SERVER_IP_1, SERVER_IP_2, SERVER_IP_3, SERVER_IP_4);
                const uint16_t server_port = 8010;

                do
                {
                    if (conn != NULL)
                    {
                        netconn_close(conn);
                        netconn_delete(conn);
                        conn = NULL;
                        rt_kprintf("清理残留TCP连接\n");
                    }

                    conn = netconn_new(NETCONN_TCP);
                    if (conn == NULL)
                    {
                        rt_kprintf("错误：创建TCP连接失败（内存不足）\n");
                        rt_thread_mdelay(RETRY_DELAY_MS);
                        break;
                    }

                    rt_kprintf("正在连接服务器...\n");
                    start_tick = rt_tick_get();
                    err = ERR_INPROGRESS;
                    while (err == ERR_INPROGRESS)
                    {
                        err = netconn_connect(conn, &server_ip, server_port);
                        if (rt_tick_get() - start_tick > rt_tick_from_millisecond(CONNECT_TIMEOUT_MS))
                        {
                            err = ERR_TIMEOUT;
                            break;
                        }
                        rt_thread_mdelay(10);
                    }
                    if (err != ERR_OK)
                    {
                        weather_erro_flag = true;
                        rt_kprintf("错误：连接服务器 %d.%d.%d.%d:%d 失败，错误码: %d\n",
                                   ip4_addr1(&server_ip), ip4_addr2(&server_ip),
                                   ip4_addr3(&server_ip), ip4_addr4(&server_ip), server_port, err);
                        rt_thread_mdelay(RETRY_DELAY_MS);
                        break;
                    }
                    weather_erro_flag = false;
                    rt_kprintf("成功连接到服务器！\n");

                    err = netconn_write(conn, http_request, strlen(http_request), NETCONN_COPY);
                    if (err != ERR_OK)
                    {
                        rt_kprintf("错误：发送请求失败，错误码: %d\n", err);
                        rt_thread_mdelay(RETRY_DELAY_MS);
                        break;
                    }

                    rt_kprintf("--- 开始接收响应 ---\n");
                    int content_length = -1;
                    bool headers_parsed = false;
                    bool recv_complete = false;
                    start_tick = rt_tick_get();

                    while (1)
                    {
                        if (rt_tick_get() - start_tick > rt_tick_from_millisecond(RECV_TIMEOUT_MS))
                        {
                            rt_kprintf("错误：接收响应超时\n");
                            err = ERR_TIMEOUT;
                            break;
                        }

                        err = netconn_recv(conn, &buf);
                        if (err == ERR_OK)
                        {
                            start_tick = rt_tick_get();
                            if (buf != NULL && buf->p != NULL)
                            {
                                struct pbuf *q;
                                for (q = buf->p; q != NULL; q = q->next)
                                {
                                    int copy_size = q->len;
                                    if (response_index + copy_size > MAX_RESPONSE_SIZE)
                                    {
                                        copy_size = MAX_RESPONSE_SIZE - response_index;
                                        rt_kprintf("警告：响应数据超出缓冲区最大容量 %d\n", MAX_RESPONSE_SIZE);
                                    }
                                    memcpy(response_buffer + response_index, (char *)q->payload, copy_size);
                                    response_index += copy_size;
                                }
                                netbuf_delete(buf);
                                buf = NULL;

                                if (!headers_parsed)
                                {
                                    response_buffer[response_index] = '\0';
                                    char *header_end = strstr(response_buffer, "\r\n\r\n");
                                    if (header_end != NULL)
                                    {
                                        headers_parsed = true;
                                        char *cl_str = strstr(response_buffer, "Content-Length:");
                                        if (cl_str != NULL)
                                        {
                                            cl_str += strlen("Content-Length:");
                                            while (*cl_str == ' ') cl_str++;
                                            sscanf(cl_str, "%d", &content_length);
                                            rt_kprintf("解析到Content-Length: %d字节\n", content_length);
                                        }
                                    }
                                }

                                if (content_length > 0 && response_index >= content_length + 4)
                                {
                                    rt_kprintf("已接收 %d字节，达到预期长度\n", response_index);
                                    recv_complete = true;
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                        else if (err == ERR_CLSD)
                        {
                            rt_kprintf("服务器主动关闭连接\n");
                            recv_complete = true;
                            break;
                        }
                        else if (err != ERR_WOULDBLOCK)
                        {
                            rt_kprintf("接收响应错误，错误码: %d\n", err);
                            break;
                        }
                        rt_thread_mdelay(10);
                    }

                    if (!recv_complete || err != ERR_OK && err != ERR_CLSD)
                    {
                        break;
                    }

                    response_buffer[response_index] = '\0';
#if DEBUG_PRINT_FULL_RESPONSE
                    printf("完整响应内容:\n%s\n", response_buffer);
#endif

                    /* ===== 7. 解析JSON响应体（时间同步已独立，这里只做天气） ===== */
                    char *header_separator = strstr(response_buffer, "\r\n\r\n");
                    if (header_separator == NULL)
                    {
                        rt_kprintf("解析失败：未找到HTTP头分隔符（\\r\\n\\r\\n）\n");
                        break;
                    }

                    char *response_body = header_separator + 4;
                    while (*response_body && (*response_body != '{' && *response_body != '['))
                    {
                        response_body++;
                    }
                    if (*response_body == '\0')
                    {
                        rt_kprintf("严重错误：处理后的JSON响应体为空\n");
                        break;
                    }
                    // rt_kprintf("待解析的JSON响应体:\n%s\n", response_body);

                    root = cJSON_Parse(response_body);
                    if (root == NULL)
                    {
                        rt_kprintf("JSON解析失败！\n");
                        const char *err_ptr = cJSON_GetErrorPtr();
                        if (err_ptr) rt_kprintf("错误位置: %s\n", err_ptr);
                        break;
                    }

                    cJSON *code_json = cJSON_GetObjectItem(root, "code");
                    if (code_json == NULL || !cJSON_IsNumber(code_json) || code_json->valueint != 200)
                    {
                        cJSON *msg_json = cJSON_GetObjectItem(root, "message");
                        rt_kprintf("接口请求失败！code=%d, message=%s\n",
                                   code_json ? code_json->valueint : -1,
                                   (msg_json && cJSON_IsString(msg_json)) ? msg_json->valuestring : "未知错误");
                        break;
                    }
                    rt_kprintf("请求成功！code=200\n");

                    cJSON *data_json = cJSON_GetObjectItem(root, "data");
                    if (data_json == NULL || !cJSON_IsObject(data_json))
                    {
                        rt_kprintf("解析失败：未找到 'data' 字段\n");
                        break;
                    }

                    cJSON *temp_json = cJSON_GetObjectItem(data_json, "temperature");
                    cJSON *weather_json = cJSON_GetObjectItem(data_json, "weathercode");

                    if (temp_json && cJSON_IsString(temp_json))
                    {
                        my_weather.temperature = atoi(temp_json->valuestring);
                        rt_kprintf("温度: %s℃\n", temp_json->valuestring);
                    }
                    else
                    {
                        rt_kprintf("警告：未获取到有效温度数据\n");
                    }

                    if (weather_json && cJSON_IsString(weather_json))
                    {
                        int weather_code = atoi(weather_json->valuestring);
                        rt_kprintf("天气代码: %d\n", weather_code);

                        const char *weather_str[] = {
                            "晴", "多云", "阴", "阵雨", "雷阵雨", "雷阵雨伴有冰雹",
                            "雨夹雪", "小雨", "中雨", "大雨", "暴雨", "大暴雨", "特大暴雨",
                            "阵雪", "小雪", "中雪", "大雪", "暴雪", "雾", "冻雨", "沙尘暴",
                            NULL, NULL, NULL, "暴雨到大暴雨", "大暴雨到特大暴雨", "小到中雪",
                            "中到大雪", "大到暴雪", "浮尘", "扬沙", "强沙尘暴"
                        };

                        if (weather_code >= 0 && weather_code < sizeof(weather_str)/sizeof(weather_str[0]) && weather_str[weather_code])
                        {
                            my_weather.weather = weather_code;
                            rt_kprintf("天气: %s\n", weather_str[weather_code]);
                        }
                        else if (weather_code == 53)
                        {
                            my_weather.weather = weather_code;
                            rt_kprintf("天气: 霾\n");
                        }
                        else if (weather_code == 301)
                        {
                            my_weather.weather = weather_code;
                            rt_kprintf("天气: 雨\n");
                        }
                        else if (weather_code == 302)
                        {
                            my_weather.weather = weather_code;
                            rt_kprintf("天气: 雪\n");
                        }
                        else
                        {
                            rt_kprintf("天气: 未知(%d)\n", weather_code);
                        }
                    }
                    else
                    {
                        rt_kprintf("警告：未获取到有效天气代码\n");
                    }

                    change_weather_flag = true;
                    geted_weather_flag = true;
                    get_city_flag = true;
                    save_begin();
                    rt_kprintf("--- 天气数据解析完成 ---\n");

                } while (0);

                if (buf != NULL)  { netbuf_delete(buf);  buf = NULL; }
                if (root != NULL) { cJSON_Delete(root);   root = NULL; }
                if (conn != NULL)
                {
                    netconn_close(conn);
                    netconn_delete(conn);
                    conn = NULL;
                    rt_kprintf("TCP连接已强制关闭并释放\n");
                }
            }
        }

        rt_thread_mdelay(500);
    }
}
