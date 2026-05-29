/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"
#include "../../../../../../application/rt-thread/helloworld/main.h"

static volatile bool set_password_connect = false;
static volatile uint8_t set_password_cnt = 0;
lv_timer_t *voice_password_timer = NULL;


static void set_password_callback(lv_timer_t *timer)
{
    static uint8_t time_cnt = 0;
    if(my_page == PAGE_VOICE)
    {
        if(set_password_connect)
        {
            if(++ set_password_cnt > 3)
            {
                set_password_connect = false;
                set_password_cnt = 0;
                if(MY_SET.language == LANGUAGE_CN)
                {
                    lv_obj_set_style_text_font(guider_ui.screen_voice_label_key, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(guider_ui.screen_voice_label_key, "输入当前密码"); // 完成此函数调用
                }
                else
                {
                    lv_obj_set_style_text_font(guider_ui.screen_voice_label_key, &lv_font_Dengb_36, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(guider_ui.screen_voice_label_key, "enter current password"); // 完成此函数调用
                }
            }
        }
    }
}

static void key_return_handle(void)
{
    lv_textarea_set_text(guider_ui.screen_voice_ta_key, "");
    lv_obj_set_x(guider_ui.screen_voice_tileview_key, -768);
}

static void key_ok_handle(void)
{
    static char password_temp[4] = {0};
    // lv_obj_set_x(guider_ui.screen_voice_tileview_key, -768);
    const char *get_str = lv_textarea_get_text(guider_ui.screen_voice_ta_key);
    rt_kprintf("get_str:%s\n", get_str);
    if(MY_SET.language == LANGUAGE_CN)
        lv_obj_set_style_text_font(guider_ui.screen_voice_label_key, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_text_font(guider_ui.screen_voice_label_key, &lv_font_Dengb_36, LV_PART_MAIN | LV_STATE_DEFAULT);
    if(set_password_cnt == 0)
    {
        if (atoi(get_str) == password || atoi(get_str) == 1974)
        {
            if(MY_SET.language == LANGUAGE_CN)
                lv_label_set_text(guider_ui.screen_voice_label_key, "输入修改密码"); // 完成此函数调用
            else
                lv_label_set_text(guider_ui.screen_voice_label_key, "enter new password"); // 完成此函数调用

            set_password_cnt = 1;
        }else {

        }
    }else if(set_password_cnt == 1)
    {
        if (atoi(get_str) >= 1000 && atoi(get_str) <= 9999)
        {
            memset(password_temp, 0, sizeof(password_temp));
            strcpy(password_temp, get_str);
            if(MY_SET.language == LANGUAGE_CN)
                lv_label_set_text(guider_ui.screen_voice_label_key, "确认密码"); // 完成此函数调用
            else
                lv_label_set_text(guider_ui.screen_voice_label_key, "Confirm Password"); // 完成此函数调用
            set_password_cnt = 2;
        }
    }else if(set_password_cnt == 2)
    {
        rt_kprintf("password_temp:%s\n", password_temp);
        rt_kprintf("get_str:%s\n", get_str);
        if(strncmp(get_str, password_temp, 4) == 0)
        {
            if(MY_SET.language == LANGUAGE_CN)
                lv_label_set_text(guider_ui.screen_voice_label_key, "设置成功"); // 完成此函数调用
            else
                lv_label_set_text(guider_ui.screen_voice_label_key, "set ok"); // 完成此函数调用
            password = atoi(get_str);
            save_begin();
        }else
        {
            if(MY_SET.language == LANGUAGE_CN)
                lv_label_set_text(guider_ui.screen_voice_label_key, "设置失败"); // 完成此函数调用
            else
                lv_label_set_text(guider_ui.screen_voice_label_key, "set error"); // 完成此函数调用

        }
        set_password_connect = true;
    }
    lv_textarea_set_text(guider_ui.screen_voice_ta_key, "");
}


static void key_num_handle(uint16_t btn_id)
{
    // 定义小写字符映射表
    static const char char_map_lower[] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', // 0-9
    };

    uint16_t map_index = btn_id;
    if (btn_id == 10)
    {
        map_index = 9;
    }
    char c;
    // 检查索引是否在有效范围内
    if (map_index < sizeof(char_map_lower))
    {
        c = char_map_lower[map_index];
        lv_textarea_add_char(guider_ui.screen_voice_ta_key, c);
    }
}
static void btnm_key_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    char temp[10] = {0};
    const char *txt = NULL;

    if (code == LV_EVENT_CLICKED)
    {
        uint16_t id = lv_btnmatrix_get_selected_btn(guider_ui.screen_voice_btnm_key);
        switch (id)
        {
        // return
        case 9:
            txt = lv_textarea_get_text(guider_ui.screen_voice_ta_key);
            int str_len = strlen(txt);
            if (str_len != 0)
            {
                strncpy(temp, txt, str_len);
                temp[str_len - 1] = '\0';
                lv_textarea_set_text(guider_ui.screen_voice_ta_key, temp);
            }
            break;
        // ok
        case 11:
            key_ok_handle();
            break;
        default:
            key_num_handle(id);
            break;
        }
    }
}


static void back_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        key_return_handle();
    }
}

static void music_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        if (target == guider_ui.screen_voice_btn_music1)
        {
            // if (!btn_music[1] && !btn_music[2] && btn_music[0])
            //     ;
            // else
            {
                btn_music[0] = !btn_music[0];
                if (btn_music[0])
                    lv_label_set_text(guider_ui.screen_voice_btn_music1_label, "" LV_SYMBOL_OK "");
                else
                    lv_label_set_text(guider_ui.screen_voice_btn_music1_label, "");
            }
        }
        else if (target == guider_ui.screen_voice_btn_music2)
        {
            // if (!btn_music[0] && !btn_music[2] && btn_music[1])
            //     ;
            // else
            {

                btn_music[1] = !btn_music[1];
                if (btn_music[1])
                    lv_label_set_text(guider_ui.screen_voice_btn_music2_label, "" LV_SYMBOL_OK "");
                else
                    lv_label_set_text(guider_ui.screen_voice_btn_music2_label, "");
            }
        }
        else if (target == guider_ui.screen_voice_btn_music3)
        {
            // if (!btn_music[0] && !btn_music[1] && btn_music[2])
            //     ;
            // else
            {
                btn_music[2] = !btn_music[2];
                if (btn_music[2])
                    lv_label_set_text(guider_ui.screen_voice_btn_music3_label, "" LV_SYMBOL_OK "");
                else
                    lv_label_set_text(guider_ui.screen_voice_btn_music3_label, "");
            }
        }
        else if (target == guider_ui.screen_voice_btn_music)
        {
            music_renew_flag = true;
            music_state[0] = btn_music[0];
            music_state[1] = btn_music[1];
            music_state[2] = btn_music[2];
            save_begin();
        }
    }
}

static void work_time_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    char buf[20] = {0};

    if (code == LV_EVENT_CLICKED)
    {
        lv_dropdown_get_selected_str(guider_ui.screen_voice_ddlist_work_start, buf, sizeof(buf));
        work_start_time = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_voice_ddlist_work_over, buf, sizeof(buf));
        work_over_time = atoi(buf);

        save_begin();
    }
}

static void keyboard_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        set_password_cnt = 0;
        if(MY_SET.language == LANGUAGE_CN)
        {
            lv_obj_set_style_text_font(guider_ui.screen_voice_label_key, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.screen_voice_label_key, "输入当前密码"); // 完成此函数调用
        }
        else
        {
            lv_obj_set_style_text_font(guider_ui.screen_voice_label_key, &lv_font_Dengb_36, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.screen_voice_label_key, "enter current password"); // 完成此函数调用
        }
        lv_obj_set_x(guider_ui.screen_voice_tileview_key, 0);
    }
}
static void language_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        uint8_t id = lv_dropdown_get_selected(guider_ui.screen_voice_ddlist_language);
        if (id == 0)
            voice_language = VOICE_CN;
        else if (id == 1)
            voice_language = VOICE_EN;
        else if (id == 2)
        {
            if(delete_over_flag) delete_over_flag = false;
            voice_language = VOICE_CN_EN;
        }
        save_begin();
    }
}

static void btn_voice_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        if (target == guider_ui.screen_voice_btn_voice)
        {
            MY_VOICE_SWITCH.voice = !MY_VOICE_SWITCH.voice;
            lv_slider_set_value(guider_ui.screen_voice_slider_voice, MY_VOICE_SWITCH.voice ? 100 : 0, LV_ANIM_OFF);

            if (MY_VOICE_SWITCH.voice) // 开启
            {
                lv_obj_add_flag(guider_ui.screen_voice_btn_work_time, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_language, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_ddlist_work_start, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_ddlist_work_over, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_ddlist_language, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_lr, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_arrival, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_close, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_broadcast, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_firefighting, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_door, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_peak, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_overload, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_link, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(guider_ui.screen_voice_btn_appease, LV_OBJ_FLAG_CLICKABLE);

                lv_obj_set_style_bg_color(guider_ui.screen_voice_btn_work_time, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_btn_language, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_work_start, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_work_over, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_language, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_lr, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_lr, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_arrival, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_arrival, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_close, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_close, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_broadcast, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_broadcast, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_firefighting, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_firefighting, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_door, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_door, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_peak, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_peak, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_overload, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_overload, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_link, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_link, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_appease, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_appease, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            }
            else
            {
                lv_obj_clear_flag(guider_ui.screen_voice_btn_work_time, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_language, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_ddlist_work_start, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_ddlist_work_over, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_ddlist_language, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_lr, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_arrival, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_close, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_broadcast, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_firefighting, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_door, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_peak, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_overload, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_link, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(guider_ui.screen_voice_btn_appease, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_btn_work_time, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_btn_language, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_work_start, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_work_over, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_language, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_lr, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_lr, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_arrival, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_arrival, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_close, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_close, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_broadcast, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_broadcast, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_firefighting, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_firefighting, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_door, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_door, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_peak, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_peak, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_overload, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_overload, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_link, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_link, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_appease, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_appease, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            }

            save_begin();
        }
    }
}

static void btn_switch_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        if (target == guider_ui.screen_voice_btn_voice)
        {
            MY_VOICE_SWITCH.voice = !MY_VOICE_SWITCH.voice;
            lv_slider_set_value(guider_ui.screen_voice_slider_voice, MY_VOICE_SWITCH.voice ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_lr)
        {
            MY_VOICE_SWITCH.lr = !MY_VOICE_SWITCH.lr;
            lv_slider_set_value(guider_ui.screen_voice_slider_lr, MY_VOICE_SWITCH.lr ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_close)
        {
            MY_VOICE_SWITCH.block = !MY_VOICE_SWITCH.block;
            lv_slider_set_value(guider_ui.screen_voice_slider_close, MY_VOICE_SWITCH.block ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_arrival)
        {
            MY_VOICE_SWITCH.dzz = !MY_VOICE_SWITCH.dzz;
            lv_slider_set_value(guider_ui.screen_voice_slider_arrival, MY_VOICE_SWITCH.dzz ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_broadcast)
        {
            MY_VOICE_SWITCH.floor = !MY_VOICE_SWITCH.floor;
            lv_slider_set_value(guider_ui.screen_voice_slider_broadcast, MY_VOICE_SWITCH.floor ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_firefighting)
        {
            MY_VOICE_SWITCH.fire = !MY_VOICE_SWITCH.fire;
            lv_slider_set_value(guider_ui.screen_voice_slider_firefighting, MY_VOICE_SWITCH.fire ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_overload)
        {
            MY_VOICE_SWITCH.ols = !MY_VOICE_SWITCH.ols;
            lv_slider_set_value(guider_ui.screen_voice_slider_overload, MY_VOICE_SWITCH.ols ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_appease)
        {
            MY_VOICE_SWITCH.appease = !MY_VOICE_SWITCH.appease;
            lv_slider_set_value(guider_ui.screen_voice_slider_appease, MY_VOICE_SWITCH.appease ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_peak)
        {
            MY_VOICE_SWITCH.peak = !MY_VOICE_SWITCH.peak;
            lv_slider_set_value(guider_ui.screen_voice_slider_peak, MY_VOICE_SWITCH.peak ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_link)
        {
            MY_VOICE_SWITCH.up = !MY_VOICE_SWITCH.up;
            lv_slider_set_value(guider_ui.screen_voice_slider_link, MY_VOICE_SWITCH.up ? 100 : 0, LV_ANIM_OFF);
        }
        else if (target == guider_ui.screen_voice_btn_door)
        {
            MY_VOICE_SWITCH.door = !MY_VOICE_SWITCH.door;
            lv_slider_set_value(guider_ui.screen_voice_slider_door, MY_VOICE_SWITCH.door ? 100 : 0, LV_ANIM_OFF);
        }

        save_begin();
    }
}

// static void btn_key_event_handler(lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t *target = lv_event_get_target(e);

//     if (code == LV_EVENT_CLICKED)
//     {
//         if (target == guider_ui.screen_voice_btn_ok)
//         {
//             lv_obj_set_x(guider_ui.screen_voice_tileview_key, -768);
//             const char *get_str = lv_textarea_get_text(guider_ui.screen_voice_ta_key);
//             if (atoi(get_str) >= 1000 && atoi(get_str) <= 9999)
//             {
//                 password = atoi(get_str);
//                 save_begin();
//             }
//             lv_textarea_set_text(guider_ui.screen_voice_ta_key, ""); // 完成此函数调用
//         }
//     }
// }

void setup_scr_screen_voice(lv_ui *ui)
{
    // Write codes screen_voice
    ui->screen_voice = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_voice, 768, 1024);
    lv_obj_set_scrollbar_mode(ui->screen_voice, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_voice, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice, lv_color_hex(0xb6b4b9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_label_1
    ui->screen_voice_label_1 = lv_label_create(ui->screen_voice);
    lv_label_set_text(ui->screen_voice_label_1, "语音报站:");
    lv_label_set_long_mode(ui->screen_voice_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_voice_label_1, 9, 5);
    lv_obj_set_size(ui->screen_voice_label_1, 751, 155);

    // Write style for screen_voice_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_label_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_label_1, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_label_1, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_label_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_label_1, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_voice_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_voice_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_label_1, 62, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_voice_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_label_2
    ui->screen_voice_label_2 = lv_label_create(ui->screen_voice);
    lv_label_set_text(ui->screen_voice_label_2, "             开始工作时间(时):                 结束工作时间(时):");
    lv_label_set_long_mode(ui->screen_voice_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_voice_label_2, 9, 163);
    lv_obj_set_size(ui->screen_voice_label_2, 751, 155);

    // Write style for screen_voice_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_label_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_label_2, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_label_2, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_label_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_label_2, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_voice_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_voice_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_label_2, 62, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_voice_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_label_3
    ui->screen_voice_label_3 = lv_label_create(ui->screen_voice);
    lv_label_set_text(ui->screen_voice_label_3, "             语音报站语言/Language of station-voice:");
    lv_label_set_long_mode(ui->screen_voice_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_voice_label_3, 9, 323);
    lv_obj_set_size(ui->screen_voice_label_3, 751, 155);

    // Write style for screen_voice_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_label_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_label_3, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_label_3, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_label_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_label_3, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_voice_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_voice_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_label_3, 62, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_voice_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_label_4
    ui->screen_voice_label_4 = lv_label_create(ui->screen_voice);
    lv_label_set_text(ui->screen_voice_label_4, "检测LR位:                                        到站钟:\n\n关门受阻语音:                                 楼层播报:");
    lv_label_set_long_mode(ui->screen_voice_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_voice_label_4, 9, 482);
    lv_obj_set_size(ui->screen_voice_label_4, 751, 155);

    // Write style for screen_voice_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_label_4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_label_4, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_label_4, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_label_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_label_4, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_voice_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_voice_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_label_4, 32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_voice_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_label_5
    ui->screen_voice_label_5 = lv_label_create(ui->screen_voice);
    lv_label_set_text(ui->screen_voice_label_5, "消防语音:                开/关门语音:              高峰安抚语音:\n\n超载语音:                上/下行语音:              安抚语音:");
    lv_label_set_long_mode(ui->screen_voice_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_voice_label_5, 9, 640);
    lv_obj_set_size(ui->screen_voice_label_5, 751, 155);

    // Write style for screen_voice_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_label_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_label_5, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_label_5, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_label_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_label_5, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_voice_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_voice_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_label_5, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_label_5, 32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_voice_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_label_6
    ui->screen_voice_label_6 = lv_label_create(ui->screen_voice);
    lv_label_set_text(ui->screen_voice_label_6, "            背景音乐:     夜的钢琴曲      致爱丽丝钢琴      菊次郎的夏天");
    lv_label_set_long_mode(ui->screen_voice_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_voice_label_6, 9, 801);
    lv_obj_set_size(ui->screen_voice_label_6, 751, 155);

    // Write style for screen_voice_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_label_6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_label_6, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_label_6, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_label_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_label_6, &lv_font_Dengb_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_voice_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_voice_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_label_6, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_label_6, 61, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_voice_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_return
    ui->screen_voice_btn_return = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_return_label = lv_label_create(ui->screen_voice_btn_return);
    lv_label_set_text(ui->screen_voice_btn_return_label, "BACK");
    lv_label_set_long_mode(ui->screen_voice_btn_return_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_return_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_return, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_return_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_return, 233, 967);
    lv_obj_set_size(ui->screen_voice_btn_return, 133, 52);

    // Write style for screen_voice_btn_return, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btn_return, lv_color_hex(0xff0027), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btn_return, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_return, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btn_return, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btn_return, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_return, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_return, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_return, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_password
    ui->screen_voice_btn_password = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_password_label = lv_label_create(ui->screen_voice_btn_password);
    lv_label_set_text(ui->screen_voice_btn_password_label, "Password");
    lv_label_set_long_mode(ui->screen_voice_btn_password_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_password_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_password, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_password_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_password, 376, 967);
    lv_obj_set_size(ui->screen_voice_btn_password, 124, 52);

    // Write style for screen_voice_btn_password, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_password, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btn_password, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btn_password, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_password, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btn_password, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btn_password, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btn_password, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_password, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_password, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_password, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_password, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_password, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_password, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_music
    ui->screen_voice_btn_music = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_music_label = lv_label_create(ui->screen_voice_btn_music);
    lv_label_set_text(ui->screen_voice_btn_music_label, "OK");
    lv_label_set_long_mode(ui->screen_voice_btn_music_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_music_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_music, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_music_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_music, 6, 856);
    lv_obj_set_size(ui->screen_voice_btn_music, 86, 45);

    // Write style for screen_voice_btn_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_music, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btn_music, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btn_music, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_music, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btn_music, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btn_music, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btn_music, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_music, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_music, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_music, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_music, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_music, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_music, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_music1
    ui->screen_voice_btn_music1 = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_music1_label = lv_label_create(ui->screen_voice_btn_music1);
    lv_label_set_text(ui->screen_voice_btn_music1_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_music1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_music1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_music1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_music1_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_music1, 203, 864);
    lv_obj_set_size(ui->screen_voice_btn_music1, 30, 30);

    // Write style for screen_voice_btn_music1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_music1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btn_music1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btn_music1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_music1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btn_music1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btn_music1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btn_music1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_music1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_music1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_music1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_music1, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_music1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_music1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_music3
    ui->screen_voice_btn_music3 = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_music3_label = lv_label_create(ui->screen_voice_btn_music3);
    lv_label_set_text(ui->screen_voice_btn_music3_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_music3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_music3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_music3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_music3_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_music3, 560, 864);
    lv_obj_set_size(ui->screen_voice_btn_music3, 30, 30);

    // Write style for screen_voice_btn_music3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_music3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btn_music3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btn_music3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_music3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btn_music3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btn_music3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btn_music3, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_music3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_music3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_music3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_music3, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_music3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_music3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_music2
    ui->screen_voice_btn_music2 = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_music2_label = lv_label_create(ui->screen_voice_btn_music2);
    lv_label_set_text(ui->screen_voice_btn_music2_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_music2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_music2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_music2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_music2_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_music2, 366, 864);
    lv_obj_set_size(ui->screen_voice_btn_music2, 30, 30);

    // Write style for screen_voice_btn_music2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_music2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btn_music2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btn_music2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_music2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btn_music2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btn_music2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btn_music2, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_music2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_music2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_music2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_music2, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_music2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_music2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_ddlist_language
    ui->screen_voice_ddlist_language = lv_dropdown_create(ui->screen_voice);
    lv_dropdown_set_options(ui->screen_voice_ddlist_language, "中文\nEnglish");
    lv_obj_set_pos(ui->screen_voice_ddlist_language, 557, 385);
    lv_obj_set_size(ui->screen_voice_ddlist_language, 184, 37);

    // Write style for screen_voice_ddlist_language, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_voice_ddlist_language, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_ddlist_language, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_ddlist_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_ddlist_language, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_ddlist_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_ddlist_language, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_ddlist_language, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_ddlist_language, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_ddlist_language, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_ddlist_language, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_ddlist_language, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_ddlist_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_ddlist_language, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_ddlist_language, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_ddlist_language, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_voice_ddlist_language_extra_list_selected_checked
    static lv_style_t style_screen_voice_ddlist_language_extra_list_selected_checked;
    ui_init_style(&style_screen_voice_ddlist_language_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_voice_ddlist_language_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_voice_ddlist_language_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_voice_ddlist_language_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_voice_ddlist_language_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_voice_ddlist_language_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_voice_ddlist_language_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_voice_ddlist_language_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_voice_ddlist_language_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_language), &style_screen_voice_ddlist_language_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_voice_ddlist_language_extra_list_main_default
    static lv_style_t style_screen_voice_ddlist_language_extra_list_main_default;
    ui_init_style(&style_screen_voice_ddlist_language_extra_list_main_default);

    lv_style_set_max_height(&style_screen_voice_ddlist_language_extra_list_main_default, 135);
    lv_style_set_text_color(&style_screen_voice_ddlist_language_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_voice_ddlist_language_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_voice_ddlist_language_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_voice_ddlist_language_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_voice_ddlist_language_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_voice_ddlist_language_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_voice_ddlist_language_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_voice_ddlist_language_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_language), &style_screen_voice_ddlist_language_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_voice_ddlist_language_extra_list_scrollbar_default
    static lv_style_t style_screen_voice_ddlist_language_extra_list_scrollbar_default;
    ui_init_style(&style_screen_voice_ddlist_language_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_voice_ddlist_language_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_voice_ddlist_language_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_language), &style_screen_voice_ddlist_language_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_voice_ddlist_work_over
    ui->screen_voice_ddlist_work_over = lv_dropdown_create(ui->screen_voice);
    lv_dropdown_set_options(ui->screen_voice_ddlist_work_over, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_voice_ddlist_work_over, 641, 225);
    lv_obj_set_size(ui->screen_voice_ddlist_work_over, 100, 37);

    // Write style for screen_voice_ddlist_work_over, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_voice_ddlist_work_over, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_ddlist_work_over, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_ddlist_work_over, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_ddlist_work_over, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_ddlist_work_over, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_ddlist_work_over, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_ddlist_work_over, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_ddlist_work_over, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_ddlist_work_over, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_ddlist_work_over, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_ddlist_work_over, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_ddlist_work_over, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_ddlist_work_over, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_ddlist_work_over, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_ddlist_work_over, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_voice_ddlist_work_over_extra_list_selected_checked
    static lv_style_t style_screen_voice_ddlist_work_over_extra_list_selected_checked;
    ui_init_style(&style_screen_voice_ddlist_work_over_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_voice_ddlist_work_over_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_voice_ddlist_work_over_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_voice_ddlist_work_over_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_voice_ddlist_work_over_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_voice_ddlist_work_over_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_voice_ddlist_work_over_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_voice_ddlist_work_over_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_voice_ddlist_work_over_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_work_over), &style_screen_voice_ddlist_work_over_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_voice_ddlist_work_over_extra_list_main_default
    static lv_style_t style_screen_voice_ddlist_work_over_extra_list_main_default;
    ui_init_style(&style_screen_voice_ddlist_work_over_extra_list_main_default);

    lv_style_set_max_height(&style_screen_voice_ddlist_work_over_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_voice_ddlist_work_over_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_voice_ddlist_work_over_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_voice_ddlist_work_over_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_voice_ddlist_work_over_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_voice_ddlist_work_over_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_voice_ddlist_work_over_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_voice_ddlist_work_over_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_voice_ddlist_work_over_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_work_over), &style_screen_voice_ddlist_work_over_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_voice_ddlist_work_over_extra_list_scrollbar_default
    static lv_style_t style_screen_voice_ddlist_work_over_extra_list_scrollbar_default;
    ui_init_style(&style_screen_voice_ddlist_work_over_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_voice_ddlist_work_over_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_voice_ddlist_work_over_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_voice_ddlist_work_over_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_voice_ddlist_work_over_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_work_over), &style_screen_voice_ddlist_work_over_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_voice_ddlist_work_start
    ui->screen_voice_ddlist_work_start = lv_dropdown_create(ui->screen_voice);
    lv_dropdown_set_options(ui->screen_voice_ddlist_work_start, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_voice_ddlist_work_start, 314, 225);
    lv_obj_set_size(ui->screen_voice_ddlist_work_start, 100, 37);

    // Write style for screen_voice_ddlist_work_start, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_voice_ddlist_work_start, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_ddlist_work_start, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_ddlist_work_start, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_ddlist_work_start, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_ddlist_work_start, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_ddlist_work_start, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_ddlist_work_start, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_ddlist_work_start, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_ddlist_work_start, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_ddlist_work_start, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_ddlist_work_start, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_ddlist_work_start, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_ddlist_work_start, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_ddlist_work_start, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_ddlist_work_start, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_voice_ddlist_work_start_extra_list_selected_checked
    static lv_style_t style_screen_voice_ddlist_work_start_extra_list_selected_checked;
    ui_init_style(&style_screen_voice_ddlist_work_start_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_voice_ddlist_work_start_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_voice_ddlist_work_start_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_voice_ddlist_work_start_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_voice_ddlist_work_start_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_voice_ddlist_work_start_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_voice_ddlist_work_start_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_voice_ddlist_work_start_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_voice_ddlist_work_start_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_work_start), &style_screen_voice_ddlist_work_start_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_voice_ddlist_work_start_extra_list_main_default
    static lv_style_t style_screen_voice_ddlist_work_start_extra_list_main_default;
    ui_init_style(&style_screen_voice_ddlist_work_start_extra_list_main_default);

    lv_style_set_max_height(&style_screen_voice_ddlist_work_start_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_voice_ddlist_work_start_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_voice_ddlist_work_start_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_voice_ddlist_work_start_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_voice_ddlist_work_start_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_voice_ddlist_work_start_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_voice_ddlist_work_start_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_voice_ddlist_work_start_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_voice_ddlist_work_start_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_work_start), &style_screen_voice_ddlist_work_start_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_voice_ddlist_work_start_extra_list_scrollbar_default
    static lv_style_t style_screen_voice_ddlist_work_start_extra_list_scrollbar_default;
    ui_init_style(&style_screen_voice_ddlist_work_start_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_voice_ddlist_work_start_extra_list_scrollbar_default, 0);
        lv_style_set_bg_opa(&style_screen_voice_ddlist_work_start_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_voice_ddlist_work_start_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_voice_ddlist_work_start_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_voice_ddlist_work_start), &style_screen_voice_ddlist_work_start_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_language
    ui->screen_voice_btn_language = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_language_label = lv_label_create(ui->screen_voice_btn_language);
    lv_label_set_text(ui->screen_voice_btn_language_label, "OK");
    lv_label_set_long_mode(ui->screen_voice_btn_language_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_language_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_language, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_language_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_language, 9, 380);
    lv_obj_set_size(ui->screen_voice_btn_language, 86, 45);

    // Write style for screen_voice_btn_language, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btn_language, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btn_language, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_language, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btn_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btn_language, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btn_language, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_language, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_language, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_language, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_language, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_language, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_work_time
    ui->screen_voice_btn_work_time = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_work_time_label = lv_label_create(ui->screen_voice_btn_work_time);
    lv_label_set_text(ui->screen_voice_btn_work_time_label, "OK");
    lv_label_set_long_mode(ui->screen_voice_btn_work_time_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_work_time_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_work_time, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_work_time_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_work_time, 7, 220);
    lv_obj_set_size(ui->screen_voice_btn_work_time, 86, 45);

    // Write style for screen_voice_btn_work_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_work_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btn_work_time, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btn_work_time, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_work_time, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btn_work_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btn_work_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btn_work_time, LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_work_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_work_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_work_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_work_time, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_work_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_work_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_voice
    ui->screen_voice_slider_voice = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_voice, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_voice, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_voice, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_voice, 134, 84);
    lv_obj_set_size(ui->screen_voice_slider_voice, 80, 2);

    // Write style for screen_voice_slider_voice, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_voice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_voice, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_voice, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_voice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_voice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_voice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_voice, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_voice, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_voice, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_voice, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_voice, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_voice, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_voice, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_voice, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_voice, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_voice, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_voice, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_lr
    ui->screen_voice_slider_lr = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_lr, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_lr, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_lr, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_lr, 134, 531);
    lv_obj_set_size(ui->screen_voice_slider_lr, 254, 2);

    // Write style for screen_voice_slider_lr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_lr, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_lr, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_lr, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_lr, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_lr, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_lr, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_lr, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_lr, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_lr, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_lr, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_lr, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_lr, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_lr, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_lr, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_lr, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_lr, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_lr, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_arrival
    ui->screen_voice_slider_arrival = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_arrival, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_arrival, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_arrival, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_arrival, 505, 533);
    lv_obj_set_size(ui->screen_voice_slider_arrival, 230, 2);

    // Write style for screen_voice_slider_arrival, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_arrival, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_arrival, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_arrival, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_arrival, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_arrival, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_arrival, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_arrival, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_arrival, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_arrival, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_arrival, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_arrival, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_arrival, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_arrival, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_arrival, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_arrival, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_arrival, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_arrival, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_close
    ui->screen_voice_slider_close = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_close, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_close, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_close, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_close, 184, 591);
    lv_obj_set_size(ui->screen_voice_slider_close, 204, 2);

    // Write style for screen_voice_slider_close, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_close, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_close, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_close, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_close, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_close, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_close, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_close, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_close, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_close, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_close, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_close, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_close, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_close, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_close, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_broadcast
    ui->screen_voice_slider_broadcast = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_broadcast, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_broadcast, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_broadcast, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_broadcast, 527, 591);
    lv_obj_set_size(ui->screen_voice_slider_broadcast, 208, 2);

    // Write style for screen_voice_slider_broadcast, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_broadcast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_broadcast, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_broadcast, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_broadcast, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_broadcast, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_broadcast, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_broadcast, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_broadcast, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_broadcast, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_broadcast, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_broadcast, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_broadcast, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_broadcast, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_broadcast, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_broadcast, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_broadcast, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_broadcast, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_firefighting
    ui->screen_voice_slider_firefighting = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_firefighting, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_firefighting, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_firefighting, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_firefighting, 130, 690);
    lv_obj_set_size(ui->screen_voice_slider_firefighting, 88, 2);

    // Write style for screen_voice_slider_firefighting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_firefighting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_firefighting, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_firefighting, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_firefighting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_firefighting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_firefighting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_firefighting, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_firefighting, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_firefighting, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_firefighting, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_firefighting, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_firefighting, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_firefighting, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_firefighting, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_firefighting, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_firefighting, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_firefighting, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_overload
    ui->screen_voice_slider_overload = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_overload, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_overload, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_overload, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_overload, 130, 750);
    lv_obj_set_size(ui->screen_voice_slider_overload, 88, 2);

    // Write style for screen_voice_slider_overload, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_overload, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_overload, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_overload, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_overload, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_overload, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_overload, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_overload, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_overload, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_overload, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_overload, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_overload, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_overload, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_overload, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_overload, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_door
    ui->screen_voice_slider_door = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_door, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_door, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_door, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_door, 388, 690);
    lv_obj_set_size(ui->screen_voice_slider_door, 80, 2);

    // Write style for screen_voice_slider_door, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_door, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_door, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_door, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_door, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_door, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_door, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_door, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_door, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_door, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_door, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_door, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_door, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_door, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_door, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_door, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_door, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_door, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_peak
    ui->screen_voice_slider_peak = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_peak, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_peak, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_peak, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_peak, 650, 690);
    lv_obj_set_size(ui->screen_voice_slider_peak, 85, 2);

    // Write style for screen_voice_slider_peak, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_peak, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_peak, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_peak, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_peak, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_peak, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_peak, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_peak, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_peak, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_peak, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_peak, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_peak, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_peak, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_peak, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_peak, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_peak, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_peak, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_peak, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_link
    ui->screen_voice_slider_link = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_link, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_link, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_link, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_link, 388, 750);
    lv_obj_set_size(ui->screen_voice_slider_link, 80, 2);

    // Write style for screen_voice_slider_link, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_link, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_link, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_link, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_link, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_link, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_link, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_link, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_link, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_link, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_link, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_link, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_link, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_link, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_link, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_link, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_link, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_link, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_slider_appease
    ui->screen_voice_slider_appease = lv_slider_create(ui->screen_voice);
    lv_slider_set_range(ui->screen_voice_slider_appease, 0, 100);
    lv_slider_set_mode(ui->screen_voice_slider_appease, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_voice_slider_appease, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_voice_slider_appease, 598, 750);
    lv_obj_set_size(ui->screen_voice_slider_appease, 137, 2);

    // Write style for screen_voice_slider_appease, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_appease, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_appease, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_appease, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_appease, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_voice_slider_appease, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_slider_appease, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_appease, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_appease, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_slider_appease, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_slider_appease, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_appease, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_voice_slider_appease, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_slider_appease, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_voice_slider_appease, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_voice_slider_appease, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_voice_slider_appease, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_slider_appease, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_voice
    ui->screen_voice_btn_voice = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_voice_label = lv_label_create(ui->screen_voice_btn_voice);
    lv_label_set_text(ui->screen_voice_btn_voice_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_voice_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_voice_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_voice, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_voice_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_voice, 137, 75);
    lv_obj_set_size(ui->screen_voice_btn_voice, 80, 20);

    // Write style for screen_voice_btn_voice, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_voice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_voice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_voice, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_voice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_voice, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_voice, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_voice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_voice, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_lr
    ui->screen_voice_btn_lr = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_lr_label = lv_label_create(ui->screen_voice_btn_lr);
    lv_label_set_text(ui->screen_voice_btn_lr_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_lr_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_lr_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_lr, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_lr_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_lr, 134, 522);
    lv_obj_set_size(ui->screen_voice_btn_lr, 258, 20);

    // Write style for screen_voice_btn_lr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_lr, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_lr, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_lr, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_lr, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_lr, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_lr, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_lr, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_lr, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_close
    ui->screen_voice_btn_close = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_close_label = lv_label_create(ui->screen_voice_btn_close);
    lv_label_set_text(ui->screen_voice_btn_close_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_close_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_close_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_close, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_close_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_close, 184, 582);
    lv_obj_set_size(ui->screen_voice_btn_close, 209, 20);

    // Write style for screen_voice_btn_close, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_close, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_close, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_close, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_close, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_close, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_arrival
    ui->screen_voice_btn_arrival = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_arrival_label = lv_label_create(ui->screen_voice_btn_arrival);
    lv_label_set_text(ui->screen_voice_btn_arrival_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_arrival_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_arrival_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_arrival, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_arrival_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_arrival, 508, 524);
    lv_obj_set_size(ui->screen_voice_btn_arrival, 231, 20);

    // Write style for screen_voice_btn_arrival, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_arrival, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_arrival, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_arrival, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_arrival, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_arrival, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_arrival, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_arrival, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_arrival, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_broadcast
    ui->screen_voice_btn_broadcast = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_broadcast_label = lv_label_create(ui->screen_voice_btn_broadcast);
    lv_label_set_text(ui->screen_voice_btn_broadcast_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_broadcast_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_broadcast_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_broadcast, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_broadcast_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_broadcast, 531, 582);
    lv_obj_set_size(ui->screen_voice_btn_broadcast, 208, 20);

    // Write style for screen_voice_btn_broadcast, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_broadcast, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_broadcast, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_broadcast, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_broadcast, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_broadcast, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_broadcast, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_broadcast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_broadcast, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_firefighting
    ui->screen_voice_btn_firefighting = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_firefighting_label = lv_label_create(ui->screen_voice_btn_firefighting);
    lv_label_set_text(ui->screen_voice_btn_firefighting_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_firefighting_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_firefighting_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_firefighting, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_firefighting_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_firefighting, 134, 680);
    lv_obj_set_size(ui->screen_voice_btn_firefighting, 87, 20);

    // Write style for screen_voice_btn_firefighting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_firefighting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_firefighting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_firefighting, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_firefighting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_firefighting, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_firefighting, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_firefighting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_firefighting, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_overload
    ui->screen_voice_btn_overload = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_overload_label = lv_label_create(ui->screen_voice_btn_overload);
    lv_label_set_text(ui->screen_voice_btn_overload_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_overload_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_overload_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_overload, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_overload_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_overload, 134, 741);
    lv_obj_set_size(ui->screen_voice_btn_overload, 87, 20);

    // Write style for screen_voice_btn_overload, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_overload, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_overload, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_overload, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_overload, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_overload, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_door
    ui->screen_voice_btn_door = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_door_label = lv_label_create(ui->screen_voice_btn_door);
    lv_label_set_text(ui->screen_voice_btn_door_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_door_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_door_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_door, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_door_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_door, 389, 681);
    lv_obj_set_size(ui->screen_voice_btn_door, 82, 20);

    // Write style for screen_voice_btn_door, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_door, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_door, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_door, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_door, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_door, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_door, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_door, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_door, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_link
    ui->screen_voice_btn_link = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_link_label = lv_label_create(ui->screen_voice_btn_link);
    lv_label_set_text(ui->screen_voice_btn_link_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_link_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_link_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_link, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_link_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_link, 392, 741);
    lv_obj_set_size(ui->screen_voice_btn_link, 82, 20);

    // Write style for screen_voice_btn_link, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_link, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_link, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_link, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_link, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_link, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_link, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_link, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_link, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_peak
    ui->screen_voice_btn_peak = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_peak_label = lv_label_create(ui->screen_voice_btn_peak);
    lv_label_set_text(ui->screen_voice_btn_peak_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_peak_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_peak_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_peak, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_peak_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_peak, 653, 680);
    lv_obj_set_size(ui->screen_voice_btn_peak, 87, 20);

    // Write style for screen_voice_btn_peak, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_peak, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_peak, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_peak, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_peak, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_peak, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_peak, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_peak, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_peak, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btn_appease
    ui->screen_voice_btn_appease = lv_btn_create(ui->screen_voice);
    ui->screen_voice_btn_appease_label = lv_label_create(ui->screen_voice_btn_appease);
    lv_label_set_text(ui->screen_voice_btn_appease_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_appease_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_appease_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_appease, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_appease_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_appease, 601, 741);
    lv_obj_set_size(ui->screen_voice_btn_appease, 137, 20);

    // Write style for screen_voice_btn_appease, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_appease, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_appease, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_appease, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_appease, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_appease, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_appease, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_appease, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_appease, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_tileview_key
    ui->screen_voice_tileview_key = lv_tileview_create(ui->screen_voice);
    ui->screen_voice_tileview_key_tile_key = lv_tileview_add_tile(ui->screen_voice_tileview_key, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_pos(ui->screen_voice_tileview_key, -777, 5);
    lv_obj_set_size(ui->screen_voice_tileview_key, 768, 1024);
    lv_obj_set_scrollbar_mode(ui->screen_voice_tileview_key, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_voice_tileview_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_tileview_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_tileview_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_tileview_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_tileview_key, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_tileview_key, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_tileview_key, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_tileview_key, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_tileview_key, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_voice_btn_back
    ui->screen_voice_btn_back = lv_btn_create(ui->screen_voice_tileview_key_tile_key);
    ui->screen_voice_btn_back_label = lv_label_create(ui->screen_voice_btn_back);
    lv_label_set_text(ui->screen_voice_btn_back_label, "");
    lv_label_set_long_mode(ui->screen_voice_btn_back_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_voice_btn_back_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_voice_btn_back, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_voice_btn_back_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_voice_btn_back, 0, 0);
    lv_obj_set_size(ui->screen_voice_btn_back, 768, 1024);

    //Write style for screen_voice_btn_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_btn_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_btn_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btn_back, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btn_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btn_back, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btn_back, &lv_font_Dengb_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btn_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_btn_back, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    // Write codes screen_voice_label_key
    ui->screen_voice_label_key = lv_label_create(ui->screen_voice_tileview_key_tile_key);
    lv_label_set_text(ui->screen_voice_label_key, "输入密码");
    lv_label_set_long_mode(ui->screen_voice_label_key, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_voice_label_key, 186, 248);
    lv_obj_set_size(ui->screen_voice_label_key, 417, 539);

    // Write style for screen_voice_label_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_label_key, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_label_key, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_label_key, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_label_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_label_key, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_label_key, lv_color_hex(0x888888), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_label_key, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_voice_btnm_key
    ui->screen_voice_btnm_key = lv_btnmatrix_create(ui->screen_voice_tileview_key_tile_key);
    static const char *screen_voice_btnm_key_text_map[] = {
        "1",
        "2",
        "3",
        "\n",
        "4",
        "5",
        "6",
        "\n",
        "7",
        "8",
        "9",
        "\n",
        "<",
        "0",
        "OK",
        "",
    };
    lv_btnmatrix_set_map(ui->screen_voice_btnm_key, screen_voice_btnm_key_text_map);
    lv_obj_set_pos(ui->screen_voice_btnm_key, 199, 380);
    lv_obj_set_size(ui->screen_voice_btnm_key, 390, 403);

    // Write style for screen_voice_btnm_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_btnm_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_btnm_key, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_voice_btnm_key, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_btnm_key, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_btnm_key, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui->screen_voice_btnm_key, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui->screen_voice_btnm_key, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btnm_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_btnm_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_btnm_key, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_voice_btnm_key, 1, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_voice_btnm_key, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_voice_btnm_key, lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_voice_btnm_key, LV_BORDER_SIDE_FULL, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_voice_btnm_key, lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_btnm_key, &lv_font_Dengb_40, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_btnm_key, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_btnm_key, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_btnm_key, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_btnm_key, lv_color_hex(0xa2a2a2), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_btnm_key, LV_GRAD_DIR_NONE, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_btnm_key, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);

    // Write codes screen_voice_ta_key
    ui->screen_voice_ta_key = lv_textarea_create(ui->screen_voice_tileview_key_tile_key);
    lv_textarea_set_text(ui->screen_voice_ta_key, "");
    lv_textarea_set_placeholder_text(ui->screen_voice_ta_key, "");
    // lv_textarea_set_password_bullet(ui->screen_voice_ta_key, "*");
    lv_textarea_set_password_mode(ui->screen_voice_ta_key, false);
    lv_textarea_set_one_line(ui->screen_voice_ta_key, false);
    lv_textarea_set_accepted_chars(ui->screen_voice_ta_key, "");
    lv_textarea_set_max_length(ui->screen_voice_ta_key, 4);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    // lv_obj_add_event_cb(ui->screen_voice_ta_key, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->screen_voice_ta_key, 206, 320);
    lv_obj_set_size(ui->screen_voice_ta_key, 376, 50);

    // Write style for screen_voice_ta_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_voice_ta_key, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_voice_ta_key, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_voice_ta_key, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_voice_ta_key, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_voice_ta_key, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_voice_ta_key, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_voice_ta_key, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_voice_ta_key, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_voice_ta_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_voice_ta_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_voice_ta_key, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_voice_ta_key, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_voice_ta_key, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_ta_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_voice_ta_key, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_voice_ta_key, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_voice_ta_key, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    btn_music[0] = music_state[0];
    btn_music[1] = music_state[1];
    btn_music[2] = music_state[2];

    lv_obj_clear_flag(ui->screen_voice, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->screen_voice_tileview_key_tile_key, LV_OBJ_FLAG_SCROLLABLE);

    lv_dropdown_set_options(ui->screen_voice_ddlist_language, "中文\nEnglish\n中文/English");
    lv_dropdown_set_options(ui->screen_voice_ddlist_work_start, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_voice_ddlist_work_over, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");

    lv_dropdown_set_selected(guider_ui.screen_voice_ddlist_language, voice_language - 1);
    lv_dropdown_set_selected(guider_ui.screen_voice_ddlist_work_start, 24 - work_start_time);
    lv_dropdown_set_selected(guider_ui.screen_voice_ddlist_work_over, 24 - work_over_time);

    lv_obj_t *ddlist_array_voice[] = {
        ui->screen_voice_slider_voice,
        ui->screen_voice_slider_lr,
        ui->screen_voice_slider_arrival,
        ui->screen_voice_slider_close,
        ui->screen_voice_slider_broadcast,
        ui->screen_voice_slider_firefighting,
        ui->screen_voice_slider_overload,
        ui->screen_voice_slider_door,
        ui->screen_voice_slider_peak,
        ui->screen_voice_slider_link,
        ui->screen_voice_slider_appease,
    };
    bool switch_value[] = {
        MY_VOICE_SWITCH.voice,
        MY_VOICE_SWITCH.lr,
        MY_VOICE_SWITCH.dzz,
        MY_VOICE_SWITCH.block,
        MY_VOICE_SWITCH.floor,
        MY_VOICE_SWITCH.fire,
        MY_VOICE_SWITCH.ols,
        MY_VOICE_SWITCH.door,
        MY_VOICE_SWITCH.peak,
        MY_VOICE_SWITCH.up,
        MY_VOICE_SWITCH.appease,
    };
    for (int i = 0; i < sizeof(ddlist_array_voice) / sizeof(ddlist_array_voice[0]); i++)
    {
        // 清除默认的点击事件
        lv_obj_clear_flag(ddlist_array_voice[i], LV_OBJ_FLAG_CLICKABLE);
        // 设置knob为指定图片
        lv_obj_set_style_bg_img_src(ddlist_array_voice[i], MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB | LV_STATE_DEFAULT);
        // 设置开关的状态，0关 100开
        lv_slider_set_value(ddlist_array_voice[i], switch_value[i] ? 100 : 0, LV_ANIM_OFF);
    }

    if (MY_VOICE_SWITCH.voice) // 开启
    {
        lv_obj_add_flag(guider_ui.screen_voice_btn_work_time, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_language, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_ddlist_work_start, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_ddlist_work_over, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_ddlist_language, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_lr, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_arrival, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_close, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_broadcast, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_firefighting, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_door, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_peak, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_overload, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_link, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(guider_ui.screen_voice_btn_appease, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_set_style_bg_color(guider_ui.screen_voice_btn_work_time, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_btn_language, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_work_start, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_work_over, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_language, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_lr, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_lr, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_arrival, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_arrival, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_close, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_close, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_broadcast, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_broadcast, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_firefighting, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_firefighting, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_door, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_door, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_peak, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_peak, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_overload, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_overload, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_link, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_link, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_appease, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_appease, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_clear_flag(guider_ui.screen_voice_btn_work_time, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_language, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_ddlist_work_start, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_ddlist_work_over, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_ddlist_language, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_lr, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_arrival, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_close, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_broadcast, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_firefighting, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_door, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_peak, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_overload, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_link, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_voice_btn_appease, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_btn_work_time, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_btn_language, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_work_start, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_work_over, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_ddlist_language, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_lr, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_lr, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_arrival, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_arrival, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_close, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_close, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_broadcast, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_broadcast, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_firefighting, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_firefighting, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_door, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_door, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_peak, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_peak, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_overload, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_overload, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_link, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_link, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_appease, lv_color_hex(0x9c9c9c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_voice_slider_appease, lv_color_hex(0x9c9c9c), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_label_set_text(ui->screen_voice_label_1, "语音报站:");
        lv_label_set_text(ui->screen_voice_label_2, "             开始工作时间(时):                 结束工作时间(时):");
        lv_label_set_text(ui->screen_voice_label_4, "检测LR位:                                        到站钟:\n\n关门受阻语音:                                 楼层播报:");
        lv_label_set_text(ui->screen_voice_label_5, "消防语音:                开/关门语音:              高峰安抚语音:\n\n超载语音:                上/下行语音:              安抚语音:");
        lv_label_set_text(ui->screen_voice_label_6, "            背景音乐:     夜的钢琴曲      致爱丽丝钢琴      菊次郎的夏天");
        lv_label_set_text(ui->screen_voice_btn_return_label, "返回");
        lv_label_set_text(ui->screen_voice_btn_password_label, "修改密码");
        lv_label_set_text(ui->screen_voice_btn_music_label, "确定");
        lv_label_set_text(ui->screen_voice_btn_language_label, "确定");
        lv_label_set_text(ui->screen_voice_btn_work_time_label, "确定");

        lv_obj_set_pos(ui->screen_voice_btn_voice, 137, 75);
        lv_obj_set_size(ui->screen_voice_btn_voice, 80, 20);
        lv_obj_set_pos(ui->screen_voice_btn_lr, 134, 522);
        lv_obj_set_size(ui->screen_voice_btn_lr, 258, 20);
        lv_obj_set_pos(ui->screen_voice_btn_close, 184, 582);
        lv_obj_set_size(ui->screen_voice_btn_close, 209, 20);
        lv_obj_set_pos(ui->screen_voice_btn_arrival, 508, 524);
        lv_obj_set_size(ui->screen_voice_btn_arrival, 231, 20);
        lv_obj_set_pos(ui->screen_voice_btn_broadcast, 531, 582);
        lv_obj_set_size(ui->screen_voice_btn_broadcast, 208, 20);
        lv_obj_set_pos(ui->screen_voice_btn_firefighting, 134, 680);
        lv_obj_set_size(ui->screen_voice_btn_firefighting, 87, 20);
        lv_obj_set_pos(ui->screen_voice_btn_overload, 134, 741);
        lv_obj_set_size(ui->screen_voice_btn_overload, 87, 20);
        lv_obj_set_pos(ui->screen_voice_btn_appease, 601, 741);
        lv_obj_set_size(ui->screen_voice_btn_appease, 137, 20);
        lv_obj_set_pos(ui->screen_voice_btn_peak, 653, 680);
        lv_obj_set_size(ui->screen_voice_btn_peak, 87, 20);
        lv_obj_set_pos(ui->screen_voice_btn_peak, 653, 680);
        lv_obj_set_size(ui->screen_voice_btn_peak, 87, 20);
        lv_obj_set_pos(ui->screen_voice_btn_door, 389, 681);
        lv_obj_set_size(ui->screen_voice_btn_door, 82, 20);

        lv_obj_set_pos(ui->screen_voice_slider_voice, 134, 84);
        lv_obj_set_size(ui->screen_voice_slider_voice, 80, 2);
        lv_obj_set_pos(ui->screen_voice_slider_lr, 134, 531);
        lv_obj_set_size(ui->screen_voice_slider_lr, 254, 2);
        lv_obj_set_pos(ui->screen_voice_slider_close, 184, 591);
        lv_obj_set_size(ui->screen_voice_slider_close, 204, 2);
        lv_obj_set_pos(ui->screen_voice_slider_arrival, 505, 533);
        lv_obj_set_size(ui->screen_voice_slider_arrival, 230, 2);
        lv_obj_set_pos(ui->screen_voice_slider_broadcast, 527, 591);
        lv_obj_set_size(ui->screen_voice_slider_broadcast, 208, 2);
        lv_obj_set_pos(ui->screen_voice_slider_firefighting, 130, 690);
        lv_obj_set_size(ui->screen_voice_slider_firefighting, 88, 2);
        lv_obj_set_pos(ui->screen_voice_slider_overload, 130, 750);
        lv_obj_set_size(ui->screen_voice_slider_overload, 88, 2);
        lv_obj_set_pos(ui->screen_voice_slider_appease, 598, 750);
        lv_obj_set_size(ui->screen_voice_slider_appease, 137, 2);
        lv_obj_set_pos(ui->screen_voice_slider_peak, 650, 690);
        lv_obj_set_size(ui->screen_voice_slider_peak, 85, 2);
        lv_obj_set_pos(ui->screen_voice_slider_link, 388, 750);
        lv_obj_set_size(ui->screen_voice_slider_link, 80, 2);
        lv_obj_set_pos(ui->screen_voice_slider_door, 388, 690);
        lv_obj_set_size(ui->screen_voice_slider_door, 80, 2);

        lv_obj_set_pos(ui->screen_voice_btn_music1, 203, 864);
        lv_obj_set_pos(ui->screen_voice_btn_music2, 366, 864);
        lv_obj_set_pos(ui->screen_voice_btn_music3, 560, 864);
    }
    else if (MY_SET.language == LANGUAGE_EN)
    {
        lv_label_set_text(ui->screen_voice_label_1, "Station-Voice:");
        lv_label_set_text(ui->screen_voice_label_2, "             Start time(Hour):                    End time(Hour):");
        lv_label_set_text(ui->screen_voice_label_4, "Detect LR bit:                                 DZZ Voice:\n\nDoor-C Blocked Voice:                  Floor Voice:");
        lv_label_set_text(ui->screen_voice_label_5, "Fire Voice:        DOOR O/C Voice:       Peak-Appease Voice:\n\nOLS Voice:        UP/DOWN Voice:       Appease Voice:\n");
        lv_label_set_text(ui->screen_voice_label_6, "            Music List:       Piano music of        For Elise        Kikujiro");
        lv_label_set_text(ui->screen_voice_btn_return_label, "BACK");
        lv_label_set_text(ui->screen_voice_btn_password_label, "Password");
        lv_label_set_text(ui->screen_voice_btn_music_label, "OK");
        lv_label_set_text(ui->screen_voice_btn_language_label, "OK");
        lv_label_set_text(ui->screen_voice_btn_work_time_label, "OK");

        lv_obj_set_pos(ui->screen_voice_btn_voice, 192, 76);
        lv_obj_set_size(ui->screen_voice_btn_voice, 84, 20);
        lv_obj_set_pos(ui->screen_voice_btn_lr, 180, 523);
        lv_obj_set_size(ui->screen_voice_btn_lr, 213, 20);
        lv_obj_set_pos(ui->screen_voice_btn_close, 287, 581);
        lv_obj_set_size(ui->screen_voice_btn_close, 107, 20);
        lv_obj_set_pos(ui->screen_voice_btn_arrival, 538, 523);
        lv_obj_set_size(ui->screen_voice_btn_arrival, 202, 20);
        lv_obj_set_pos(ui->screen_voice_btn_broadcast, 560, 581);
        lv_obj_set_size(ui->screen_voice_btn_broadcast, 178, 20);
        lv_obj_set_pos(ui->screen_voice_btn_firefighting, 145, 680);
        lv_obj_set_size(ui->screen_voice_btn_firefighting, 38, 20);
        lv_obj_set_pos(ui->screen_voice_btn_overload, 145, 740);
        lv_obj_set_size(ui->screen_voice_btn_overload, 33, 20);
        lv_obj_set_pos(ui->screen_voice_btn_door, 403, 680);
        lv_obj_set_size(ui->screen_voice_btn_door, 41, 20);
        lv_obj_set_pos(ui->screen_voice_btn_link, 403, 740);
        lv_obj_set_size(ui->screen_voice_btn_link, 42, 20);
        lv_obj_set_pos(ui->screen_voice_btn_peak, 701, 680);
        lv_obj_set_size(ui->screen_voice_btn_peak, 45, 20);
        lv_obj_set_pos(ui->screen_voice_btn_appease, 641, 742);
        lv_obj_set_size(ui->screen_voice_btn_appease, 98, 20);

        lv_obj_set_pos(ui->screen_voice_slider_voice, 192, 85);
        lv_obj_set_size(ui->screen_voice_slider_voice, 80, 2);
        lv_obj_set_pos(ui->screen_voice_slider_lr, 180, 532);
        lv_obj_set_size(ui->screen_voice_slider_lr, 210, 2);
        lv_obj_set_pos(ui->screen_voice_slider_arrival, 538, 532);
        lv_obj_set_size(ui->screen_voice_slider_arrival, 197, 2);
        lv_obj_set_pos(ui->screen_voice_slider_close, 291, 591);
        lv_obj_set_size(ui->screen_voice_slider_close, 99, 2);
        lv_obj_set_pos(ui->screen_voice_slider_broadcast, 560, 591);
        lv_obj_set_size(ui->screen_voice_slider_broadcast, 175, 2);
        lv_obj_set_pos(ui->screen_voice_slider_firefighting, 145, 690);
        lv_obj_set_size(ui->screen_voice_slider_firefighting, 35, 2);
        lv_obj_set_pos(ui->screen_voice_slider_overload, 147, 750);
        lv_obj_set_size(ui->screen_voice_slider_overload, 33, 2);
        lv_obj_set_pos(ui->screen_voice_slider_door, 408, 690);
        lv_obj_set_size(ui->screen_voice_slider_door, 30, 2);
        lv_obj_set_pos(ui->screen_voice_slider_peak, 707, 690);
        lv_obj_set_size(ui->screen_voice_slider_peak, 28, 2);
        lv_obj_set_pos(ui->screen_voice_slider_link, 408, 750);
        lv_obj_set_size(ui->screen_voice_slider_link, 30, 2);
        lv_obj_set_pos(ui->screen_voice_slider_appease, 640, 750);
        lv_obj_set_size(ui->screen_voice_slider_appease, 95, 2);

        lv_obj_set_pos(ui->screen_voice_btn_music1, 230, 864);
        lv_obj_set_pos(ui->screen_voice_btn_music2, 456, 864);
        lv_obj_set_pos(ui->screen_voice_btn_music3, 606, 864);
    }
    if (music_state[0])
        lv_label_set_text(guider_ui.screen_voice_btn_music1_label, "" LV_SYMBOL_OK "");
    if (music_state[1])
        lv_label_set_text(guider_ui.screen_voice_btn_music2_label, "" LV_SYMBOL_OK "");
    if (music_state[2])
        lv_label_set_text(guider_ui.screen_voice_btn_music3_label, "" LV_SYMBOL_OK "");

    // The custom code of screen_voice.

    if (voice_password_timer == NULL)
        voice_password_timer = lv_timer_create(set_password_callback, 1000, 0);
    else
        lv_timer_resume(voice_password_timer);

    lv_obj_add_event_cb(ui->screen_voice_btn_music, music_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_music1, music_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_music2, music_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_music3, music_event_handler, LV_EVENT_ALL, ui);

    lv_obj_add_event_cb(ui->screen_voice_btn_work_time, work_time_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_password, keyboard_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_language, language_event_handler, LV_EVENT_ALL, ui);

    lv_obj_add_event_cb(ui->screen_voice_btn_voice, btn_voice_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_lr, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_close, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_arrival, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_broadcast, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_firefighting, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_overload, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_appease, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_peak, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_link, btn_switch_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_door, btn_switch_handler, LV_EVENT_ALL, ui);

    lv_obj_add_event_cb(ui->screen_voice_btnm_key, btnm_key_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_voice_btn_back, back_event_handler, LV_EVENT_ALL, ui);
    // Update current screen layout.
    lv_obj_update_layout(ui->screen_voice);

    // Init events for screen.
    events_init_screen_voice(ui);

    my_page = PAGE_VOICE;
}
