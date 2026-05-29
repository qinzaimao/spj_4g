#include "backlight.h"

uint32_t pin_blen = 0;
uint32_t pin_emmc = 0;

void BLEN_Init(void)
{
    pin_blen = rt_pin_get("PE.18");
    rt_pin_mode(pin_blen, PIN_MODE_OUTPUT);
    rt_pin_write(pin_blen, PIN_HIGH);

    backlight_set(MY_SET.backlight);
}

void EMMC_Init(uint8_t state)
{
    pin_emmc = rt_pin_get("PD.6");
    rt_pin_mode(pin_emmc, PIN_MODE_OUTPUT);
    rt_pin_write(pin_emmc, state == 0 ? PIN_LOW : PIN_HIGH);
}


void backlight_set(uint8_t level)
{
#if defined(KERNEL_RTTHREAD) && defined(AIC_PWM_BACKLIGHT_CHANNEL)
    struct rt_device_pwm *pwm_dev;

    if(level >= 95) level = 90;
    if(level <= 5) level = 5;

    pwm_dev = (struct rt_device_pwm *)rt_device_find("pwm");
    if(pwm_dev == NULL)
    {
        rt_kprintf("backlight_set: find pwm device failed!\n");
        return; // 设备不存在，直接返回，避免空指针
    }
    /* pwm frequency: 1KHz = 1000000ns */
    /* pwm frequency: 10KHz = 100000ns */
    /* pwm frequency: 50KHz = 20000ns */
    /* pwm frequency: 100KHz = 10000ns */
    rt_uint32_t period_ns = 20000; // 50KHz
    rt_uint32_t pulse_ns = (period_ns * level) / 100; // 直接按百分比计算占空比（更直观）

    if(rt_pwm_set(pwm_dev, AIC_PWM_BACKLIGHT_CHANNEL, period_ns, pulse_ns) != RT_EOK)
    {
        rt_kprintf("backlight_set: set pwm failed!\n");
        return;
    }
    rt_pwm_enable(pwm_dev, AIC_PWM_BACKLIGHT_CHANNEL);
#endif
}

void backlight_OFF()
{
#if defined(KERNEL_RTTHREAD) && defined(AIC_PWM_BACKLIGHT_CHANNEL)
    struct rt_device_pwm *pwm_dev;

    pwm_dev = (struct rt_device_pwm *)rt_device_find("pwm");
    /* pwm frequency: 1KHz = 1000000ns */
    rt_pwm_set(pwm_dev, AIC_PWM_BACKLIGHT_CHANNEL,
               20000, 10000 * 0);
    rt_pwm_enable(pwm_dev, AIC_PWM_BACKLIGHT_CHANNEL);
#endif
}
