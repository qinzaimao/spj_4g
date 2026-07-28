#include "my_rtc.h"

uint8_t get_day_cnt(uint16_t year, uint8_t month)
{
    if (month == 1 || month == 3 || month == 5 || month == 7 ||
        month == 8 || month == 10 || month == 12)
        return 31;
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    else if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return 29;
        return 28;
    }
    else
        return 0;
}

/* 线程函数 */
void rtc_thread_entry(void *parameter)
{
    time_t now;
    rt_err_t ret = RT_EOK;
    struct tm *local_time;
    static char my_str[30] = {0};
    static time_t energy_conservation_start = 0; // 记录开始计时的RTC时间（秒）
    static time_t last_checked_time = 0;         // 上一次检查的时间
    rt_thread_mdelay(100);

    // 先获取当前RTC时间
    now = time(RT_NULL);
    local_time = localtime(&now);
    // 检查当前年份
    int current_year = local_time->tm_year + 1900;
    rt_kprintf("Current RTC year: %d\n", current_year);
    if (current_year < 2025)
    {
        ret = set_date(2025, 6, 19);
        if (ret != RT_EOK)
            rt_kprintf("set RTC date failed");
        // delay 1 ms for rtc sync
        rt_thread_mdelay(1);
        // set time with local timezone
        ret = set_time(9, 14, 33);
        if (ret != RT_EOK)
            rt_kprintf("set RTC time failed");
    }
    while (1)
    {
        /* 读取 RTC 时间*/
        now = time(RT_NULL);
        local_time = localtime(&now);
        if (reset_time_flag)
        {
            if (local_time->tm_hour == day_reset_time && local_time->tm_min == 0 && local_time->tm_sec <= 1)
            {
                wdt_immediate_reset();
            }
            if(day_reset_time == 24)
            {
                if(local_time->tm_hour == 0 && local_time->tm_min == 0 && local_time->tm_sec <= 1)
                {
                    wdt_immediate_reset();
                }
            }
        }
        if (MY_VOICE_SWITCH.voice)
        {
            if (work_start_time != work_over_time)
            {
                // 工作时间判断
                // 先提取当前小时，简化代码可读性
                int current_hour = local_time->tm_hour;

                // 正确的工作时间判断逻辑（覆盖跨天/不跨天）
                if (
                    // 情况1：正常时段（开始 < 结束，如 9-18点）
                    (work_start_time < work_over_time && current_hour >= work_start_time && current_hour < work_over_time)
                    ||
                    // 情况2：跨天时段（开始 > 结束，如 22-6点）
                    (work_start_time > work_over_time && (current_hour >= work_start_time || current_hour < work_over_time))
                ) {
                    // rt_kprintf("work time: %d-%d\n", work_start_time, work_over_time);
                    if (!voice_start_work)
                        voice_start_work = true;
                }
                else
                {
                    if (voice_start_work)
                    {
                        rt_kprintf("语音不在工作时间内\n");
                        music_renew_flag = true;
                        voice_start_work = false;
                    }
                }
            }
            else if (voice_start_work)
            {
                music_renew_flag = true;
                voice_start_work = false;
            }
        }
        else if (voice_start_work) // 总开关关闭，如果之前工作中，则停止工作
        {
            music_renew_flag = true;
            voice_start_work = false;
        }
        // 节能模式判断（抗时间修改版本）
        if ((!MY_VOICE_SWITCH.lr) && !energy_conservation)
        {
            // 计算与上次检查的时间差（处理时间可能被修改的情况）
            int32_t time_diff = now - last_checked_time;
            // 如果时间差为正且合理（小于10秒，避免时间大幅跳变的影响）
            if (time_diff > 0 && time_diff < 10)
            {
                accumulated_seconds += time_diff;
                rt_kprintf("累计时间：%d秒\n", accumulated_seconds);
            }
            // 如果检测到时间被回退或大幅修改，重置累计时间
            else if (time_diff <= 0)
            {
                accumulated_seconds = 0;
            }
            // 累计时间达到15分钟，进入节能模式
            if (accumulated_seconds >= (60 * 15))
            {

                in_arr_flag = false;
                memset(&my_cnt, 0, sizeof(my_cnt));
                accumulated_seconds = 0;
                rt_kprintf("15分钟没有信号 进入节能模式\n");
                if (!energy_show_image_flag)
                    energy_show_image_flag = true;
                if (is_tcp_connected && MY_SET_DHCP.host_state == false)
                {

                }else{
                    backlight_set(MY_SET.e_con_backlight);
                }
                if (is_tcp_connected && MY_SET_DHCP.host_state == true)
                {
                    send_light(true, MY_SET.e_con_backlight);
                }

                music_renew_flag = true;
                energy_conservation = true;
            }
        }
        else if (elevator_change_no_lr_flag) // 有信号时，累计时间清零
        {
            if (energy_conservation) // 退出节能模式
            {
                break_energy_flag = true;
                if (is_tcp_connected && MY_SET_DHCP.host_state == false)
                {

                }else{
                    backlight_set(MY_SET.backlight);
                }
                if (is_tcp_connected && MY_SET_DHCP.host_state == true)
                {
                    send_light(false, MY_SET.backlight);
                }
                energy_conservation = false;
                if (energy_show_image_flag)
                    energy_show_image_flag = false;
                rt_kprintf("退出节能模式，计数清零\n");
            }

            elevator_change_no_lr_flag = false;
            accumulated_seconds = 0;
        }
        else if (elevator_change_flag && energy_conservation)
        {
            break_energy_flag = true;
            elevator_change_flag = false;
            accumulated_seconds = 0;
            if (is_tcp_connected && MY_SET_DHCP.host_state == false)
            {

            }else{
                backlight_set(MY_SET.backlight);
            }
            if (is_tcp_connected && MY_SET_DHCP.host_state == true)
            {
                send_light(false, MY_SET.backlight);
            }
            if (energy_show_image_flag)
                energy_show_image_flag = false;
            rt_kprintf("退出节能模式\n");
            rt_thread_mdelay(500);
            energy_conservation = false; // 有信号时，退出节能模式
        }
        else
        {
            // 不满足条件时重置累计时间
            accumulated_seconds = 0;
        }

        // 更新上一次检查时间
        last_checked_time = now;

        rt_thread_mdelay(1000);
    }
}
