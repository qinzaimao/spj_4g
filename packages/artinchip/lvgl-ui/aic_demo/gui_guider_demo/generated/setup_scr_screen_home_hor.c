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
// #include "D:\shipinji\jwzh\jwzh2\luban-lite-master\packages\artinchip\lvgl-ui\lvgl_v9\lvgl\src\libs\freetype\lv_freetype.h"

volatile static bool frist_set_mouse_image_flag = true;//切换限制

lv_timer_t *home_hor_set_time_timer = NULL;
lv_timer_t *home_hor_set_weather_timer = NULL;
lv_timer_t *home_hor_move_timer = NULL;
lv_timer_t *home_hor_refresh_picture_timer = NULL;
lv_timer_t *update_hor_timer = NULL;
lv_timer_t *frist_hor_timer = NULL;

static volatile bool show_weather_error_flag = false;

static volatile bool show_arrow_flag = false;
static volatile bool show_gif_arrow_flag = false;
static volatile bool show_gif_flag = false;
static volatile bool gif_temp = false;
static bool show_frist_flag = false;
static uint8_t get_ctrl_num = 0;
static uint8_t last_dir_arrow = 100;

static bool first_set_num = false;

static void show_update_floor(void);
char image_filename_gif_swd_up[][100] = {
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up2.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up3.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up4.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up5.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up6.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up7.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up8.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up9.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up10.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up11.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up13.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up14.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up15.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up16.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up17.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up18.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up19.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up20.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up21.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up22.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up23.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up24.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up25.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up26.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up27.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up28.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up29.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up30.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up31.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up32.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up33.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up34.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up35.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up36.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/up/up37.png),
};
char image_filename_gif_swd_down[][100] = {
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down2.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down3.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down4.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down5.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down6.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down7.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down8.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down9.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down10.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down11.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down13.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down14.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down15.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down16.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down17.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down18.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down19.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down20.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down21.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down22.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down23.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down24.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down25.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down26.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down27.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down28.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down29.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down30.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down31.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down32.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down33.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down34.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down35.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down36.png),
    MY_LVGL_IMAGE_PATH(picture/gif/swd/down/down37.png),
};
char image_filename_gif_xio_down[][100] = {
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down1.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down2.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down3.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down4.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down5.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down6.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down7.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down8.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down9.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down10.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down11.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down12.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down13.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down14.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down15.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down16.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down17.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down18.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down19.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down20.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down21.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down22.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down23.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down24.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down25.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down26.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down27.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down28.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down29.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down30.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down31.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down32.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down33.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down34.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down35.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down36.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down37.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down38.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down39.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down40.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down41.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down42.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down43.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down44.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down45.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down46.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down47.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down48.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down49.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down50.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down51.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down52.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down53.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down54.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down55.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down56.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down57.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down58.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down59.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down60.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down61.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down62.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down63.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down64.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down65.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down66.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down67.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down68.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down69.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down70.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down71.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/down/down72.png),
};

char image_filename_gif_xio_up[][100] = {
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up1.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up2.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up3.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up4.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up5.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up6.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up7.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up8.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up9.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up10.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up11.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up12.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up13.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up14.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up15.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up16.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up17.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up18.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up19.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up20.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up21.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up22.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up23.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up24.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up25.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up26.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up27.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up28.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up29.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up30.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up31.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up32.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up33.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up34.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up35.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up36.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up37.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up38.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up39.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up40.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up41.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up42.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up43.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up44.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up45.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up46.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up47.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up48.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up49.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up51.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up52.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up53.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up54.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up55.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up56.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up57.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up58.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up59.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up60.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up61.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up62.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up63.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up64.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up65.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up66.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up67.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up68.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up69.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up70.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up71.png),
    MY_LVGL_IMAGE_PATH(picture/gif/xio/up/up72.png),
};

char image_filename_c401_hor[][100] = {
    MY_LVGL_IMAGE_PATH(picture/background/C401_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C401_hor1.png),
    MY_LVGL_IMAGE_PATH(picture/background/C401_hor2.png),
    MY_LVGL_IMAGE_PATH(picture/background/C401_hor3.png),
};
char image_flash_c403_hor[][100] = {
    "L:/data/init0.jpg",
    "L:/data/init1.png",
    "L:/data/init2.png",
    "L:/data/init3.png",
};
char image_flash_c201_hor[][100] = {
    "L:/data/init1.png",
    "L:/data/init2.png",
    "L:/data/init3.png",
};
char image_filename_c402_hor[][100] = {
    MY_LVGL_IMAGE_PATH(picture/background/C402_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C402_hor1.png),
    MY_LVGL_IMAGE_PATH(picture/background/C402_hor2.png),
    MY_LVGL_IMAGE_PATH(picture/background/C402_hor3.png),
};

char image_filename_c403_hor[][100] = {
    MY_LVGL_IMAGE_PATH(picture/background/C403_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C403_hor1.png),
    MY_LVGL_IMAGE_PATH(picture/background/C403_hor2.png),
    MY_LVGL_IMAGE_PATH(picture/background/C403_hor3.png),
};

char image_filename_c201_hor[][100] = {
    MY_LVGL_IMAGE_PATH(picture/background/C401_hor2.png),
    MY_LVGL_IMAGE_PATH(picture/background/C402_hor3.png),
    MY_LVGL_IMAGE_PATH(picture/background/C403_hor2.png),
};

static void set_time_callback(lv_timer_t *timer)
{
    time_t now;
    struct tm *local_time;
    static const char *week_day_en[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    static const char *week_day_cn[7] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
    static uint8_t last_min = 100;
    static uint8_t last_set_min = 100;
    now = time(RT_NULL);
    local_time = localtime(&now);

    // 提取时间组件，避免重复计算
    int year = local_time->tm_year + 1900;
    int month = local_time->tm_mon + 1;
    int day = local_time->tm_mday;
    int hour = local_time->tm_hour;
    int minute = local_time->tm_min;
    int wday = local_time->tm_wday;

    if(last_set_min != minute || tcp_set_time_flag)
    {
        if(tcp_set_time_flag) tcp_set_time_flag = false;
        last_set_min = minute;
        if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
        {
            // lv_label_set_text_fmt(guider_ui.screen_home_hor_label_date, "%04d/%02d/%02d", year, month, day);
            // lv_label_set_text_fmt(guider_ui.screen_home_hor_label_C201, "%s/%s", week_day_en[wday], week_day_cn[wday]);
            // lv_label_set_text_fmt(guider_ui.screen_home_hor_label_time, "%02d:%02d",hour, minute);
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_date, "%04d/%02d/%02d                                                %s/%s",
                                year, month, day,
                                week_day_en[wday], week_day_cn[wday]);
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_time, "%02d:%02d",
                                hour, minute);
        }
        else if (MY_SET_IMAGE.image == IMAGE_C203_hor)
        {
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_date, "%04d/%02d/%02d   %s/%s",
                                year, month, day,
                                week_day_en[wday], week_day_cn[wday]);
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_time, "%02d:%02d", hour, minute);
        }
        else if (MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
        {
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_date, "%04d/%02d/%02d", year, month, day);
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_date_c3, "%s/%s", week_day_en[wday], week_day_cn[wday]);
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_time, "%02d:%02d", hour, minute);
        }
        else if (MY_SET_IMAGE.image == IMAGE_C401_hor)
        {
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_date, "%04d-%02d-%02d   %02d:%02d",
                                year, month, day,
                                hour, minute);
        }
        else if (MY_SET_IMAGE.image == IMAGE_C403_hor)
        {
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_date, "%04d/%02d/%02d    %02d:%02d",
                                year, month, day,
                                hour, minute);
        }
        else if (MY_SET_IMAGE.image == IMAGE_C404_hor)
        {
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_date, "%02d/%02d %s",
                                month, day, week_day_en[wday]);
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_time, "%02d:%02d",
                                hour, minute);
        }
    }
    if (is_tcp_connected && MY_SET_DHCP.host_state == true)
    {
         if(last_min != local_time->tm_min || tcp_raw_time_flag)
         {
            if(tcp_raw_time_flag) tcp_raw_time_flag = false;
            last_min = local_time->tm_min;
            char time_str[20] = {0};
            sprintf(time_str, "%04d/%02d/%02d %02d:%02d",
            local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
            local_time->tm_hour, local_time->tm_min);
            // rt_kprintf("time_str:%s\n", time_str);
            tcp_send_raw(time_str, 16);
        }
    }
}

static void move_callback(lv_timer_t *timer)
{
    static int index = 0;
    static bool logo = false;
    static bool set_show_image_flag = false;
    static bool set_image_flag = false;
    static bool energy_image_flag = false;
    static uint8_t set_image_time_count = 0;
    static uint8_t time_count = 0;
    static uint8_t last_arrow_ctrl = 0;

    static uint8_t last_ctrl_num = 100;

    static uint8_t time_gif_cnt = 0;

    if(show_gif_flag && ++ time_gif_cnt > 20)
    {
        show_gif_flag = false;
        time_gif_cnt = 0;
        lv_obj_clear_flag(guider_ui.screen_home_hor_img_gif, LV_OBJ_FLAG_HIDDEN);
    }

    // 状态视频部分设置ui显示和不显示的
    if (video_select)
    {
        if (++set_image_time_count >= 75)
        {
            set_image_time_count = 0;
            set_image_flag = true;

            if (MY_SET_IMAGE.image == IMAGE_C403_hor)
            {
                if (MY_SET.play_mode == PLAY_IMAGE)
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            else if (MY_SET_IMAGE.image == IMAGE_C401_hor)
            {
                if (MY_SET.play_mode == PLAY_IMAGE)
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_flag(guider_ui.screen_home_hor_img_shade_up, LV_OBJ_FLAG_HIDDEN);     // 不可见
                lv_obj_add_flag(guider_ui.screen_home_hor_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 不可见
            }
            else if (MY_SET_IMAGE.image == IMAGE_C402_hor)
            {
                if (MY_SET.play_mode == PLAY_IMAGE)
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_flag(guider_ui.screen_home_hor_img_shade_left, LV_OBJ_FLAG_HIDDEN); // 不可见
            }
            else if (MY_SET_IMAGE.image == IMAGE_C203_hor)
            {
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            else if (MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                     MY_SET_IMAGE.image == IMAGE_C303_hor)
            {
                if (MY_SET.play_mode == PLAY_IMAGE)
                {
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
            if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                MY_SET_IMAGE.image == IMAGE_C404_hor)
                ;
            else
            {
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_flag(guider_ui.screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN);      // 不可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN);      // 不可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN);      // 不可见
            }
        }
    }
    else if (set_image_flag && state_video_end_flag)
    {
        set_image_flag = false;
        state_video_end_flag = false;
        lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_arrow_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_arrow_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        if (MY_SET_IMAGE.image == IMAGE_C403_hor)
        {
            if (MY_SET.play_mode == PLAY_IMAGE)
            {
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
        else if (MY_SET_IMAGE.image == IMAGE_C401_hor)
        {
            if (MY_SET.play_mode == PLAY_IMAGE)
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_shade_up, LV_OBJ_FLAG_HIDDEN);     // 不可见
            lv_obj_clear_flag(guider_ui.screen_home_hor_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 不可见
        }
        else if (MY_SET_IMAGE.image == IMAGE_C402_hor)
        {
            if (MY_SET.play_mode == PLAY_IMAGE)
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_shade_left, LV_OBJ_FLAG_HIDDEN); // 不可见
        }
        else if (MY_SET_IMAGE.image == IMAGE_C203_hor)
        {
            lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else if (MY_SET.play_mode == PLAY_IMAGE)
        {
            if (MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                MY_SET_IMAGE.image == IMAGE_C303_hor)
            {
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
        if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
            MY_SET_IMAGE.image == IMAGE_C404_hor)
            ;
        else
        {
            if (MY_SET_IMAGE.image != IMAGE_C203_hor)
            {
                // rt_kprintf("here 2\n");
                lv_obj_clear_flag(guider_ui.screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            if (have_floor_num == 1)
            {
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN);   // 不可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
            }
            else if (have_floor_num == 2)
            {
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
            }
            else if (have_floor_num == 3)
            {
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            else
            {
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN); // 不可见
            }
        }
    }

    // lv_obj_set_style_img_opa(ui->screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_img_opa(ui->screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    show_update_floor();

    switch (elevator_data.arrow_ctrl)
    {
    case 0:
        arrow_num = 0;
        show_arrow_flag = true;
        if (last_ctrl_num != 0)
        {
            if (MY_SET_IMAGE.arrow == ARROW_XIO)
            {
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 不可见
            }
            else
            {
                lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 不可见
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
            {
                lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 不可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 不可见
                show_gif_flag = true;
                show_gif_arrow_flag = true;
                show_arrow_flag = false;
                gif_temp = true;
                rt_kprintf("gif_temp = true\n");
            }
            lv_img_set_offset_y(guider_ui.screen_home_hor_img_arrow_xio, 0);
            lv_img_set_offset_y(guider_ui.screen_home_hor_img_arrow_swd, 0);
            last_ctrl_num = elevator_data.arrow_ctrl;
            last_arrow_ctrl = elevator_data.dir_arrow;
            get_ctrl_num = elevator_data.dir_arrow;
        }
        break;
    case 1:
        if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
        {
            lv_obj_add_flag(guider_ui.screen_home_hor_img_gif, LV_OBJ_FLAG_HIDDEN);
            set_gif_arrow = 0;
            rt_kprintf("set_gif_arrow = 0;\n");
            gif_temp = false;
            show_gif_arrow_flag = false;
        }
        arrow_num = 1;
        show_arrow_flag = true;
        show_gif_arrow_flag = false;
        lv_img_set_offset_y(guider_ui.screen_home_hor_img_arrow_xio, 0);
        lv_img_set_offset_y(guider_ui.screen_home_hor_img_arrow_swd, 0);
        time_count++;

        if (time_count >= 25)
        {

            time_count = 0;
            logo = !logo;
            if (MY_SET_IMAGE.arrow == ARROW_XIO)
            {
                if (logo)
                    lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 不可见
                else
                    lv_obj_clear_flag(guider_ui.screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 可见

            }
            else if (MY_SET_IMAGE.logo == LOGO_SWD)
            {
                if (logo)
                    lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 不可见
                else
                    lv_obj_clear_flag(guider_ui.screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 可见
            }
        }
        last_ctrl_num = elevator_data.arrow_ctrl;
        last_arrow_ctrl = elevator_data.dir_arrow;
        get_ctrl_num = elevator_data.dir_arrow;
        break;
    case 2:
        arrow_num = 2;
        if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
        {
            lv_obj_add_flag(guider_ui.screen_home_hor_img_gif, LV_OBJ_FLAG_HIDDEN);
            set_gif_arrow = 0;
            rt_kprintf("set_gif_arrow = 0;\n");
            gif_temp = false;
            show_gif_arrow_flag = false;
        }
        show_arrow_flag = true;
        if (last_arrow_ctrl != elevator_data.dir_arrow)
        {
            if (MY_SET_IMAGE.arrow == ARROW_XIO)
            {
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 不可见
            }
            else
            {
                lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 不可见
                lv_obj_clear_flag(guider_ui.screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            index = 0;
        }
        last_arrow_ctrl = elevator_data.dir_arrow;
        get_ctrl_num = elevator_data.dir_arrow;
        if (elevator_data.dir_arrow)
        {
            if (MY_SET_IMAGE.image == IMAGE_C203_hor)
            {
                index += 2;
                if (index >= 260)
                    index = -260;
            }
            else
            {
                index += 1;
                if (index >= 140)
                    index = -140;
            }
        }
        else
        {
            if (MY_SET_IMAGE.image == IMAGE_C203_hor)
            {
                index -= 2;
                if (index <= -260)
                    index = 0;
            }
            else
            {
                index -= 1;
                if (index <= -140)
                    index = 0;
            }
        }

        if (MY_SET_IMAGE.arrow == ARROW_XIO)
            lv_img_set_offset_y(guider_ui.screen_home_hor_img_arrow_xio, index);
        else if (MY_SET_IMAGE.arrow == ARROW_SWD)
            lv_img_set_offset_y(guider_ui.screen_home_hor_img_arrow_swd, index);
        last_ctrl_num = elevator_data.arrow_ctrl;
        break;
    case 3:
        arrow_num = 3;
        show_arrow_flag = false;
        if (last_ctrl_num != 3)
        {
            lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 不可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 不可见
            if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
            {
                lv_obj_add_flag(guider_ui.screen_home_hor_img_gif, LV_OBJ_FLAG_HIDDEN);
                set_gif_arrow = 0;
                rt_kprintf("set_gif_arrow = 0;\n");
                show_gif_arrow_flag = false;
                gif_temp = false;
            }
            last_ctrl_num = elevator_data.arrow_ctrl;
            last_arrow_ctrl = elevator_data.dir_arrow;
        }
        break;
    default:
        break;
    }
    static bool set_dir_temp = false;
    if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
    {
        if(!last_dir_arrow && !elevator_data.dir_arrow && !gif_temp && !set_dir_temp)
        {
            rt_kprintf("set_dir_temp = true\n");
            set_dir_temp = true;
        }
        if(set_dir_temp && gif_temp)
        {
            last_dir_arrow = 10;
            set_dir_temp = false;
        }
    }
    if (last_dir_arrow != elevator_data.dir_arrow && !set_dir_temp)
    {
        last_dir_arrow = elevator_data.dir_arrow;
        if (elevator_data.dir_arrow)
        {
            // rt_kprintf("elevator_data.dir_arrow = 1\n");
            if (MY_SET_IMAGE.image != IMAGE_C203_hor)
            {
                if(MY_SET_IMAGE.image == IMAGE_C201_hor)
                {
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C201.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C201.png));
                }else if(MY_SET_IMAGE.image == IMAGE_C202_hor)
                {
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C202.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C202.png));
                }else if(MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor || MY_SET_IMAGE.image == IMAGE_C403_hor)
                {
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C401.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C401.png));
                }else if(MY_SET_IMAGE.image == IMAGE_C404_hor)
                {
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C404.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C404.png));
                }else
                {
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down.png));
                }
                if(gif_temp)
                {
                    lv_obj_add_flag(guider_ui.screen_home_hor_img_gif, LV_OBJ_FLAG_HIDDEN);
                    // lv_gif_set_src(guider_ui.screen_home_hor_img_gif_swd, MY_LVGL_IMAGE_PATH(picture/gif/down.gif));
                    // lv_obj_invalidate(guider_ui.screen_home_hor_img_gif_swd);  // 标记控件为无效，触发重绘
                    set_gif_arrow = DOWN_ARROW;
                    rt_kprintf("set_gif_arrow = DOWN_ARROW;\n");
                    show_gif_flag = true;
                }
            }
            else
            {
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C203.png));
                lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C203.png));
            }
        }
        else
        {
            // rt_kprintf("elevator_data.dir_arrow = 0\n");
            if (MY_SET_IMAGE.image != IMAGE_C203_hor)
            {
                if(MY_SET_IMAGE.image == IMAGE_C201_hor)
                {
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C201.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C201.png));
                }else if(MY_SET_IMAGE.image == IMAGE_C202_hor)
                {
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C202.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C202.png));
                }else if(MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor || MY_SET_IMAGE.image == IMAGE_C403_hor)
                {
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C401.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C401.png));
                }else if(MY_SET_IMAGE.image == IMAGE_C404_hor)
                {
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C404.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C404.png));
                }else
                {
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up.png));
                    lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up.png));
                }
                // lv_gif_set_src(guider_ui.screen_home_hor_img_gif_swd, MY_LVGL_IMAGE_PATH(picture/gif/up.gif));
                if(gif_temp)
                {
                    lv_obj_add_flag(guider_ui.screen_home_hor_img_gif, LV_OBJ_FLAG_HIDDEN);
                    set_gif_arrow = UP_ARROW;
                    rt_kprintf("set_gif_arrow = UP_ARROW;\n");
                    // lv_gif_set_src(guider_ui.screen_home_hor_img_gif_swd, MY_LVGL_IMAGE_PATH(picture/gif/up.gif));
                    // lv_obj_invalidate(guider_ui.screen_home_hor_img_gif_swd);  // 标记控件为无效，触发重绘
                    show_gif_flag = true;
                }
            }
            else
            {
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C203.png));
                lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C203.png));
            }
        }
    }
}

static void refresh_picture_callback(lv_timer_t *timer)
{
    static bool tcp_raw_flag = false;
    static uint8_t show_image_num = 1;
    static uint8_t time_cnt = 0;
    static uint8_t time_cnt2 = 0;
    static uint8_t image_cnt_temp = 1;
    char current_image_path[50];



    if(MY_SET.play_mode == PLAY_VIDEO && video_in_updating &&
        (MY_SET_IMAGE.image == IMAGE_C401_hor ||
        MY_SET_IMAGE.image == IMAGE_C402_hor || MY_SET_IMAGE.image == IMAGE_C403_hor) )
    {
        // rt_kprintf("video_in_updating 255\n");
        lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    }else if(MY_SET.play_mode == PLAY_VIDEO && !video_in_updating &&
        (MY_SET_IMAGE.image == IMAGE_C301_hor ||MY_SET_IMAGE.image == IMAGE_C302_hor ||
        MY_SET_IMAGE.image == IMAGE_C303_hor ||MY_SET_IMAGE.image == IMAGE_C401_hor ||
        MY_SET_IMAGE.image == IMAGE_C402_hor ||MY_SET_IMAGE.image == IMAGE_C403_hor) )
    {
        lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }



    if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
        MY_SET_IMAGE.image == IMAGE_C203_hor || MY_SET_IMAGE.image == IMAGE_C301_hor ||
        MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor ||
        MY_SET_IMAGE.image == IMAGE_C402_hor || MY_SET_IMAGE.image == IMAGE_C404_hor)
    {
        if (have_overload_flag)
        {
            if(MY_SET.play_mode == PLAY_VIDEO  && voice_language != VOICE_EN && (MY_SET_IMAGE.image == IMAGE_C301_hor ||
                MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor ||
                MY_SET_IMAGE.image == IMAGE_C402_hor))
            {
                if(video_select)
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                else
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            lv_obj_clear_flag(guider_ui.screen_home_hor_label_overload, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
        {
            if(MY_SET.play_mode == PLAY_VIDEO && !video_in_updating && (MY_SET_IMAGE.image == IMAGE_C301_hor ||
                MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor ||
                MY_SET_IMAGE.image == IMAGE_C402_hor))
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(guider_ui.screen_home_hor_label_overload, LV_OBJ_FLAG_HIDDEN); // 不可见
        }
        if (have_fire_flag)
            lv_obj_clear_flag(guider_ui.screen_home_hor_label_fire, LV_OBJ_FLAG_HIDDEN); // 可见
        else
            lv_obj_add_flag(guider_ui.screen_home_hor_label_fire, LV_OBJ_FLAG_HIDDEN); // 可见
    }
    else if (MY_SET_IMAGE.image == IMAGE_C401_hor)
    {
        if (have_overload_flag)
        {
            if(MY_SET.play_mode == PLAY_VIDEO && voice_language != VOICE_EN)
            {
                if(video_select)
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                else
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            lv_obj_clear_flag(guider_ui.screen_home_hor_label_overload_401, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
        {
            if(MY_SET.play_mode == PLAY_VIDEO && !video_in_updating)
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(guider_ui.screen_home_hor_label_overload_401, LV_OBJ_FLAG_HIDDEN); // 不可见
        }
        if (have_fire_flag)
        {
            if(have_overload_flag)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_label_fire_401, 341, 723);
            }
            else{
                lv_obj_set_pos(guider_ui.screen_home_hor_label_fire_401, 111, 723);
            }
            lv_obj_clear_flag(guider_ui.screen_home_hor_label_fire_401, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
            lv_obj_add_flag(guider_ui.screen_home_hor_label_fire_401, LV_OBJ_FLAG_HIDDEN); // 不可见
        if (have_overload_flag || have_fire_flag)
            lv_obj_add_flag(guider_ui.screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 不可见
        else
        {
            if(!video_select)
                lv_obj_clear_flag(guider_ui.screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 可见
        }
    }
    else if (MY_SET_IMAGE.image == IMAGE_C403_hor)
    {
        if (have_overload_flag)
        {
            if(MY_SET.play_mode == PLAY_VIDEO && voice_language != VOICE_EN)
            {
                if(video_select)
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                else
                    lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            lv_obj_clear_flag(guider_ui.screen_home_hor_label_overload_403, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
        {
            if(MY_SET.play_mode == PLAY_VIDEO && !video_in_updating)
                lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(guider_ui.screen_home_hor_label_overload_403, LV_OBJ_FLAG_HIDDEN); // 不可见
        }
        if (have_fire_flag)
        {
            if(have_overload_flag)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_label_fire_403, 345, 705);
            }else{
                lv_obj_set_pos(guider_ui.screen_home_hor_label_fire_403, 82, 705);
            }
            lv_obj_clear_flag(guider_ui.screen_home_hor_label_fire_403, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
            lv_obj_add_flag(guider_ui.screen_home_hor_label_fire_403, LV_OBJ_FLAG_HIDDEN); // 不可见
        if (have_overload_flag || have_fire_flag)
            lv_obj_add_flag(guider_ui.screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 不可见
        else
        {
            if(!video_select)
                lv_obj_clear_flag(guider_ui.screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 可见
        }
    }
    if(show_video_label_flag && MY_SET.play_mode == PLAY_VIDEO)
    {
        if(MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor ||
            MY_SET_IMAGE.image == IMAGE_C403_hor)
        {
            lv_obj_set_style_img_opa(guider_ui.screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_hor_label_video, " 视频格式不支持 ");
        else
            lv_label_set_text(guider_ui.screen_home_hor_label_video, "Video no support");
    }else {
            lv_label_set_text(guider_ui.screen_home_hor_label_video, " ");
    }

    if (mouse_leave_flag)
    {
        mouse_leave_flag = false;
        // lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_background));
        lv_img_set_src(guider_ui.screen_home_hor_img_background, image_filename[MY_SET_IMAGE.image - 1]);
    }
    if (MY_SET.play_mode == PLAY_IMAGE && page_image_cnt[update_page_num])
    {
        if(is_tcp_connected && MY_SET_DHCP.host_state == false && !video_in_updating)
        {

        }else
        {
            if (MY_SET_IMAGE.image != IMAGE_C203_hor && MY_SET_IMAGE.image != IMAGE_C301_hor &&
                MY_SET_IMAGE.image != IMAGE_C302_hor && MY_SET_IMAGE.image != IMAGE_C303_hor)
            {
                if (time_cnt >= 24 && !tcp_raw_flag)
                {
                    if (image_cnt_temp >= page_image_cnt[update_page_num])
                        image_cnt_temp = 0;
                    if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
                    {
                        tcp_raw_flag = true;
                        char tcp_temp[5] = {0};
                        sprintf(tcp_temp, "ida%d", image_cnt_temp);
                        tcp_send_raw(tcp_temp, 4);
                    }
                }
                if (++time_cnt >= 25)
                {
                    time_cnt = 0;
                    tcp_raw_flag = false;
                    const char *ext = ".jpg"; // 默认扩展名
                    // 构建示例路径来获取扩展名
                    char temp_path[30];
                    sprintf(temp_path, "%s%d.png", image_flash_path_prefix[update_page_num], image_cnt_temp);
                    if (access(temp_path, 0) == 0)
                    ext = ".png";
                    else
                    {
                        sprintf(temp_path, "%s%d.jpg", image_flash_path_prefix[update_page_num], image_cnt_temp);
                        if (access(temp_path, 0) == 0)
                        ext = ".jpg";
                        else
                        {
                            sprintf(temp_path, "%s%d.bmp", image_flash_path_prefix[update_page_num], image_cnt_temp);
                            if (access(temp_path, 0) == 0)
                            ext = ".bmp";
                        }
                    }
                    // 构建最新图片路径（假设最后一个导入的图片为当前显示图片）
                    sprintf(current_image_path, "L:%s%d%s", image_flash_path_prefix[update_page_num], image_cnt_temp, ext);
                    rt_kprintf("image_path:%s\n", current_image_path);
                    image_cnt_temp ++;
                    // 刷新图片
                    if (MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C201_hor ||
                    MY_SET_IMAGE.image == IMAGE_C202_hor)
                    {
                        lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_img));
                        lv_img_set_src(guider_ui.screen_home_hor_img_img, current_image_path);
                        lv_image_set_inner_align(guider_ui.screen_home_hor_img_img, LV_IMAGE_ALIGN_STRETCH);
                    }
                    else
                    {
                        lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_background));
                        lv_img_set_src(guider_ui.screen_home_hor_img_background, current_image_path);
                        lv_image_set_inner_align(guider_ui.screen_home_hor_img_background, LV_IMAGE_ALIGN_STRETCH);
                    }
                }
             }
        }
    }
    else if (!page_image_cnt[update_page_num] && MY_SET.play_mode == PLAY_IMAGE)
    {
        if (MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor ||
            MY_SET_IMAGE.image == IMAGE_C403_hor || MY_SET_IMAGE.image == IMAGE_C201_hor ||
            MY_SET_IMAGE.image == IMAGE_C202_hor || MY_SET_IMAGE.image == IMAGE_C404_hor)
        {
            if(is_tcp_connected && MY_SET_DHCP.host_state == false && !video_in_updating)
            {

            }else{
                if (time_cnt2 >= 24 && !tcp_raw_flag)
                {
                    if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                    MY_SET_IMAGE.image == IMAGE_C404_hor)
                    {
                        if (show_image_num >= 3) show_image_num = 0;
                    }else
                    {
                        if (show_image_num >= 4) show_image_num = 0;
                    }
                    if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
                    {
                        tcp_raw_flag = true;
                        char tcp_temp[5] = {0};
                        sprintf(tcp_temp, "img%d", show_image_num);
                        tcp_send_raw(tcp_temp, 4);
                    }
                }
                if (++time_cnt2 >= 25)
                {
                    time_cnt2 = 0;
                    tcp_raw_flag = false;
                    if (MY_SET_IMAGE.image == IMAGE_C401_hor)
                    {
                        lv_img_set_src(guider_ui.screen_home_hor_img_background, image_flash_c403_hor[show_image_num]);
                    }
                    else if (MY_SET_IMAGE.image == IMAGE_C402_hor)
                    {
                        lv_img_set_src(guider_ui.screen_home_hor_img_background, image_flash_c403_hor[show_image_num]);
                    }
                    else if (MY_SET_IMAGE.image == IMAGE_C403_hor)
                    {
                        lv_img_set_src(guider_ui.screen_home_hor_img_background, image_flash_c403_hor[show_image_num]);
                    }else if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                    MY_SET_IMAGE.image == IMAGE_C404_hor)
                    {
                        lv_img_set_src(guider_ui.screen_home_hor_img_img, image_flash_c201_hor[show_image_num]);
                    }
                    show_image_num++;
                }
            }
        }
    }
}

static void update_callback(lv_timer_t *timer)
{
    // if (udisk_update_state == VIDEO_UPDATE_FINISH)
    // {
    // }

    static uint8_t time_cnt = 0;
    static uint8_t light_cnt = 0;
    static bool init_img_flag = false;
    static bool light_flag = false;
    if(++ light_cnt > 3 && !light_flag)
    {
        light_flag = true;
        BLEN_Init();
    }
    //10 11 12
    if(++ time_cnt > DELAY_LOGO && !init_img_flag)
    {
        // lv_img_set_src(guider_ui.screen_home_hor_img_init, MY_LVGL_IMAGE_PATH(picture/background/black_hor.png));
        lv_obj_add_flag(guider_ui.screen_home_hor_img_init_logo, LV_OBJ_FLAG_HIDDEN);

        if(time_cnt > (DELAY_LOGO + 1))
        {
            init_set_img_ok = true;
        }
        if(time_cnt > (DELAY_LOGO +2))
        {
            time_cnt = 0;
            init_img_flag = true;
            lv_obj_add_flag(guider_ui.screen_home_hor_img_init, LV_OBJ_FLAG_HIDDEN);
        }
    }

    //设置鼠标插入时显示的鼠标图片
    if (mouse_plugged && frist_set_mouse_image_flag && init_set_img_ok)
    {
        lv_obj_clear_flag(cursor_img, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_src(cursor_img, MY_LVGL_IMAGE_PATH(picture/mouse/mouse_white.png));
        frist_set_mouse_image_flag = false;
    }
    else if(!mouse_plugged && !frist_set_mouse_image_flag)
    {
        lv_obj_add_flag(cursor_img, LV_OBJ_FLAG_HIDDEN);
        frist_set_mouse_image_flag = true;
        mouse_leave_flag = true; // 鼠标离开标志
    }

    if (udisk_update_state == IMAGE_UPDATE_FINISH)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "更新完成,\n拔出U盘后播放");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "Update complete,\nremove the usb drive and play");
        if (page_image_cnt[update_page_num] == 0)
        {
            lv_img_set_src(guider_ui.screen_home_hor_img_img, MY_LVGL_IMAGE_PATH(picture/background/world.jpg));
            lv_img_set_src(guider_ui.screen_home_hor_img_background, image_filename[MY_SET_IMAGE.image - 1]);
        }
        save_begin();
    }
    else if (udisk_update_state == VIDEO_IS_UPDATED)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "视频已是最新");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "video is new");
    }else if (udisk_update_state == VIDEO_OVER_WIGHT)
    {
        // udisk_update_state = 0;
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_38, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "视频分辨率超限,\n请确认视频分辨率小于1024*768或768*1024,\n请拔出U盘退出");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "video size over 1024*768 or 768*1024,\nplease check.\nremove usb and exit");
    }else if (udisk_update_state == VIDEO_TYPE_NONE)
    {
        // udisk_update_state = 0;
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_45, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "暂不支持,\n请更新编码为H264的MP4或AVI视频");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "Update video to MP4/AVI with H.264");
    }
    else if (udisk_update_state == VIDEO_NO_FIDE)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "未找到视频");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "No video");
    }
    else if (udisk_update_state == IMAGE_NO_FIDE)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "未找到图片");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "No picture");
    }else if (udisk_update_state == VIDEO_UPDATING)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "视频更新中: %d%%(%d/%d)", read_percent, video_num + 1, media_count);
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "video update: %d%%(%d/%d)", read_percent, video_num + 1, media_count);
    }else if (udisk_update_state == IMAGE_UPDATING)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "图片 %d/%d: %d%%", current_media_idx + 1, usb_image_cnt, read_percent);
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "picture %d/%d: %d%%", current_media_idx + 1, usb_image_cnt, read_percent);
    }else if (udisk_update_state == VIDEO_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        lv_label_set_text(guider_ui.screen_home_hor_label_update, "video error");
    }else if (udisk_update_state == TEXT_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "请检查文本格式");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "please check text format");
    }else if (udisk_update_state == TEXT_OK)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "文本更新完成");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "text update complete");
    }
    else if (udisk_update_state == VIDEO_UPDATE_NO_MP4)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        lv_label_set_text(guider_ui.screen_home_hor_label_update, "NO MP4");
    }else if (udisk_update_state == VIDEO_UPDATE_ALL_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        lv_label_set_text(guider_ui.screen_home_hor_label_update, "ALL OVER 4G");
    }
    else if (udisk_update_state == VIDEO_UPDATE_FLASH_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        lv_label_set_text(guider_ui.screen_home_hor_label_update, "FLASH USED 90%%");
    }
    else if (udisk_update_state == VIDEO_UPDATE_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "视频超过500M");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_hor_label_update, "Video over 500M");
    }
    static bool set_wait_flag=false;
    // 如果更新完成，设置更新标志位，清空更新提示
    if (update_ok_flag)
    {
        udisk_update_state = UPDATE_NONE;
        update_ok_flag = false;
        lv_label_set_text_fmt(guider_ui.screen_home_hor_label_update, "");
    }
    if(tcp_return_home_flag && !set_wait_flag)
    {
        set_wait_flag = true;
        lv_obj_clear_flag(guider_ui.screen_home_hor_img_wait, LV_OBJ_FLAG_HIDDEN); // 可见
    }else if(!tcp_return_home_flag && set_wait_flag)
    {
        set_wait_flag = false;
        lv_obj_add_flag(guider_ui.screen_home_hor_img_wait, LV_OBJ_FLAG_HIDDEN); // 不可见
    }

    if(home_hor_video_flag)
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

static void weather_callback(lv_timer_t *timer)
{
    uint16_t img_x = 747;
    uint16_t img_y = 19;
    static bool square_flag = false;

    if(change_weather_flag)
    {
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_temperature, LV_OBJ_FLAG_HIDDEN);       // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_img_weather, LV_OBJ_FLAG_HIDDEN);       // 可见
        change_weather_flag = false;
        switch(my_weather.weather)
        {
            case 0:
            case 53:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+18, img_y+82);
            break;
            case 1:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+27, img_y+56);
            break;
            case 2:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+23, img_y+52);
            break;
            case 3:
            case 13:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+46);
            break;
            case 4:
            case 5:
            case 12:
            case 17:
            case 25:
            case 28:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+34);
            break;
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 14:
            case 15:
            case 16:
            case 21:
            case 22:
            case 23:
            case 26:
            case 27:
            case 301:
            case 302:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+39);
            break;
            case 18:
            case 24:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+37);
            break;
            case 19:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+21, img_y+37);
            break;
            case 20:
            case 29:
            case 30:
            case 31:
                lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+16, img_y+75);
            break;
            default: break;
        }
        lv_label_set_text_fmt(guider_ui.screen_home_hor_label_temperature, "%d°C", my_weather.temperature);
        if(my_weather.weather >= 0 && my_weather.weather <= 31)
            lv_img_set_src(guider_ui.screen_home_hor_img_weather, image_weather_filename[my_weather.weather]);
        else if(my_weather.weather == 53)
            lv_img_set_src(guider_ui.screen_home_hor_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/53.png));
        else if(my_weather.weather == 301)
            lv_img_set_src(guider_ui.screen_home_hor_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/301.png));
        else if(my_weather.weather == 302)
            lv_img_set_src(guider_ui.screen_home_hor_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/302.png));
    }

    if(weather_erro_flag && !show_weather_error_flag)
    {
        show_weather_error_flag = true;
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_weather_erro, LV_OBJ_FLAG_HIDDEN);       // 可见
    }else if(show_weather_error_flag && !weather_erro_flag){
        show_weather_error_flag = false;
        lv_obj_add_flag(guider_ui.screen_home_hor_label_weather_erro, LV_OBJ_FLAG_HIDDEN);       // 不可见
    }

    static bool weather_init_flag = false;
    if(get_lwip_flag && !weather_init_flag)
    {
        weather_init_flag = true;
        if(MY_SET_IMAGE.image == IMAGE_C404_hor);
        else
        {
            lv_obj_add_flag(guider_ui.screen_home_hor_label_temperature, LV_OBJ_FLAG_HIDDEN);       // 不可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_weather, LV_OBJ_FLAG_HIDDEN);       // 不可见
        }
    }else if(!get_lwip_flag && weather_init_flag)
    {
        weather_init_flag = false;
        weather_erro_flag = false;
        show_weather_error_flag = false;
        set_sync_cnt = 0;
        lv_obj_add_flag(guider_ui.screen_home_hor_label_weather_erro, LV_OBJ_FLAG_HIDDEN);       // 不可见
        lv_obj_add_flag(guider_ui.screen_home_hor_label_temperature, LV_OBJ_FLAG_HIDDEN);       // 不可见
        lv_obj_add_flag(guider_ui.screen_home_hor_img_weather, LV_OBJ_FLAG_HIDDEN);       // 不可见
    }

    if(txt_renew_flag)
    {
        txt_renew_flag = false;
        FILE *flash_txt_file = fopen(txt_path[txt_update_page_num], "r");
        if (!flash_txt_file)
        {
            rt_kprintf("【update.txt】错误：无法打开文件\n");
        }

        long txt_file_len = safe_get_file_size(flash_txt_file);
        if (txt_file_len <= 0 || txt_file_len >= sizeof(update_txt_content))
        {
            rt_kprintf("【update.txt】错误：文件为空、过大或大小获取失败。\n");
        }
        int read_len = -1;
        read_len = fread(update_txt_content, 1, txt_file_len, flash_txt_file);
        if (read_len == txt_file_len)
        {
            update_txt_content[read_len] = '\0';
            lv_font_t *font;
            if (MY_SET_IMAGE.image == IMAGE_C402_hor)
            {
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                24,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }else if(MY_SET_IMAGE.image == IMAGE_C404_hor){
                lv_obj_set_size(guider_ui.screen_home_hor_label_welecom, 330, 38);
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                29,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }else{
                lv_obj_set_size(guider_ui.screen_home_hor_label_welecom, 319, 38);
                if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
                {
                    lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 697, 691);
                }
                else if(MY_SET_IMAGE.image == IMAGE_C401_hor)
                {
                    lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 101, 721);
                }else if(MY_SET_IMAGE.image == IMAGE_C403_hor)
                {
                    lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 97, 701);
                }
                if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
                {
                    font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                    LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                    32,
                    LV_FREETYPE_FONT_STYLE_NORMAL);
                }else{
                    font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                    LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                    28,
                    LV_FREETYPE_FONT_STYLE_NORMAL);
                }

            }

            if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
            {
                lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_201_welcome, font, LV_PART_MAIN|LV_STATE_DEFAULT);

                if(txt_mode[txt_update_page_num])
                    lv_label_set_long_mode(guider_ui.screen_home_hor_label_201_welcome, LV_LABEL_LONG_SCROLL_CIRCULAR);
                else
                    lv_label_set_long_mode(guider_ui.screen_home_hor_label_201_welcome, LV_LABEL_LONG_WRAP);
            }else{
                lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_welecom, font, LV_PART_MAIN|LV_STATE_DEFAULT);

                if(txt_mode[txt_update_page_num])
                    lv_label_set_long_mode(guider_ui.screen_home_hor_label_welecom, LV_LABEL_LONG_SCROLL_CIRCULAR);
                else
                    lv_label_set_long_mode(guider_ui.screen_home_hor_label_welecom, LV_LABEL_LONG_WRAP);
            }

            lv_refr_now(NULL);
            if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
                lv_label_set_text_fmt(guider_ui.screen_home_hor_label_201_welcome, "%s", update_txt_content);
            else
                lv_label_set_text_fmt(guider_ui.screen_home_hor_label_welecom, "%s", update_txt_content);
            rt_kprintf("update_txt_content:%s\n",  update_txt_content);
            rt_kprintf("len:%d\n",  strlen(update_txt_content));
            // int32_t  text_real_w  = lv_text_get_width(
            //                 update_txt_content,               // 要计算的中英文文本
            //                 0,                  // 传0 = 计算整个文本的宽度，无需手动传字符长度
            //                 font,  // 必须传你的中英文混合字体！！
            //                 0       // 字符间距，和下面设置的一致
            //             );
            // if(!txt_mode[txt_update_page_num] && text_real_w > 290)
            // {
            //     if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
            //     {
            //         lv_label_set_long_mode(guider_ui.screen_home_hor_label_201_welcome, LV_LABEL_LONG_SCROLL_CIRCULAR);
            //     }else{
            //         lv_label_set_long_mode(guider_ui.screen_home_hor_label_welecom, LV_LABEL_LONG_SCROLL_CIRCULAR);
            //     }
            // }
            // printf("text_real_w : %d\n", text_real_w);
            lv_refr_now(NULL);
        }
        if (flash_txt_file)
        {
            fclose(flash_txt_file);
        }
    }
}

static void frist_callback(lv_timer_t *timer)
{
    static uint8_t show_frist_cnt = 0;
    if(show_frist_flag)
    {
        if(++show_frist_cnt > 2)
        {
            show_frist_cnt = 0;
            show_frist_flag = false;
            // if(long_text_flag) long_text_flag = false;
        }

        if(txt_mode[txt_update_page_num])
        {
            // printf("frist set long text\n");
            if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
                lv_label_set_long_mode(guider_ui.screen_home_hor_label_201_welcome, LV_LABEL_LONG_SCROLL_CIRCULAR);
            else
                lv_label_set_long_mode(guider_ui.screen_home_hor_label_welecom, LV_LABEL_LONG_SCROLL_CIRCULAR);
        }
        else
        {
            // printf("frist set no long text\n");
            if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
                lv_label_set_long_mode(guider_ui.screen_home_hor_label_201_welcome, LV_LABEL_LONG_WRAP);
            else
                lv_label_set_long_mode(guider_ui.screen_home_hor_label_welecom, LV_LABEL_LONG_WRAP);
        }

            if (have_floor_num == 1)
        {
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN);   // 不可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
        }
        else if (have_floor_num == 2)
        {
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
        }
        else if (have_floor_num == 3)
        {
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
        {
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 不可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN); // 不可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN); // 不可见
        }
        //
        if((MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor) && have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 130, 204);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 130, 204);
        }else if((MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor) && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 82, 140);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 82, 140);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 82, 140);
            // lv_refr_now(NULL);
        }else if(MY_SET_IMAGE.image == IMAGE_C203_hor && have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 221, 338);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 221, 338);
            // lv_refr_now(NULL);
        }else if(MY_SET_IMAGE.image == IMAGE_C203_hor && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 145, 260);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 145, 260);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 145, 260);
            // lv_refr_now(NULL);
        }else if((MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                MY_SET_IMAGE.image == IMAGE_C303_hor) && have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 124, 204);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 124, 204);
            // lv_refr_now(NULL);
        }else if((MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                MY_SET_IMAGE.image == IMAGE_C303_hor) && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 80, 140);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 80, 140);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 80, 140);
            // lv_refr_now(NULL);
        }
        else if(MY_SET_IMAGE.image == IMAGE_C401_hor && have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 100, 126);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 100, 126);
            // rt_kprintf("hsdsafasdsfsfsdfsdsdf");
            // lv_refr_now(NULL);
        }else if(MY_SET_IMAGE.image == IMAGE_C401_hor && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 65, 110);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 65, 110);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 65, 110);
            // lv_refr_now(NULL);
        }else if(MY_SET_IMAGE.image == IMAGE_C402_hor && have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 130, 155);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 130, 155);
            // lv_refr_now(NULL);
        }else if(MY_SET_IMAGE.image == IMAGE_C402_hor && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 83, 130);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 83, 130);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 83, 130);
            // lv_refr_now(NULL);
        }else if(MY_SET_IMAGE.image == IMAGE_C403_hor && have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 142, 172);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 142, 172);
            // lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 125, 180);
            // lv_refr_now(NULL);
        }else if(MY_SET_IMAGE.image == IMAGE_C403_hor && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 92, 140);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 92, 140);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 92, 140);
            // lv_refr_now(NULL);
        }else if(MY_SET_IMAGE.image == IMAGE_C404_hor && have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 135, 174);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 135, 174);
            // lv_refr_now(NULL);
        }else if (MY_SET_IMAGE.image == IMAGE_C404_hor &&  have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 85, 150);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 85, 150);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 85, 150);
            // lv_refr_now(NULL);
        }else if ((MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                   MY_SET_IMAGE.image == IMAGE_C303_hor) && have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 125, 198);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 125, 198);
            // lv_refr_now(NULL);
        }else if (MY_SET_IMAGE.image != IMAGE_C203_hor && MY_SET_IMAGE.image != IMAGE_C201_hor &&
            MY_SET_IMAGE.image != IMAGE_C202_hor)
        {
            lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 89, 140);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 89, 140);
            lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 89, 140);
            // lv_refr_now(NULL);
        }

        if(MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor ||
                 MY_SET_IMAGE.image == IMAGE_C403_hor || MY_SET_IMAGE.image == IMAGE_C404_hor)
        {
            lv_img_set_src(guider_ui.screen_home_hor_img_num1, image_num_c4_filename[show_floor_num[0]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num2, image_num_c4_filename[show_floor_num[1]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num3, image_num_c4_filename[show_floor_num[2]]);
        }else if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                 MY_SET_IMAGE.image == IMAGE_C303_hor)
        {
            lv_img_set_src(guider_ui.screen_home_hor_img_num1, image_num_filename[show_floor_num[0]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num2, image_num_filename[show_floor_num[1]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num3, image_num_filename[show_floor_num[2]]);
        }
        else
        {
            lv_img_set_src(guider_ui.screen_home_hor_img_num1, image_num_big_filename[show_floor_num[0]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num2, image_num_big_filename[show_floor_num[1]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num3, image_num_big_filename[show_floor_num[2]]);
        }
        lv_refr_now(NULL);
        lv_timer_handler();
    }

    if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
    {
        if(set_gif_arrow == UP_ARROW || set_gif_arrow == DOWN_ARROW)
        {
            uint8_t best_cnt;
            static uint8_t show_cnt = 0;

            // lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_gif));
            // lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_gif));
            if(set_gif_arrow == UP_ARROW)//上
            {
                if(MY_SET_IMAGE.arrow == ARROW_SWD)
                {
                    best_cnt = sizeof(image_filename_gif_swd_up) / sizeof(image_filename_gif_swd_up[0]);;
                    if(show_cnt >= best_cnt - 1) show_cnt = 0;
                    lv_img_set_src(guider_ui.screen_home_hor_img_gif, image_filename_gif_swd_up[show_cnt ++]);
                }else
                {
                    best_cnt = sizeof(image_filename_gif_xio_up) / sizeof(image_filename_gif_xio_up[0]);
                    if(show_cnt >= best_cnt - 1) show_cnt = 0;
                    lv_img_set_src(guider_ui.screen_home_hor_img_gif, image_filename_gif_xio_up[show_cnt ++]);
                }
            }else if(set_gif_arrow == DOWN_ARROW)
            {
                if(MY_SET_IMAGE.arrow == ARROW_SWD)
                {
                    best_cnt = sizeof(image_filename_gif_swd_down) / sizeof(image_filename_gif_swd_down[0]);;
                    if(show_cnt >= best_cnt - 1) show_cnt = 0;
                    lv_img_set_src(guider_ui.screen_home_hor_img_gif, image_filename_gif_swd_down[show_cnt ++]);
                }else {

                    best_cnt = sizeof(image_filename_gif_xio_down) / sizeof(image_filename_gif_xio_down[0]);
                    if(show_cnt >= best_cnt - 1) show_cnt = 0;
                    lv_img_set_src(guider_ui.screen_home_hor_img_gif, image_filename_gif_xio_down[show_cnt ++]);
                }
            }
        }
    }

    static uint8_t last_tcp_num = 11;
    if(is_tcp_connected && MY_SET_DHCP.host_state == false && !video_in_updating)
    {
        if(!page_image_cnt[update_page_num] && MY_SET.play_mode == PLAY_IMAGE)
        {
            if (MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor ||
                MY_SET_IMAGE.image == IMAGE_C403_hor || MY_SET_IMAGE.image == IMAGE_C201_hor ||
                MY_SET_IMAGE.image == IMAGE_C202_hor || MY_SET_IMAGE.image == IMAGE_C404_hor)
            {
                if(tcp_img_num && last_tcp_num != tcp_img_num)
                {
                    last_tcp_num = tcp_img_num;

                    if (MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor ||
                        MY_SET_IMAGE.image == IMAGE_C403_hor)
                    {
                        lv_img_set_src(guider_ui.screen_home_hor_img_background, image_flash_c403_hor[tcp_img_num - 1]);
                    }else if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                              MY_SET_IMAGE.image == IMAGE_C404_hor)
                    {
                        lv_img_set_src(guider_ui.screen_home_hor_img_img, image_flash_c201_hor[tcp_img_num - 1]);
                    }
                }
            }
        }else if(page_image_cnt[update_page_num] && MY_SET.play_mode == PLAY_IMAGE)
        {
            if(tcp_img_num && last_tcp_num != tcp_img_num)
            {
                last_tcp_num = tcp_img_num;
                const char *ext = ".jpg"; // 默认扩展名
                // 构建示例路径来获取扩展名
                char temp_path[30];
                sprintf(temp_path, "%s%d.png", image_flash_path_prefix[update_page_num], tcp_img_num - 1);
                if (access(temp_path, 0) == 0)
                ext = ".png";
                else
                {
                    sprintf(temp_path, "%s%d.jpg", image_flash_path_prefix[update_page_num], tcp_img_num - 1);
                    if (access(temp_path, 0) == 0)
                    ext = ".jpg";
                }
                // 构建最新图片路径（假设最后一个导入的图片为当前显示图片）
                char current_image_path[50] = {0};
                sprintf(current_image_path, "L:%s%d%s", image_flash_path_prefix[update_page_num], tcp_img_num - 1, ext);
                rt_kprintf("image_path:%s\n", current_image_path);
                // image_cnt_temp ++;
                // 刷新图片
                if (MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C201_hor ||
                    MY_SET_IMAGE.image == IMAGE_C202_hor)
                {
                    lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_img));
                    lv_img_set_src(guider_ui.screen_home_hor_img_img, current_image_path);
                    lv_image_set_inner_align(guider_ui.screen_home_hor_img_img, LV_IMAGE_ALIGN_STRETCH);
                }
                else
                {
                    lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_background));
                    lv_img_set_src(guider_ui.screen_home_hor_img_background, current_image_path);
                    lv_image_set_inner_align(guider_ui.screen_home_hor_img_background, LV_IMAGE_ALIGN_STRETCH);
                }
            }
        }
    }
}

static void show_update_floor(void)
{
    static uint8_t show_temp = 10;
    if (show_floor_flag)
    {
        show_floor_flag = false;
        if (elevator_data.disp_len == 0 && show_temp != 0)
        {
            show_temp = 0;
            have_floor_num = 0;
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN);   // 不可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN);   // 不可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
        }
        else if (elevator_data.disp_len == 1 && show_temp != 1)
        {
            show_temp = 1;
            have_floor_num = 1;
            // lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 89, 140);
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN);   // 不可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
            if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 97, 381);
                image_num_pos[0].x = 97;
                image_num_pos[0].y = 381;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C203_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 486, 177);
                image_num_pos[0].x = 486;
                image_num_pos[0].y = 177;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C301_hor ||
                     MY_SET_IMAGE.image == IMAGE_C302_hor ||
                     MY_SET_IMAGE.image == IMAGE_C303_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 92, 382);
                image_num_pos[0].x = 92;
                image_num_pos[0].y = 382;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C401_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 231, 70);
                image_num_pos[0].x = 231;
                image_num_pos[0].y = 70;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C402_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 86, 313);
                image_num_pos[0].x = 86;
                image_num_pos[0].y = 313;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C403_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 497, 299);
                image_num_pos[0].x = 497;
                image_num_pos[0].y = 299;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C404_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 65, 379);
                image_num_pos[0].x = 65;
                image_num_pos[0].y = 379;
            }
        }
        else if (elevator_data.disp_len == 2 && show_temp != 2)
        {
            show_temp = 2;
            have_floor_num = 2;
            // lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 89, 140);
            // lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 89, 140);
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_add_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
            if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 32, 381);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 154, 381);
                lv_refr_now(NULL);
                image_num_pos[0].x = 32;
                image_num_pos[1].x = 154;
                image_num_pos[0].y = image_num_pos[1].y = 381;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C203_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 486, 177);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 707, 177);
                lv_refr_now(NULL);
                image_num_pos[0].x = 486;
                image_num_pos[1].x = 707;
                image_num_pos[0].y = image_num_pos[1].y = 177;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C301_hor ||
                     MY_SET_IMAGE.image == IMAGE_C302_hor ||
                     MY_SET_IMAGE.image == IMAGE_C303_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 30, 382);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 158, 382);
                lv_refr_now(NULL);
                image_num_pos[0].x = 30;
                image_num_pos[1].x = 158;
                image_num_pos[0].y = image_num_pos[1].y = 382;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C401_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 181, 70);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 277, 70);
                lv_refr_now(NULL);
                image_num_pos[0].x = 181;
                image_num_pos[1].x = 277;
                image_num_pos[0].y = image_num_pos[1].y = 70;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C402_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 27, 313);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 149, 313);
                lv_refr_now(NULL);
                image_num_pos[0].x = 27;
                image_num_pos[1].x = 149;
                image_num_pos[0].y = image_num_pos[1].y = 313;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C403_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 425, 299);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 557, 299);
                lv_refr_now(NULL);
                image_num_pos[0].x = 425;
                image_num_pos[1].x = 557;
                image_num_pos[0].y = image_num_pos[1].y = 299;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C404_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 3, 381);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 126, 381);
                lv_refr_now(NULL);
                image_num_pos[0].x = 3;
                image_num_pos[1].x = 126;
                image_num_pos[0].y = image_num_pos[1].y = 381;
            }
        }
        else if (elevator_data.disp_len == 3 && show_temp != 3)
        {
            show_temp = 3;
            have_floor_num = 3;
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_num3, LV_OBJ_FLAG_HIDDEN); // 可见
            if (MY_SET_IMAGE.image == IMAGE_C201_hor ||
                MY_SET_IMAGE.image == IMAGE_C202_hor)
            {
                rt_kprintf("set size in 201\n");
                first_set_num = true;

                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 36, 411);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 117, 411);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num3, 199, 411);
                lv_refr_now(NULL);
                image_num_pos[0].x = 36;
                image_num_pos[1].x = 117;
                image_num_pos[2].x = 199;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 411;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C203_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 491, 215);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 633, 215);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num3, 778, 215);
                lv_refr_now(NULL);
                image_num_pos[0].x = 491;
                image_num_pos[1].x = 633;
                image_num_pos[2].x = 778;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 215;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C301_hor ||
                     MY_SET_IMAGE.image == IMAGE_C302_hor ||
                     MY_SET_IMAGE.image == IMAGE_C303_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 32, 413);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 115, 413);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num3, 199, 413);
                lv_refr_now(NULL);
                image_num_pos[0].x = 32;
                image_num_pos[1].x = 115;
                image_num_pos[2].x = 199;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 413;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C401_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 182, 85);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 244, 85);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num3, 307, 85);
                lv_refr_now(NULL);
                image_num_pos[0].x = 182;
                image_num_pos[1].x = 244;
                image_num_pos[2].x = 307;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 85;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C402_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 31, 325);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 112, 325);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num3, 193, 325);
                lv_refr_now(NULL);
                image_num_pos[0].x = 31;
                image_num_pos[1].x = 112;
                image_num_pos[2].x = 193;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 325;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C403_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 430, 320);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 515, 320);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num3, 601, 320);
                lv_refr_now(NULL);
                image_num_pos[0].x = 430;
                image_num_pos[1].x = 515;
                image_num_pos[2].x = 601;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 320;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C404_hor)
            {
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 10, 387);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 91, 387);
                lv_obj_set_pos(guider_ui.screen_home_hor_img_num3, 172, 387);
                lv_refr_now(NULL);
                image_num_pos[0].x = 10;
                image_num_pos[1].x = 91;
                image_num_pos[2].x = 172;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 387;
            }
        }
        static uint8_t size_temp = 10;
        if(size_temp != have_floor_num)
        {
            size_temp = have_floor_num;
            if((MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor) && have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 130, 204);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 130, 204);
                lv_refr_now(NULL);
            }else if((MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor) && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 82, 140);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 82, 140);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 82, 140);
                lv_refr_now(NULL);
            }else if(MY_SET_IMAGE.image == IMAGE_C203_hor && have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 221, 338);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 221, 338);
                lv_refr_now(NULL);
            }else if(MY_SET_IMAGE.image == IMAGE_C203_hor && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 145, 260);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 145, 260);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 145, 260);
                lv_refr_now(NULL);
            }else if((MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                      MY_SET_IMAGE.image == IMAGE_C303_hor) && have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 124, 204);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 124, 204);
                lv_refr_now(NULL);
            }else if((MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                      MY_SET_IMAGE.image == IMAGE_C303_hor) && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 80, 140);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 80, 140);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 80, 140);
                lv_refr_now(NULL);
            }
            else if(MY_SET_IMAGE.image == IMAGE_C401_hor && have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 100, 126);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 100, 126);
                lv_refr_now(NULL);
            }else if(MY_SET_IMAGE.image == IMAGE_C401_hor && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 65, 110);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 65, 110);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 65, 110);
                lv_refr_now(NULL);
            }else if(MY_SET_IMAGE.image == IMAGE_C402_hor && have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 130, 155);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 130, 155);
                lv_refr_now(NULL);
            }else if(MY_SET_IMAGE.image == IMAGE_C402_hor && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 83, 130);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 83, 130);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 83, 130);
                lv_refr_now(NULL);
            }else if(MY_SET_IMAGE.image == IMAGE_C403_hor && have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 142, 172);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 142, 172);
                // lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 125, 180);
                lv_refr_now(NULL);
            }else if(MY_SET_IMAGE.image == IMAGE_C403_hor && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 92, 140);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 92, 140);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 92, 140);
                lv_refr_now(NULL);
            }else if(MY_SET_IMAGE.image == IMAGE_C404_hor && have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 135, 174);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 135, 174);
                lv_refr_now(NULL);
            }else if (MY_SET_IMAGE.image == IMAGE_C404_hor &&  have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 85, 150);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 85, 150);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 85, 150);
                lv_refr_now(NULL);
            }else if ((MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                    MY_SET_IMAGE.image == IMAGE_C303_hor) && have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 125, 198);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 125, 198);
                lv_refr_now(NULL);
            }
            else if (MY_SET_IMAGE.image != IMAGE_C203_hor && MY_SET_IMAGE.image != IMAGE_C201_hor &&
                    MY_SET_IMAGE.image != IMAGE_C202_hor)
            {
                lv_obj_set_size(guider_ui.screen_home_hor_img_num1, 89, 140);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num2, 89, 140);
                lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 89, 140);
                lv_refr_now(NULL);
            }
        }

        if(MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor ||
        MY_SET_IMAGE.image == IMAGE_C403_hor || MY_SET_IMAGE.image == IMAGE_C404_hor)
        {
            lv_img_set_src(guider_ui.screen_home_hor_img_num1, image_num_c4_filename[show_floor_num[0]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num2, image_num_c4_filename[show_floor_num[1]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num3, image_num_c4_filename[show_floor_num[2]]);
        }else if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
                 MY_SET_IMAGE.image == IMAGE_C303_hor)
        {
            lv_img_set_src(guider_ui.screen_home_hor_img_num1, image_num_filename[show_floor_num[0]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num2, image_num_filename[show_floor_num[1]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num3, image_num_filename[show_floor_num[2]]);
        }
        else
        {
            lv_img_set_src(guider_ui.screen_home_hor_img_num1, image_num_big_filename[show_floor_num[0]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num2, image_num_big_filename[show_floor_num[1]]);
            lv_img_set_src(guider_ui.screen_home_hor_img_num3, image_num_big_filename[show_floor_num[2]]);
        }
    }
}

FAKE_IMAGE_DECLARE(bg_hor_dark);
void setup_scr_screen_home_hor(lv_ui *ui)
{
    FAKE_IMAGE_INIT(bg_hor_dark, 1024, 768, 0, 0x00000000);
    // Write codes screen_home_hor
    ui->screen_home_hor = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_home_hor, 1024, 768);
    lv_obj_set_scrollbar_mode(ui->screen_home_hor, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_img_src(ui->screen_home_hor, FAKE_IMAGE_NAME(bg_hor_dark), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Write style for screen_home_hor, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_home_hor, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_home_hor, lv_color_hex(0xc3c3c3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_home_hor, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_background
    ui->screen_home_hor_img_background = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_background, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_background, &_C401_hor_alpha_1024x768);
    lv_img_set_pivot(ui->screen_home_hor_img_background, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_background, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_background, 0, 0);
    lv_obj_set_size(ui->screen_home_hor_img_background, 1024, 768);

    // Write style for screen_home_hor_img_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_background, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_shade_up
    ui->screen_home_hor_img_shade_up = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_shade_up, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_shade_up, &_hor_alpha_1024x245);
    lv_img_set_pivot(ui->screen_home_hor_img_shade_up, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_shade_up, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_shade_up, 0, 0);
    lv_obj_set_size(ui->screen_home_hor_img_shade_up, 1024, 245);

    // Write style for screen_home_hor_img_shade_up, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_shade_up, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_shade_up, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_shade_up, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_shade_up, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_home_hor_img_weather
    ui->screen_home_hor_img_weather = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_weather, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_weather, &_duoyun_alpha_118x96);
    lv_img_set_pivot(ui->screen_home_hor_img_weather, 50,50);
    lv_img_set_angle(ui->screen_home_hor_img_weather, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_weather, 747, 19);
    lv_obj_set_size(ui->screen_home_hor_img_weather, 118, 96);

    //Write style for screen_home_hor_img_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_hor_img_weather, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_weather, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_shade_down
    ui->screen_home_hor_label_shade_down = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_shade_down, "");
    lv_label_set_long_mode(ui->screen_home_hor_label_shade_down, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_shade_down, 0, 710);
    lv_obj_set_size(ui->screen_home_hor_label_shade_down, 1024, 58);

    // Write style for screen_home_hor_label_shade_down, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_shade_down, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_shade_down, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_shade_down, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_shade_down, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_shade_down, 179, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_home_hor_label_shade_down, lv_color_hex(0x333638), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_home_hor_label_shade_down, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_shade_left
    ui->screen_home_hor_img_shade_left = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_shade_left, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_shade_left, &_402_alpha_350x768);
    lv_img_set_pivot(ui->screen_home_hor_img_shade_left, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_shade_left, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_shade_left, 0, 0);
    lv_obj_set_size(ui->screen_home_hor_img_shade_left, 334, 768);

    // Write style for screen_home_hor_img_shade_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_shade_left, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_shade_left, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_shade_left, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_shade_left, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_home_hor_img_wait = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_wait, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_home_hor_img_wait, MY_LVGL_IMAGE_PATH(picture/logo/wait.png));
    lv_img_set_pivot(ui->screen_home_hor_img_wait, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_wait, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_wait, 480, 350);
    lv_obj_set_size(ui->screen_home_hor_img_wait, 327, 100);

    // Write style for screen_home_hor_img_wait, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_wait, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_wait, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_wait, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_wait, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_fire
    ui->screen_home_hor_label_fire = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_fire, "消防\nFIRE");
    lv_label_set_long_mode(ui->screen_home_hor_label_fire, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_fire, 50, 615);
    lv_obj_set_size(ui->screen_home_hor_label_fire, 79, 70);

    // Write style for screen_home_hor_label_fire, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_fire, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_fire, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_fire, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_fire, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_overload
    ui->screen_home_hor_label_overload = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_overload, "超载\nOVERLOAD");
    lv_label_set_long_mode(ui->screen_home_hor_label_overload, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_overload, 123, 615);
    lv_obj_set_size(ui->screen_home_hor_label_overload, 172, 70);

    // Write style for screen_home_hor_label_overload, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_overload, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_overload, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_overload, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_overload, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_fire_401
    ui->screen_home_hor_label_fire_401 = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_fire_401, "消防 FIRE");
    lv_label_set_long_mode(ui->screen_home_hor_label_fire_401, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_fire_401, 341, 723);
    lv_obj_set_size(ui->screen_home_hor_label_fire_401, 164, 35);

    // Write style for screen_home_hor_label_fire_401, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_fire_401, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_fire_401, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_fire_401, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_fire_401, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_overload_401
    ui->screen_home_hor_label_overload_401 = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_overload_401, "超载  OVERLOAD");
    lv_label_set_long_mode(ui->screen_home_hor_label_overload_401, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_overload_401, 81, 723);
    lv_obj_set_size(ui->screen_home_hor_label_overload_401, 256, 35);

    // Write style for screen_home_hor_label_overload_401, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_overload_401, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_overload_401, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_overload_401, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_overload_401, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_home_hor_label_square
    ui->screen_home_hor_label_square = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_square, "");
    lv_label_set_long_mode(ui->screen_home_hor_label_square, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_square, 305, 124);
    lv_obj_set_size(ui->screen_home_hor_label_square, 702, 522);
// rt_kprintf("here 2\n");
    //Write style for screen_home_hor_label_square, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_square, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_home_hor_label_square, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_home_hor_label_square, lv_color_hex(0x585858), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_home_hor_label_square, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_square, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_square, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_square, &lv_font_Dengb_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_square, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_square, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_square, 11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->screen_home_hor_label_square, lv_color_hex(0x7b7b7b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->screen_home_hor_label_square, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->screen_home_hor_label_square, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->screen_home_hor_label_square, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_fire_403
    ui->screen_home_hor_label_fire_403 = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_fire_403, "消防  FIRE");
    lv_label_set_long_mode(ui->screen_home_hor_label_fire_403, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_fire_403, 345, 705);
    lv_obj_set_size(ui->screen_home_hor_label_fire_403, 164, 35);

    // Write style for screen_home_hor_label_fire_403, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_fire_403, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_fire_403, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_fire_403, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_fire_403, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_fire_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_overload_403
    ui->screen_home_hor_label_overload_403 = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_overload_403, "超载  OVERLOAD");
    lv_label_set_long_mode(ui->screen_home_hor_label_overload_403, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_overload_403, 87, 705);
    lv_obj_set_size(ui->screen_home_hor_label_overload_403, 256, 35);

    // Write style for screen_home_hor_label_overload_403, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_overload_403, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_overload_403, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_overload_403, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_overload_403, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_overload_403, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_img
    ui->screen_home_hor_img_img = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_img, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_img, &_C301_hor_alpha_680x460);
    lv_img_set_pivot(ui->screen_home_hor_img_img, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_img, 0);
    // lv_obj_set_pos(ui->screen_home_hor_img_img, 330, 141);
    // lv_obj_set_size(ui->screen_home_hor_img_img, 680, 460);

    // Write style for screen_home_hor_img_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_img, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_arrow_xio
    ui->screen_home_hor_img_arrow_xio = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_arrow_xio, &_xio_up_alpha_146x140);
    lv_img_set_pivot(ui->screen_home_hor_img_arrow_xio, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_arrow_xio, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_arrow_xio, 11, 49);
    lv_obj_set_size(ui->screen_home_hor_img_arrow_xio, 146, 140);

    // Write style for screen_home_hor_img_arrow_xio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_arrow_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_arrow_xio, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_arrow_swd
    ui->screen_home_hor_img_arrow_swd = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_arrow_swd, &_swd_up_alpha_121x140);
    lv_img_set_pivot(ui->screen_home_hor_img_arrow_swd, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_arrow_swd, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_arrow_swd, 21, 49);
    lv_obj_set_size(ui->screen_home_hor_img_arrow_swd, 121, 140);

    // Write style for screen_home_hor_img_arrow_swd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_arrow_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_arrow_swd, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Write codes screen_home_hor_img_num1
    ui->screen_home_hor_img_num1 = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_num1, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_num1, &_2_alpha_89x140);
    lv_img_set_pivot(ui->screen_home_hor_img_num1, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_num1, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_num1, 179, 49);
    // lv_obj_set_size(ui->screen_home_hor_img_num1, 89, 140);

    // Write style for screen_home_hor_img_num1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_num1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_hor_img_num1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_num1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_num1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_num1, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_num2
    ui->screen_home_hor_img_num2 = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_num2, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_num2, &_2_alpha_89x140);
    lv_img_set_pivot(ui->screen_home_hor_img_num2, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_num2, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_num2, 271, 49);
    // lv_obj_set_size(ui->screen_home_hor_img_num2, 89, 140);

    // Write style for screen_home_hor_img_num2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_num2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_hor_img_num2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_num2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_num2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_num2, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_num3
    ui->screen_home_hor_img_num3 = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_num3, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_num3, &_2_alpha_89x140);
    lv_img_set_pivot(ui->screen_home_hor_img_num3, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_num3, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_num3, 271, 49);
    // lv_obj_set_size(ui->screen_home_hor_img_num3, 89, 140);

    // Write style for screen_home_hor_img_num3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_num3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_hor_img_num3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_num3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_num3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_num3, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_date
    ui->screen_home_hor_label_date = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_date, "");
    lv_label_set_long_mode(ui->screen_home_hor_label_date, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_date, 683, 726);
    lv_obj_set_size(ui->screen_home_hor_label_date, 319, 33);

    // Write style for screen_home_hor_label_date, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_date, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_date, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_home_hor_label_date_c3
    ui->screen_home_hor_label_date_c3 = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_date_c3, "Thu/星期四");
    lv_label_set_long_mode(ui->screen_home_hor_label_date_c3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_date_c3, 829, 65);
    lv_obj_set_size(ui->screen_home_hor_label_date_c3, 176, 48);

    //Write style for screen_home_hor_label_date_c3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_date_c3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_date_c3, &lv_font_Dengb_27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_date_c3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_date_c3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_date_c3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_welecom
    ui->screen_home_hor_label_welecom = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_welecom, "斯沃德杭州西奥本电梯欢迎您");

        lv_label_set_long_mode(ui->screen_home_hor_label_welecom, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_welecom, 101, 726);
    lv_obj_set_size(ui->screen_home_hor_label_welecom, 319, 33);

    // Write style for screen_home_hor_label_welecom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_welecom, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_welecom, &lv_font_YaHei_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_welecom, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_welecom, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_welecom, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_home_hor, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_home_hor_label_201_welcome
    ui->screen_home_hor_label_201_welcome = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_201_welcome, "WELCOME");
    lv_label_set_long_mode(ui->screen_home_hor_label_201_welcome, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_201_welcome, 309, 687);
    lv_obj_set_size(ui->screen_home_hor_label_201_welcome, 330, 43);

    //Write style for screen_home_hor_label_201_welcome, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_201_welcome, lv_color_hex(0x5A2E31), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_201_welcome, &lv_font_Dengb_36, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_201_welcome, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_201_welcome, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_201_welcome, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_201_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_logo_xio
    ui->screen_home_hor_img_logo_xio = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_logo_xio, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_logo_xio, &_logo_xio_alpha_301x33);
    lv_img_set_pivot(ui->screen_home_hor_img_logo_xio, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_logo_xio, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_logo_xio, 710, 14);
    lv_obj_set_size(ui->screen_home_hor_img_logo_xio, 280, 30);

    // Write style for screen_home_hor_img_logo_xio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_logo_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_logo_xio, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_logo_swd
    ui->screen_home_hor_img_logo_swd = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_logo_swd, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_logo_swd, &_logo_swd_alpha_180x36);+
    lv_img_set_pivot(ui->screen_home_hor_img_logo_swd, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_logo_swd, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_logo_swd, 834, 14);
    lv_obj_set_size(ui->screen_home_hor_img_logo_swd, 180, 35);

    // Write style for screen_home_hor_img_logo_swd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_logo_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_logo_swd, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    // rt_kprintf("here 3\n");
    // Write codes screen_home_hor_label_time
    ui->screen_home_hor_label_time = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_time, "");
    lv_label_set_long_mode(ui->screen_home_hor_label_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_time, 871, 4);
    lv_obj_set_size(ui->screen_home_hor_label_time, 142, 45);

    // Write style for screen_home_hor_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_time, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_time, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Write codes screen_home_hor_label_C201
    ui->screen_home_hor_label_C201 = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_C201, "");
    lv_label_set_long_mode(ui->screen_home_hor_label_C201, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_C201, 871, 4);
    lv_obj_set_size(ui->screen_home_hor_label_C201, 142, 45);

    // Write style for screen_home_hor_label_C201, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_C201, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_C201, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_C201, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_C201, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_C201, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_C201, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_label_update
    ui->screen_home_hor_label_update = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_update, "");
    lv_label_set_long_mode(ui->screen_home_hor_label_update, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_update, 400, 384);
    lv_obj_set_size(ui->screen_home_hor_label_update, 768, 150);

    // Write style for screen_home_hor_label_update, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_update, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_update, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_update, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_hor_img_smoke
    ui->screen_home_hor_img_smoke = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_smoke, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_smoke, &_6_alpha_50x49);
    lv_img_set_pivot(ui->screen_home_hor_img_smoke, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_smoke, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_smoke, 226, 664);
    lv_obj_set_size(ui->screen_home_hor_img_smoke, 43, 43);

    // Write style for screen_home_hor_img_smoke, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_smoke, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_smoke, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_smoke, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_smoke, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes screen_home_hor_label_temperature
    ui->screen_home_hor_label_temperature = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_temperature, "");
    lv_label_set_long_mode(ui->screen_home_hor_label_temperature, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_temperature, 770, 76);
    lv_obj_set_size(ui->screen_home_hor_label_temperature, 44, 16);

    //Write style for screen_home_hor_label_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_temperature, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_temperature, &lv_font_Alatsi_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_temperature, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);



    // Write codes screen_home_hor_img_init
    ui->screen_home_hor_img_init = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_init, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_home_hor_img_init, MY_LVGL_IMAGE_PATH(picture/background/black_hor.png));
    lv_img_set_pivot(ui->screen_home_hor_img_init, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_init, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_init, 0, 0);
    lv_obj_set_size(ui->screen_home_hor_img_init, 1024, 768);

    // Write style for screen_home_hor_img_init, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_init, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_init, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_init, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_init, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    if(init_set_img_ok) lv_obj_add_flag(guider_ui.screen_home_hor_img_init, LV_OBJ_FLAG_HIDDEN);

    ui->screen_home_hor_img_init_logo = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_init_logo, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_home_hor_img_init_logo, MY_LVGL_IMAGE_PATH(picture/background/init.png));
    lv_img_set_pivot(ui->screen_home_hor_img_init_logo, 50, 50);
    lv_img_set_angle(ui->screen_home_hor_img_init_logo, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_init_logo, 380, 336);
    lv_obj_set_size(ui->screen_home_hor_img_init_logo, 265, 101);

    // Write style for screen_home_hor_img_init_logo, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_init_logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_init_logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_init_logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_init_logo, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_image_set_inner_align(guider_ui.screen_home_hor_img_init_logo, LV_IMAGE_ALIGN_STRETCH);
    if(init_set_img_ok) lv_obj_add_flag(guider_ui.screen_home_hor_img_init_logo, LV_OBJ_FLAG_HIDDEN);
    ui->screen_home_hor_img_gif = lv_img_create(ui->screen_home_hor);
    lv_obj_add_flag(ui->screen_home_hor_img_gif, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_hor_img_gif, &_duoyun_alpha_118x96);
    lv_img_set_pivot(ui->screen_home_hor_img_gif, 123,106);
    lv_img_set_angle(ui->screen_home_hor_img_gif, 0);
    lv_obj_set_pos(ui->screen_home_hor_img_gif, 31, 105);
    lv_obj_set_size(ui->screen_home_hor_img_gif, 246, 209);

    //Write style for screen_home_hor_img_gif, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_gif, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_hor_img_gif, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_img_gif, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_hor_img_gif, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_flag(guider_ui.screen_home_hor_img_gif, LV_OBJ_FLAG_HIDDEN);

    //Write codes screen_home_hor_label_video
    ui->screen_home_hor_label_video = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_video, "  ");
    lv_label_set_long_mode(ui->screen_home_hor_label_video, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_video, 494, 312);
    lv_obj_set_size(ui->screen_home_hor_label_video, 429, 75);

    //Write style for screen_home_hor_label_video, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_video, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_video, &lv_font_Dengb_50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_video, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_video, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_video, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_hor_label_weather_erro
    ui->screen_home_hor_label_weather_erro = lv_label_create(ui->screen_home_hor);
    lv_label_set_text(ui->screen_home_hor_label_weather_erro, "请检查IP");
    lv_label_set_long_mode(ui->screen_home_hor_label_weather_erro, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_hor_label_weather_erro, 730, 64);
    lv_obj_set_size(ui->screen_home_hor_label_weather_erro, 128, 28);

    //Write style for screen_home_hor_label_weather_erro, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_hor_label_weather_erro, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_hor_label_weather_erro, &lv_font_Dengb_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_hor_label_weather_erro, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_hor_label_weather_erro, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_hor_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_flag(ui->screen_home_hor_label_weather_erro, LV_OBJ_FLAG_HIDDEN);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // The custom code of screen_home_hor.
    lv_obj_clear_flag(ui->screen_home_hor, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->screen_home_hor_img_background, LV_OBJ_FLAG_SCROLLABLE);

    rt_kprintf("image:%s\n", image_filename[MY_SET_IMAGE.image - 1]);
    lv_img_set_src(ui->screen_home_hor_img_background, image_filename[MY_SET_IMAGE.image - 1]);
    lv_img_set_src(ui->screen_home_hor_img_smoke, MY_LVGL_IMAGE_PATH(picture/logo/smoke.png));
    lv_img_set_src(ui->screen_home_hor_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio.png));
    lv_img_set_src(ui->screen_home_hor_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd.png));
    lv_img_set_src(ui->screen_home_hor_img_shade_up, MY_LVGL_IMAGE_PATH(picture/shade/up_hor.png));
    lv_img_set_src(ui->screen_home_hor_img_shade_left, MY_LVGL_IMAGE_PATH(picture/shade/left_hor.png));

    uint16_t img_x = 747;
    uint16_t img_y = 19;
    switch(my_weather.weather)
    {
        case 0:
        case 53:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+18, img_y+82);
        break;
        case 1:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+27, img_y+56);
        break;
        case 2:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+23, img_y+52);
        break;
        case 3:
        case 13:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+46);
        break;
        case 4:
        case 12:
        case 17:
        case 25:
        case 28:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+34);
        break;
        case 5:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+34);
        break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 14:
        case 15:
        case 16:
        case 21:
        case 22:
        case 23:
        case 26:
        case 27:
        case 301:
        case 302:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+39);
        break;
        case 18:
        case 24:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+29, img_y+37);
        break;
        case 19:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+21, img_y+37);
        break;
        case 20:
        case 29:
        case 30:
        case 31:
            lv_obj_set_pos(guider_ui.screen_home_hor_label_temperature, img_x+16, img_y+75);
        break;
        default: break;
    }
    if(get_lwip_flag && geted_weather_flag)
    {
        lv_label_set_text_fmt(guider_ui.screen_home_hor_label_temperature, "%d°C", my_weather.temperature);
        if(my_weather.weather >= 0 && my_weather.weather <= 31)
            lv_img_set_src(guider_ui.screen_home_hor_img_weather, image_weather_filename[my_weather.weather]);
        else if(my_weather.weather == 53)
            lv_img_set_src(guider_ui.screen_home_hor_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/53.png));
        else if(my_weather.weather == 301)
            lv_img_set_src(guider_ui.screen_home_hor_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/301.png));
        else if(my_weather.weather == 302)
            lv_img_set_src(guider_ui.screen_home_hor_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/302.png));
    }

    lv_image_set_inner_align(ui->screen_home_hor_img_background, LV_IMAGE_ALIGN_STRETCH);
    if(MY_SET_IMAGE.image == IMAGE_C404_hor)
    {
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_temperature, LV_OBJ_FLAG_HIDDEN);       // 不可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_img_weather, LV_OBJ_FLAG_HIDDEN);       // 不可见
    }
    else
    {
        lv_obj_add_flag(guider_ui.screen_home_hor_label_temperature, LV_OBJ_FLAG_HIDDEN);       // 不可见
        lv_obj_add_flag(guider_ui.screen_home_hor_img_weather, LV_OBJ_FLAG_HIDDEN);       // 不可见
    }
    lv_obj_add_flag(guider_ui.screen_home_hor_img_shade_up, LV_OBJ_FLAG_HIDDEN);       // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_img_shade_left, LV_OBJ_FLAG_HIDDEN);     // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_shade_down, LV_OBJ_FLAG_HIDDEN);   // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_overload, LV_OBJ_FLAG_HIDDEN);     // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_overload_401, LV_OBJ_FLAG_HIDDEN); // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_overload_403, LV_OBJ_FLAG_HIDDEN); // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_fire, LV_OBJ_FLAG_HIDDEN);         // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_fire_401, LV_OBJ_FLAG_HIDDEN);     // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_fire_403, LV_OBJ_FLAG_HIDDEN);     // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_201_welcome, LV_OBJ_FLAG_HIDDEN); // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_date_c3, LV_OBJ_FLAG_HIDDEN); // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_label_C201, LV_OBJ_FLAG_HIDDEN); // 不可见
    lv_obj_add_flag(guider_ui.screen_home_hor_img_wait, LV_OBJ_FLAG_HIDDEN); // 不可见

    if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_square, LV_OBJ_FLAG_HIDDEN);       // 不可见
    else
        lv_obj_add_flag(guider_ui.screen_home_hor_label_square, LV_OBJ_FLAG_HIDDEN);       // 不可见
    if (elevator_data.dir_arrow)
    {
        if(MY_SET_IMAGE.image == IMAGE_C201_hor)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C201.png));
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C201.png));
        }else if(MY_SET_IMAGE.image == IMAGE_C202_hor)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C202.png));
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C202.png));
        }else if(MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor || MY_SET_IMAGE.image == IMAGE_C403_hor)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C401.png));
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C401.png));
        }else if(MY_SET_IMAGE.image == IMAGE_C404_hor)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C404.png));
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C404.png));
        }
        else{
            lv_img_set_src(ui->screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down.png));
            lv_img_set_src(ui->screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down.png));
        }
    }
    else
    {
        if(MY_SET_IMAGE.image == IMAGE_C201_hor)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C201.png));
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C201.png));
        }else if(MY_SET_IMAGE.image == IMAGE_C202_hor)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C202.png));
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C202.png));
        }else if(MY_SET_IMAGE.image == IMAGE_C401_hor || MY_SET_IMAGE.image == IMAGE_C402_hor || MY_SET_IMAGE.image == IMAGE_C403_hor)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C401.png));
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C401.png));
        }else if(MY_SET_IMAGE.image == IMAGE_C404_hor)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C404.png));
            lv_img_set_src(guider_ui.screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C404.png));
        }else
        {
            lv_img_set_src(ui->screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up.png));
            lv_img_set_src(ui->screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up.png));
        }
    }
    // lv_img_set_src(ui->screen_home_hor_img_num1, MY_LVGL_IMAGE_PATH(picture/num/2.jpg));
    // lv_img_set_src(ui->screen_home_hor_img_num2, MY_LVGL_IMAGE_PATH(picture/num/9.jpg));

    // if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
    // {
    //     lv_obj_set_pos(ui->screen_home_hor_label_fire, 192, 684);
    //     lv_obj_set_pos(ui->screen_home_hor_label_overload, 8, 684);

    //     lv_obj_set_pos(ui->screen_home_hor_img_img, 306, 125);
    //     lv_obj_set_size(ui->screen_home_hor_img_img, 700, 520);
    //     lv_obj_set_pos(ui->screen_home_hor_label_update, 420, 0);
    //     lv_obj_set_pos(ui->screen_home_hor_img_smoke, 138, 671);
    //     lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 74, 137);
    //     lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 171, 166);
    //     lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 93, 137);
    //     lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 130, 166);

    //     lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 16, 27);
    //     lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 290, 31);
    //     lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 19, 25);
    //     lv_obj_set_size(guider_ui.screen_home_hor_img_logo_swd, 180, 35);
    //     lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 325, 692);
    //     lv_obj_set_pos(ui->screen_home_hor_label_time, 859, 687);
    //     lv_obj_set_size(ui->screen_home_hor_label_time, 158, 55);
    //     lv_obj_set_pos(ui->screen_home_hor_label_C201, 885, 58);
    //     lv_obj_set_size(ui->screen_home_hor_label_C201, 125, 30);
    //     lv_obj_set_pos(ui->screen_home_hor_label_date, 324, 60);
    //     lv_obj_set_size(ui->screen_home_hor_label_date, 155, 23);
    //     lv_obj_clear_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
    //     lv_obj_clear_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN); // 可见
    //     lv_obj_clear_flag(guider_ui.screen_home_hor_label_201_welcome, LV_OBJ_FLAG_HIDDEN); // 可见
    //     // lv_obj_clear_flag(guider_ui.screen_home_hor_label_C201, LV_OBJ_FLAG_HIDDEN); // 可见
    //     lv_obj_set_style_text_align(ui->screen_home_hor_label_date, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_align(ui->screen_home_hor_label_time, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_align(ui->screen_home_hor_label_C201, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     if (MY_SET.play_mode == PLAY_IMAGE)
    //         lv_obj_clear_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 可见
    //     else
    //         lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 不可见
    //     lv_obj_set_style_text_font(ui->screen_home_hor_label_time, &lv_font_YaHei_52, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_YaHei_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_font(ui->screen_home_hor_label_C201, &lv_font_YaHei_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     if (MY_SET_IMAGE.image == IMAGE_C201_hor)
    //     {
    //         lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_time, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_C201, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_201_welcome, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num1, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num2, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num3, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0x163072), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0x132b2b), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0x5A2E31 ), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     }else if (MY_SET_IMAGE.image == IMAGE_C202_hor)
    //     {
    //         lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date, lv_color_hex(0x706F6F), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_time, lv_color_hex(0x706F6F), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_201_welcome, lv_color_hex(0x706F6F), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0x163072), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0x132b2b), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //         lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     }
    // }
    // else if (MY_SET_IMAGE.image == IMAGE_C202_hor)
    // {
    //     lv_obj_set_pos(ui->screen_home_hor_label_fire, 192, 684);
    //     lv_obj_set_pos(ui->screen_home_hor_label_overload, 8, 684);

    //     lv_obj_set_pos(ui->screen_home_hor_img_img, 306, 125);
    //     lv_obj_set_size(ui->screen_home_hor_img_img, 700, 520);
    //     lv_obj_set_pos(ui->screen_home_hor_label_update, 420, 0);
    //     lv_obj_set_pos(ui->screen_home_hor_img_smoke, 139, 671);

    //     lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 74, 137);
    //     lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 171, 166);
    //     lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 93, 137);
    //     lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 130, 166);

    //     lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 16, 27);
    //     lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 290, 31);
    //     lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 19, 25);
    //     lv_obj_set_size(guider_ui.screen_home_hor_img_logo_swd, 180, 35);

    //     lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 325, 692);
    //     lv_obj_set_pos(ui->screen_home_hor_label_time, 859, 687);
    //     lv_obj_set_size(ui->screen_home_hor_label_time, 158, 55);
    //     lv_obj_set_pos(ui->screen_home_hor_label_C201, 885, 58);
    //     lv_obj_set_size(ui->screen_home_hor_label_C201, 125, 25);
    //     lv_obj_set_pos(ui->screen_home_hor_label_date, 324, 60);
    //     lv_obj_set_size(ui->screen_home_hor_label_date, 155, 23);
    //     lv_obj_clear_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
    //     lv_obj_clear_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN); // 可见
    //     lv_obj_clear_flag(guider_ui.screen_home_hor_label_201_welcome, LV_OBJ_FLAG_HIDDEN); // 可见
    //     lv_obj_clear_flag(guider_ui.screen_home_hor_label_C201, LV_OBJ_FLAG_HIDDEN); // 可见
    //     lv_obj_set_style_text_align(ui->screen_home_hor_label_date, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_align(ui->screen_home_hor_label_time, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_align(ui->screen_home_hor_label_C201, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     if (MY_SET.play_mode == PLAY_IMAGE)
    //         lv_obj_clear_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 可见
    //     else
    //         lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 不可见
    //     lv_obj_set_style_text_font(ui->screen_home_hor_label_time, &lv_font_YaHei_52, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_YaHei_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_font(ui->screen_home_hor_label_C201, &lv_font_YaHei_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date, lv_color_hex(0x6f6e6c), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_time, lv_color_hex(0x6f6e6c), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_201_welcome, lv_color_hex(0x6f6e6c), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0x163072), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0x132b2b), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //     lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    // }
    if (MY_SET_IMAGE.image == IMAGE_C201_hor)
    {
        lv_obj_set_pos(ui->screen_home_hor_label_fire, 192, 684);
        lv_obj_set_pos(ui->screen_home_hor_label_overload, 8, 684);

        lv_obj_set_pos(ui->screen_home_hor_img_img, 306, 125);
        lv_obj_set_size(ui->screen_home_hor_img_img, 700, 520);
        if(MY_SET.play_mode == PLAY_IMAGE)
            lv_obj_set_size(ui->screen_home_hor_label_square, 701, 521);
        else
            lv_obj_set_size(ui->screen_home_hor_label_square, 702, 522);
        lv_obj_set_pos(ui->screen_home_hor_label_update, 420, 0);
        lv_obj_set_pos(ui->screen_home_hor_img_smoke, 138, 671);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 74, 137);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 171, 166);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 93, 137);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 130, 166);

        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 16, 27);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 290, 31);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 19, 25);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_swd, 180, 35);
        lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 325, 692);
        lv_obj_set_pos(ui->screen_home_hor_label_time, 800, 671);
        lv_obj_set_size(ui->screen_home_hor_label_time, 223, 59);
        lv_obj_set_pos(ui->screen_home_hor_label_date, 306, 59);
        lv_obj_set_size(ui->screen_home_hor_label_date, 697, 33);
        lv_obj_add_flag(guider_ui.screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 不可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_201_welcome, LV_OBJ_FLAG_HIDDEN); // 可见
        if (MY_SET.play_mode == PLAY_IMAGE)
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 可见
        else
            lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 不可见
        lv_obj_set_style_text_font(ui->screen_home_hor_label_time, &lv_font_Dengb_60, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_Dengb_28, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date, lv_color_hex(0x5a2d32), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_time, lv_color_hex(0x5a2d32), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_201_welcome, lv_color_hex(0x5a2d32), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num1, lv_color_hex(0x5a2d32), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num2, lv_color_hex(0x5a2d32), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num3, lv_color_hex(0x5a2d32), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0x163072), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0x132b2b), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_hor_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C201.png));
        lv_img_set_src(ui->screen_home_hor_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C201.png));
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0x5a2d32), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0x5a2d32), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (MY_SET_IMAGE.image == IMAGE_C202_hor)
    {
        lv_obj_set_pos(ui->screen_home_hor_label_fire, 192, 684);
        lv_obj_set_pos(ui->screen_home_hor_label_overload, 8, 684);

        if(MY_SET.play_mode == PLAY_IMAGE)
            lv_obj_set_size(ui->screen_home_hor_label_square, 701, 521);
        else
            lv_obj_set_size(ui->screen_home_hor_label_square, 702, 522);

        lv_obj_set_pos(ui->screen_home_hor_img_img, 306, 125);
        lv_obj_set_size(ui->screen_home_hor_img_img, 700, 520);
        lv_obj_set_pos(ui->screen_home_hor_label_update, 420, 0);
        lv_obj_set_pos(ui->screen_home_hor_img_smoke, 139, 671);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 74, 137);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 171, 166);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 93, 137);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 130, 166);

        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 16, 27);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 290, 31);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 19, 25);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_swd, 180, 35);
        lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 325, 692);
        lv_obj_set_pos(ui->screen_home_hor_label_time, 800, 671);
        lv_obj_set_size(ui->screen_home_hor_label_time, 223, 59);
        lv_obj_set_pos(ui->screen_home_hor_label_date, 306, 59);
        lv_obj_set_size(ui->screen_home_hor_label_date, 697, 33);
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_201_welcome, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_add_flag(guider_ui.screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 不可见
        if (MY_SET.play_mode == PLAY_IMAGE)
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 可见
        else
            lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 不可见
        lv_obj_set_style_text_font(ui->screen_home_hor_label_time, &lv_font_Dengb_60, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_Dengb_28, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date, lv_color_hex(0x6f6e6c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_time, lv_color_hex(0x6f6e6c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_201_welcome, lv_color_hex(0x6f6e6c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0x163072), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0x132b2b), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_hor_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C201.png));
        lv_img_set_src(ui->screen_home_hor_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C201.png));
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (MY_SET_IMAGE.image == IMAGE_C203_hor)
    {
        lv_obj_set_pos(ui->screen_home_hor_label_fire, 279, 678);
        lv_obj_set_pos(ui->screen_home_hor_label_overload, 98, 678);

        lv_obj_set_pos(ui->screen_home_hor_label_update, 420, 0);
        lv_obj_set_pos(ui->screen_home_hor_img_smoke, 229, 665);
        lv_obj_set_pos(ui->screen_home_hor_img_arrow_xio, 117, 215);
        lv_obj_set_size(ui->screen_home_hor_img_arrow_xio, 269, 261);
        lv_obj_set_pos(ui->screen_home_hor_img_arrow_swd, 146, 215);
        lv_obj_set_size(ui->screen_home_hor_img_arrow_swd, 210, 261);

        lv_obj_set_pos(ui->screen_home_hor_img_logo_xio, 30, 30);
        lv_obj_set_pos(ui->screen_home_hor_img_logo_swd, 30, 30);
        lv_obj_set_pos(ui->screen_home_hor_label_welecom, 325, 692);
        lv_obj_set_pos(ui->screen_home_hor_label_time, 823, 673);
        lv_obj_set_pos(ui->screen_home_hor_label_date, 500, 673);
        lv_obj_set_size(ui->screen_home_hor_label_date, 351, 33);
        lv_obj_clear_flag(ui->screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN);  // 可见
        lv_obj_clear_flag(ui->screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN);  // 可见
        lv_obj_add_flag(ui->screen_home_hor_label_welecom, LV_OBJ_FLAG_HIDDEN); // 不可见
        lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 不可见
        lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_Dengb_28, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_hor_label_time, &lv_font_Dengb_28, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (elevator_data.dir_arrow)
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(ui->screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_big.png));
            lv_img_set_src(ui->screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C203.png));
        }
        else
        {
            lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(ui->screen_home_hor_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_big.png));
            lv_img_set_src(ui->screen_home_hor_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C203.png));
        }

        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date, lv_color_hex(0xf4eb35), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_time, lv_color_hex(0xf4eb35), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_welecom, lv_color_hex(0xf4eb35), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num1, lv_color_hex(0xf4eb35), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num2, lv_color_hex(0xf4eb35), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num3, lv_color_hex(0xf4eb35), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0xf4eb35), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0xf4eb35), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
    {
        lv_obj_set_pos(ui->screen_home_hor_label_update, 420, 0);
        if (MY_SET_IMAGE.image == IMAGE_C301_hor)
        {
            lv_obj_set_pos(ui->screen_home_hor_label_fire, 195, 635);
            lv_obj_set_pos(ui->screen_home_hor_label_overload, 7, 635);
            lv_obj_set_pos(ui->screen_home_hor_img_smoke, 145, 624);
        }
        else if (MY_SET_IMAGE.image == IMAGE_C302_hor)
        {
            lv_obj_set_pos(ui->screen_home_hor_label_fire, 192, 608);
            lv_obj_set_pos(ui->screen_home_hor_label_overload, 7, 608);
            lv_obj_set_pos(ui->screen_home_hor_img_smoke, 142, 598);
        }
        else if (MY_SET_IMAGE.image == IMAGE_C303_hor)
        {
            lv_obj_set_pos(ui->screen_home_hor_label_fire, 206, 618);
            lv_obj_set_pos(ui->screen_home_hor_label_overload, 13, 618);
            lv_obj_set_pos(ui->screen_home_hor_img_smoke, 142, 605);
        }
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 63, 122);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 186, 178);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 84, 122);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 143, 178);
        // lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 77, 379);
        // lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 183, 379);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_hor_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C301.png));
        lv_img_set_src(ui->screen_home_hor_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C301.png));
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 41, 39);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 394, 42);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 51, 43);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_swd, 220, 42);
        lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 697, 696);
        lv_obj_set_pos(ui->screen_home_hor_label_time, 591, 49);
        lv_obj_set_size(ui->screen_home_hor_label_time, 227, 77);
        lv_obj_set_pos(ui->screen_home_hor_label_date, 829, 97);
        lv_obj_set_size(ui->screen_home_hor_label_date, 176, 48);
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_date_c3, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN);      // 不可见
        lv_obj_set_style_text_font(ui->screen_home_hor_label_time, &lv_font_date_82, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_date_28, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date_c3, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_time, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_welecom, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num1, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num2, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num3, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_num1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_num2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_num3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0xa4a4a4), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (MY_SET_IMAGE.image == IMAGE_C401_hor)
    {
        lv_obj_set_pos(ui->screen_home_hor_label_update, 341, 0);
        lv_obj_set_pos(ui->screen_home_hor_img_smoke, 42, 718);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 42, 75);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 126, 121);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 50, 72);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 110, 123);

        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 710, 15);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 280, 33);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 834, 15);
        lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 101, 726);
        lv_obj_set_pos(ui->screen_home_hor_label_date, 691, 722);
        lv_obj_set_size(ui->screen_home_hor_label_date, 337, 33);
        lv_obj_clear_flag(guider_ui.screen_home_hor_img_shade_up, LV_OBJ_FLAG_HIDDEN);     // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN);       // 可见
        lv_obj_add_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN);         // 不可见
        lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN);            // 不可见
        lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_date, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_date_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui->screen_home_hor_label_date, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_hor_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C401.png));
        lv_img_set_src(ui->screen_home_hor_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C401.png));
    }
    else if (MY_SET_IMAGE.image == IMAGE_C402_hor)
    {
        lv_obj_set_pos(ui->screen_home_hor_label_fire, 176, 604);
        lv_obj_set_pos(ui->screen_home_hor_label_overload, 1, 604);

        lv_obj_set_pos(ui->screen_home_hor_label_update, 341, 0);
        lv_obj_set_pos(ui->screen_home_hor_img_smoke, 126, 590);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 58, 94);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 179, 172);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 73, 92);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 146, 176);
        // lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 68, 303);
        // lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 165, 303);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 725, 18);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 285, 31);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 834, 17);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_swd, 180, 35);
        lv_obj_set_style_text_align(ui->screen_home_hor_label_welecom, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 20, 522);
        lv_obj_set_size(guider_ui.screen_home_hor_label_welecom, 247, 31);
        lv_obj_set_pos(ui->screen_home_hor_label_date, 683, 726);
        lv_obj_set_size(ui->screen_home_hor_label_date, 319, 33);

        lv_obj_clear_flag(guider_ui.screen_home_hor_img_shade_left, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_add_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN);       // 不可见
        lv_obj_add_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN);       // 不可见
        lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN);          // 不可见
        lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
        if(MY_SET.language == LANGUAGE_CN)
            lv_obj_set_style_text_font(ui->screen_home_hor_label_welecom, &lv_font_medium_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        else
            lv_obj_set_style_text_font(ui->screen_home_hor_label_welecom, &lv_font_YaHei_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_hor_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C401.png));
        lv_img_set_src(ui->screen_home_hor_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C401.png));
    }
    else if (MY_SET_IMAGE.image == IMAGE_C403_hor)
    {
        lv_obj_set_pos(ui->screen_home_hor_label_update, 344, 0);
        lv_obj_set_pos(ui->screen_home_hor_img_smoke, 43, 699);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 274, 313);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 143, 151);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 289, 313);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 110, 151);
        // lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, 433, 324);
        // lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, 522, 324);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 19, 16);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 305, 33);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 23, 15);
        lv_obj_set_size(ui->screen_home_hor_img_logo_swd, 180, 35);
        lv_obj_set_pos(ui->screen_home_hor_label_date, 700, 27);
        lv_obj_set_size(ui->screen_home_hor_label_date, 336, 33);
        lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 97, 706);
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_add_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN);   // 不可见
        lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN);      // 不可见

        lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_date_34, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui->screen_home_hor_label_date, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_date, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_hor_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C401.png));
        lv_img_set_src(ui->screen_home_hor_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C401.png));
    }
    else if (MY_SET_IMAGE.image == IMAGE_C404_hor)
    {
        // lv_obj_clear_flag(guider_ui.screen_home_hor_label_overload, LV_OBJ_FLAG_HIDDEN); // 可见
        // lv_obj_clear_flag(guider_ui.screen_home_hor_label_fire, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_set_pos(ui->screen_home_hor_label_overload, 137, 614);
        lv_obj_set_pos(ui->screen_home_hor_label_fire, 14, 614);
        lv_obj_set_pos(ui->screen_home_hor_img_img, 257, 166);
        lv_obj_set_size(ui->screen_home_hor_img_img, 767, 434);
        lv_obj_set_pos(ui->screen_home_hor_label_update, 400, 0);
        lv_obj_set_pos(ui->screen_home_hor_img_smoke, 95, 614);

        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_xio, 49, 167);
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_xio, 166, 160);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_arrow_swd, 69, 167 );
        lv_obj_set_size(guider_ui.screen_home_hor_img_arrow_swd, 125, 160);

        lv_obj_set_size(guider_ui.screen_home_hor_img_num3, 80, 120);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_xio, 22, 27);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_xio, 285, 31);
        lv_obj_set_pos(guider_ui.screen_home_hor_img_logo_swd, 26, 25);
        lv_obj_set_size(guider_ui.screen_home_hor_img_logo_swd, 180, 35);
        lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 28, 716);
        lv_obj_set_size(guider_ui.screen_home_hor_label_welecom, 330, 33);
        lv_obj_set_pos(ui->screen_home_hor_label_time, 873, 18);
        lv_obj_set_pos(ui->screen_home_hor_label_date, 856, 60);
        lv_obj_set_size(ui->screen_home_hor_label_date, 154, 33);
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_time, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_hor_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
        if (MY_SET.play_mode == PLAY_IMAGE)
            lv_obj_clear_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 可见
        else
            lv_obj_add_flag(guider_ui.screen_home_hor_img_img, LV_OBJ_FLAG_HIDDEN); // 不可见
        lv_obj_set_style_text_font(ui->screen_home_hor_label_welecom, &lv_font_YaHei_32, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(ui->screen_home_hor_label_201_welcome, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_hor_label_time, &lv_font_date_41, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_hor_label_date, &lv_font_date_28, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_date, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_hor_label_welecom, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num1, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num2, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_hor_img_num3, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_xio, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(ui->screen_home_hor_img_logo_swd, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_xio, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(ui->screen_home_hor_img_arrow_swd, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_hor_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_hor_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C404.png));
        lv_img_set_src(ui->screen_home_hor_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C404.png));
    }
    lv_obj_set_pos(ui->screen_home_hor_label_update, 166, 335);
    lv_img_set_src(ui->screen_home_hor_img_img, MY_LVGL_IMAGE_PATH(picture/background/C403_hor2.png));
    lv_image_set_inner_align(ui->screen_home_hor_img_img, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_hor_img_arrow_swd, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_hor_img_arrow_xio, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_hor_img_logo_xio, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_hor_img_logo_swd, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(guider_ui.screen_home_hor_img_num1, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(guider_ui.screen_home_hor_img_num2, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(guider_ui.screen_home_hor_img_num3, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(guider_ui.screen_home_hor_img_init_logo, LV_IMAGE_ALIGN_STRETCH);

    if (MY_SET_IMAGE.image != IMAGE_C203_hor && MY_SET_IMAGE.image != IMAGE_C301_hor &&
        MY_SET_IMAGE.image != IMAGE_C302_hor && MY_SET_IMAGE.image != IMAGE_C303_hor)
    {
        // if (image_have_cnt)
        if (page_image_cnt[update_page_num])
        {
            const char *ext = ".jpg"; // 默认扩展名
            // 构建示例路径来获取扩展名
            char temp_path[30];
            sprintf(temp_path, "%s0.png", image_flash_path_prefix[update_page_num]);
            if (access(temp_path, 0) == 0)
                ext = ".png";
            else
            {
                sprintf(temp_path, "%s0.jpg", image_flash_path_prefix[update_page_num]);
                if (access(temp_path, 0) == 0)
                    ext = ".jpg";
                else
                {
                    sprintf(temp_path, "%s0.bmp", image_flash_path_prefix[update_page_num]);
                    if (access(temp_path, 0) == 0)
                        ext = ".bmp";
                }
            }
            char show_image_path[30];
            // 构建最新图片路径（假设最后一个导入的图片为当前显示图片）
            sprintf(show_image_path, "L:%s0%s", image_flash_path_prefix[update_page_num], ext);
            rt_kprintf("image_path:%s\n", show_image_path);
            // 刷新图片
            if (MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C201_hor ||
                MY_SET_IMAGE.image == IMAGE_C202_hor)
            {
                // lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_img));
                lv_img_set_src(guider_ui.screen_home_hor_img_img, show_image_path);
            }
            else
            {
                // lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_hor_img_background));
                lv_img_set_src(guider_ui.screen_home_hor_img_background, show_image_path);
            }
        }
    }


    lv_obj_set_pos(guider_ui.screen_home_hor_img_num1, image_num_pos[0].x, image_num_pos[0].y);
    lv_obj_set_pos(guider_ui.screen_home_hor_img_num2, image_num_pos[1].x, image_num_pos[1].y);
    lv_obj_set_pos(guider_ui.screen_home_hor_img_num3, image_num_pos[2].x, image_num_pos[2].y);

    if (MY_SET.language == LANGUAGE_CN)
        lv_label_set_text(ui->screen_home_hor_label_weather_erro, "请检查IP");
    else
        lv_label_set_text(ui->screen_home_hor_label_weather_erro, "check IP");

    if (MY_SET_IMAGE.logo == LOGO_XIO)
    {
        lv_obj_clear_flag(ui->screen_home_hor_img_logo_xio, LV_OBJ_FLAG_HIDDEN); // 显示
        lv_obj_add_flag(ui->screen_home_hor_img_logo_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(ui->screen_home_hor_label_welecom, "杭州西奥电梯欢迎您");
        else
            lv_label_set_text(ui->screen_home_hor_label_welecom, "Welcome to Xiolift!");
    }
    else if (MY_SET_IMAGE.logo == LOGO_SWD)
    {
        lv_obj_clear_flag(ui->screen_home_hor_img_logo_swd, LV_OBJ_FLAG_HIDDEN); // 显示
        lv_obj_add_flag(ui->screen_home_hor_img_logo_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(ui->screen_home_hor_label_welecom, "斯沃德欢迎您");
        else
            lv_label_set_text(ui->screen_home_hor_label_welecom, "Welcome to SWORD!");
    }
    else
    {
        lv_obj_add_flag(ui->screen_home_hor_img_logo_xio, LV_OBJ_FLAG_HIDDEN); // 隐藏
        lv_obj_add_flag(ui->screen_home_hor_img_logo_swd, LV_OBJ_FLAG_HIDDEN); // 隐藏
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(ui->screen_home_hor_label_welecom, "本电梯欢迎您");
        else
            lv_label_set_text(ui->screen_home_hor_label_welecom, "Welcome!");
    }
    //402 404
    if(have_txt_flag[txt_update_page_num])
    {
        FILE *flash_txt_file = fopen(txt_path[txt_update_page_num], "r");
        if (!flash_txt_file)
        {
            rt_kprintf("【update.txt】错误：无法打开文件\n");
        }

        long txt_file_len = safe_get_file_size(flash_txt_file);
        if (txt_file_len <= 0 || txt_file_len >= sizeof(update_txt_content))
        {
            rt_kprintf("【update.txt】错误：文件为空、过大或大小获取失败。\n");
        }
        int read_len = -1;
        read_len = fread(update_txt_content, 1, txt_file_len, flash_txt_file);
        if (read_len == txt_file_len)
        {
            update_txt_content[read_len] = '\0';

            lv_font_t *font;
            if (MY_SET_IMAGE.image == IMAGE_C402_hor)
            {
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                24,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }else if(MY_SET_IMAGE.image == IMAGE_C404_hor){
                lv_obj_set_size(guider_ui.screen_home_hor_label_welecom, 330, 38);
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                29,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }else{
                lv_obj_set_size(guider_ui.screen_home_hor_label_welecom, 319, 38);
                if(MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
                {
                    lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 697, 691);
                }
                else if(MY_SET_IMAGE.image == IMAGE_C401_hor)
                {
                    lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 101, 721);
                }else if(MY_SET_IMAGE.image == IMAGE_C403_hor)
                {
                    lv_obj_set_pos(guider_ui.screen_home_hor_label_welecom, 97, 701);
                }
                if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
                {
                    font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                    LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                    32,
                    LV_FREETYPE_FONT_STYLE_NORMAL);
                }else{
                    font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                    LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                    28,
                    LV_FREETYPE_FONT_STYLE_NORMAL);
                }
            }
            if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
            {
                lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_201_welcome, font, LV_PART_MAIN|LV_STATE_DEFAULT);

                if(txt_mode[txt_update_page_num])
                    lv_label_set_long_mode(guider_ui.screen_home_hor_label_201_welcome, LV_LABEL_LONG_SCROLL_CIRCULAR);
                else
                    lv_label_set_long_mode(guider_ui.screen_home_hor_label_201_welcome, LV_LABEL_LONG_WRAP);
            }else{
                lv_obj_set_style_text_font(guider_ui.screen_home_hor_label_welecom, font, LV_PART_MAIN|LV_STATE_DEFAULT);

                if(txt_mode[txt_update_page_num])
                    lv_label_set_long_mode(guider_ui.screen_home_hor_label_welecom, LV_LABEL_LONG_SCROLL_CIRCULAR);
                else
                    lv_label_set_long_mode(guider_ui.screen_home_hor_label_welecom, LV_LABEL_LONG_WRAP);
            }

            lv_refr_now(NULL);
            if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
                lv_label_set_text_fmt(guider_ui.screen_home_hor_label_201_welcome, "%s", update_txt_content);
            else
                lv_label_set_text_fmt(guider_ui.screen_home_hor_label_welecom, "%s", update_txt_content);
            rt_kprintf("update_txt_content:%s\n",  update_txt_content);
            rt_kprintf("len:%d\n",  strlen(update_txt_content));

            // int32_t  text_real_w  = lv_text_get_width(
            //     update_txt_content,               // 要计算的中英文文本
            //     0,                  // 传0 = 计算整个文本的宽度，无需手动传字符长度
            //     font,  // 必须传你的中英文混合字体！！
            //     0       // 字符间距，和下面设置的一致
            // );
            // if(!txt_mode[txt_update_page_num] && text_real_w > 290)
            // {
            //     printf("long text\n");
            //     if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
            //     {
            //         lv_label_set_long_mode(guider_ui.screen_home_hor_label_201_welcome, LV_LABEL_LONG_SCROLL_CIRCULAR);
            //     }else{
            //         lv_label_set_long_mode(guider_ui.screen_home_hor_label_welecom, LV_LABEL_LONG_SCROLL_CIRCULAR);
            //     }
            // }
            // printf("text_real_w : %d\n", text_real_w);

            lv_refr_now(NULL);
            rt_kprintf("设置滚动显示\n");
        }
        if (flash_txt_file)
        {
            fclose(flash_txt_file);
        }
    }
     // lv_label_set_text(ui->screen_home_hor_label_fire, "消防\nFIRE");
     // lv_label_set_text(ui->screen_home_hor_label_fire_401, "消防 FIRE");
     // lv_label_set_text(ui->screen_home_hor_label_fire_403, "消防  FIRE");
     // lv_label_set_text(ui->screen_home_hor_label_overload, "超载\nOVERLOAD");
     // lv_label_set_text(ui->screen_home_hor_label_overload_401, "超载  OVERLOAD");
     // lv_label_set_text(ui->screen_home_hor_label_overload_403, "超载  OVERLOAD");
    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_label_set_text(ui->screen_home_hor_label_fire, "消防");
        lv_label_set_text(ui->screen_home_hor_label_fire_401, "消防     ");
        lv_label_set_text(ui->screen_home_hor_label_fire_403, "消防      ");
        lv_label_set_text(ui->screen_home_hor_label_overload, "超载");
        lv_label_set_text(ui->screen_home_hor_label_overload_401, "超载                    ");
        lv_label_set_text(ui->screen_home_hor_label_overload_403, "超载                    ");
    }else{
        lv_label_set_text(ui->screen_home_hor_label_fire, "    \nFIRE");
        lv_label_set_text(ui->screen_home_hor_label_fire_401, "        FIRE");
        lv_label_set_text(ui->screen_home_hor_label_fire_403, "         FIRE");
        lv_label_set_text(ui->screen_home_hor_label_overload, "\nOVERLOAD");
        lv_label_set_text(ui->screen_home_hor_label_overload_401, "         OVERLOAD");
        lv_label_set_text(ui->screen_home_hor_label_overload_403, "      OVERLOAD");
    }
    // if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
    // {
    //     lv_label_set_text(ui->screen_home_hor_label_welecom, "WELCOME");
    // }
    if(show_gif_arrow_flag)
    {
        lv_obj_add_flag(ui->screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        lv_obj_add_flag(ui->screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        lv_obj_clear_flag(ui->screen_home_hor_img_gif, LV_OBJ_FLAG_HIDDEN);   // 隐藏
    }else{
        if(!show_arrow_flag)
        {
            lv_obj_add_flag(ui->screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
            lv_obj_add_flag(ui->screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        }else {
            if (MY_SET_IMAGE.arrow == ARROW_XIO)
            {
                lv_obj_clear_flag(ui->screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 显示
                lv_obj_add_flag(ui->screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
            }
            else if (MY_SET_IMAGE.arrow == ARROW_SWD)
            {
                lv_obj_clear_flag(ui->screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 显示
                lv_obj_add_flag(ui->screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
            }
            if(arrow_num == 3)
            {
                lv_obj_add_flag(ui->screen_home_hor_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
                lv_obj_add_flag(ui->screen_home_hor_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
            }
        }
    }

    if(tcp_return_home_flag)
    {
        lv_obj_clear_flag(guider_ui.screen_home_hor_img_wait, LV_OBJ_FLAG_HIDDEN); // 可见
    }else if(!tcp_return_home_flag)
    {
        lv_obj_add_flag(guider_ui.screen_home_hor_img_wait, LV_OBJ_FLAG_HIDDEN); // 不可见
    }

    // 定时器
    if (home_hor_set_time_timer == NULL)
        home_hor_set_time_timer = lv_timer_create(set_time_callback, 1000, 0);
    else
        lv_timer_resume(home_hor_set_time_timer);

    if (home_hor_move_timer == NULL)
        home_hor_move_timer = lv_timer_create(move_callback, 20, 0);
    else
        lv_timer_resume(home_hor_move_timer);

    if (home_hor_refresh_picture_timer == NULL)
        home_hor_refresh_picture_timer = lv_timer_create(refresh_picture_callback, 200, 0);
    else
        lv_timer_resume(home_hor_refresh_picture_timer);

    if (update_hor_timer == NULL)
        update_hor_timer = lv_timer_create(update_callback, 500, 0);
    else
        lv_timer_resume(update_hor_timer);

    if (home_hor_set_weather_timer == NULL)
        home_hor_set_weather_timer = lv_timer_create(weather_callback, 1000, 0);
    else
        lv_timer_resume(home_hor_set_weather_timer);

    // Update current screen layout.
    lv_obj_update_layout(ui->screen_home_hor);

    if(frist_hor_timer == NULL)
        frist_hor_timer = lv_timer_create(frist_callback, 50, 0);
    else
        lv_timer_resume(frist_hor_timer);


    if (MY_SET.play_mode == PLAY_IMAGE || energy_conservation ||
        MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
        MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C203_hor)
        {
            if(MY_SET_IMAGE.image != IMAGE_C301_hor && MY_SET_IMAGE.image != IMAGE_C302_hor && MY_SET_IMAGE.image != IMAGE_C303_hor)
            {
                if(MY_SET_IMAGE.image == IMAGE_C201_hor)
                   lv_obj_set_style_img_opa(ui->screen_home_hor_img_background, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                else
                    lv_obj_set_style_img_opa(ui->screen_home_hor_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
    else
        lv_obj_set_style_img_opa(ui->screen_home_hor_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    my_page = PAGE_HOME_HOR;

    up_down_cnt = 0;
    have_door_flag = false;
    show_frist_flag = true;
    last_dir_arrow = 100;
    memset(&my_cnt, 0, sizeof(my_cnt));
    up_down_cnt = 0;
    in_arr_flag = false;
    tcp_set_time_flag = true;
    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    home_hor_video_flag = false;
    have_two_video_flag = false;
    rt_mutex_release(video_mutex);
            // rt_kprintf("play_elevator.video_overload = %d\n", play_elevator.video_overload);
            // rt_kprintf("wait_overload_mp3_flag = %d\n", wait_overload_mp3_flag);
            // rt_kprintf("wait_overload_cnt_flag = %d\n", wait_overload_cnt_flag);
            // rt_kprintf("wait_overload_flag = %d\n", wait_overload_flag);
            // rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
            // rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);
            // rt_kprintf("delete_over_flag = %d\n", delete_over_flag);
}
