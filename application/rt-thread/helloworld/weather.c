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

void lwip_thread_entry(void *parameter)
{
    static uint8_t time_cnt = 0;
    static bool get_city_flag = false;
    struct netconn *conn = NULL;  // 提升作用域，便于全路径释放

    while (1)
    {
        if (MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
        {
            // 定时重试逻辑（1小时后重试）
            if (get_city_flag)
            {
                if (++time_cnt > 60 * 60)
                {
                    start_set_lwip_flag = true;
                    get_city_flag = false;
                    time_cnt = 0;  // 重置计数器，避免溢出
                }
            }

            // 满足条件时执行天气请求
            if (start_set_lwip_flag && get_lwip_flag &&
                (my_page == PAGE_HOME || my_page == PAGE_HOME_HOR))
            {
                rt_thread_mdelay(3000);  // 延迟1秒，等待界面切换完成
                time_cnt = 0;
                start_set_lwip_flag = false;
                struct netbuf *buf = NULL;
                err_t err;
                cJSON *root = NULL;  // JSON根节点，统一释放
                rt_tick_t start_tick;  // 用于超时计时

                // 清空接收缓冲区
                memset(response_buffer, 0, MAX_RESPONSE_SIZE);
                int response_index = 0;

                rt_kprintf("\n\n--- 开始新的天气请求 --- \n");

                // 1. 构造HTTP请求
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

                // 2. 设置服务器IP和端口
                ip_addr_t server_ip;
                IP4_ADDR(&server_ip, SERVER_IP_1, SERVER_IP_2, SERVER_IP_3, SERVER_IP_4);
                const uint16_t server_port = 8010;

                // 资源保护域：确保任何分支都能释放资源
                do
                {
                    // 清理残留连接（防止之前未释放的连接占用资源）
                    if (conn != NULL)
                    {
                        netconn_close(conn);
                        netconn_delete(conn);
                        conn = NULL;
                        rt_kprintf("清理残留TCP连接\n");
                    }

                    // 3. 创建TCP连接
                    conn = netconn_new(NETCONN_TCP);
                    if (conn == NULL)
                    {
                        rt_kprintf("错误：创建TCP连接失败（内存不足）\n");
                        rt_thread_mdelay(RETRY_DELAY_MS);
                        break;
                    }

                    // 4. 连接服务器（带超时检测，替代netconn_set_connecttimeout）
                    rt_kprintf("正在连接服务器...\n");
                    start_tick = rt_tick_get();  // 记录开始时间
                    err = ERR_INPROGRESS;

                    // 非阻塞连接：循环检查连接结果，超时则主动断开
                    while (err == ERR_INPROGRESS)
                    {
                        err = netconn_connect(conn, &server_ip, server_port);

                        // 检查是否超时
                        if (rt_tick_get() - start_tick > rt_tick_from_millisecond(CONNECT_TIMEOUT_MS))
                        {
                            err = ERR_TIMEOUT;
                            break;
                        }
                        rt_thread_mdelay(10);  // 避免CPU空转
                    }

                    // 处理连接结果
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

                    // 5. 发送HTTP请求
                    err = netconn_write(conn, http_request, strlen(http_request), NETCONN_COPY);
                    if (err != ERR_OK)
                    {
                        rt_kprintf("错误：发送请求失败，错误码: %d\n", err);
                        rt_thread_mdelay(RETRY_DELAY_MS);
                        break;
                    }

                    // 6. 接收服务器响应（带超时检测）
                    rt_kprintf("--- 开始接收响应 ---\n");
                    int content_length = -1;
                    bool headers_parsed = false;
                    bool recv_complete = false;
                    start_tick = rt_tick_get();  // 重置超时计时

                    while (1)
                    {
                        // 检查接收是否超时
                        if (rt_tick_get() - start_tick > rt_tick_from_millisecond(RECV_TIMEOUT_MS))
                        {
                            rt_kprintf("错误：接收响应超时\n");
                            err = ERR_TIMEOUT;
                            break;
                        }

                        // 非阻塞接收：避免长期阻塞
                        err = netconn_recv(conn, &buf);
                        if (err == ERR_OK)
                        {
                            start_tick = rt_tick_get();  // 接收成功，重置超时计时

                            if (buf != NULL && buf->p != NULL)
                            {
                                struct pbuf *q;
                                for (q = buf->p; q != NULL; q = q->next)
                                {
                                    int copy_size = q->len;
                                    // 防止缓冲区溢出
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

                                // 解析响应头，获取Content-Length
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

                                // 检查是否接收完毕（根据Content-Length判断）
                                if (content_length > 0 && response_index >= content_length + 4)  // +4对应"\r\n\r\n"
                                {
                                    rt_kprintf("已接收 %d字节，达到预期长度\n", response_index);
                                    recv_complete = true;
                                    break;
                                }
                            }
                            else
                            {
                                break;  // buf为空，接收结束
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
                            // 非“资源暂时不可用”的错误，直接退出
                            rt_kprintf("接收响应错误，错误码: %d\n", err);
                            break;
                        }

                        rt_thread_mdelay(10);  // 避免CPU占用过高
                    }

                    // 接收失败或未完成，退出流程
                    if (!recv_complete || err != ERR_OK && err != ERR_CLSD)
                    {
                        break;
                    }

                    // 添加字符串结束符，便于后续解析
                    response_buffer[response_index] = '\0';
#if DEBUG_PRINT_FULL_RESPONSE
                    printf("完整响应内容:\n%s\n", response_buffer);
#endif

                    // 7. 解析JSON响应体
                    char *header_separator = strstr(response_buffer, "\r\n\r\n");
                    if (header_separator == NULL)
                    {
                        rt_kprintf("解析失败：未找到HTTP头分隔符（\\r\\n\\r\\n）\n");
                        break;
                    }

                    // 跳过HTTP头，定位到JSON响应体
                    char *response_body = header_separator + 4;
                    // 清除响应体前的非法字符（如空格、换行）
                    while (*response_body && (*response_body != '{' && *response_body != '['))
                    {
                        response_body++;
                    }

                    // 检查响应体是否为空
                    if (*response_body == '\0')
                    {
                        rt_kprintf("严重错误：处理后的JSON响应体为空\n");
                        break;
                    }

                    rt_kprintf("待解析的JSON响应体:\n%s\n", response_body);

                    // 解析JSON
                    root = cJSON_Parse(response_body);
                    if (root == NULL)
                    {
                        rt_kprintf("JSON解析失败！\n");
                        const char *err_ptr = cJSON_GetErrorPtr();
                        if (err_ptr) rt_kprintf("错误位置: %s\n", err_ptr);
                        break;
                    }

                    // 检查接口响应码（必须为200）
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

                    // 解析data字段（核心天气数据）
                    cJSON *data_json = cJSON_GetObjectItem(root, "data");
                    if (data_json == NULL || !cJSON_IsObject(data_json))
                    {
                        rt_kprintf("解析失败：未找到 'data' 字段\n");
                        break;
                    }

                    // 提取温度和天气代码
                    cJSON *temp_json = cJSON_GetObjectItem(data_json, "temperature");
                    cJSON *weather_json = cJSON_GetObjectItem(data_json, "weathercode");

                    // 处理温度数据
                    if (temp_json && cJSON_IsString(temp_json))
                    {
                        my_weather.temperature = atoi(temp_json->valuestring);
                        rt_kprintf("温度: %s℃\n", temp_json->valuestring);
                    }
                    else
                    {
                        rt_kprintf("警告：未获取到有效温度数据\n");
                    }

                    // 处理天气代码（映射为中文天气）
                    if (weather_json && cJSON_IsString(weather_json))
                    {
                        int weather_code = atoi(weather_json->valuestring);
                        rt_kprintf("天气代码: %d\n", weather_code);

                        // 天气代码映射表（需与接口文档一致）
                        const char *weather_str[] = {
                            "晴", "多云", "阴", "阵雨", "雷阵雨", "雷阵雨伴有冰雹",
                            "雨夹雪", "小雨", "中雨", "大雨", "暴雨", "大暴雨", "特大暴雨",
                            "阵雪", "小雪", "中雪", "大雪", "暴雪", "雾", "冻雨", "沙尘暴",
                            NULL, NULL, NULL, "暴雨到大暴雨", "大暴雨到特大暴雨", "小到中雪",
                            "中到大雪", "大到暴雪", "浮尘", "扬沙", "强沙尘暴"
                        };

                        // 匹配天气代码
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

                    // 标记天气更新并保存数据
                    change_weather_flag = true;
                    geted_weather_flag = true;
                    get_city_flag = true;
                    save_begin();
                    rt_kprintf("--- 天气数据解析完成 ---\n");

                } while (0);  // 资源保护域结束

                // 强制释放所有资源（无论正常/异常，确保不泄漏）
                if (buf != NULL)
                {
                    netbuf_delete(buf);
                    buf = NULL;
                }
                if (root != NULL)
                {
                    cJSON_Delete(root);
                    root = NULL;
                }
                if (conn != NULL)
                {
                    netconn_close(conn);    // 强制关闭TCP连接
                    netconn_delete(conn);   // 释放连接内存
                    conn = NULL;
                    rt_kprintf("TCP连接已强制关闭并释放\n");
                }
            }
        }
        rt_thread_mdelay(500);  // 线程主循环延迟，降低CPU占用
    }
}
