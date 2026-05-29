#ifndef __BACKLIGHT_H__
#define __BACKLIGHT_H__

#include "main.h"

void BLEN_Init(void);
void backlight_set(uint8_t level);
void backlight_OFF();
void EMMC_Init(uint8_t state);
#endif /* __BACKLIGHT_H__ */
