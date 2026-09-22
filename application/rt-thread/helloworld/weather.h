#ifndef __WEATHER_H_
#define __WEATHER_H_

#include "main.h"

// 替换为你查询到的服务器 IP（示例：113.105.186.XX）
#define SERVER_IP_1 120
#define SERVER_IP_2 24
#define SERVER_IP_3 52
#define SERVER_IP_4 86
// 定义一个宏来控制是否打印完整的响应内容，方便调试
#define DEBUG_PRINT_FULL_RESPONSE 0

#define MAX_RESPONSE_SIZE 1024 * 8 // 增大缓冲区，确保能容纳完整响应




void weather_thread_entry(void *parameter);


#endif
