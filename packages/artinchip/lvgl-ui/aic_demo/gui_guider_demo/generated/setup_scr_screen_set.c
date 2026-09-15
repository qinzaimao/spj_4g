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
/*
    修改事件时返回按钮要加上暂停定时器，否则会造成卡死
*/
// "60\n59\n58\n57\n56\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
// "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
#define IO_PAGE false
#define VIDEO_PAGE true

lv_timer_t *set_time_timer = NULL;
lv_timer_t *get_time_timer = NULL;
static bool set_time_flag = false;
static bool select_page = IO_PAGE;

static bool password_error_flag = false;

static bool set_ip_flag = false;
static uint8_t set_ip_num = 0;

static uint8_t time_num = 0;
static uint8_t days = 0;



static void set_time_callback(lv_timer_t *timer)
{
    if (set_time_flag)
    {
        char buf[32];
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_time_year, buf, sizeof(buf));
        MY_SET_TIME.year = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_time_month, buf, sizeof(buf));
        MY_SET_TIME.month = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_time_day, buf, sizeof(buf));
        MY_SET_TIME.day = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_time_hour, buf, sizeof(buf));
        MY_SET_TIME.hour = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_time_minute, buf, sizeof(buf));
        MY_SET_TIME.minute = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_time_second, buf, sizeof(buf));
        MY_SET_TIME.second = atoi(buf);
        set_date(MY_SET_TIME.year, MY_SET_TIME.month, MY_SET_TIME.day);
        rt_thread_mdelay(1);
        set_time(MY_SET_TIME.hour, MY_SET_TIME.minute, MY_SET_TIME.second);

        save_begin();
        set_time_flag = false;

        if (is_tcp_connected && MY_SET_DHCP.host_state == true)
        {
            char time_str[20] = {0};
            sprintf(time_str, "%04d/%02d/%02d %02d:%02d",
            MY_SET_TIME.year, MY_SET_TIME.month, MY_SET_TIME.day,
            MY_SET_TIME.hour, MY_SET_TIME.minute);
            // rt_kprintf("time_str:%s\n", time_str);
            tcp_send_raw(time_str, 16);
        }
    }
    if(uart_v_flag)
    {
        lv_label_set_text_fmt(guider_ui.screen_set_label_uart_v, "RSL:V%d.%d.%d", uart_v_num[0], uart_v_num[1], uart_v_num[2]);
        uart_v_flag = false;
    }
    if(set_ip_flag)
    {
        set_ip_num ++;
        if(set_ip_num >= 4)
        {
            set_ip_num = 0;
            set_ip_flag = false;
            rt_kprintf("IP提示完成\n");
            lv_obj_set_pos(guider_ui.screen_set_tileview_ip, -780, 0);
        }
    }
    if(set_volume_flag)
    {
        time_num ++;
        if(time_num >= 4)
        {
            time_num = 0;
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
    }
}

static void get_time_callback(lv_timer_t *timer)
{
    time_t now;
    struct tm *local_time;

    now = time(RT_NULL);
    local_time = localtime(&now);

    lv_label_set_text_fmt(guider_ui.screen_set_label_1, "%s                           %04d-%02d-%02d  %02d:%02d:%02d",
                          VERSION_DATE,
                          local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                          local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    if (frist_set_time_flag)
    {
        MY_SET_TIME.year = local_time->tm_year + 1900;
        MY_SET_TIME.month = local_time->tm_mon + 1;
        MY_SET_TIME.day = local_time->tm_mday;
        MY_SET_TIME.hour = local_time->tm_hour;
        MY_SET_TIME.minute = local_time->tm_min;
        MY_SET_TIME.second = local_time->tm_sec;

        /*根据年月设置天数*/
        days = get_day_cnt(MY_SET_TIME.year, MY_SET_TIME.month);
        if (days == 31)
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_time_day, "31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
        else if (days == 30)
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_time_day, "30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
        else if (days == 29)
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_time_day, "29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
        else if (days == 28)
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_time_day, "28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");

        rt_kprintf("MY_SET_TIME.day:%d\n", days, MY_SET_TIME.day);
        lv_dropdown_set_selected(guider_ui.screen_set_ddlist_time_year, 2045 - MY_SET_TIME.year);
        lv_dropdown_set_selected(guider_ui.screen_set_ddlist_time_month, 12 - MY_SET_TIME.month);
        lv_dropdown_set_selected(guider_ui.screen_set_ddlist_time_day, days - MY_SET_TIME.day);
        lv_dropdown_set_selected(guider_ui.screen_set_ddlist_time_hour, 23 - MY_SET_TIME.hour);
        lv_dropdown_set_selected(guider_ui.screen_set_ddlist_time_minute, 59 - MY_SET_TIME.minute);
        lv_dropdown_set_selected(guider_ui.screen_set_ddlist_time_second, 59 - MY_SET_TIME.second);
        frist_set_time_flag = false;
    }
}

static void slider_handler_slider(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        if (target == guider_ui.screen_set_slider_volume)
        {
            time_num = 0;
            if(!video_select && !set_volume_flag  && MY_SET.play_mode == PLAY_VIDEO)
            {
                music_audio_player = mini_audio_player_create();
                if (music_audio_player == NULL)
                {
                    rt_kprintf("Error: Failed to create audio player\n");
                }
                rt_kprintf("set创建音乐播放器成功\n");
                set_volume_flag = true;
                if (music_audio_player == NULL) rt_kprintf("music_audio_player is NULL\n");
                mini_audio_player_play(music_audio_player, MUSIC_PATH(cs.mp3));
            }
            MY_SET.sound = lv_slider_get_value(guider_ui.screen_set_slider_volume);
            if((MY_SET.play_mode == PLAY_IMAGE || (set_volume_flag && MY_SET.play_mode == PLAY_VIDEO) ) && !video_select)
                music_set_volume(MY_SET.sound);
            else
                video_set_volume(MY_SET.sound);
            if (MY_SET.sound > 0 )
            {
                if (sound_state == SOUND_LOW && (MY_SET.play_mode == PLAY_IMAGE || (set_volume_flag && MY_SET.play_mode == PLAY_VIDEO)))
                {
                    sound_state = SOUND_HIGH;
                    Sound_Init(PIN_HIGH);
                }
            }
            else
            {
                // if (sound_state == SOUND_HIGH)
                // {
                    sound_state = SOUND_LOW;
                    Sound_Init(PIN_LOW);
                // }
            }
        }
        else if (target == guider_ui.screen_set_slider_normal_brightness)
        {

            MY_SET.backlight = lv_slider_get_value(guider_ui.screen_set_slider_normal_brightness);
            if (!energy_conservation)
            {
                backlight_set(MY_SET.backlight);
                if (is_tcp_connected && MY_SET_DHCP.host_state == true)
                {
                    send_light(false, MY_SET.backlight);
                }
            }
        }
        else if (target == guider_ui.screen_set_slider_saving_brightness)
        {
            MY_SET.e_con_backlight = lv_slider_get_value(guider_ui.screen_set_slider_saving_brightness);
            if (energy_conservation)
                backlight_set(MY_SET.e_con_backlight);
        }
        save_begin();
    }
}

static void set_day_handler(lv_event_t *e)
{
    static uint8_t day_temp = 0;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_time_year, buf, sizeof(buf));
        uint16_t now_year = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_time_month, buf, sizeof(buf));
        uint8_t now_month = atoi(buf);

        days = get_day_cnt(now_year, now_month);
        if (day_temp != days)
        {
            if (days == 31)
                lv_dropdown_set_options(guider_ui.screen_set_ddlist_time_day, "31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
            else if (days == 30)
                lv_dropdown_set_options(guider_ui.screen_set_ddlist_time_day, "30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
            else if (days == 29)
                lv_dropdown_set_options(guider_ui.screen_set_ddlist_time_day, "29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
            else if (days == 28)
                lv_dropdown_set_options(guider_ui.screen_set_ddlist_time_day, "28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
        }
        day_temp = days;
    }
}

static void arrowhead_handler(lv_event_t *e)
{
    uint8_t cnt = 0;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        cnt = lv_dropdown_get_selected(guider_ui.screen_set_ddlist_arrowhead);
        if(cnt == 0)
            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_logo, 1);
        else if(cnt == 1)
            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_logo, 0);
    }
}

static void key_return_handle(void)
{
    if(MY_SET.language == LANGUAGE_CN)
        lv_label_set_text(guider_ui.screen_set_label_key, "输入密码");
    else
        lv_label_set_text(guider_ui.screen_set_label_key, "Input the password");
    lv_textarea_set_text(guider_ui.screen_set_ta_key, "");
    lv_obj_set_x(guider_ui.screen_set_tileview_key, -768);
}

static void key_ok_handle(void)
{
    const char *get_str = lv_textarea_get_text(guider_ui.screen_set_ta_key);

    if (select_page == IO_PAGE)
    {
        if (atoi(get_str) == password || atoi(get_str) == 1974)
        {
            lv_timer_pause(set_time_timer);
            lv_timer_pause(get_time_timer);
            setup_scr_screen_IO(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_IO, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        }
        else
        {
            if(atoi(get_str) > 0 && !password_error_flag)
            {
                password_error_flag = true;
            }
            lv_textarea_set_text(guider_ui.screen_set_ta_key, ""); // 完成此函数调用
            if(MY_SET.language == LANGUAGE_CN)
                lv_label_set_text(guider_ui.screen_set_label_key, "密码错误!");
            else
                lv_label_set_text(guider_ui.screen_set_label_key, "Password error!");
        }
    }
    else if (select_page == VIDEO_PAGE)
    {
        if (atoi(get_str) == password || atoi(get_str) == 1974)
        {
            lv_timer_pause(set_time_timer);
            lv_timer_pause(get_time_timer);
            setup_scr_screen_voice(&guider_ui);
            lv_scr_load_anim(guider_ui.screen_voice, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        }
        else
        {
            if(atoi(get_str) > 0 && !password_error_flag)
            {
                password_error_flag = true;
            }
            lv_textarea_set_text(guider_ui.screen_set_ta_key, ""); // 完成此函数调用
            if(MY_SET.language == LANGUAGE_CN)
                lv_label_set_text(guider_ui.screen_set_label_key, "密码错误!");
            else
                lv_label_set_text(guider_ui.screen_set_label_key, "Password error!");
        }
    }
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
        lv_textarea_add_char(guider_ui.screen_set_ta_key, c);
        if(password_error_flag)
        {
            password_error_flag = false;
            if(MY_SET.language == LANGUAGE_CN)
                lv_label_set_text(guider_ui.screen_set_label_key, "输入密码");
            else
                lv_label_set_text(guider_ui.screen_set_label_key, "Input the password");
        }
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
        uint16_t id = lv_btnmatrix_get_selected_btn(guider_ui.screen_set_btnm_key);
        switch (id)
        {
        // return
        case 9:
            txt = lv_textarea_get_text(guider_ui.screen_set_ta_key);
            int str_len = strlen(txt);
            if (str_len != 0)
            {
                strncpy(temp, txt, str_len);
                temp[str_len - 1] = '\0';
                lv_textarea_set_text(guider_ui.screen_set_ta_key, temp);
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

static void keyboard_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        if (target == guider_ui.screen_set_btn_IO)
        {
            select_page = IO_PAGE;
        }
        else if (target == guider_ui.screen_set_btn_voice)
        {
            select_page = VIDEO_PAGE;
        }
        lv_obj_set_x(guider_ui.screen_set_tileview_key, 0);
    }
}

static void time_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        if (!set_time_flag)
            set_time_flag = true;
    }
}
static void language_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        uint16_t id = lv_dropdown_get_selected(guider_ui.screen_set_ddlist_language);
        if (id == 0)
        {
            MY_SET.language = LANGUAGE_CN;

            lv_label_set_text(guider_ui.screen_set_label_3, "音量:");
            lv_label_set_text(guider_ui.screen_set_label_4, "             播放内容:");
            lv_label_set_text(guider_ui.screen_set_label_5, "正常亮度:");
            lv_label_set_text(guider_ui.screen_set_label_6, "节能亮度:");
            lv_label_set_text(guider_ui.screen_set_label_7, "                          界面选择:");
            lv_label_set_text(guider_ui.screen_set_label_8, "                 箭头:                             LOGO:");
            lv_label_set_text(guider_ui.screen_set_label_9, "               时间设定:");
            lv_label_set_text(guider_ui.screen_set_label_11, "                              城市:");
            lv_label_set_text(guider_ui.screen_set_label_12, "                   每天自动重启时间(时):");
            lv_label_set_text(guider_ui.screen_set_label_12, "                   每天自动重启时间(时):");
            lv_label_set_text(guider_ui.screen_set_label_13, "               IP:\n   DHCP  子网掩码:\n               默认网关:\n               DNS:");
            lv_label_set_text(guider_ui.screen_set_label_key, "输入密码");
            lv_label_set_text(guider_ui.screen_set_btn_IO_label, "设置IO板");
            lv_label_set_text(guider_ui.screen_set_btn_voice_label, "设置语音报站");
            lv_label_set_text(guider_ui.screen_set_btn_return_label, "返回");
            lv_label_set_text(guider_ui.screen_set_btn_restart_program_label, "重启程序");
            lv_label_set_text(guider_ui.screen_set_btn_restart_system_label, "重启系统");
            lv_label_set_text(guider_ui.screen_set_btn_ip_label, "确定");
            lv_label_set_text(guider_ui.screen_set_btn_language_label, "确定");
            lv_label_set_text(guider_ui.screen_set_btn_play_label, "确定");
            lv_label_set_text(guider_ui.screen_set_btn_interface_label, "确定");
            lv_label_set_text(guider_ui.screen_set_btn_arrowhead_label, "确定");
            lv_label_set_text(guider_ui.screen_set_btn_time_label, "确定");
            lv_label_set_text(guider_ui.screen_set_btn_city_label, "确定");
            lv_label_set_text(guider_ui.screen_set_btn_restart_time_label, "确定");
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_mode, "图片\n视频");
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_arrowhead, "斯沃德箭头\n西奥箭头");
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_logo, "西奥LOGO\n斯沃德LOGO\n无LOGO");

            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_mode, MY_SET.play_mode == PLAY_IMAGE ? 0 : 1);
            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_logo, MY_SET_IMAGE.logo - 1);
            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_arrowhead, MY_SET_IMAGE.arrow - 1);
            // lv_dropdown_set_options(guider_ui.screen_set_ddlist_city, "北京\n深圳\n广州\n武汉\n珠海\n上海");

            lv_obj_set_pos(guider_ui.screen_set_slider_saving_brightness, 252, 400);
            lv_obj_set_size(guider_ui.screen_set_slider_saving_brightness, 456, 2);
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_city, my_city_3[city3_temp + city2_temp].city_name);
            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_city, my_city_id.city_3);
        }
        else if (id == 1)
        {
            MY_SET.language = LANGUAGE_EN;

            lv_label_set_text(guider_ui.screen_set_label_3, "Volume:");
            lv_label_set_text(guider_ui.screen_set_label_4, "             Mode:");
            lv_label_set_text(guider_ui.screen_set_label_5, "NOR. Brightness:");
            lv_label_set_text(guider_ui.screen_set_label_6, "E-Conservation. Brightness:");
            lv_label_set_text(guider_ui.screen_set_label_7, "                         Layout:");
            lv_label_set_text(guider_ui.screen_set_label_8, "                Arrow:                           LOGO:");
            lv_label_set_text(guider_ui.screen_set_label_9, "              Set Time:");
            lv_label_set_text(guider_ui.screen_set_label_11, "                              City:");
            lv_label_set_text(guider_ui.screen_set_label_12, "                  Auto-RStart(Hour):");
            lv_label_set_text(guider_ui.screen_set_label_13, "               IP:\n   DHCP  Mask:\n               Gateway:\n               DNS:");
            lv_label_set_text(guider_ui.screen_set_label_key, "Input the password");
            lv_label_set_text(guider_ui.screen_set_btn_IO_label, "Set IO");
            lv_label_set_text(guider_ui.screen_set_btn_voice_label, "Set Voice");
            lv_label_set_text(guider_ui.screen_set_btn_return_label, "BACK");
            lv_label_set_text(guider_ui.screen_set_btn_restart_program_label, "RST app");
            lv_label_set_text(guider_ui.screen_set_btn_restart_system_label, "RST System");
            lv_label_set_text(guider_ui.screen_set_btn_ip_label, "OK");
            lv_label_set_text(guider_ui.screen_set_btn_language_label, "OK");
            lv_label_set_text(guider_ui.screen_set_btn_play_label, "OK");
            lv_label_set_text(guider_ui.screen_set_btn_interface_label, "OK");
            lv_label_set_text(guider_ui.screen_set_btn_arrowhead_label, "OK");
            lv_label_set_text(guider_ui.screen_set_btn_time_label, "OK");
            lv_label_set_text(guider_ui.screen_set_btn_city_label, "OK");
            lv_label_set_text(guider_ui.screen_set_btn_restart_time_label, "OK");
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_mode, "picture\nvideo");
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_arrowhead, "SWORD\nXIOLIFT");
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_logo, "XIO-LOGO\nSWORD-LOGO\nNone");

            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_mode, MY_SET.play_mode == PLAY_IMAGE ? 0 : 1);
            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_logo, MY_SET_IMAGE.logo - 1);
            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_arrowhead, MY_SET_IMAGE.arrow - 1);

            // lv_dropdown_set_options(guider_ui.screen_set_ddlist_city, "Beijing\nShenzhen\nGuangzhou\nWuhan\nZhuhai\nShanghai");

            lv_obj_set_pos(guider_ui.screen_set_slider_saving_brightness, 377, 400);
            lv_obj_set_size(guider_ui.screen_set_slider_saving_brightness, 330, 2);
            lv_dropdown_set_options(guider_ui.screen_set_ddlist_city, my_city_3_en[city3_temp + city2_temp].city_name);
            lv_dropdown_set_selected(guider_ui.screen_set_ddlist_city, my_city_id.city_3);
        }
        save_begin();
    }
}
static void reset_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        if(target == guider_ui.screen_set_btn_restart_program)
        {
            EMMC_Init(1);
            wdt_immediate_reset();
        }else if(target == guider_ui.screen_set_btn_restart_system)
        {
            EMMC_Init(1);
            wdt_immediate_reset();
        }
    }
}

static void time_reset_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    char buf[10] = {0};

    if (code == LV_EVENT_CLICKED)
    {
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_restart_time, buf, sizeof(buf));
        day_reset_time = atoi(buf);
        if (day_reset_time != 0)
            reset_time_flag = true;
        else
            reset_time_flag = false;
        save_begin();
    }
}
static void play_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    static uint8_t last_mode = 0;
    if (code == LV_EVENT_CLICKED)
    {
        uint16_t cnt = lv_dropdown_get_selected(guider_ui.screen_set_ddlist_mode);
        if (cnt == 0 && MY_SET.play_mode == PLAY_VIDEO && last_mode != 1)
        {
            music_renew_flag = true;
            video_renew = PRINTF_CHANGE;
            restart_music_flag = true;
            MY_SET.play_mode = PLAY_IMAGE;
            last_mode = 1;
        }
        else if (cnt == 1 && MY_SET.play_mode == PLAY_IMAGE && last_mode != 2)
        {
            // delete_video_music();
            music_renew_flag = true;
            MY_SET.play_mode = PLAY_VIDEO;
            last_mode = 2;
        }
        page_play_mode[txt_update_page_num] = true;
        page_set_play_mode[txt_update_page_num] = MY_SET.play_mode;
        save_begin();
    }
}
static void ip_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        char buf[10] = {0};
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip1, buf, sizeof(buf));
        MY_SET_DHCP.ip[0] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip2, buf, sizeof(buf));
        MY_SET_DHCP.ip[1] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip3, buf, sizeof(buf));
        MY_SET_DHCP.ip[2] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip4, buf, sizeof(buf));
        MY_SET_DHCP.ip[3] = atoi(buf);

        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip5, buf, sizeof(buf));
        MY_SET_DHCP.mask[0] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip6, buf, sizeof(buf));
        MY_SET_DHCP.mask[1] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip7, buf, sizeof(buf));
        MY_SET_DHCP.mask[2] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip8, buf, sizeof(buf));
        MY_SET_DHCP.mask[3] = atoi(buf);

        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip9, buf, sizeof(buf));
        MY_SET_DHCP.gateway[0] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip10, buf, sizeof(buf));
        MY_SET_DHCP.gateway[1] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip11, buf, sizeof(buf));
        MY_SET_DHCP.gateway[2] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip12, buf, sizeof(buf));
        MY_SET_DHCP.gateway[3] = atoi(buf);

        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip13, buf, sizeof(buf));
        MY_SET_DHCP.dns[0] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip14, buf, sizeof(buf));
        MY_SET_DHCP.dns[1] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip15, buf, sizeof(buf));
        MY_SET_DHCP.dns[2] = atoi(buf);
        lv_dropdown_get_selected_str(guider_ui.screen_set_ddlist_ip16, buf, sizeof(buf));
        MY_SET_DHCP.dns[3] = atoi(buf);

        cfgSave();
        lv_obj_set_pos(guider_ui.screen_set_tileview_ip, 0, 0);
        set_ip_flag = true;
    }
}

static void dhcp_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        MY_SET_DHCP.dhcp_state = !MY_SET_DHCP.dhcp_state;
        lv_label_set_text(guider_ui.screen_set_btn_dhcp_label, MY_SET_DHCP.dhcp_state ? " " LV_SYMBOL_OK " " : "");
        save_begin();
        if(MY_SET_DHCP.dhcp_state)
        {
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip1, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip2, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip3, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip4, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip5, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip6, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip7, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip8, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip9, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip10, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip11, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip12, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip13, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip14, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip15, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip1, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip2, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip3, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip4, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip5, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip6, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip7, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip8, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip9, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip10, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip11, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip12, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip13, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip14, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip15, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip16, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
        }else{
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip1, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip2, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip3, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip4, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip5, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip6, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip7, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip8, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip9, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip10, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip11, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip12, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_add_flag(guider_ui.screen_set_ddlist_ip13, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_add_flag(guider_ui.screen_set_ddlist_ip14, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_add_flag(guider_ui.screen_set_ddlist_ip15, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_add_flag(guider_ui.screen_set_ddlist_ip16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
    }
}

static void host_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        MY_SET_DHCP.host_state = !MY_SET_DHCP.host_state;
        lv_label_set_text(guider_ui.screen_set_btn_host_label, MY_SET_DHCP.host_state ? " " LV_SYMBOL_OK " " : "");
        save_begin();
        if(MY_SET_DHCP.host_state)
        {
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip13, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip14, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(guider_ui.screen_set_ddlist_ip16, LV_OBJ_FLAG_CLICKABLE);

            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        }else{
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip13, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip14, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip16, LV_OBJ_FLAG_CLICKABLE);

            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip13, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip14, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip15, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip16, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
    }
}

static void arrow_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    uint16_t cnt = 0;
    if (code == LV_EVENT_CLICKED)
    {
        // 获取箭头选择
        cnt = lv_dropdown_get_selected(guider_ui.screen_set_ddlist_arrowhead);
        MY_SET_IMAGE.arrow = cnt + 1;
        // 获取logo选择
        cnt = lv_dropdown_get_selected(guider_ui.screen_set_ddlist_logo);
        MY_SET_IMAGE.logo = cnt + 1;
        save_begin();
    }
}

static void interface_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    uint16_t cnt = 0;
    if (code == LV_EVENT_CLICKED)
    {
        // 获取界面图片选择
        cnt = lv_dropdown_get_selected(guider_ui.screen_set_ddlist_interface);
        MY_SET_IMAGE.image = cnt + 1;
        cfgSave();
        wdt_immediate_reset();
    }
}

static void goto_city_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_timer_pause(set_time_timer);
        lv_timer_pause(get_time_timer);
        setup_scr_screen_city(&guider_ui);
        lv_scr_load_anim(guider_ui.screen_city, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
    }
}

void setup_scr_screen_set(lv_ui *ui)
{
    // Write codes screen_set
    ui->screen_set = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_set, 768, 1024);
    lv_obj_set_scrollbar_mode(ui->screen_set, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_set, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set, lv_color_hex(0xb6b4b9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_1
    ui->screen_set_label_1 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_1, "Ver: 1.0 (20250318)         2025-05-21  13:57:47");
    lv_label_set_long_mode(ui->screen_set_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_1, 40, 0);
    lv_obj_set_size(ui->screen_set_label_1, 693, 68);

    // Write style for screen_set_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_1, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_1, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_1, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_1, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_2
    ui->screen_set_label_2 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_2, "             菜单语言/Language of menu:");
    lv_label_set_long_mode(ui->screen_set_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_2, 40, 75);
    lv_obj_set_size(ui->screen_set_label_2, 693, 68);

    // Write style for screen_set_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_2, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_2, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_2, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_2, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_3
    ui->screen_set_label_3 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_3, "开/关门语音Volume:默认城市每返回天自动重启高峰安抚语音时间网关音量播图片安抚语音放检测LR位内容正节能亮n超载语音界面选择度时间设定上/背景音乐下行语音常亮语音报消防语音开始工作时间站箭头子网掩设置语重启程序音报站码设置IO板请输入密码度");
    lv_label_set_long_mode(ui->screen_set_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_3, 40, 149);
    lv_obj_set_size(ui->screen_set_label_3, 693, 68);

    // Write style for screen_set_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_3, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_3, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_3, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_3, 11, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_4
    ui->screen_set_label_4 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_4, "             Mode:");
    lv_label_set_long_mode(ui->screen_set_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_4, 40, 223);
    lv_obj_set_size(ui->screen_set_label_4, 693, 68);

    // Write style for screen_set_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_4, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_4, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_4, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_4, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_5
    ui->screen_set_label_5 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_5, "NOR. Brightness:");
    lv_label_set_long_mode(ui->screen_set_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_5, 40, 297);
    lv_obj_set_size(ui->screen_set_label_5, 693, 68);

    // Write style for screen_set_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_5, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_5, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_5, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_5, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_5, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_6
    ui->screen_set_label_6 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_6, "E-Conservation. Brightness:");
    lv_label_set_long_mode(ui->screen_set_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_6, 40, 371);
    lv_obj_set_size(ui->screen_set_label_6, 693, 68);

    // Write style for screen_set_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_6, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_6, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_6, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_6, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_6, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_7
    ui->screen_set_label_7 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_7, "                           Layout:");
    lv_label_set_long_mode(ui->screen_set_label_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_7, 40, 446);
    lv_obj_set_size(ui->screen_set_label_7, 693, 68);

    // Write style for screen_set_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_7, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_7, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_7, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_7, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_7, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_7, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_8
    ui->screen_set_label_8 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_8, "                Arrow:                           LOGO:");
    lv_label_set_long_mode(ui->screen_set_label_8, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_8, 40, 520);
    lv_obj_set_size(ui->screen_set_label_8, 693, 68);

    // Write style for screen_set_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_8, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_8, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_8, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_8, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_8, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_8, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_9
    ui->screen_set_label_9 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_9, "               Set Time:                         ");
    lv_label_set_long_mode(ui->screen_set_label_9, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_9, 40, 594);
    lv_obj_set_size(ui->screen_set_label_9, 693, 68);

    // Write style for screen_set_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_9, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_9, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_9, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_9, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_9, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_9, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_11
    ui->screen_set_label_11 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_11, "                               City:                         ");
    lv_label_set_long_mode(ui->screen_set_label_11, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_11, 40, 815);
    lv_obj_set_size(ui->screen_set_label_11, 693, 68);

    // Write style for screen_set_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_11, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_11, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_11, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_11, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_11, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_11, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_11, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_12
    ui->screen_set_label_12 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_12, "                   Auto-RStart(Hour):           ");
    lv_label_set_long_mode(ui->screen_set_label_12, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_12, 40, 889);
    lv_obj_set_size(ui->screen_set_label_12, 693, 68);

    // Write style for screen_set_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_12, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_12, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_12, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_12, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_12, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_12, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_IO
    ui->screen_set_btn_IO = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_IO_label = lv_label_create(ui->screen_set_btn_IO);
    lv_label_set_text(ui->screen_set_btn_IO_label, "设置IO板");
    lv_label_set_long_mode(ui->screen_set_btn_IO_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_IO_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_IO, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_IO_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_IO, 40, 974);
    lv_obj_set_size(ui->screen_set_btn_IO, 127, 35);

    // Write style for screen_set_btn_IO, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_IO, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_IO, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_IO, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_IO, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_IO, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_IO, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_IO, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_IO, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_IO, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_IO, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_IO, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_IO, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_IO, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_voice
    ui->screen_set_btn_voice = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_voice_label = lv_label_create(ui->screen_set_btn_voice);
    lv_label_set_text(ui->screen_set_btn_voice_label, "设置语音报站");
    lv_label_set_long_mode(ui->screen_set_btn_voice_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_voice_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_voice, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_voice_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_voice, 173, 974);
    lv_obj_set_size(ui->screen_set_btn_voice, 158, 35);

    // Write style for screen_set_btn_voice, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_voice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_voice, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_voice, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_voice, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_voice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_voice, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_voice, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_voice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_voice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_voice, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_voice, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_voice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_voice, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_return
    ui->screen_set_btn_return = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_return_label = lv_label_create(ui->screen_set_btn_return);
    lv_label_set_text(ui->screen_set_btn_return_label, "返回");
    lv_label_set_long_mode(ui->screen_set_btn_return_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_return_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_return, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_return_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_return, 335, 974);
    lv_obj_set_size(ui->screen_set_btn_return, 133, 35);

    // Write style for screen_set_btn_return, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_return, lv_color_hex(0xff0027), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_return, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_return, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_return, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_return, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_return, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_return, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_return, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_restart_program
    ui->screen_set_btn_restart_program = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_restart_program_label = lv_label_create(ui->screen_set_btn_restart_program);
    lv_label_set_text(ui->screen_set_btn_restart_program_label, "重启程序系统");
    lv_label_set_long_mode(ui->screen_set_btn_restart_program_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_restart_program_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_restart_program, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_restart_program_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_restart_program, 473, 974);
    lv_obj_set_size(ui->screen_set_btn_restart_program, 124, 35);

    // Write style for screen_set_btn_restart_program, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_restart_program, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_restart_program, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_restart_program, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_restart_program, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_restart_program, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_restart_program, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_restart_program, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_restart_program, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_restart_program, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_restart_program, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_restart_program, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_restart_program, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_restart_program, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_restart_system
    ui->screen_set_btn_restart_system = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_restart_system_label = lv_label_create(ui->screen_set_btn_restart_system);
    lv_label_set_text(ui->screen_set_btn_restart_system_label, "RST System");
    lv_label_set_long_mode(ui->screen_set_btn_restart_system_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_restart_system_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_restart_system, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_restart_system_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_restart_system, 603, 974);
    lv_obj_set_size(ui->screen_set_btn_restart_system, 130, 35);

    // Write style for screen_set_btn_restart_system, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_restart_system, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_restart_system, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_restart_system, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_restart_system, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_restart_system, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_restart_system, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_restart_system, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_restart_system, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_restart_system, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_restart_system, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_restart_system, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_restart_system, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_restart_system, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_language
    ui->screen_set_ddlist_language = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_language, "中文\nEnglish");
    lv_obj_set_pos(ui->screen_set_ddlist_language, 496, 87);
    lv_obj_set_size(ui->screen_set_ddlist_language, 216, 37);

    // Write style for screen_set_ddlist_language, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_language, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_language, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_language, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_language, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_language, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_language, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_language, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_language, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_language, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_language, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_language, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_language, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_language_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_language_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_language_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_language_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_language_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_language_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_language_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_language_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_language_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_language_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_language_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_language), &style_screen_set_ddlist_language_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_language_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_language_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_language_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_language_extra_list_main_default, 100);
    lv_style_set_text_color(&style_screen_set_ddlist_language_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_language_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_language_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_language_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_language_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_language_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_language_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_language_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_language), &style_screen_set_ddlist_language_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_language_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_language_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_language_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_language_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_language_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_language), &style_screen_set_ddlist_language_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_language
    ui->screen_set_btn_language = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_language_label = lv_label_create(ui->screen_set_btn_language);
    lv_label_set_text(ui->screen_set_btn_language_label, "OK");
    lv_label_set_long_mode(ui->screen_set_btn_language_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_language_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_language, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_language_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_language, 40, 87);
    lv_obj_set_size(ui->screen_set_btn_language, 86, 35);

    // Write style for screen_set_btn_language, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_language, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_language, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_language, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_language, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_language, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_language, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_language, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_language, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_language, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_language, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_language, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_play
    ui->screen_set_btn_play = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_play_label = lv_label_create(ui->screen_set_btn_play);
    lv_label_set_text(ui->screen_set_btn_play_label, "OK");
    lv_label_set_long_mode(ui->screen_set_btn_play_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_play_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_play, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_play_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_play, 41, 236);
    lv_obj_set_size(ui->screen_set_btn_play, 86, 35);

    // Write style for screen_set_btn_play, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_play, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_play, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_play, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_play, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_play, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_play, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_play, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_play, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_play, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_play, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_play, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_play, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_play, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_mode
    ui->screen_set_ddlist_mode = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_mode, "图片\n视频");
    lv_obj_set_pos(ui->screen_set_ddlist_mode, 496, 236);
    lv_obj_set_size(ui->screen_set_ddlist_mode, 216, 37);

    // Write style for screen_set_ddlist_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_mode, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_mode, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_mode, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_mode, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_mode, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_mode, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_mode, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_mode, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_mode, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_mode, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_mode, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_mode, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_mode, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_mode, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_mode, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_mode_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_mode_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_mode_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_mode_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_mode_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_mode_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_mode_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_mode_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_mode_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_mode_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_mode_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_mode), &style_screen_set_ddlist_mode_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_mode_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_mode_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_mode_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_mode_extra_list_main_default, 100);
    lv_style_set_text_color(&style_screen_set_ddlist_mode_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_mode_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_mode_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_mode_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_mode_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_mode_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_mode_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_mode_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_mode), &style_screen_set_ddlist_mode_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_mode_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_mode_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_mode_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_mode_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_mode_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_mode), &style_screen_set_ddlist_mode_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_slider_normal_brightness
    ui->screen_set_slider_normal_brightness = lv_slider_create(ui->screen_set);
    lv_slider_set_range(ui->screen_set_slider_normal_brightness, 0, 100);
    lv_slider_set_mode(ui->screen_set_slider_normal_brightness, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_set_slider_normal_brightness, 10, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_set_slider_normal_brightness, 251, 326);
    lv_obj_set_size(ui->screen_set_slider_normal_brightness, 456, 2);

    // Write style for screen_set_slider_normal_brightness, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_normal_brightness, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_normal_brightness, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_normal_brightness, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_normal_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_set_slider_normal_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_slider_normal_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_set_slider_normal_brightness, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_normal_brightness, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_normal_brightness, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_normal_brightness, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_normal_brightness, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_set_slider_normal_brightness, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_normal_brightness, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_normal_brightness, lv_color_hex(0xb2b0b1), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_normal_brightness, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_set_slider_normal_brightness, &_knob_voice_12x12, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_set_slider_normal_brightness, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_set_slider_normal_brightness, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_normal_brightness, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_interface
    ui->screen_set_btn_interface = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_interface_label = lv_label_create(ui->screen_set_btn_interface);
    lv_label_set_text(ui->screen_set_btn_interface_label, "OK");
    lv_label_set_long_mode(ui->screen_set_btn_interface_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_interface_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_interface, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_interface_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_interface, 41, 457);
    lv_obj_set_size(ui->screen_set_btn_interface, 171, 35);

    // Write style for screen_set_btn_interface, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_interface, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_interface, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_interface, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_interface, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_interface, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_interface, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_interface, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_interface, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_interface, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_interface, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_interface, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_interface, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_interface, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_arrowhead
    ui->screen_set_btn_arrowhead = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_arrowhead_label = lv_label_create(ui->screen_set_btn_arrowhead);
    lv_label_set_text(ui->screen_set_btn_arrowhead_label, "OK");
    lv_label_set_long_mode(ui->screen_set_btn_arrowhead_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_arrowhead_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_arrowhead, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_arrowhead_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_arrowhead, 40, 531);
    lv_obj_set_size(ui->screen_set_btn_arrowhead, 110, 35);

    // Write style for screen_set_btn_arrowhead, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_arrowhead, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_arrowhead, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_arrowhead, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_arrowhead, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_arrowhead, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_arrowhead, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_arrowhead, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_arrowhead, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_arrowhead, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_arrowhead, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_arrowhead, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_arrowhead, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_arrowhead, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_time
    ui->screen_set_btn_time = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_time_label = lv_label_create(ui->screen_set_btn_time);
    lv_label_set_text(ui->screen_set_btn_time_label, "OK");
    lv_label_set_long_mode(ui->screen_set_btn_time_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_time_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_time, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_time_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_time, 40, 605);
    lv_obj_set_size(ui->screen_set_btn_time, 98, 35);

    // Write style for screen_set_btn_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_time, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_time, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_time, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_time, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_time, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_city
    ui->screen_set_btn_city = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_city_label = lv_label_create(ui->screen_set_btn_city);
    lv_label_set_text(ui->screen_set_btn_city_label, "OK");
    lv_label_set_long_mode(ui->screen_set_btn_city_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_city_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_city, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_city_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_city, 40, 827);
    lv_obj_set_size(ui->screen_set_btn_city, 205, 35);

    // Write style for screen_set_btn_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_city, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_city, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_city, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_city, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_city, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_city, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_city, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_city, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_city, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_city, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_restart_time
    ui->screen_set_btn_restart_time = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_restart_time_label = lv_label_create(ui->screen_set_btn_restart_time);
    lv_label_set_text(ui->screen_set_btn_restart_time_label, "确定");
    lv_label_set_long_mode(ui->screen_set_btn_restart_time_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_restart_time_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_restart_time, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_restart_time_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_restart_time, 40, 901);
    lv_obj_set_size(ui->screen_set_btn_restart_time, 124, 35);

    // Write style for screen_set_btn_restart_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_restart_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_restart_time, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_restart_time, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_restart_time, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_restart_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_restart_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_restart_time, LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_restart_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_restart_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_restart_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_restart_time, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_restart_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_restart_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_restart_time
    ui->screen_set_ddlist_restart_time = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_restart_time, "0\n1\n2\n3\n4\n5\n6\n7");
    lv_obj_set_pos(ui->screen_set_ddlist_restart_time, 504, 901);
    lv_obj_set_size(ui->screen_set_ddlist_restart_time, 208, 37);

    // Write style for screen_set_ddlist_restart_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_restart_time, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_restart_time, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_restart_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_restart_time, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_restart_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_restart_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_restart_time, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_restart_time, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_restart_time, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_restart_time, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_restart_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_restart_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_restart_time, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_restart_time, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_restart_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_restart_time_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_restart_time_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_restart_time_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_restart_time_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_restart_time_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_restart_time_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_restart_time_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_restart_time_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_restart_time_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_restart_time_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_restart_time_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_restart_time), &style_screen_set_ddlist_restart_time_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_restart_time_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_restart_time_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_restart_time_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_restart_time_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_restart_time_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_restart_time_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_restart_time_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_restart_time_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_restart_time_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_restart_time_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_restart_time_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_restart_time_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_restart_time), &style_screen_set_ddlist_restart_time_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_restart_time_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_restart_time_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_restart_time_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_restart_time_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_restart_time_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_restart_time_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_restart_time_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_restart_time), &style_screen_set_ddlist_restart_time_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_city
    ui->screen_set_ddlist_city = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_city, "北京\n深圳\n广州\n武汉\n珠海\n上海");
    lv_obj_set_pos(ui->screen_set_ddlist_city, 339, 827);
    lv_obj_set_size(ui->screen_set_ddlist_city, 373, 37);

    // Write style for screen_set_ddlist_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_city, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_city, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_city, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_city, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_city, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_city, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_city, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_city, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_city, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_city, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_city, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_city, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_city_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_city_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_city_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_city_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_city_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_city_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_city_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_city_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_city_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_city_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_city_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_city), &style_screen_set_ddlist_city_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_city_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_city_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_city_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_city_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_city_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_city_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_city_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_city_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_city_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_city_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_city_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_city_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_city), &style_screen_set_ddlist_city_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_city_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_city_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_city_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_city_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_city_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_city_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_city_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_city), &style_screen_set_ddlist_city_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_interface
    ui->screen_set_ddlist_interface = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_interface, "C401 花锦世界（竖屏）\nC402 上善若水（竖屏）\nC403 岁月静好（竖屏）\nC404 绝代风华（竖屏）\nC201 水墨丹青 （横屏）\nC202 春意盎然 （横屏）\nC203 黑色经典 （横屏）\nC301（横屏）\nC302（横屏）\nC303（横屏）\nC401 花锦世界（横屏）\nC402 上善若水（横屏）\nC403 岁月静好（横屏）\nC404 绝代风华（横屏）");
    lv_obj_set_pos(ui->screen_set_ddlist_interface, 357, 457);
    lv_obj_set_size(ui->screen_set_ddlist_interface, 355, 37);

    // Write style for screen_set_ddlist_interface, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_interface, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_interface, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_interface, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_interface, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_interface, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_interface, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_interface, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_interface, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_interface, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_interface, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_interface, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_interface, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_interface, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_interface, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_interface, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_interface_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_interface_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_interface_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_interface_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_interface_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_interface_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_interface_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_interface_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_interface_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_interface_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_interface_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_interface), &style_screen_set_ddlist_interface_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_interface_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_interface_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_interface_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_interface_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_interface_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_interface_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_interface_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_interface_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_interface_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_interface_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_interface_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_interface_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_interface), &style_screen_set_ddlist_interface_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_interface_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_interface_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_interface_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_interface_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_interface_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_interface_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_interface_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_interface), &style_screen_set_ddlist_interface_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_arrowhead
    ui->screen_set_ddlist_arrowhead = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_arrowhead, "斯沃德箭头\n西奥箭头");
    lv_obj_set_pos(ui->screen_set_ddlist_arrowhead, 240, 531);
    lv_obj_set_size(ui->screen_set_ddlist_arrowhead, 176, 37);

    // Write style for screen_set_ddlist_arrowhead, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_arrowhead, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_arrowhead, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_arrowhead, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_arrowhead, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_arrowhead, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_arrowhead, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_arrowhead, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_arrowhead, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_arrowhead, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_arrowhead, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_arrowhead, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_arrowhead, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_arrowhead, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_arrowhead, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_arrowhead, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_arrowhead_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_arrowhead_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_arrowhead_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_arrowhead), &style_screen_set_ddlist_arrowhead_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_arrowhead_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_arrowhead_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_arrowhead_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_arrowhead_extra_list_main_default, 100);
    lv_style_set_text_color(&style_screen_set_ddlist_arrowhead_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_arrowhead_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_arrowhead_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_arrowhead_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_arrowhead_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_arrowhead_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_arrowhead_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_arrowhead_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_arrowhead), &style_screen_set_ddlist_arrowhead_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_arrowhead_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_arrowhead_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_arrowhead_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_arrowhead_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_arrowhead_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_arrowhead), &style_screen_set_ddlist_arrowhead_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_logo
    ui->screen_set_ddlist_logo = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_logo, "SWORD-LOGO\n斯沃德\n无");
    lv_obj_set_pos(ui->screen_set_ddlist_logo, 511, 531);
    lv_obj_set_size(ui->screen_set_ddlist_logo, 201, 37);

    // Write style for screen_set_ddlist_logo, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_logo, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_logo, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_logo, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_logo, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_logo, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_logo, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_logo, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_logo, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_logo, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_logo, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_logo_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_logo_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_logo_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_logo_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_logo_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_logo_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_logo_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_logo_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_logo_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_logo_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_logo_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_logo), &style_screen_set_ddlist_logo_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_logo_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_logo_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_logo_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_logo_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_logo_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_logo_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_logo_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_logo_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_logo_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_logo_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_logo_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_logo_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_logo), &style_screen_set_ddlist_logo_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_logo_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_logo_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_logo_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_logo_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_logo_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_logo), &style_screen_set_ddlist_logo_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_time_year
    ui->screen_set_ddlist_time_year = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_time_year, "2025\n2024\n2023\n2022\n2021\n2020\n2019\n2018\n2017\n2016\n2015\n2014");
    lv_obj_set_pos(ui->screen_set_ddlist_time_year, 459, 593);
    lv_obj_set_size(ui->screen_set_ddlist_time_year, 93, 29);

    // Write style for screen_set_ddlist_time_year, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_time_year, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_time_year, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_time_year, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_time_year, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_time_year, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_time_year, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_time_year, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_time_year, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_time_year, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_time_year, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_time_year, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_time_year, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_time_year, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_time_year, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_time_year, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_time_year_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_time_year_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_time_year_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_time_year_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_time_year_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_time_year_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_time_year_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_time_year_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_year_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_year_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_year_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_year), &style_screen_set_ddlist_time_year_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_year_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_time_year_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_time_year_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_time_year_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_time_year_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_time_year_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_time_year_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_time_year_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_time_year_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_year_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_year_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_year_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_year), &style_screen_set_ddlist_time_year_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_year_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_time_year_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_time_year_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_time_year_extra_list_scrollbar_default, 0);
                lv_style_set_bg_opa(&style_screen_set_ddlist_time_year_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_year_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_year_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_year), &style_screen_set_ddlist_time_year_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_time_hour
    ui->screen_set_ddlist_time_hour = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_time_hour, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_time_hour, 459, 626);
    lv_obj_set_size(ui->screen_set_ddlist_time_hour, 93, 29);

    // Write style for screen_set_ddlist_time_hour, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_time_hour, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_time_hour, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_time_hour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_time_hour, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_time_hour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_time_hour, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_time_hour, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_time_hour, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_time_hour, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_time_hour, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_time_hour, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_time_hour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_time_hour, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_time_hour, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_time_hour, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_time_hour_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_time_hour_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_time_hour_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_time_hour_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_time_hour_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_time_hour_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_time_hour_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_time_hour_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_hour_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_hour_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_hour_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_hour), &style_screen_set_ddlist_time_hour_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_hour_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_time_hour_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_time_hour_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_time_hour_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_time_hour_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_time_hour_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_time_hour_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_time_hour_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_time_hour_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_hour_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_hour_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_hour_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_hour), &style_screen_set_ddlist_time_hour_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_hour_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_time_hour_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_time_hour_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_time_hour_extra_list_scrollbar_default, 0);
                    lv_style_set_bg_opa(&style_screen_set_ddlist_time_hour_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_hour_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_hour_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_hour), &style_screen_set_ddlist_time_hour_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_time_month
    ui->screen_set_ddlist_time_month = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_time_month, "12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_time_month, 557, 593);
    lv_obj_set_size(ui->screen_set_ddlist_time_month, 75, 29);

    // Write style for screen_set_ddlist_time_month, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_time_month, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_time_month, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_time_month, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_time_month, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_time_month, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_time_month, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_time_month, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_time_month, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_time_month, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_time_month, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_time_month, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_time_month, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_time_month, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_time_month, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_time_month, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_time_month_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_time_month_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_time_month_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_time_month_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_time_month_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_time_month_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_time_month_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_time_month_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_month_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_month_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_month_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_month), &style_screen_set_ddlist_time_month_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_month_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_time_month_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_time_month_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_time_month_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_time_month_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_time_month_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_time_month_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_time_month_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_time_month_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_month_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_month_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_month_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_month), &style_screen_set_ddlist_time_month_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_month_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_time_month_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_time_month_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_time_month_extra_list_scrollbar_default, 0);
                        lv_style_set_bg_opa(&style_screen_set_ddlist_time_month_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_month_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_month_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_month), &style_screen_set_ddlist_time_month_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_time_minute
    ui->screen_set_ddlist_time_minute = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_time_minute, "60\n59\n58\n57\n56\n54\n53\n52\n51\n50\n49\n48\n47");
    lv_obj_set_pos(ui->screen_set_ddlist_time_minute, 557, 626);
    lv_obj_set_size(ui->screen_set_ddlist_time_minute, 75, 29);

    // Write style for screen_set_ddlist_time_minute, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_time_minute, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_time_minute, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_time_minute, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_time_minute, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_time_minute, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_time_minute, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_time_minute, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_time_minute, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_time_minute, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_time_minute, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_time_minute, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_time_minute, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_time_minute, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_time_minute, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_time_minute, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_time_minute_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_time_minute_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_time_minute_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_time_minute_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_time_minute_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_time_minute_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_time_minute_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_time_minute_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_minute_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_minute_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_minute_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_minute), &style_screen_set_ddlist_time_minute_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_minute_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_time_minute_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_time_minute_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_time_minute_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_time_minute_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_time_minute_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_time_minute_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_time_minute_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_time_minute_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_minute_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_minute_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_minute_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_minute), &style_screen_set_ddlist_time_minute_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_minute_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_time_minute_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_time_minute_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_time_minute_extra_list_scrollbar_default, 0);
                            lv_style_set_bg_opa(&style_screen_set_ddlist_time_minute_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_minute_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_minute_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_minute), &style_screen_set_ddlist_time_minute_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_time_day
    ui->screen_set_ddlist_time_day = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_time_day, "31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18");
    lv_obj_set_pos(ui->screen_set_ddlist_time_day, 637, 593);
    lv_obj_set_size(ui->screen_set_ddlist_time_day, 75, 29);

    // Write style for screen_set_ddlist_time_day, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_time_day, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_time_day, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_time_day, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_time_day, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_time_day, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_time_day, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_time_day, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_time_day, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_time_day, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_time_day, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_time_day, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_time_day, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_time_day, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_time_day, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_time_day, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_time_day_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_time_day_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_time_day_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_time_day_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_time_day_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_time_day_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_time_day_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_time_day_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_day_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_day_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_day_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_day), &style_screen_set_ddlist_time_day_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_day_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_time_day_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_time_day_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_time_day_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_time_day_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_time_day_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_time_day_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_time_day_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_time_day_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_day_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_day_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_day_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_day), &style_screen_set_ddlist_time_day_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_day_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_time_day_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_time_day_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_time_day_extra_list_scrollbar_default, 0);
                                lv_style_set_bg_opa(&style_screen_set_ddlist_time_day_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_day_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_day_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_day), &style_screen_set_ddlist_time_day_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_slider_volume
    ui->screen_set_slider_volume = lv_slider_create(ui->screen_set);
    lv_slider_set_range(ui->screen_set_slider_volume, 0, 100);
    lv_slider_set_mode(ui->screen_set_slider_volume, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_set_slider_volume, 10, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_set_slider_volume, 252, 178);
    lv_obj_set_size(ui->screen_set_slider_volume, 456, 2);

    // Write style for screen_set_slider_volume, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_volume, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_volume, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_volume, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_volume, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_set_slider_volume, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_slider_volume, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_set_slider_volume, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_volume, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_volume, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_volume, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_volume, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_set_slider_volume, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_volume, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_volume, lv_color_hex(0xb2b0b1), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_volume, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_set_slider_volume, &_knob_12x12, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_set_slider_volume, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_set_slider_volume, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_volume, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_set_slider_saving_brightness
    ui->screen_set_slider_saving_brightness = lv_slider_create(ui->screen_set);
    lv_slider_set_range(ui->screen_set_slider_saving_brightness, 0, 100);
    lv_slider_set_mode(ui->screen_set_slider_saving_brightness, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_set_slider_saving_brightness, 10, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_set_slider_saving_brightness, 377, 400);
    lv_obj_set_size(ui->screen_set_slider_saving_brightness, 330, 2);

    // Write style for screen_set_slider_saving_brightness, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_saving_brightness, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_saving_brightness, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_saving_brightness, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_saving_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_set_slider_saving_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_slider_saving_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_set_slider_saving_brightness, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_saving_brightness, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_saving_brightness, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_saving_brightness, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_saving_brightness, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for screen_set_slider_saving_brightness, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_slider_saving_brightness, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_slider_saving_brightness, lv_color_hex(0xb2b0b1), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_slider_saving_brightness, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_src(ui->screen_set_slider_saving_brightness, &_knob_12x12, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_set_slider_saving_brightness, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_set_slider_saving_brightness, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_slider_saving_brightness, 1, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_set_ddlist_time_second
    ui->screen_set_ddlist_time_second = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_time_second, "60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45");
    lv_obj_set_pos(ui->screen_set_ddlist_time_second, 637, 626);
    lv_obj_set_size(ui->screen_set_ddlist_time_second, 75, 29);

    // Write style for screen_set_ddlist_time_second, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_time_second, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_time_second, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_time_second, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_time_second, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_time_second, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_time_second, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_time_second, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_time_second, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_time_second, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_time_second, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_time_second, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_time_second, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_time_second, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_time_second, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_time_second, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_time_second_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_time_second_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_time_second_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_time_second_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_time_second_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_time_second_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_time_second_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_time_second_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_second_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_second_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_second_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_second), &style_screen_set_ddlist_time_second_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_second_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_time_second_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_time_second_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_time_second_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_time_second_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_time_second_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_time_second_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_time_second_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_time_second_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_second_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_second_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_second_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_second), &style_screen_set_ddlist_time_second_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_time_second_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_time_second_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_time_second_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_time_second_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_time_second_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_time_second_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_time_second_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_time_second), &style_screen_set_ddlist_time_second_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_set_label_13
    ui->screen_set_label_13 = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_13, "               IP:\n   DHCP  Mask:\n               Gateway:\n               DNS:");
    lv_label_set_long_mode(ui->screen_set_label_13, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_13, 40, 669);
    lv_obj_set_size(ui->screen_set_label_13, 693, 140);

    // Write style for screen_set_label_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_13, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_label_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_label_13, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_label_13, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_13, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_13, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_13, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_13, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_ip
    ui->screen_set_btn_ip = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_ip_label = lv_label_create(ui->screen_set_btn_ip);
    lv_label_set_text(ui->screen_set_btn_ip_label, "OK");
    lv_label_set_long_mode(ui->screen_set_btn_ip_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_ip_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_ip, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_ip_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_ip, 40, 753);
    lv_obj_set_size(ui->screen_set_btn_ip, 98, 35);

    // Write style for screen_set_btn_ip, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_ip, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_ip, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_ip, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_ip, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_ip, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_ip, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_ip, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_ip, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_ip, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_ip, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_ip, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_ip, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_ip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btn_dhcp
    ui->screen_set_btn_dhcp = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_dhcp_label = lv_label_create(ui->screen_set_btn_dhcp);
    lv_label_set_text(ui->screen_set_btn_dhcp_label, " " LV_SYMBOL_OK " ");
    lv_label_set_long_mode(ui->screen_set_btn_dhcp_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_dhcp_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_dhcp, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_dhcp_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_dhcp, 40, 709);
    lv_obj_set_size(ui->screen_set_btn_dhcp, 20, 20);

    // Write style for screen_set_btn_dhcp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_dhcp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btn_dhcp, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_dhcp, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_dhcp, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btn_dhcp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btn_dhcp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btn_dhcp, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_dhcp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_dhcp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_dhcp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_dhcp, &lv_font_Deng_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_dhcp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_dhcp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip1
    ui->screen_set_ddlist_ip1 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip1, "240\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip1, 379, 669);
    lv_obj_set_size(ui->screen_set_ddlist_ip1, 80, 29);

    //Write style for screen_set_ddlist_ip1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip1, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip1, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip1, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip1, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip1_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip1_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip1_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip1_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip1_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip1_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip1_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip1_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip1), &style_screen_set_ddlist_ip1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip1_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip1_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip1_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip1_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip1_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip1_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip1_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip1_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip1_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip1_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip1_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip1_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip1), &style_screen_set_ddlist_ip1_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip1_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip1_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip1_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip1_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip1_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip1), &style_screen_set_ddlist_ip1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip2
    ui->screen_set_ddlist_ip2 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip2, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip2, 466, 669);
    lv_obj_set_size(ui->screen_set_ddlist_ip2, 80, 29);

    //Write style for screen_set_ddlist_ip2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip2, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip2, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip2, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip2, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip2_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip2_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip2_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip2_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip2_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip2_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip2_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip2_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip2_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip2_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip2_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip2), &style_screen_set_ddlist_ip2_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip2_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip2_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip2_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip2_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip2_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip2_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip2_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip2_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip2_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip2_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip2_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip2_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip2), &style_screen_set_ddlist_ip2_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip2_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip2_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip2_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip2_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip2_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip2), &style_screen_set_ddlist_ip2_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip3
    ui->screen_set_ddlist_ip3 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip3, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip3, 554, 669);
    lv_obj_set_size(ui->screen_set_ddlist_ip3, 80, 29);

    //Write style for screen_set_ddlist_ip3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip3, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip3, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip3, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip3, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip3, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip3_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip3_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip3_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip3_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip3_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip3_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip3_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip3_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip3_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip3_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip3_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip3), &style_screen_set_ddlist_ip3_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip3_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip3_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip3_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip3_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip3_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip3_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip3_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip3_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip3_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip3_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip3_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip3_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip3), &style_screen_set_ddlist_ip3_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip3_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip3_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip3_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip3_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip3_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip3), &style_screen_set_ddlist_ip3_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip4
    ui->screen_set_ddlist_ip4 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip4, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip4, 641, 669);
    lv_obj_set_size(ui->screen_set_ddlist_ip4, 80, 29);

    //Write style for screen_set_ddlist_ip4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip4, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip4, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip4, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip4, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip4, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip4_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip4_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip4_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip4_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip4_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip4_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip4_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip4_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip4_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip4_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip4_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip4), &style_screen_set_ddlist_ip4_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip4_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip4_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip4_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip4_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip4_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip4_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip4_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip4_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip4_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip4_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip4_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip4_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip4), &style_screen_set_ddlist_ip4_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip4_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip4_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip4_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip4_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip4_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip4), &style_screen_set_ddlist_ip4_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip5
    ui->screen_set_ddlist_ip5 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip5, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip5, 379, 705);
    lv_obj_set_size(ui->screen_set_ddlist_ip5, 80, 29);

    //Write style for screen_set_ddlist_ip5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip5, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip5, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip5, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip5, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip5, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip5, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip5, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip5_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip5_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip5_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip5_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip5_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip5_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip5_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip5_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip5_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip5_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip5_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip5), &style_screen_set_ddlist_ip5_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip5_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip5_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip5_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip5_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip5_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip5_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip5_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip5_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip5_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip5_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip5_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip5_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip5), &style_screen_set_ddlist_ip5_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip5_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip5_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip5_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip5_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip5_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip5), &style_screen_set_ddlist_ip5_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip6
    ui->screen_set_ddlist_ip6 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip6, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip6, 466, 705);
    lv_obj_set_size(ui->screen_set_ddlist_ip6, 80, 29);

    //Write style for screen_set_ddlist_ip6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip6, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip6, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip6, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip6, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip6, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip6, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip6, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip6_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip6_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip6_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip6_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip6_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip6_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip6_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip6_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip6_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip6_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip6_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip6), &style_screen_set_ddlist_ip6_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip6_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip6_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip6_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip6_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip6_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip6_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip6_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip6_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip6_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip6_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip6_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip6_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip6), &style_screen_set_ddlist_ip6_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip6_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip6_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip6_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip6_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip6_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip6), &style_screen_set_ddlist_ip6_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip7
    ui->screen_set_ddlist_ip7 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip7, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip7, 554, 705);
    lv_obj_set_size(ui->screen_set_ddlist_ip7, 80, 29);

    //Write style for screen_set_ddlist_ip7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip7, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip7, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip7, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip7, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip7, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip7, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip7, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip7_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip7_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip7_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip7_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip7_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip7_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip7_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip7_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip7_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip7_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip7_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip7), &style_screen_set_ddlist_ip7_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip7_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip7_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip7_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip7_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip7_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip7_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip7_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip7_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip7_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip7_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip7_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip7_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip7), &style_screen_set_ddlist_ip7_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip7_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip7_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip7_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip7_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip7_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip7), &style_screen_set_ddlist_ip7_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip8
    ui->screen_set_ddlist_ip8 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip8, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip8, 641, 705);
    lv_obj_set_size(ui->screen_set_ddlist_ip8, 80, 29);

    //Write style for screen_set_ddlist_ip8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip8, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip8, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip8, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip8, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip8, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip8, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip8, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip8, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip8_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip8_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip8_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip8_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip8_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip8_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip8_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip8_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip8_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip8_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip8_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip8), &style_screen_set_ddlist_ip8_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip8_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip8_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip8_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip8_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip8_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip8_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip8_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip8_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip8_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip8_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip8_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip8_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip8), &style_screen_set_ddlist_ip8_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip8_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip8_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip8_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip8_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip8_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip8), &style_screen_set_ddlist_ip8_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip9
    ui->screen_set_ddlist_ip9 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip9, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip9, 379, 741);
    lv_obj_set_size(ui->screen_set_ddlist_ip9, 80, 29);

    //Write style for screen_set_ddlist_ip9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip9, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip9, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip9, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip9, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip9, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip9, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip9, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip9, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip9_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip9_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip9_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip9_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip9_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip9_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip9_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip9_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip9_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip9_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip9_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip9), &style_screen_set_ddlist_ip9_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip9_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip9_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip9_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip9_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip9_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip9_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip9_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip9_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip9_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip9_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip9_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip9_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip9), &style_screen_set_ddlist_ip9_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip9_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip9_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip9_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip9_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip9_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip9), &style_screen_set_ddlist_ip9_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip10
    ui->screen_set_ddlist_ip10 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip10, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip10, 466, 741);
    lv_obj_set_size(ui->screen_set_ddlist_ip10, 80, 29);

    //Write style for screen_set_ddlist_ip10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip10, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip10, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip10, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip10, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip10, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip10, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip10, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip10_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip10_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip10_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip10_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip10_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip10_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip10_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip10_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip10_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip10_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip10_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip10), &style_screen_set_ddlist_ip10_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip10_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip10_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip10_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip10_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip10_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip10_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip10_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip10_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip10_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip10_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip10_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip10_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip10), &style_screen_set_ddlist_ip10_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip10_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip10_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip10_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip10_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip10_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip10), &style_screen_set_ddlist_ip10_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip11
    ui->screen_set_ddlist_ip11 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip11, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip11, 554, 741);
    lv_obj_set_size(ui->screen_set_ddlist_ip11, 80, 29);

    //Write style for screen_set_ddlist_ip11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip11, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip11, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip11, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip11, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip11, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip11, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip11, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip11, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip11_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip11_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip11_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip11_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip11_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip11_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip11_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip11_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip11_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip11_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip11_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip11), &style_screen_set_ddlist_ip11_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip11_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip11_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip11_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip11_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip11_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip11_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip11_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip11_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip11_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip11_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip11_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip11_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip11), &style_screen_set_ddlist_ip11_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip11_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip11_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip11_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip11_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip11_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip11), &style_screen_set_ddlist_ip11_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip12
    ui->screen_set_ddlist_ip12 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip12, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip12, 641, 741);
    lv_obj_set_size(ui->screen_set_ddlist_ip12, 80, 29);

    //Write style for screen_set_ddlist_ip12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip12, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip12, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip12, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip12, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip12, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip12, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip12, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip12, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip12, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip12_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip12_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip12_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip12_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip12_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip12_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip12_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip12_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip12_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip12_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip12_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip12), &style_screen_set_ddlist_ip12_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip12_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip12_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip12_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip12_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip12_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip12_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip12_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip12_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip12_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip12_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip12_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip12_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip12), &style_screen_set_ddlist_ip12_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip12_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip12_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip12_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip12_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip12_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip12), &style_screen_set_ddlist_ip12_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip13
    ui->screen_set_ddlist_ip13 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip13, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip13, 379, 775);
    lv_obj_set_size(ui->screen_set_ddlist_ip13, 80, 29);

    //Write style for screen_set_ddlist_ip13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip13, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip13, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip13, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip13, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip13, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip13, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip13, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip13, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip13, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip13_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip13_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip13_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip13_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip13_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip13_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip13_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip13_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip13_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip13_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip13_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip13), &style_screen_set_ddlist_ip13_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip13_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip13_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip13_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip13_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip13_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip13_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip13_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip13_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip13_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip13_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip13_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip13_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip13), &style_screen_set_ddlist_ip13_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip13_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip13_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip13_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip13_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip13_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip13), &style_screen_set_ddlist_ip13_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip14
    ui->screen_set_ddlist_ip14 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip14, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip14, 466, 775);
    lv_obj_set_size(ui->screen_set_ddlist_ip14, 80, 29);

    //Write style for screen_set_ddlist_ip14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip14, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip14, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip14, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip14, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip14, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip14, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip14, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip14, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip14, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip14_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip14_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip14_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip14_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip14_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip14_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip14_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip14_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip14_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip14_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip14_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip14), &style_screen_set_ddlist_ip14_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip14_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip14_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip14_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip14_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip14_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip14_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip14_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip14_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip14_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip14_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip14_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip14_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip14), &style_screen_set_ddlist_ip14_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip14_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip14_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip14_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip14_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip14_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip14), &style_screen_set_ddlist_ip14_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip15
    ui->screen_set_ddlist_ip15 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip15, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip15, 554, 775);
    lv_obj_set_size(ui->screen_set_ddlist_ip15, 80, 29);

    //Write style for screen_set_ddlist_ip15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip15, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip15, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip15, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip15, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip15, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip15, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip15, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip15, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip15, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip15_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip15_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip15_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip15_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip15_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip15_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip15_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip15_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip15_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip15_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip15_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip15), &style_screen_set_ddlist_ip15_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip15_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip15_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip15_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip15_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip15_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip15_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip15_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip15_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip15_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip15_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip15_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip15_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip15), &style_screen_set_ddlist_ip15_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip15_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip15_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip15_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip15_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip15_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip15), &style_screen_set_ddlist_ip15_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_set_ddlist_ip16
    ui->screen_set_ddlist_ip16 = lv_dropdown_create(ui->screen_set);
    lv_dropdown_set_options(ui->screen_set_ddlist_ip16, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_obj_set_pos(ui->screen_set_ddlist_ip16, 641, 775);
    lv_obj_set_size(ui->screen_set_ddlist_ip16, 80, 29);

    //Write style for screen_set_ddlist_ip16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ddlist_ip16, lv_color_hex(0x474747), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ddlist_ip16, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ddlist_ip16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ddlist_ip16, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_ddlist_ip16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_ddlist_ip16, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_ddlist_ip16, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ddlist_ip16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ddlist_ip16, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ddlist_ip16, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ddlist_ip16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ddlist_ip16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ddlist_ip16, lv_color_hex(0xb6b4b9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ddlist_ip16, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ddlist_ip16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_set_ddlist_ip16_extra_list_selected_checked
    static lv_style_t style_screen_set_ddlist_ip16_extra_list_selected_checked;
    ui_init_style(&style_screen_set_ddlist_ip16_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_set_ddlist_ip16_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_set_ddlist_ip16_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_set_ddlist_ip16_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_set_ddlist_ip16_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_set_ddlist_ip16_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip16_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip16_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip16_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip16), &style_screen_set_ddlist_ip16_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip16_extra_list_main_default
    static lv_style_t style_screen_set_ddlist_ip16_extra_list_main_default;
    ui_init_style(&style_screen_set_ddlist_ip16_extra_list_main_default);

    lv_style_set_max_height(&style_screen_set_ddlist_ip16_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_set_ddlist_ip16_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_set_ddlist_ip16_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_set_ddlist_ip16_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_set_ddlist_ip16_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_set_ddlist_ip16_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip16_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip16_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip16_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip16), &style_screen_set_ddlist_ip16_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_set_ddlist_ip16_extra_list_scrollbar_default
    static lv_style_t style_screen_set_ddlist_ip16_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip16_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip16_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip16_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_set_ddlist_ip16), &style_screen_set_ddlist_ip16_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    #if USE_TCP_SYNC
        //Write codes screen_set_btn_host
        ui->screen_set_btn_host = lv_btn_create(ui->screen_set);
        ui->screen_set_btn_host_label = lv_label_create(ui->screen_set_btn_host);
        lv_label_set_text(guider_ui.screen_set_btn_host_label, MY_SET_DHCP.host_state ? " " LV_SYMBOL_OK " " : "");
        lv_label_set_long_mode(ui->screen_set_btn_host_label, LV_LABEL_LONG_WRAP);
        lv_obj_align(ui->screen_set_btn_host_label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_pad_all(ui->screen_set_btn_host, 0, LV_STATE_DEFAULT);
        lv_obj_set_width(ui->screen_set_btn_host_label, LV_PCT(100));
        lv_obj_set_pos(ui->screen_set_btn_host, 40, 676);
        lv_obj_set_size(ui->screen_set_btn_host, 20, 20);

        //Write style for screen_set_btn_host, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
        lv_obj_set_style_bg_opa(ui->screen_set_btn_host, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui->screen_set_btn_host, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(ui->screen_set_btn_host, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui->screen_set_btn_host, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui->screen_set_btn_host, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui->screen_set_btn_host, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui->screen_set_btn_host, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_radius(ui->screen_set_btn_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui->screen_set_btn_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(ui->screen_set_btn_host, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_set_btn_host, &lv_font_Deng_12, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(ui->screen_set_btn_host, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui->screen_set_btn_host, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);


        //Write codes screen_set_label_host
        ui->screen_set_label_host = lv_label_create(ui->screen_set);
        lv_label_set_text(ui->screen_set_label_host, "HOST");
        lv_label_set_long_mode(ui->screen_set_label_host, LV_LABEL_LONG_WRAP);
        lv_obj_set_pos(ui->screen_set_label_host, 63, 672);
        lv_obj_set_size(ui->screen_set_label_host, 77, 27);

        //Write style for screen_set_label_host, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
        lv_obj_set_style_border_width(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_radius(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(ui->screen_set_label_host, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_set_label_host, &lv_font_Dengb_26, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(ui->screen_set_label_host, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_line_space(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui->screen_set_label_host, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui->screen_set_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    #endif

    //Write codes screen_set_btn_select_city
    ui->screen_set_btn_select_city = lv_btn_create(ui->screen_set);
    ui->screen_set_btn_select_city_label = lv_label_create(ui->screen_set_btn_select_city);
    lv_label_set_text(ui->screen_set_btn_select_city_label, "");
    lv_label_set_long_mode(ui->screen_set_btn_select_city_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_select_city_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_select_city, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_select_city_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_select_city, 333, 822);
    lv_obj_set_size(ui->screen_set_btn_select_city, 385, 47);

    //Write style for screen_set_btn_select_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_select_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_select_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_select_city, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_select_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_select_city, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_select_city, &lv_font_Dengb_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_select_city, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_select_city, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    // Write codes screen_set_tileview_key
    ui->screen_set_tileview_key = lv_tileview_create(ui->screen_set);
    ui->screen_set_tileview_key_tile_key = lv_tileview_add_tile(ui->screen_set_tileview_key, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_pos(ui->screen_set_tileview_key, -930, 0);
    lv_obj_set_size(ui->screen_set_tileview_key, 768, 1024);
    lv_obj_set_scrollbar_mode(ui->screen_set_tileview_key, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_set_tileview_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_tileview_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_tileview_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_tileview_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_set_tileview_key, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_tileview_key, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_tileview_key, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_tileview_key, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_tileview_key, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_set_btn_back
    ui->screen_set_btn_back = lv_btn_create(ui->screen_set_tileview_key_tile_key);
    ui->screen_set_btn_back_label = lv_label_create(ui->screen_set_btn_back);
    lv_label_set_text(ui->screen_set_btn_back_label, "");
    lv_label_set_long_mode(ui->screen_set_btn_back_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_set_btn_back_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_set_btn_back, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_set_btn_back_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_set_btn_back, 0, 0);
    lv_obj_set_size(ui->screen_set_btn_back, 768, 1024);

    //Write style for screen_set_btn_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_btn_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_btn_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btn_back, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btn_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btn_back, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btn_back, &lv_font_Dengb_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btn_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_btn_back, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    // Write codes screen_set_label_key
    ui->screen_set_label_key = lv_label_create(ui->screen_set_tileview_key_tile_key);
    lv_label_set_text(ui->screen_set_label_key, "输入密码");
    lv_label_set_long_mode(ui->screen_set_label_key, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_key, 186, 248);
    lv_obj_set_size(ui->screen_set_label_key, 417, 539);

    // Write style for screen_set_label_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_key, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_key, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_key, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_key, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_label_key, lv_color_hex(0x888888), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_label_key, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_btnm_key
    ui->screen_set_btnm_key = lv_btnmatrix_create(ui->screen_set_tileview_key_tile_key);
    static const char *screen_set_btnm_key_text_map[] = {
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
    lv_btnmatrix_set_map(ui->screen_set_btnm_key, screen_set_btnm_key_text_map);
    lv_obj_set_pos(ui->screen_set_btnm_key, 199, 380);
    lv_obj_set_size(ui->screen_set_btnm_key, 390, 403);

    // Write style for screen_set_btnm_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_btnm_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_btnm_key, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_btnm_key, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_btnm_key, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_btnm_key, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui->screen_set_btnm_key, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui->screen_set_btnm_key, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btnm_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_btnm_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_set_btnm_key, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_btnm_key, 1, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_btnm_key, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_btnm_key, lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_btnm_key, LV_BORDER_SIDE_FULL, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_btnm_key, lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_btnm_key, &lv_font_Dengb_40, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_btnm_key, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_btnm_key, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_btnm_key, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_btnm_key, lv_color_hex(0xa2a2a2), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_btnm_key, LV_GRAD_DIR_NONE, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_btnm_key, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);

    // Write codes screen_set_ta_key
    ui->screen_set_ta_key = lv_textarea_create(ui->screen_set_tileview_key_tile_key);
    lv_textarea_set_text(ui->screen_set_ta_key, "");
    lv_textarea_set_placeholder_text(ui->screen_set_ta_key, "");
    lv_textarea_set_password_bullet(ui->screen_set_ta_key, "*");
    lv_textarea_set_password_mode(ui->screen_set_ta_key, true);
    lv_textarea_set_one_line(ui->screen_set_ta_key, true);
    lv_textarea_set_accepted_chars(ui->screen_set_ta_key, "");
    lv_textarea_set_max_length(ui->screen_set_ta_key, 4);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    // lv_obj_add_event_cb(ui->screen_set_ta_key, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->screen_set_ta_key, 206, 320);
    lv_obj_set_size(ui->screen_set_ta_key, 376, 50);

    // Write style for screen_set_ta_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_set_ta_key, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_ta_key, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_ta_key, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_ta_key, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_ta_key, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_ta_key, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_ta_key, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_ta_key, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_set_ta_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_ta_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_ta_key, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_ta_key, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_ta_key, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ta_key, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for screen_set_ta_key, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_ta_key, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_ta_key, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    //Write codes screen_set_label_uart_v
    ui->screen_set_label_uart_v = lv_label_create(ui->screen_set);
    lv_label_set_text(ui->screen_set_label_uart_v, " ");
    lv_label_set_long_mode(ui->screen_set_label_uart_v, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_uart_v, 278, 16);
    lv_obj_set_size(ui->screen_set_label_uart_v, 178, 32);

    //Write style for screen_set_label_uart_v, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_uart_v, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_uart_v, &lv_font_Dengb_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_uart_v, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_uart_v, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_uart_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_tileview_ip
    ui->screen_set_tileview_ip = lv_tileview_create(ui->screen_set);
    ui->screen_set_tileview_ip_tile_ip = lv_tileview_add_tile(ui->screen_set_tileview_ip, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_pos(ui->screen_set_tileview_ip, -780, 0);
    lv_obj_set_size(ui->screen_set_tileview_ip, 768, 1024);
    lv_obj_set_scrollbar_mode(ui->screen_set_tileview_ip, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_tileview_ip, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_tileview_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_tileview_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_tileview_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_set_tileview_ip, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set_tileview_ip, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_tileview_ip, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_tileview_ip, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_tileview_ip, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



    //Write codes screen_set_label_save_ip
    ui->screen_set_label_save_ip = lv_label_create(ui->screen_set_tileview_ip_tile_ip);
    lv_label_set_text(ui->screen_set_label_save_ip, "\n\n\nSave Success,Please restart the program to take effect");
    lv_label_set_long_mode(ui->screen_set_label_save_ip, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_set_label_save_ip, 159, 342);
    lv_obj_set_size(ui->screen_set_label_save_ip, 467, 232);

    //Write style for screen_set_label_save_ip, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_save_ip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_label_save_ip, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_label_save_ip, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_save_ip, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_save_ip, &lv_font_Dengb_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_save_ip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_save_ip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_save_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (set_time_timer == NULL)
        set_time_timer = lv_timer_create(set_time_callback, 1000, 0);
    else
        lv_timer_resume(set_time_timer);
    if (get_time_timer == NULL)
        get_time_timer = lv_timer_create(get_time_callback, 1000, 0);
    else
        lv_timer_resume(get_time_timer);

    lv_obj_set_style_bg_img_src(ui->screen_set_slider_normal_brightness, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_set_slider_volume, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_set_slider_saving_brightness, MY_LVGL_IMAGE_PATH(knob_voice.png), LV_PART_KNOB | LV_STATE_DEFAULT);
    // set page
    lv_obj_clear_flag(ui->screen_set, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->screen_set_ta_key, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->screen_set_tileview_key_tile_key, LV_OBJ_FLAG_SCROLLABLE);

    // set init
    lv_slider_set_value(ui->screen_set_slider_volume, MY_SET.sound, LV_ANIM_OFF);
    lv_slider_set_value(ui->screen_set_slider_normal_brightness, MY_SET.backlight, LV_ANIM_OFF);
    lv_slider_set_value(ui->screen_set_slider_saving_brightness, MY_SET.e_con_backlight, LV_ANIM_OFF);

    lv_label_set_text(ui->screen_set_btn_dhcp_label, MY_SET_DHCP.dhcp_state ? " " LV_SYMBOL_OK " " : "");

    // lv_dropdown_set_options(ui->screen_set_ddlist_city, "北京\n深圳\n广州\n武汉\n珠海\n上海");
    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_dropdown_set_options(ui->screen_set_ddlist_city, my_city_3[city3_temp + city2_temp].city_name);
    }else
        lv_dropdown_set_options(ui->screen_set_ddlist_city, my_city_3_en[city3_temp + city2_temp].city_name);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_city, my_city_id.city_3);

    lv_dropdown_set_options(ui->screen_set_ddlist_interface, "C401 花锦世界（竖屏）\nC402 上善若水（竖屏）\nC403 岁月静好（竖屏）\nC404 绝代风华（竖屏）\nC201 水墨丹青（横屏）\nC202 春意盎然（横屏）\nC203 黑色经典（横屏）\nC301 大千世界（横屏）\nC302 我心澎湃（横屏）\nC303 星空浩渺（横屏）\nC401 花锦世界（横屏）\nC402 上善若水（横屏）\nC403 岁月静好（横屏）\nC404 绝代风华（横屏）");
    lv_dropdown_set_options(ui->screen_set_ddlist_time_year, "2045\n2044\n2043\n2042\n2041\n2040\n2039\n2038\n2037\n2036\n2035\n2034\n2033\n2032\n2031\n2030\n2029\n2028\n2027\n2026\n2025");
    lv_dropdown_set_options(ui->screen_set_ddlist_time_month, "12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_dropdown_set_options(ui->screen_set_ddlist_time_day, "31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
    lv_dropdown_set_options(ui->screen_set_ddlist_time_hour, "23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_time_minute, "59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_time_second, "59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_restart_time, "24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");



    lv_dropdown_set_options(ui->screen_set_ddlist_ip1, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip2, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip3, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip4, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");

    lv_dropdown_set_options(ui->screen_set_ddlist_ip9, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip10, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip11, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip12, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");

    lv_dropdown_set_options(ui->screen_set_ddlist_ip13, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip14, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip15, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip16, "200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\
        \n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\
        \n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");

    lv_dropdown_set_options(ui->screen_set_ddlist_ip5, "255\n254\n253\n252\n251\n250\n249\n248\n247\n246\n245\n244\n243\n242\n241\n240\n239\n238\n237\n236\n235\n234\n233\n232\n231\n230\n229\n228\n227\n226\n225\n224\n223\n222\n221\n220\n219\n218\n217\n216\n215\n214\n213\n212\n211\n210\n209\n208\n207\n206\n205\n204\n203\n202\n201\n\
        200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n\
        134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\
        \n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip6, "255\n254\n253\n252\n251\n250\n249\n248\n247\n246\n245\n244\n243\n242\n241\n240\n239\n238\n237\n236\n235\n234\n233\n232\n231\n230\n229\n228\n227\n226\n225\n224\n223\n222\n221\n220\n219\n218\n217\n216\n215\n214\n213\n212\n211\n210\n209\n208\n207\n206\n205\n204\n203\n202\n201\n\
        200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n\
        134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\
        \n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip7, "255\n254\n253\n252\n251\n250\n249\n248\n247\n246\n245\n244\n243\n242\n241\n240\n239\n238\n237\n236\n235\n234\n233\n232\n231\n230\n229\n228\n227\n226\n225\n224\n223\n222\n221\n220\n219\n218\n217\n216\n215\n214\n213\n212\n211\n210\n209\n208\n207\n206\n205\n204\n203\n202\n201\n\
        200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n\
        134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\
        \n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
    lv_dropdown_set_options(ui->screen_set_ddlist_ip8, "255\n254\n253\n252\n251\n250\n249\n248\n247\n246\n245\n244\n243\n242\n241\n240\n239\n238\n237\n236\n235\n234\n233\n232\n231\n230\n229\n228\n227\n226\n225\n224\n223\n222\n221\n220\n219\n218\n217\n216\n215\n214\n213\n212\n211\n210\n209\n208\n207\n206\n205\n204\n203\n202\n201\n\
        200\n199\n198\n197\n196\n195\n194\n193\n192\n191\n190\n189\n188\n187\n186\n185\n184\n183\n182\n181\n180\n179\n178\n177\n176\n175\n174\n173\n172\n171\n170\n169\n168\n167\n166\n165\n164\n163\n162\n161\n160\n159\n158\n157\n156\n155\n154\n153\n152\n151\n150\n149\n148\n147\n146\n145\n144\n143\n142\n141\n140\n139\n138\n137\n136\n135\n\
        134\n133\n132\n131\n130\n129\n128\n127\n126\n125\n124\n123\n122\n121\n120\n119\n118\n117\n116\n115\n114\n113\n112\n111\n110\n109\n108\n107\n106\n105\n104\n103\n102\n101\n100\n99\n98\n97\n96\n95\n94\n93\n92\n91\n90\n89\n88\n87\n86\n85\n84\n83\n82\n81\n80\n79\n78\n77\n76\n75\n74\n73\n72\n71\n70\n69\n68\n67\n66\n65\n64\n63\n62\n61\n60\
        \n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");


    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_dropdown_set_selected(guider_ui.screen_set_ddlist_language, 0);
        lv_label_set_text(ui->screen_set_label_save_ip, "\n\n\n\n保存成功,请重启程序生效");
        lv_label_set_text(ui->screen_set_label_3, "音量:");
        lv_label_set_text(ui->screen_set_label_4, "             播放内容:");
        lv_label_set_text(ui->screen_set_label_5, "正常亮度:");
        lv_label_set_text(ui->screen_set_label_6, "节能亮度:");
        lv_label_set_text(ui->screen_set_label_7, "                          界面选择:");
        lv_label_set_text(ui->screen_set_label_8, "                 箭头:                             LOGO:");
        lv_label_set_text(ui->screen_set_label_9, "               时间设定:");
        lv_label_set_text(ui->screen_set_label_11, "                              城市:");
        lv_label_set_text(ui->screen_set_label_12, "                   每天自动重启时间(时):");
        lv_label_set_text(ui->screen_set_label_13, "               IP:\n   DHCP  子网掩码:\n               默认网关:\n               DNS:");
        lv_label_set_text(ui->screen_set_label_key, "输入密码");
        lv_label_set_text(ui->screen_set_btn_IO_label, "设置IO板");
        lv_label_set_text(ui->screen_set_btn_voice_label, "设置语音报站");
        lv_label_set_text(ui->screen_set_btn_return_label, "返回");
        lv_label_set_text(ui->screen_set_btn_restart_program_label, "重启程序");
        lv_label_set_text(ui->screen_set_btn_restart_system_label, "重启系统");
        lv_label_set_text(ui->screen_set_btn_language_label, "确定");
        lv_label_set_text(ui->screen_set_btn_play_label, "确定");
        lv_label_set_text(ui->screen_set_btn_ip_label, "确定");
        lv_label_set_text(ui->screen_set_btn_interface_label, "确定");
        lv_label_set_text(ui->screen_set_btn_arrowhead_label, "确定");
        lv_label_set_text(ui->screen_set_btn_time_label, "确定");
        lv_label_set_text(ui->screen_set_btn_city_label, "确定");
        lv_label_set_text(ui->screen_set_btn_restart_time_label, "确定");
        lv_dropdown_set_options(ui->screen_set_ddlist_mode, "图片\n视频");
        lv_dropdown_set_options(ui->screen_set_ddlist_arrowhead, "斯沃德箭头\n西奥箭头");
        lv_dropdown_set_options(ui->screen_set_ddlist_logo, "西奥LOGO\n斯沃德LOGO\n无LOGO");
        // lv_dropdown_set_options(ui->screen_set_ddlist_city, "北京\n深圳\n广州\n武汉\n珠海\n上海");

        lv_obj_set_pos(ui->screen_set_slider_saving_brightness, 252, 400);
        lv_obj_set_size(ui->screen_set_slider_saving_brightness, 456, 2);
    }
    else if (MY_SET.language == LANGUAGE_EN)
    {
        lv_dropdown_set_selected(guider_ui.screen_set_ddlist_language, 1);
        lv_label_set_text(ui->screen_set_label_save_ip, "\n\n\nSave Success,Please restart the program to take effect");
        lv_label_set_text(ui->screen_set_label_3, "Volume:");
        lv_label_set_text(ui->screen_set_label_4, "             Mode:");
        lv_label_set_text(ui->screen_set_label_5, "NOR. Brightness:");
        lv_label_set_text(ui->screen_set_label_6, "E-Conservation. Brightness:");
        lv_label_set_text(ui->screen_set_label_7, "                         Layout:");
        lv_label_set_text(ui->screen_set_label_8, "                Arrow:                           LOGO:");
        lv_label_set_text(ui->screen_set_label_9, "              Set Time:");
        lv_label_set_text(ui->screen_set_label_11, "                              City:");
        lv_label_set_text(ui->screen_set_label_12, "                  Auto-RStart(Hour):");
        lv_label_set_text(ui->screen_set_label_13, "               IP:\n   DHCP  Mask:\n               Gateway:\n               DNS:");
        lv_label_set_text(ui->screen_set_label_key, "Input the password");
        lv_label_set_text(ui->screen_set_btn_IO_label, "Set IO");
        lv_label_set_text(ui->screen_set_btn_voice_label, "Set Voice");
        lv_label_set_text(ui->screen_set_btn_return_label, "BACK");
        lv_label_set_text(ui->screen_set_btn_restart_program_label, "RST app");
        lv_label_set_text(ui->screen_set_btn_restart_system_label, "RST System");
        lv_label_set_text(ui->screen_set_btn_language_label, "OK");
        lv_label_set_text(ui->screen_set_btn_ip_label, "OK");
        lv_label_set_text(ui->screen_set_btn_play_label, "OK");
        lv_label_set_text(ui->screen_set_btn_interface_label, "OK");
        lv_label_set_text(ui->screen_set_btn_arrowhead_label, "OK");
        lv_label_set_text(ui->screen_set_btn_time_label, "OK");
        lv_label_set_text(ui->screen_set_btn_city_label, "OK");
        lv_label_set_text(ui->screen_set_btn_restart_time_label, "OK");
        lv_dropdown_set_options(ui->screen_set_ddlist_mode, "picture\nvideo");
        lv_dropdown_set_options(ui->screen_set_ddlist_arrowhead, "SWORD\nXIOLIFT");
        lv_dropdown_set_options(ui->screen_set_ddlist_logo, "XIO-LOGO\nSWORD-LOGO\nNone");
        // lv_dropdown_set_options(ui->screen_set_ddlist_city, "Beijing\nShenzhen\nGuangzhou\nWuhan\nZhuhai\nShanghai");

        lv_obj_set_pos(ui->screen_set_slider_saving_brightness, 377, 400);
        lv_obj_set_size(ui->screen_set_slider_saving_brightness, 330, 2);
    }

    // if (MY_SET.play_mode == PLAY_IMAGE)
    // else
    //     lv_dropdown_set_selected(guider_ui.screen_set_ddlist_mode, 1);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_mode, MY_SET.play_mode == PLAY_IMAGE ? 0 : 1);

    if(MY_SET_DHCP.dhcp_state)
    {
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip3, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip4, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip5, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip6, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip7, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip8, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip9, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip10, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip11, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip12, LV_OBJ_FLAG_CLICKABLE);
        // lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip13, LV_OBJ_FLAG_CLICKABLE);
        // lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip14, LV_OBJ_FLAG_CLICKABLE);
        // lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip15, LV_OBJ_FLAG_CLICKABLE);
        // lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip16, LV_OBJ_FLAG_CLICKABLE);
    }else {
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    }

    if(MY_SET_DHCP.host_state)
    {
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.screen_set_ddlist_ip16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    }else{
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip13, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip14, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip15, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(guider_ui.screen_set_ddlist_ip16, LV_OBJ_FLAG_CLICKABLE);
    }

    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_restart_time, 24 - day_reset_time);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_interface, MY_SET_IMAGE.image - 1);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_logo, MY_SET_IMAGE.logo - 1);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_arrowhead, MY_SET_IMAGE.arrow - 1);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip1, 200 - MY_SET_DHCP.ip[0]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip2, 200 - MY_SET_DHCP.ip[1]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip3, 200 - MY_SET_DHCP.ip[2]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip4, 200 - MY_SET_DHCP.ip[3]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip5, 255 - MY_SET_DHCP.mask[0]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip6, 255 - MY_SET_DHCP.mask[1]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip7, 255 - MY_SET_DHCP.mask[2]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip8, 255 - MY_SET_DHCP.mask[3]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip9, 200 - MY_SET_DHCP.gateway[0]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip10, 200 - MY_SET_DHCP.gateway[1]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip11, 200 - MY_SET_DHCP.gateway[2]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip12, 200 - MY_SET_DHCP.gateway[3]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip13, 200 - MY_SET_DHCP.dns[0]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip14, 200 - MY_SET_DHCP.dns[1]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip15, 200 - MY_SET_DHCP.dns[2]);
    lv_dropdown_set_selected(guider_ui.screen_set_ddlist_ip16, 200 - MY_SET_DHCP.dns[3]);

            // 2. 创建数组，存储所有下拉列表的指针（根据实际变量名填写）
    lv_obj_t *ddlist_array_set[16] = {
        ui->screen_set_ddlist_ip1,ui->screen_set_ddlist_ip2,ui->screen_set_ddlist_ip3,ui->screen_set_ddlist_ip4,
        ui->screen_set_ddlist_ip5,ui->screen_set_ddlist_ip6,ui->screen_set_ddlist_ip7,ui->screen_set_ddlist_ip8,
        ui->screen_set_ddlist_ip9,ui->screen_set_ddlist_ip10,ui->screen_set_ddlist_ip11,ui->screen_set_ddlist_ip12,
        ui->screen_set_ddlist_ip13,ui->screen_set_ddlist_ip14,ui->screen_set_ddlist_ip15,ui->screen_set_ddlist_ip16,
};

    static lv_style_t style_screen_set_ddlist_ip_extra_list_scrollbar_default;
    ui_init_style(&style_screen_set_ddlist_ip_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_set_ddlist_ip_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_set_ddlist_ip_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_set_ddlist_ip_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_set_ddlist_ip_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    for(int i = 0; i < 16; i++)
    {
        if (ddlist_array_set[i] != NULL)
            lv_obj_add_style(lv_dropdown_get_list(ddlist_array_set[i]), &style_screen_set_ddlist_ip_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    }

    // The custom code of screen_set.

    // lv_obj_add_event_cb(ui->screen_set_btn_exit, btn_key_event_handler, LV_EVENT_ALL, ui);
    // lv_obj_add_event_cb(ui->screen_set_btn_ok, btn_key_event_handler, LV_EVENT_ALL, ui);
    // lv_obj_add_event_cb(ui->screen_set_btn_less, btn_key_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btnm_key, btnm_key_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_back, back_event_handler, LV_EVENT_ALL, ui);


    lv_obj_add_event_cb(ui->screen_set_btn_IO, keyboard_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_voice, keyboard_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_time, time_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_language, language_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_restart_program, reset_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_restart_system, reset_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_restart_time, time_reset_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_play, play_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_ip, ip_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_dhcp, dhcp_event_handler, LV_EVENT_ALL, ui);
    #if USE_TCP_SYNC
        lv_obj_add_event_cb(ui->screen_set_btn_host, host_event_handler, LV_EVENT_ALL, ui);
    #endif
    lv_obj_add_event_cb(ui->screen_set_btn_arrowhead, arrow_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_interface, interface_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_btn_select_city, goto_city_handler, LV_EVENT_ALL, ui);

    lv_obj_add_event_cb(ui->screen_set_slider_volume, slider_handler_slider, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_slider_normal_brightness, slider_handler_slider, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_slider_saving_brightness, slider_handler_slider, LV_EVENT_ALL, ui);

    lv_obj_add_event_cb(ui->screen_set_ddlist_time_month, set_day_handler, LV_EVENT_VALUE_CHANGED, ui);
    lv_obj_add_event_cb(ui->screen_set_ddlist_arrowhead, arrowhead_handler, LV_EVENT_VALUE_CHANGED, ui);
    // lv_obj_add_event_cb(ui->screen_set_ddlist_arrowhead, ddlist_arrowhead_event_handler, LV_EVENT_VALUE_CHANGED, ui);
    // lv_obj_add_event_cb(ui->screen_set_ddlist_logo, ddlist_logo_event_handler, LV_EVENT_ALL, ui);

    // Update current screen layout.
    lv_obj_update_layout(ui->screen_set);

    // Init events for screen.
    events_init_screen_set(ui);
    my_page = PAGE_SET;
    frist_set_time_flag = true;
    home_video_flag = false;
    send_version_query();
}
