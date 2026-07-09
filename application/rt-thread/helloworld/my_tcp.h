
#ifndef __MY_TCP_H__
#define __MY_TCP_H__

#include "main.h"



void tcp_info_thread_entry(void *parameter);
err_t tcp_send_raw(uint8_t *buf, uint16_t len);

extern rt_tick_t last_recv_tick;

#endif
