/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "../../../../../../application/rt-thread/helloworld/main.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

static void screen_set_btn_return_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
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
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_set_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
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
        break;
    }
    default:
        break;
    }
}

void events_init_screen_set (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_set_btn_return, screen_set_btn_return_event_handler, LV_EVENT_ALL, ui);
}

static void screen_voice_btn_return_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_timer_pause(voice_password_timer);
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_set, guider_ui.screen_set_del, &guider_ui.screen_voice_del, setup_scr_screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}


static void screen_IO_btn_return_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_timer_pause(get_state_timer);
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_set, guider_ui.screen_set_del, &guider_ui.screen_IO_del, setup_scr_screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}
static void screen_set_hor_btn_retur_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
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
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_home_hor, guider_ui.screen_home_hor_del, &guider_ui.screen_set_hor_del, setup_scr_screen_home_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
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
        break;
    }
    default:
        break;
    }
}

static void screen_voice_hor_btn_return_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_timer_pause(voice_hor_password_timer);
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_set_hor, guider_ui.screen_set_hor_del, &guider_ui.screen_voice_hor_del, setup_scr_screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_IO_hor_btn_return_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_timer_pause(get_state_hor_timer);
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_set_hor, guider_ui.screen_set_hor_del, &guider_ui.screen_IO_hor_del, setup_scr_screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}


void events_init_screen_set_hor (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_set_hor_btn_retur, screen_set_hor_btn_retur_event_handler, LV_EVENT_ALL, ui);
}

void events_init_screen_voice_hor (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_voice_hor_btn_return, screen_voice_hor_btn_return_event_handler, LV_EVENT_ALL, ui);
}

void events_init_screen_voice (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_voice_btn_return, screen_voice_btn_return_event_handler, LV_EVENT_ALL, ui);
}

void events_init_screen_IO (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_IO_btn_return, screen_IO_btn_return_event_handler, LV_EVENT_ALL, ui);
}

void events_init_screen_IO_hor (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_IO_hor_btn_return, screen_IO_hor_btn_return_event_handler, LV_EVENT_ALL, ui);
}
void events_init(lv_ui *ui)
{

}
