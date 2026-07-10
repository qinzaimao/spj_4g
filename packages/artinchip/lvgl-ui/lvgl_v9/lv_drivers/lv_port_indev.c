/*
 * Copyright (c) 2024, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors:  Ning Fang <ning.fang@artinchip.com>
 */

#include <rtconfig.h>
#ifdef KERNEL_RTTHREAD
#include <lvgl.h>
#include <stdbool.h>
#include <rtthread.h>
#include <../components/drivers/include/drivers/touch.h>
#include "../../../../../../application/rt-thread/helloworld/main.h"

#define LV_USE_BUTTON_ADC_COMMON_INDEV 0
#define LV_USE_BUTTON_ADC_SIMULATOR_ENCODER 0
#if LV_USE_BUTTON_ADC_INDEV || LV_USE_BUTTON_ADC_SIMULATOR_ENCODER
#define LV_USE_BUTTON_ADC_INDEV 1
#endif

static lv_indev_state_t last_state = LV_INDEV_STATE_REL;
static rt_int16_t last_x = 0;
static rt_int16_t last_y = 0;

static void input_read(lv_indev_t *indev_drv, lv_indev_data_t *data)
{
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = last_state;
}

// void mouse_handle_wheel(int16_t delta)
// {
//     if (wheel_mutex)
//     {
//         rt_mutex_take(wheel_mutex, RT_WAITING_FOREVER);
//         wheel_diff += delta;  // 累积滚轮变化量
//         rt_mutex_release(wheel_mutex);
//     }
// }
extern rt_thread_t video_thread;
static void mouse_read(lv_indev_t *indev_drv, lv_indev_data_t *data)
{
    /* 获取当前鼠标坐标 */

    mouse_get_xy(&data->point.x, &data->point.y);

    if (mouse_btn_left)
    {
        /* 获取鼠标按键状态 */
        // rt_kprintf("left button pressed\n");

        data->state = LV_INDEV_STATE_PR;
        if (my_page == PAGE_HOME)
        {
            if (data->point.x > 924 && data->point.y <= 100 && init_set_img_ok)
            {
                mouse_btn_left = false;
                if (MY_SET.play_mode == PLAY_VIDEO)
                {
                    if(home_delete_flag)
                    {
                        home_delete_flag = false;
                        lv_timer_pause(frist_timer);
                        lv_timer_pause(update_timer);
                        lv_timer_pause(home_move_timer);
                        lv_timer_pause(home_set_time_timer);
                        lv_timer_pause(home_set_weather_timer);
                        lv_timer_pause(home_refresh_picture_timer);
                        setup_scr_screen_set(&guider_ui);
                        lv_scr_load_anim(guider_ui.screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                    }
                    else if(my_lvgl_player_ctx.player != NULL)
                    {
                       home_delete_flag = true;
                       // 重启操作
                    }else{
                        lv_timer_pause(frist_timer);
                        lv_timer_pause(update_timer);
                        lv_timer_pause(home_move_timer);
                        lv_timer_pause(home_set_time_timer);
                        lv_timer_pause(home_set_weather_timer);
                        lv_timer_pause(home_refresh_picture_timer);
                        setup_scr_screen_set(&guider_ui);
                        lv_scr_load_anim(guider_ui.screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                    }
                }
                else
                {
                    lv_timer_pause(frist_timer);
                    lv_timer_pause(update_timer);
                    lv_timer_pause(home_move_timer);
                    lv_timer_pause(home_set_time_timer);
                    lv_timer_pause(home_set_weather_timer);
                    lv_timer_pause(home_refresh_picture_timer);
                    setup_scr_screen_set(&guider_ui);
                    lv_scr_load_anim(guider_ui.screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                }
            }
            // printf("x:%d, y:%d\n", data->point.x, data->point.y);
        }
        else if (my_page == PAGE_HOME_HOR)
        {
            // rt_kprintf("x:%d, y:%d\n", data->point.x, data->point.y);
            if (data->point.x < 100 && data->point.y < 100 && init_set_img_ok)
            {
                mouse_btn_left = false;
                if (MY_SET.play_mode == PLAY_VIDEO)
                {
                    if(home_hor_delete_flag)
                    {
                        home_hor_delete_flag = false;
                        lv_timer_pause(frist_hor_timer);
                        lv_timer_pause(update_hor_timer);
                        lv_timer_pause(home_hor_move_timer);
                        lv_timer_pause(home_hor_set_time_timer);
                        lv_timer_pause(home_hor_set_weather_timer);
                        lv_timer_pause(home_hor_refresh_picture_timer);
                        setup_scr_screen_set_hor(&guider_ui);
                        lv_scr_load_anim(guider_ui.screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                    }
                    else if(my_lvgl_player_ctx.player != NULL)
                    {
                        home_hor_delete_flag = true;

                    }else{
                        lv_timer_pause(frist_hor_timer);
                        lv_timer_pause(update_hor_timer);
                        lv_timer_pause(home_hor_move_timer);
                        lv_timer_pause(home_hor_set_time_timer);
                        lv_timer_pause(home_hor_set_weather_timer);
                        lv_timer_pause(home_hor_refresh_picture_timer);
                        setup_scr_screen_set_hor(&guider_ui);
                        lv_scr_load_anim(guider_ui.screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                    }
                }
                else
                {
                    lv_timer_pause(frist_hor_timer);
                    lv_timer_pause(update_hor_timer);
                    lv_timer_pause(home_hor_move_timer);
                    lv_timer_pause(home_hor_set_time_timer);
                    lv_timer_pause(home_hor_set_weather_timer);
                    lv_timer_pause(home_hor_refresh_picture_timer);
                    setup_scr_screen_set_hor(&guider_ui);
                    lv_scr_load_anim(guider_ui.screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                }
            }
        }
    }
    else if (mouse_btn_right)
    {
        mouse_btn_right = false;
        data->state = LV_INDEV_STATE_REL; // <--- 右键只触发切换，不触发按下
        last_state = LV_INDEV_STATE_REL;  // <--- 强制清除触摸/鼠标按下状态

        if ((my_page == PAGE_HOME || my_page == PAGE_HOME_HOR) && MY_SET.play_mode == PLAY_VIDEO)
        {
            if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
            {
                tcp_send_raw("vdes", 4);
                last_recv_tick = rt_tick_get();
                rt_kprintf("vdes\n");
            }
            rt_kprintf("video_renew = PRINTF_RENEW;\n");
            video_renew = PRINTF_RENEW;
        }
        if (my_page == PAGE_SET)
        {
            if(set_volume_flag)
            {
                set_volume_flag = false;
                if (music_audio_player != NULL)
                {
                    mini_audio_player_stop(music_audio_player);
                    int ret = mini_audio_player_destroy(music_audio_player);
                    if (ret == 0)
                    {
                        rt_kprintf("音乐播放器销毁成功\n");
                        music_audio_player = NULL;
                    }
                    else
                    {
                        rt_kprintf("音乐播放器销毁失败\n");
                    }
                }
                else
                {
                    music_des_flag = true;
                    rt_kprintf("音乐播放器 is NULL\n");
                }
            }
            lv_timer_pause(set_time_timer);
            lv_timer_pause(get_time_timer);
            setup_scr_screen_home(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_home, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
            if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
            {
                tcp_send_raw("vdes", 4);
                last_recv_tick = rt_tick_get();
                rt_kprintf("vdes\n");
            }
            if(MY_SET.play_mode == PLAY_VIDEO)
            {
                rt_kprintf("video_renew = PRINTF_RENEW;\n");
                video_renew = PRINTF_RENEW;
            }
        }
        else if (my_page == PAGE_SET_HOR)
        {
            if(set_volume_flag)
            {
                set_volume_flag = false;
                if (music_audio_player != NULL)
                {
                    mini_audio_player_stop(music_audio_player);
                    int ret = mini_audio_player_destroy(music_audio_player);
                    if (ret == 0)
                    {
                        rt_kprintf("音乐播放器销毁成功\n");
                        music_audio_player = NULL;
                    }
                    else
                    {
                        rt_kprintf("音乐播放器销毁失败\n");
                    }
                }
                else
                {
                    music_des_flag = true;
                    rt_kprintf("音乐播放器 is NULL\n");
                }
            }
            lv_timer_pause(hor_set_time_timer);
            lv_timer_pause(hor_get_time_timer);
            setup_scr_screen_home_hor(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_home_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
            if(MY_SET.play_mode == PLAY_VIDEO)
            {
                rt_kprintf("video_renew = PRINTF_RENEW;\n");
                video_renew = PRINTF_RENEW;
            }
            if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
            {
                tcp_send_raw("vdes", 4);
                last_recv_tick = rt_tick_get();
                rt_kprintf("vdes\n");
            }
        }
        else if (my_page == PAGE_VOICE)
        {
            lv_timer_pause(voice_password_timer);
            setup_scr_screen_set(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        }
        else if (my_page == PAGE_VOICE_HOR)
        {
            lv_timer_pause(voice_hor_password_timer);
            setup_scr_screen_set_hor(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        }
        else if (my_page == PAGE_IO)
        {
            lv_timer_pause(get_state_timer);
            setup_scr_screen_set(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        }
        else if (my_page == PAGE_IO_HOR)
        {
            lv_timer_pause(get_state_hor_timer);
            setup_scr_screen_set_hor(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        }else if (my_page == PAGE_CITY)
        {
            setup_scr_screen_set(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        }
        else if (my_page == PAGE_CITY_HOR)
        {
            setup_scr_screen_set_hor(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        }
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
    /* 处理滚轮事件 */
    rt_mutex_take(wheel_mutex, RT_WAITING_FOREVER);
    data->enc_diff = wheel_diff; // 将滚轮差异值传递给LVGL
    wheel_diff = 0;              // 重置差异值
    rt_mutex_release(wheel_mutex);
}

/* 获取鼠标坐标函数 */
void mouse_get_xy(int32_t *x, int32_t *y)
{
    rt_mutex_take(mouse_x_y_mutex, RT_WAITING_FOREVER);
    // if (my_page == PAGE_HOME)
    // {
    //     *x = 10;
    //     *y = 10;
    // }
    // else
    // {
    if (hengping == 1)
    {
        *x = mouse_current_x;
        *y = mouse_current_y;
    }
    else
    {
        *x = 1024 - mouse_current_y;
        *y = mouse_current_x;
    }
    // }
    rt_mutex_release(mouse_x_y_mutex);
}

void aic_touch_inputevent_cb(rt_int16_t x, rt_int16_t y, rt_uint8_t state)
{
#ifdef AIC_BT_BT8858A
    extern int bt_hid_set_touch_event(int, unsigned short, unsigned short);
    bt_hid_set_touch_event(state, y, x);
#endif
    switch (state)
    {
    case RT_TOUCH_EVENT_UP:
        last_state = LV_INDEV_STATE_RELEASED;
        break;
    case RT_TOUCH_EVENT_MOVE:
    case RT_TOUCH_EVENT_DOWN:
        last_x = x;
        last_y = y;
        last_state = LV_INDEV_STATE_PRESSED;
        break;
#ifdef AIC_MONKEY_TEST
    case RT_TOUCH_MONKEY_TEST:
        last_x = x;
        last_y = y;
        last_state = LV_INDEV_STATE_PRESSED;
        break;
#endif
    }
}
lv_obj_t *cursor_img = NULL;
void lv_port_indev_init(void)
{
    static lv_indev_t *indev_drv;
    indev_drv = lv_indev_create();
    lv_indev_set_type(indev_drv, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_drv, mouse_read);

    cursor_img = lv_img_create(lv_scr_act());
    lv_obj_set_pos(cursor_img, 768, 1024);
    lv_indev_set_cursor(indev_drv, cursor_img);
}
#endif
