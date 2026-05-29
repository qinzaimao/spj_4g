#ifndef __MY_RTC_H__
#define __MY_RTC_H__

#include "main.h"

uint8_t get_day_cnt(uint16_t year, uint8_t month);

void rtc_thread_entry(void *parameter);




#endif /* __MY_RTC_H__ */
