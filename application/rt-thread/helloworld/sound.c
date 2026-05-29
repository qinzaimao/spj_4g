#include "sound.h"

uint32_t pin_sound = 0;

//只控制视频的声音，不控制系统声音
void Sound_Init(uint8_t level)
{
    pin_sound = rt_pin_get("PA.7");
    rt_pin_mode(pin_sound, PIN_MODE_OUTPUT);
    rt_pin_write(pin_sound, level);
}
