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



lv_timer_t *home_set_time_timer = NULL;
lv_timer_t *home_move_timer = NULL;
lv_timer_t *update_timer = NULL;
lv_timer_t *home_set_weather_timer = NULL;
lv_timer_t *home_refresh_picture_timer = NULL;
lv_timer_t *frist_timer = NULL;

volatile static bool frist_set_mouse_image_flag = true;//切换限制

static uint8_t last_dir_arrow = 100;
static volatile bool show_arrow_flag = false;
static bool show_frist_flag = false;

static volatile bool show_weather_error_flag = false;

static void show_update_floor(void);

char image_filename_c401_ver[][100] = {
    MY_LVGL_IMAGE_PATH(picture/background/C401_ver.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C401_ver1.png),
    MY_LVGL_IMAGE_PATH(picture/background/C401_ver2.png),
    MY_LVGL_IMAGE_PATH(picture/background/C401_ver3.png),
};

char image_filename_c402_ver[][100] = {
    MY_LVGL_IMAGE_PATH(picture/background/C402_ver.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C402_ver1.png),
    MY_LVGL_IMAGE_PATH(picture/background/C402_ver2.png),
    MY_LVGL_IMAGE_PATH(picture/background/C402_ver3.png),
};

char image_filename_c403_ver[][100] = {
    MY_LVGL_IMAGE_PATH(picture/background/C403_ver.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C403_ver1.png),
    MY_LVGL_IMAGE_PATH(picture/background/C403_ver2.png),
    MY_LVGL_IMAGE_PATH(picture/background/C403_ver3.png),
};
char image_flash_c403_ver[][100] = {
    "L:/data/init0.jpg",
    "L:/data/init1.png",
    "L:/data/init2.png",
    "L:/data/init3.png",
};
char image_filename[][100] = {
    MY_LVGL_IMAGE_PATH(picture/background/C401_ver.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C402_ver.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C403_ver.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C404_ver.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C201_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C202_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C203_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C301_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C302_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C303_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C401_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C402_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C403_hor.jpg),
    MY_LVGL_IMAGE_PATH(picture/background/C404_hor.jpg),
};
char image_num_filename[][100] = {
    MY_LVGL_IMAGE_PATH(picture/num/0.png),
    MY_LVGL_IMAGE_PATH(picture/num/1.png),
    MY_LVGL_IMAGE_PATH(picture/num/2.png),
    MY_LVGL_IMAGE_PATH(picture/num/3.png),
    MY_LVGL_IMAGE_PATH(picture/num/4.png),
    MY_LVGL_IMAGE_PATH(picture/num/5.png),
    MY_LVGL_IMAGE_PATH(picture/num/6.png),
    MY_LVGL_IMAGE_PATH(picture/num/7.png),
    MY_LVGL_IMAGE_PATH(picture/num/8.png),
    MY_LVGL_IMAGE_PATH(picture/num/9.png),
    MY_LVGL_IMAGE_PATH(picture/num/a.png),
    MY_LVGL_IMAGE_PATH(picture/num/b.png),
    MY_LVGL_IMAGE_PATH(picture/num/c.png),
    MY_LVGL_IMAGE_PATH(picture/num/d.png),
    MY_LVGL_IMAGE_PATH(picture/num/e.png),
    MY_LVGL_IMAGE_PATH(picture/num/f.png),
    MY_LVGL_IMAGE_PATH(picture/num/g.png),
    MY_LVGL_IMAGE_PATH(picture/num/h.png),
    MY_LVGL_IMAGE_PATH(picture/num/i.png),
    MY_LVGL_IMAGE_PATH(picture/num/j.png),
    MY_LVGL_IMAGE_PATH(picture/num/k.png),
    MY_LVGL_IMAGE_PATH(picture/num/l.png),
    MY_LVGL_IMAGE_PATH(picture/num/m.png),
    MY_LVGL_IMAGE_PATH(picture/num/n.png),
    MY_LVGL_IMAGE_PATH(picture/num/o.png),
    MY_LVGL_IMAGE_PATH(picture/num/p.png),
    MY_LVGL_IMAGE_PATH(picture/num/q.png),
    MY_LVGL_IMAGE_PATH(picture/num/r.png),
    MY_LVGL_IMAGE_PATH(picture/num/s.png),
    MY_LVGL_IMAGE_PATH(picture/num/t.png),
    MY_LVGL_IMAGE_PATH(picture/num/u.png),
    MY_LVGL_IMAGE_PATH(picture/num/v.png),
    MY_LVGL_IMAGE_PATH(picture/num/w.png),
    MY_LVGL_IMAGE_PATH(picture/num/x.png),
    MY_LVGL_IMAGE_PATH(picture/num/y.png),
    MY_LVGL_IMAGE_PATH(picture/num/z.png),
    MY_LVGL_IMAGE_PATH(picture/num/minu.png),
    // MY_LVGL_IMAGE_PATH(picture/num/space.png),

};

char image_num_c4_filename[][100] = {
    MY_LVGL_IMAGE_PATH(picture/num/0_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/1_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/2_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/3_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/4_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/5_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/6_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/7_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/8_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/9_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/a_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/b_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/c_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/d_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/e_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/f_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/g_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/h_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/i_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/j_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/k_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/l_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/m_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/n_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/o_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/p_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/q_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/r_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/s_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/t_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/u_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/v_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/w_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/x_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/y_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/z_c4.png),
    MY_LVGL_IMAGE_PATH(picture/num/minu_c4.png),
};

char image_num_big_filename[][100] = {
    MY_LVGL_IMAGE_PATH(picture/num/0_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/1_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/2_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/3_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/4_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/5_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/6_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/7_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/8_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/9_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/a_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/b_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/c_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/d_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/e_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/f_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/g_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/h_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/i_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/j_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/k_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/l_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/m_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/n_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/o_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/p_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/q_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/r_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/s_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/t_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/u_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/v_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/w_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/x_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/y_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/z_big.png),
    MY_LVGL_IMAGE_PATH(picture/num/minu_big.png),
    // MY_LVGL_IMAGE_PATH(picture/num/space.png),
};

char image_weather_filename[][100] = {
    MY_LVGL_IMAGE_PATH(picture/weather/00.png),
    MY_LVGL_IMAGE_PATH(picture/weather/01.png),
    MY_LVGL_IMAGE_PATH(picture/weather/02.png),
    MY_LVGL_IMAGE_PATH(picture/weather/03.png),
    MY_LVGL_IMAGE_PATH(picture/weather/04.png),
    MY_LVGL_IMAGE_PATH(picture/weather/04.png),
    MY_LVGL_IMAGE_PATH(picture/weather/06.png),
    MY_LVGL_IMAGE_PATH(picture/weather/07.png),
    MY_LVGL_IMAGE_PATH(picture/weather/08.png),
    MY_LVGL_IMAGE_PATH(picture/weather/09.png),
    MY_LVGL_IMAGE_PATH(picture/weather/10.png),
    MY_LVGL_IMAGE_PATH(picture/weather/11.png),
    MY_LVGL_IMAGE_PATH(picture/weather/12.png),
    MY_LVGL_IMAGE_PATH(picture/weather/13.png),
    MY_LVGL_IMAGE_PATH(picture/weather/14.png),
    MY_LVGL_IMAGE_PATH(picture/weather/15.png),
    MY_LVGL_IMAGE_PATH(picture/weather/16.png),
    MY_LVGL_IMAGE_PATH(picture/weather/17.png),
    MY_LVGL_IMAGE_PATH(picture/weather/18.png),
    MY_LVGL_IMAGE_PATH(picture/weather/19.png),
    MY_LVGL_IMAGE_PATH(picture/weather/20.png),
    MY_LVGL_IMAGE_PATH(picture/weather/21.png),
    MY_LVGL_IMAGE_PATH(picture/weather/22.png),
    MY_LVGL_IMAGE_PATH(picture/weather/23.png),
    MY_LVGL_IMAGE_PATH(picture/weather/24.png),
    MY_LVGL_IMAGE_PATH(picture/weather/25.png),
    MY_LVGL_IMAGE_PATH(picture/weather/26.png),
    MY_LVGL_IMAGE_PATH(picture/weather/27.png),
    MY_LVGL_IMAGE_PATH(picture/weather/28.png),
    MY_LVGL_IMAGE_PATH(picture/weather/29.png),
    MY_LVGL_IMAGE_PATH(picture/weather/30.png),
    MY_LVGL_IMAGE_PATH(picture/weather/31.png),
};

static void set_time_callback(lv_timer_t *timer)
{
    static const char *week_day[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    time_t now;
    struct tm *local_time;

    now = time(RT_NULL);
    local_time = localtime(&now);
    if (MY_SET_IMAGE.image != IMAGE_C404_ver)
    {
        if (MY_SET_IMAGE.image == IMAGE_C403_ver)
        {
            lv_label_set_text_fmt(guider_ui.screen_home_label_date, "%04d/%02d/%02d   %02d:%02d",
                              local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                              local_time->tm_hour, local_time->tm_min);
        }else if (MY_SET_IMAGE.image == IMAGE_C401_ver)
        {
            lv_label_set_text_fmt(guider_ui.screen_home_label_date, "%04d-%02d-%02d  %02d:%02d",
                local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                local_time->tm_hour, local_time->tm_min);
        }else
        {
            lv_label_set_text_fmt(guider_ui.screen_home_label_date, "%04d-%02d-%02d    %02d:%02d",
                local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                local_time->tm_hour, local_time->tm_min);
        }
    }
    else
    {
        lv_label_set_text_fmt(guider_ui.screen_home_label_date, "%02d/%02d %s",
                              local_time->tm_mon + 1, local_time->tm_mday, week_day[local_time->tm_wday]);
        lv_label_set_text_fmt(guider_ui.screen_home_label_time, "%02d:%02d",
                              local_time->tm_hour, local_time->tm_min);
    }
}
static void refresh_picture_callback(lv_timer_t *timer)
{
    static uint8_t time_cnt = 0;
    static uint8_t time_cnt2 = 0;
    static uint8_t image_cnt_temp = 1;
    char current_image_path[50];

    if(MY_SET.play_mode == PLAY_VIDEO && video_in_updating &&
        (MY_SET_IMAGE.image == IMAGE_C401_ver ||
        MY_SET_IMAGE.image == IMAGE_C402_ver ||MY_SET_IMAGE.image == IMAGE_C403_ver) )
    {
        // rt_kprintf("video_in_updating 255\n");
        lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    }else if(MY_SET.play_mode == PLAY_VIDEO && !video_in_updating &&
        (MY_SET_IMAGE.image == IMAGE_C401_ver ||
        MY_SET_IMAGE.image == IMAGE_C402_ver ||MY_SET_IMAGE.image == IMAGE_C403_ver) )
    {
        lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (MY_SET_IMAGE.image == IMAGE_C401_ver || MY_SET_IMAGE.image == IMAGE_C403_ver)
    {
        if (have_overload_flag)
        {
            if(MY_SET.play_mode == PLAY_VIDEO && voice_language != VOICE_EN)
            {
                if(video_select)
                {
                    lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                else
                    lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            lv_obj_clear_flag(guider_ui.screen_home_label_overload_401, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
        {
            if(MY_SET.play_mode == PLAY_VIDEO && !video_in_updating)
            {
                lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            lv_obj_add_flag(guider_ui.screen_home_label_overload_401, LV_OBJ_FLAG_HIDDEN); // 不可见
        }
        if (have_fire_flag)
        {
            if(MY_SET_IMAGE.image == IMAGE_C401_ver)
            {
                if(have_overload_flag)
                {
                    lv_obj_set_pos(guider_ui.screen_home_label_fire_401, 304, 978);
                }else{
                    lv_obj_set_pos(guider_ui.screen_home_label_fire_401, 51, 978);
                }
            }
            else if(MY_SET_IMAGE.image == IMAGE_C403_ver)
            {
                if(have_overload_flag)
                {
                    lv_obj_set_pos(guider_ui.screen_home_label_fire_401, 322, 974);
                }else{
                    lv_obj_set_pos(guider_ui.screen_home_label_fire_401, 64, 974);
                }
            }
            lv_obj_clear_flag(guider_ui.screen_home_label_fire_401, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
        {
            lv_obj_add_flag(guider_ui.screen_home_label_fire_401, LV_OBJ_FLAG_HIDDEN); // 汪可见
        }

        if(have_overload_flag || have_fire_flag)
            lv_obj_add_flag(guider_ui.screen_home_label_welecom, LV_OBJ_FLAG_HIDDEN); // 不可见
        else
        {
            if(!video_select)
                lv_obj_clear_flag(guider_ui.screen_home_label_welecom, LV_OBJ_FLAG_HIDDEN); // 可见
        }
    }else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
    {
        if (have_overload_flag)
        {
            if(MY_SET.play_mode == PLAY_VIDEO && voice_language != VOICE_EN)
            {
                if(video_select)
                    lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                else
                    lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            lv_obj_clear_flag(guider_ui.screen_home_label_overload_402, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
        {
            if(MY_SET.play_mode == PLAY_VIDEO && !video_in_updating)
                lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(guider_ui.screen_home_label_overload_402, LV_OBJ_FLAG_HIDDEN); // 不可见
        }

        if (have_fire_flag)
            lv_obj_clear_flag(guider_ui.screen_home_label_fire_402, LV_OBJ_FLAG_HIDDEN); // 可见
        else
            lv_obj_add_flag(guider_ui.screen_home_label_fire_402, LV_OBJ_FLAG_HIDDEN); // 汪可见
    }
    else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
    {
        if (have_overload_flag)
            lv_obj_clear_flag(guider_ui.screen_home_label_overload, LV_OBJ_FLAG_HIDDEN); // 可见
        else
            lv_obj_add_flag(guider_ui.screen_home_label_overload, LV_OBJ_FLAG_HIDDEN); // 不可见

        if (have_fire_flag)
            lv_obj_clear_flag(guider_ui.screen_home_label_fire, LV_OBJ_FLAG_HIDDEN); // 可见
        else
            lv_obj_add_flag(guider_ui.screen_home_label_fire, LV_OBJ_FLAG_HIDDEN); // 汪可见
    }

    if (mouse_leave_flag)
    {
        mouse_leave_flag = false;
        // lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_img_background));
        lv_img_set_src(guider_ui.screen_home_img_background, image_filename[MY_SET_IMAGE.image - 1]);
    }
    if (++time_cnt >= 25)
    {
        time_cnt = 0;
        if (MY_SET.play_mode == PLAY_IMAGE && page_image_cnt[update_page_num])
        {
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
                // else
                // {
                //     sprintf(temp_path, "%s%d.bmp", image_flash_path_prefix[update_page_num], image_cnt_temp);
                //     if (access(temp_path, 0) == 0)
                //         ext = ".bmp";
                // }
            }
            // 构建最新图片路径（假设最后一个导入的图片为当前显示图片）
            sprintf(current_image_path, "L:%s%d%s", image_flash_path_prefix[update_page_num], image_cnt_temp, ext);
            rt_kprintf("image_path:%s\n", current_image_path);
            if (++image_cnt_temp >= page_image_cnt[update_page_num])
                image_cnt_temp = 0;
            // 刷新图片
            if (MY_SET_IMAGE.image == IMAGE_C404_ver)
            {
                lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_img_img));
                lv_img_set_src(guider_ui.screen_home_img_img, current_image_path);
                lv_image_set_inner_align(guider_ui.screen_home_img_img, LV_IMAGE_ALIGN_STRETCH);
                // lv_img_set_src(guider_ui.screen_home_img_img, current_image_path);
            }
            else
            {
                // lv_img_set_src(guider_ui.screen_home_img_background, current_image_path);
                lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_img_background));
                lv_img_set_src(guider_ui.screen_home_img_background, current_image_path);
                lv_image_set_inner_align(guider_ui.screen_home_img_background, LV_IMAGE_ALIGN_STRETCH);
            }
        }

    }
    if(!page_image_cnt[update_page_num] && MY_SET.play_mode == PLAY_IMAGE)
    {
        if(MY_SET_IMAGE.image == IMAGE_C401_ver || MY_SET_IMAGE.image == IMAGE_C402_ver ||
           MY_SET_IMAGE.image == IMAGE_C403_ver || MY_SET_IMAGE.image == IMAGE_C404_ver)
        {
            if (++time_cnt2 >= 25)
            {
                time_cnt2 = 0;
                static uint8_t show_image_num = 1;
                if(show_image_num >= 4) show_image_num = 0;
                // lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_img_background));
                if(MY_SET_IMAGE.image == IMAGE_C401_ver)
                {
                    lv_img_set_src(guider_ui.screen_home_img_background, image_flash_c403_ver[show_image_num]);
                }else if(MY_SET_IMAGE.image == IMAGE_C402_ver)
                {
                    lv_img_set_src(guider_ui.screen_home_img_background, image_flash_c403_ver[show_image_num]);
                }else if(MY_SET_IMAGE.image == IMAGE_C403_ver)
                {
                    lv_img_set_src(guider_ui.screen_home_img_background, image_flash_c403_ver[show_image_num]);
                }else if(MY_SET_IMAGE.image == IMAGE_C404_ver)
                {
                    lv_img_set_src(guider_ui.screen_home_img_img, image_flash_c403_ver[show_image_num]);
                }
                show_image_num ++;
            }
        }
    }
}

static void move_callback(lv_timer_t *timer)
{
    static int index = 0;
    static bool logo = false;
    static bool set_image_flag = false;
    static bool energy_image_flag = false;
    static uint8_t set_image_time_count = 0;
    static uint8_t time_count = 0;
    static uint8_t last_arrow_ctrl = 0;

    static uint8_t last_ctrl_num = 100;

    // if (energy_show_image_flag && !energy_image_flag && MY_SET.play_mode == PLAY_VIDEO)
    // {
    //     if (MY_SET_IMAGE.image == IMAGE_C401_ver)
    //     {
    //         // lv_obj_clear_flag(guider_ui.screen_home_img_shade_up, LV_OBJ_FLAG_HIDDEN);     // 可见
    //         // lv_obj_clear_flag(guider_ui.screen_home_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 可见
    //         lv_obj_set_style_img_opa(guider_ui.screen_home_img_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    //         energy_image_flag = true;
    //     }
    //     else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
    //     {
    //         // lv_obj_clear_flag(guider_ui.screen_home_img_shade_left, LV_OBJ_FLAG_HIDDEN); // 可见
    //         lv_obj_set_style_img_opa(guider_ui.screen_home_img_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    //         energy_image_flag = true;
    //     }
    //     else if (MY_SET_IMAGE.image == IMAGE_C403_ver)
    //     {
    //         lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    //         energy_image_flag = true;
    //     }
    //     else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
    //     {
    //         lv_obj_set_style_img_opa(guider_ui.screen_home_img_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    //         energy_image_flag = true;
    //     }
    // }
    // else if (energy_image_flag && !energy_show_image_flag && MY_SET.play_mode == PLAY_VIDEO)
    // {
    //     if (MY_SET_IMAGE.image == IMAGE_C401_ver)
    //     {
    //         // lv_obj_add_flag(guider_ui.screen_home_img_shade_up, LV_OBJ_FLAG_HIDDEN);     // 可见
    //         // lv_obj_add_flag(guider_ui.screen_home_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 可见
    //         lv_obj_set_style_img_opa(guider_ui.screen_home_img_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    //         energy_image_flag = true;
    //     }
    //     else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
    //     {
    //         // lv_obj_add_flag(guider_ui.screen_home_img_shade_left, LV_OBJ_FLAG_HIDDEN); // 可见
    //         lv_obj_set_style_img_opa(guider_ui.screen_home_img_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    //         energy_image_flag = true;
    //     }
    //     else if (MY_SET_IMAGE.image == IMAGE_C403_ver)
    //     {
    //         lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    //         energy_image_flag = false;
    //     }
    //     else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
    //     {
    //         lv_obj_set_style_img_opa(guider_ui.screen_home_img_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    //         energy_image_flag = true;
    //     }
    // }

    if (video_select)
    {
        if (++set_image_time_count >= 75)
        {
            set_image_time_count = 0;
            set_image_flag = true;
            if (MY_SET.play_mode == PLAY_IMAGE)
            {
                if (MY_SET_IMAGE.image != IMAGE_C404_ver)
                    lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                if (MY_SET_IMAGE.image == IMAGE_C401_ver)
                {
                    lv_obj_add_flag(guider_ui.screen_home_img_shade_up, LV_OBJ_FLAG_HIDDEN); // 不可见
                    lv_obj_add_flag(guider_ui.screen_home_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 不可见
                }else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
                {
                    lv_obj_add_flag(guider_ui.screen_home_img_shade_left, LV_OBJ_FLAG_HIDDEN); // 不可见
                }
                // else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
                // {
                //     lv_obj_set_style_img_opa(guider_ui.screen_home_img_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                // }
            }
            if (MY_SET_IMAGE.image == IMAGE_C404_ver);
            else {

                lv_obj_set_style_img_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_add_flag(guider_ui.screen_home_label_welecom, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_set_style_img_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_add_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN); // 不可见
            }
        }
    }
    else if (set_image_flag && video_select == 0)
    {
        set_image_flag = false;
        lv_obj_set_style_img_opa(guider_ui.screen_home_img_arrow_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_opa(guider_ui.screen_home_img_arrow_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.play_mode == PLAY_IMAGE)
        {
            lv_obj_set_style_img_opa(guider_ui.screen_home_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            if (MY_SET_IMAGE.image == IMAGE_C401_ver)
            {
                lv_obj_clear_flag(guider_ui.screen_home_img_shade_up, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_clear_flag(guider_ui.screen_home_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 可见
            }else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
            {
                lv_obj_clear_flag(guider_ui.screen_home_img_shade_left, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            // else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
            // {
            //     lv_obj_set_style_img_opa(guider_ui.screen_home_img_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            // }
        }
        if (MY_SET_IMAGE.image == IMAGE_C404_ver);
        else
        {

            lv_obj_clear_flag(guider_ui.screen_home_label_welecom, LV_OBJ_FLAG_HIDDEN); // 可见
            if (have_floor_num == 1)
            {
                lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_add_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN);   // 不可见
                lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
            }
            else if (have_floor_num == 2)
            {
                lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_clear_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
            }
            else if (have_floor_num == 3)
            {
                lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_clear_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
                lv_obj_clear_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            else
            {
                lv_obj_add_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_add_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN); // 不可见
            }
        }
    }

    show_update_floor();

    if (last_dir_arrow != elevator_data.dir_arrow)
    {
        last_dir_arrow = elevator_data.dir_arrow;
        if (elevator_data.dir_arrow)
        {
            // rt_kprintf("设置向上\r\n");
            if(MY_SET_IMAGE.image == IMAGE_C401_ver)
            {
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C401.png));
                lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C401.png));
            }else if(MY_SET_IMAGE.image == IMAGE_C404_ver)
            {
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C404.png));
                lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C404.png));
            }else{
                lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down.png));
                lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down.png));
            }
        }
        else
        {
            // rt_kprintf("设置向上\r\n");
            if(MY_SET_IMAGE.image == IMAGE_C401_ver)
            {
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C401.png));
                lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C401.png));
            }else if(MY_SET_IMAGE.image == IMAGE_C404_ver)
            {
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C404.png));
                lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C404.png));
            }else
            {
                lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up.png));
                lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up.png));
            }
        }
    }
    switch (elevator_data.arrow_ctrl)
    {
    case 0:
        arrow_num = 0;
        show_arrow_flag = true;
        if (last_ctrl_num != 0)
        {
            if (MY_SET_IMAGE.arrow == ARROW_XIO)
            {

                lv_obj_add_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_clear_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            else
            {
                lv_obj_add_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_clear_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            lv_img_set_offset_y(guider_ui.screen_home_img_arrow_xio, 0);
            lv_img_set_offset_y(guider_ui.screen_home_img_arrow_swd, 0);
            last_ctrl_num = elevator_data.arrow_ctrl;
            last_arrow_ctrl = elevator_data.dir_arrow;
        }
        break;
    case 1:
        arrow_num = 1;
        show_arrow_flag = true;
        lv_img_set_offset_y(guider_ui.screen_home_img_arrow_xio, 0);
        lv_img_set_offset_y(guider_ui.screen_home_img_arrow_swd, 0);
        time_count++;

        if (time_count >= 25)
        {
            time_count = 0;
            logo = !logo;
            if (MY_SET_IMAGE.arrow == ARROW_XIO)
            {
                if (logo)
                    lv_obj_add_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 不可见
                else
                    lv_obj_clear_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            else if (MY_SET_IMAGE.logo == LOGO_SWD)
            {
                if (logo)
                    lv_obj_add_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 不可见
                else
                    lv_obj_clear_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 可见
            }
        }
        last_ctrl_num = elevator_data.arrow_ctrl;
        last_arrow_ctrl = elevator_data.dir_arrow;
        break;
    case 2:
        arrow_num = 2;
        show_arrow_flag = true;
        if (last_arrow_ctrl != elevator_data.dir_arrow)
        {
            if (MY_SET_IMAGE.arrow == ARROW_XIO)
            {
                lv_obj_add_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_clear_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            else
            {
                lv_obj_add_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 不可见
                lv_obj_clear_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 可见
            }
            index = 0;
        }
        last_arrow_ctrl = elevator_data.dir_arrow;
        if (elevator_data.dir_arrow)
        {
            index += 1;
            if (index >= 140)
                index = -140;
        }
        else
        {
            index -= 1;
            if (index <= -140)
                index = 0;
        }

        if (MY_SET_IMAGE.arrow == ARROW_XIO)
            lv_img_set_offset_y(guider_ui.screen_home_img_arrow_xio, index);
        else if (MY_SET_IMAGE.arrow == ARROW_SWD)
            lv_img_set_offset_y(guider_ui.screen_home_img_arrow_swd, index);
        last_ctrl_num = elevator_data.arrow_ctrl;
        break;
    case 3:
        arrow_num = 3;
        show_arrow_flag = false;
        if (last_ctrl_num != 3)
        {
            lv_obj_add_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 不可见
            lv_obj_add_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 不可见
            last_ctrl_num = elevator_data.arrow_ctrl;
            last_arrow_ctrl = elevator_data.dir_arrow;
        }
        break;
    default:
        break;
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
        lv_img_set_src(guider_ui.screen_home_img_init, MY_LVGL_IMAGE_PATH(picture/background/black_ver.png));

        if(time_cnt > (DELAY_LOGO + 1))
        {
            init_set_img_ok = true;
        }
        if(time_cnt > (DELAY_LOGO + 2))
        {
            time_cnt = 0;
            init_img_flag = true;
            lv_obj_add_flag(guider_ui.screen_home_img_init, LV_OBJ_FLAG_HIDDEN);
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
        udisk_update_state = 0;
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_label_update, "更新完成,\n拔出U盘后播放");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_label_update, "Update complete,\nremove the usb drive and play");
        if (page_image_cnt[update_page_num] == 0)
        {
            lv_img_set_src(guider_ui.screen_home_img_img, MY_LVGL_IMAGE_PATH(picture/background/world.jpg));
            lv_img_set_src(guider_ui.screen_home_img_background, image_filename[MY_SET_IMAGE.image - 1]);
        }
        save_begin();
    }
    else if (udisk_update_state == VIDEO_IS_UPDATED)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_label_update, "视频已是最新");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_label_update, "video is new");
    }else if (udisk_update_state == VIDEO_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        // if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_label_update, "video error");
        // else if (MY_SET.language == LANGUAGE_EN)
        //     lv_label_set_text(guider_ui.screen_home_hor_label_update, "Video over 500M");
    }else if (udisk_update_state == VIDEO_OVER_WIGHT)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_38, LV_PART_MAIN | LV_STATE_DEFAULT);
        // udisk_update_state = 0;
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_label_update, "视频分辨率超限,\n请确认视频分辨率小于1024*768或768*1024,\n请拔出U盘退出");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_label_update, "video size over 1024*768 or 768*1024,\nplease check.\nremove usb and exit");
    }else if (udisk_update_state == VIDEO_TYPE_NONE)
    {
        // udisk_update_state = 0;
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_45, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_label_update, "暂不支持,\n请更新编码为H264的MP4或AVI视频");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_label_update, "Update video to MP4/AVI with H.264");
    }
    else if (udisk_update_state == VIDEO_UPDATING)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);

        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text_fmt(guider_ui.screen_home_label_update, "视频更新中: %d%%(%d/%d)", read_percent, video_num + 1, media_count);
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text_fmt(guider_ui.screen_home_label_update, "video update: %d%%(%d/%d)", read_percent, video_num + 1, media_count);
    }else if (udisk_update_state == IMAGE_UPDATING)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text_fmt(guider_ui.screen_home_label_update, "图片 %d/%d: %d%%", current_media_idx + 1, usb_image_cnt, read_percent);
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text_fmt(guider_ui.screen_home_label_update, "picture %d/%d: %d%%", current_media_idx + 1, usb_image_cnt, read_percent);
    }else if (udisk_update_state == TEXT_OK)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text_fmt(guider_ui.screen_home_label_update, "文本更新完成");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text_fmt(guider_ui.screen_home_label_update, "text update complete");
    }
    else if (udisk_update_state == VIDEO_NO_FIDE)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_label_update, "未找到视频");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_label_update, "No video");
    }
    else if (udisk_update_state == IMAGE_NO_FIDE)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_label_update, "未找到图片");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_label_update, "No picture");
    }
    else if (udisk_update_state == VIDEO_UPDATE_NO_MP4)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        lv_label_set_text(guider_ui.screen_home_label_update, "NO MP4");
    }else if (udisk_update_state == VIDEO_UPDATE_ALL_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        lv_label_set_text(guider_ui.screen_home_label_update, "ALL OVER 2G");
    }else if (udisk_update_state == VIDEO_UPDATE_FLASH_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        lv_label_set_text(guider_ui.screen_home_label_update, "FLASH USED 90%%");
    }
    else if (udisk_update_state == VIDEO_UPDATE_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        udisk_update_state = 0;
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(guider_ui.screen_home_label_update, "视频超过500M");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text(guider_ui.screen_home_label_update, "Video over 500M");
    }else if (udisk_update_state == TEXT_ERROR)
    {
        lv_obj_set_style_text_font(guider_ui.screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (MY_SET.language == LANGUAGE_CN)
            lv_label_set_text_fmt(guider_ui.screen_home_label_update, "请检查文本格式");
        else if (MY_SET.language == LANGUAGE_EN)
            lv_label_set_text_fmt(guider_ui.screen_home_label_update, "please check text format");
    }
    // 如果更新完成，设置更新标志位，清空更新提示
    if (update_ok_flag)
    {
        udisk_update_state = UPDATE_NONE;
        update_ok_flag = false;
        lv_label_set_text_fmt(guider_ui.screen_home_label_update, "");
    }
    if(home_video_flag)
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

static void weather_callback(lv_timer_t *timer)
{
    uint16_t img_x = 24;
    uint16_t img_y = 454;

    if(change_weather_flag)
    {
        lv_obj_clear_flag(guider_ui.screen_home_label_temperature, LV_OBJ_FLAG_HIDDEN);       // 可见
        lv_obj_clear_flag(guider_ui.screen_home_img_weather, LV_OBJ_FLAG_HIDDEN);       // 可见
        change_weather_flag = false;
        switch(my_weather.weather)
        {
            case 0:
            case 53:
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+18, img_y+82);
            break;
            case 1:
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+27, img_y+56);
            break;
            case 2:
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+23, img_y+52);
            break;
            case 3:
            case 13:
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+46);
            break;
            case 4:
            case 5:
            case 12:
            case 17:
            case 25:
            case 28:
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+34);
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
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+39);
            break;
            case 18:
            case 24:
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+37);
            break;
            case 19:
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+21, img_y+37);
            break;
            case 20:
            case 29:
            case 30:
            case 31:
                lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+16, img_y+75);
            break;
            default: break;
        }
        // lv_obj_set_pos(guider_ui.screen_home_label_temperature, 43, 544);
        lv_label_set_text_fmt(guider_ui.screen_home_label_temperature, "%d°C", my_weather.temperature);
        if(my_weather.weather >= 0 && my_weather.weather <= 31)
            lv_img_set_src(guider_ui.screen_home_img_weather, image_weather_filename[my_weather.weather]);
        else if(my_weather.weather == 53)
            lv_img_set_src(guider_ui.screen_home_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/53.png));
        else if(my_weather.weather == 301)
            lv_img_set_src(guider_ui.screen_home_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/301.png));
        else if(my_weather.weather == 302)
            lv_img_set_src(guider_ui.screen_home_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/302.png));
    }

    if(weather_erro_flag && !show_weather_error_flag)
    {
        show_weather_error_flag = true;
        lv_obj_clear_flag(guider_ui.screen_home_label_weather_erro, LV_OBJ_FLAG_HIDDEN);       // 可见
    }else if(show_weather_error_flag && !weather_erro_flag){
        show_weather_error_flag = false;
        lv_obj_add_flag(guider_ui.screen_home_label_weather_erro, LV_OBJ_FLAG_HIDDEN);       // 不可见
    }

    static bool weather_init_flag = false;
    if(get_lwip_flag && !weather_init_flag)
    {
        weather_init_flag = true;
        if(MY_SET_IMAGE.image == IMAGE_C404_ver);
        else
        {
            lv_obj_add_flag(guider_ui.screen_home_label_temperature, LV_OBJ_FLAG_HIDDEN);       // 不可见
            lv_obj_add_flag(guider_ui.screen_home_img_weather, LV_OBJ_FLAG_HIDDEN);       // 不可见
        }
    }else if(!get_lwip_flag && weather_init_flag)
    {
        weather_init_flag = false;
        weather_erro_flag = false;
        show_weather_error_flag = false;
        lv_obj_add_flag(guider_ui.screen_home_label_weather_erro, LV_OBJ_FLAG_HIDDEN);       // 不可见
        lv_obj_add_flag(guider_ui.screen_home_label_temperature, LV_OBJ_FLAG_HIDDEN);       // 不可见
        lv_obj_add_flag(guider_ui.screen_home_img_weather, LV_OBJ_FLAG_HIDDEN);       // 不可见
    }
     if(txt_renew_flag)
    {
        txt_renew_flag = false;
        FILE *flash_txt_file = fopen(txt_path[txt_update_page_num], "rb");
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
            if (MY_SET_IMAGE.image == IMAGE_C402_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_label_welecom, 24, 651);
                lv_obj_set_size(guider_ui.screen_home_label_welecom, 250, 32);
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                24,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }else if(MY_SET_IMAGE.image == IMAGE_C404_ver){
                lv_obj_set_pos(guider_ui.screen_home_label_welecom, 245, 500);
                lv_obj_set_size(guider_ui.screen_home_label_welecom, 403, 40);
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                34,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }else{
                if(MY_SET_IMAGE.image == IMAGE_C401_ver)
                {
                    lv_obj_set_pos(guider_ui.screen_home_label_welecom, 86, 977);
                    lv_obj_set_size(guider_ui.screen_home_label_welecom, 319, 38);
                }else if(MY_SET_IMAGE.image == IMAGE_C403_ver)
                {
                    lv_obj_set_pos(guider_ui.screen_home_label_welecom, 84, 969);
                    lv_obj_set_size(guider_ui.screen_home_label_welecom, 295, 38);
                }
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                28,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }
            lv_obj_set_style_text_font(guider_ui.screen_home_label_welecom, font, LV_PART_MAIN|LV_STATE_DEFAULT);
            if(txt_mode[txt_update_page_num])
                lv_label_set_long_mode(guider_ui.screen_home_label_welecom, LV_LABEL_LONG_SCROLL_CIRCULAR);
            else
                lv_label_set_long_mode(guider_ui.screen_home_label_welecom, LV_LABEL_LONG_WRAP);
            lv_refr_now(NULL);
            lv_label_set_text_fmt(guider_ui.screen_home_label_welecom, "%s", update_txt_content);
            rt_kprintf("update_txt_content:%s\n",  update_txt_content);
            rt_kprintf("len:%d\n",  strlen(update_txt_content));
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
        }

        if(txt_mode[txt_update_page_num])
            lv_label_set_long_mode(guider_ui.screen_home_label_welecom, LV_LABEL_LONG_SCROLL_CIRCULAR);
        else
            lv_label_set_long_mode(guider_ui.screen_home_label_welecom, LV_LABEL_LONG_WRAP);

        if (MY_SET_IMAGE.image == IMAGE_C401_ver &&  have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_img_num1, 105, 126);
            lv_obj_set_size(guider_ui.screen_home_img_num2, 105, 126);
        }else if (MY_SET_IMAGE.image == IMAGE_C401_ver && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_img_num1, 66, 100);
            lv_obj_set_size(guider_ui.screen_home_img_num2, 66, 100);
            lv_obj_set_size(guider_ui.screen_home_img_num3, 66, 100);
        }else if (MY_SET_IMAGE.image == IMAGE_C402_ver &&  have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_img_num1, 130, 155);
            lv_obj_set_size(guider_ui.screen_home_img_num2, 130, 155);
        }else if (MY_SET_IMAGE.image == IMAGE_C402_ver && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_img_num1, 85, 120);
            lv_obj_set_size(guider_ui.screen_home_img_num2, 85, 120);
            lv_obj_set_size(guider_ui.screen_home_img_num3, 85, 120);
        }else if (MY_SET_IMAGE.image == IMAGE_C403_ver &&  have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_img_num1, 140, 170);
            lv_obj_set_size(guider_ui.screen_home_img_num2, 140, 170);
        }else if (MY_SET_IMAGE.image == IMAGE_C403_ver && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_img_num1, 90, 130);
            lv_obj_set_size(guider_ui.screen_home_img_num2, 90, 130);
            lv_obj_set_size(guider_ui.screen_home_img_num3, 90, 130);
        }else if (MY_SET_IMAGE.image == IMAGE_C404_ver &&  have_floor_num != 3)
        {
            lv_obj_set_size(guider_ui.screen_home_img_num1, 140, 182);
            lv_obj_set_size(guider_ui.screen_home_img_num2, 140, 182);
        }else if (MY_SET_IMAGE.image == IMAGE_C404_ver && have_floor_num == 3)
        {
            lv_obj_set_size(guider_ui.screen_home_img_num1, 90, 130);
            lv_obj_set_size(guider_ui.screen_home_img_num2, 90, 130);
            lv_obj_set_size(guider_ui.screen_home_img_num3, 90, 130);
        }

        if (have_floor_num == 1)
        {
            lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_add_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN);   // 不可见
            lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
        }
        else if (have_floor_num == 2)
        {
            lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
        }
        else if (have_floor_num == 3)
        {
            lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN); // 可见
        }
        else
        {
            lv_obj_add_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 不可见
            lv_obj_add_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 不可见
            lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN); // 不可见
        }
        lv_img_set_src(guider_ui.screen_home_img_num1, image_num_c4_filename[show_floor_num[0]]);
        lv_img_set_src(guider_ui.screen_home_img_num2, image_num_c4_filename[show_floor_num[1]]);
        lv_img_set_src(guider_ui.screen_home_img_num3, image_num_c4_filename[show_floor_num[2]]);
        lv_refr_now(NULL);
        lv_timer_handler();
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
            lv_obj_add_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN);   // 不可见
            lv_obj_add_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN);   // 不可见
            lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
        }
        else if (elevator_data.disp_len == 1 && show_temp != 1)
        {
            show_temp = 1;
            have_floor_num = 1;
            lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_add_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN);   // 不可见
            lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
            if (MY_SET_IMAGE.image == IMAGE_C401_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 222, 51);
                image_num_pos[0].x = 222;
                image_num_pos[0].y = 51;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 74, 419);
                image_num_pos[0].x = 74;
                image_num_pos[0].y = 419;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C403_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 528, 194);
                image_num_pos[0].x = 528;
                image_num_pos[0].y = 194;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 380, 205);
                image_num_pos[0].x = 380;
                image_num_pos[0].y = 205;
            }
        }
        else if (elevator_data.disp_len == 2 && show_temp != 2)
        {
            show_temp = 2;
            have_floor_num = 2;
            lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_add_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN);   // 不可见
            if (MY_SET_IMAGE.image == IMAGE_C401_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 171, 51);
                lv_obj_set_pos(guider_ui.screen_home_img_num2, 267, 51);
                image_num_pos[0].x = 171;
                image_num_pos[1].x = 267;
                image_num_pos[0].y = image_num_pos[1].y = 51;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 13, 419);
                lv_obj_set_pos(guider_ui.screen_home_img_num2, 136, 419);
                image_num_pos[0].x = 13;
                image_num_pos[1].x = 136;
                image_num_pos[0].y = image_num_pos[1].y = 419;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C403_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 465, 194);
                lv_obj_set_pos(guider_ui.screen_home_img_num2, 595, 194);
                image_num_pos[0].x = 465;
                image_num_pos[1].x = 595;
                image_num_pos[0].y = image_num_pos[1].y = 194;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 316, 205);
                lv_obj_set_pos(guider_ui.screen_home_img_num2, 444, 205);
                image_num_pos[0].x = 316;
                image_num_pos[1].x = 444;
                image_num_pos[0].y = image_num_pos[1].y = 205;
            }
        }
        else if (elevator_data.disp_len == 3 && show_temp != 3)
        {
            show_temp = 3;
            have_floor_num = 3;
            lv_obj_clear_flag(guider_ui.screen_home_img_num1, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_img_num2, LV_OBJ_FLAG_HIDDEN); // 可见
            lv_obj_clear_flag(guider_ui.screen_home_img_num3, LV_OBJ_FLAG_HIDDEN); // 可见
            if (MY_SET_IMAGE.image == IMAGE_C401_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 176, 65);
                lv_obj_set_pos(guider_ui.screen_home_img_num2, 239, 65);
                lv_obj_set_pos(guider_ui.screen_home_img_num3, 303, 65);
                image_num_pos[0].x = 176;
                image_num_pos[1].x = 239;
                image_num_pos[2].x = 303;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 65;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 17, 437);
                lv_obj_set_pos(guider_ui.screen_home_img_num2, 96, 437);
                lv_obj_set_pos(guider_ui.screen_home_img_num3, 177, 437);
                image_num_pos[0].x = 17;
                image_num_pos[1].x = 96;
                image_num_pos[2].x = 177;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 437;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C403_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 470, 220);
                lv_obj_set_pos(guider_ui.screen_home_img_num2, 554, 220);
                lv_obj_set_pos(guider_ui.screen_home_img_num3, 640, 220);
                image_num_pos[0].x = 470;
                image_num_pos[1].x = 554;
                image_num_pos[2].x = 640;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 220;
            }
            else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_img_num1, 323, 233);
                lv_obj_set_pos(guider_ui.screen_home_img_num2, 407, 233);
                lv_obj_set_pos(guider_ui.screen_home_img_num3, 491, 233);
                image_num_pos[0].x = 323;
                image_num_pos[1].x = 407;
                image_num_pos[2].x = 491;
                image_num_pos[0].y = image_num_pos[1].y = image_num_pos[2].y = 233;
            }
        }
        static uint8_t size_temp = 10;
        if(size_temp != have_floor_num)
        {
            size_temp = have_floor_num;
            if (MY_SET_IMAGE.image == IMAGE_C401_ver &&  have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_img_num1, 105, 126);
                lv_obj_set_size(guider_ui.screen_home_img_num2, 105, 126);
                lv_refr_now(NULL);
            }else if (MY_SET_IMAGE.image == IMAGE_C401_ver && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_img_num1, 66, 100);
                lv_obj_set_size(guider_ui.screen_home_img_num2, 66, 100);
                lv_obj_set_size(guider_ui.screen_home_img_num3, 66, 100);
                lv_refr_now(NULL);
            }else if (MY_SET_IMAGE.image == IMAGE_C402_ver &&  have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_img_num1, 130, 155);
                lv_obj_set_size(guider_ui.screen_home_img_num2, 130, 155);
                lv_refr_now(NULL);
            }else if (MY_SET_IMAGE.image == IMAGE_C402_ver && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_img_num1, 85, 120);
                lv_obj_set_size(guider_ui.screen_home_img_num2, 85, 120);
                lv_obj_set_size(guider_ui.screen_home_img_num3, 85, 120);
                lv_refr_now(NULL);
            }else if (MY_SET_IMAGE.image == IMAGE_C403_ver &&  have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_img_num1, 140, 170);
                lv_obj_set_size(guider_ui.screen_home_img_num2, 140, 170);
                lv_refr_now(NULL);
            }else if (MY_SET_IMAGE.image == IMAGE_C403_ver && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_img_num1, 90, 130);
                lv_obj_set_size(guider_ui.screen_home_img_num2, 90, 130);
                lv_obj_set_size(guider_ui.screen_home_img_num3, 90, 130);
                lv_refr_now(NULL);
            }else if (MY_SET_IMAGE.image == IMAGE_C404_ver &&  have_floor_num != 3)
            {
                lv_obj_set_size(guider_ui.screen_home_img_num1, 140, 182);
                lv_obj_set_size(guider_ui.screen_home_img_num2, 140, 182);
                lv_refr_now(NULL);
            }else if (MY_SET_IMAGE.image == IMAGE_C404_ver && have_floor_num == 3)
            {
                lv_obj_set_size(guider_ui.screen_home_img_num1, 90, 130);
                lv_obj_set_size(guider_ui.screen_home_img_num2, 90, 130);
                lv_obj_set_size(guider_ui.screen_home_img_num3, 90, 130);
                lv_refr_now(NULL);
            }
        }
        lv_img_set_src(guider_ui.screen_home_img_num1, image_num_c4_filename[show_floor_num[0]]);
        lv_img_set_src(guider_ui.screen_home_img_num2, image_num_c4_filename[show_floor_num[1]]);
        lv_img_set_src(guider_ui.screen_home_img_num3, image_num_c4_filename[show_floor_num[2]]);
    }
}

FAKE_IMAGE_DECLARE(bg_dark);
void setup_scr_screen_home(lv_ui *ui)
{
    FAKE_IMAGE_INIT(bg_dark, 768, 1024, 0, 0x00000000);
    // Write codes screen_home
    ui->screen_home = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_home, 768, 1024);
    lv_obj_set_scrollbar_mode(ui->screen_home, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_img_src(ui->screen_home, FAKE_IMAGE_NAME(bg_dark), LV_PART_MAIN | LV_STATE_DEFAULT);
    // Write style for screen_home, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_home, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_home, lv_color_hex(0xc3c3c3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_home, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_background
    ui->screen_home_img_background = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_background, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_background, MY_LVGL_IMAGE_PATH(picture/background/C401wu.png));
    lv_img_set_pivot(ui->screen_home_img_background, 50, 50);
    lv_img_set_angle(ui->screen_home_img_background, 0);
    lv_obj_set_pos(ui->screen_home_img_background, 0, 0);
    lv_obj_set_size(ui->screen_home_img_background, 768, 1024);

    // Write style for screen_home_img_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_background, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_shade_up
    ui->screen_home_img_shade_up = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_shade_up, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_shade_up, &_alpha_1024x245);
    lv_img_set_pivot(ui->screen_home_img_shade_up, 50, 50);
    lv_img_set_angle(ui->screen_home_img_shade_up, 0);
    lv_obj_set_pos(ui->screen_home_img_shade_up, 0, 0);
    lv_obj_set_size(ui->screen_home_img_shade_up, 768, 237);

    // Write style for screen_home_img_shade_up, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_shade_up, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_shade_up, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_shade_up, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_shade_up, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_shade_down
    ui->screen_home_label_shade_down = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_shade_down, "");
    lv_label_set_long_mode(ui->screen_home_label_shade_down, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_shade_down, 0, 964);
    lv_obj_set_size(ui->screen_home_label_shade_down, 768, 64);

    // Write style for screen_home_label_shade_down, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_shade_down, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_shade_down, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_shade_down, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_shade_down, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_shade_down, 179, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_home_label_shade_down, lv_color_hex(0x333638), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_home_label_shade_down, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_shade_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_shade_left
    ui->screen_home_img_shade_left = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_shade_left, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_shade_left, &_402_alpha_350x768);
    lv_img_set_pivot(ui->screen_home_img_shade_left, 50, 50);
    lv_img_set_angle(ui->screen_home_img_shade_left, 0);
    lv_obj_set_pos(ui->screen_home_img_shade_left, 0, 0);
    lv_obj_set_size(ui->screen_home_img_shade_left, 304, 1024);

    // Write style for screen_home_img_shade_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_shade_left, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_shade_left, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_shade_left, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_shade_left, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_overload
    ui->screen_home_label_overload = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_overload, "超载\nOVERLOAD");
    lv_label_set_long_mode(ui->screen_home_label_overload, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_overload, 594, 303);
    lv_obj_set_size(ui->screen_home_label_overload, 160, 70);

    // Write style for screen_home_label_overload, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_overload, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_overload, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_overload, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_overload, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_overload, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_fire
    ui->screen_home_label_fire = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_fire, "消防\nFIRE");
    lv_label_set_long_mode(ui->screen_home_label_fire, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_fire, 630, 185);
    lv_obj_set_size(ui->screen_home_label_fire, 82, 70);

    // Write style for screen_home_label_fire, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_fire, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_fire, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_fire, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_fire, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_overload_401
    ui->screen_home_label_overload_401 = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_overload_401, "超载  OVERLOAD");
    lv_label_set_long_mode(ui->screen_home_label_overload_401, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_overload_401, 58, 978);
    lv_obj_set_size(ui->screen_home_label_overload_401, 264, 35);

    // Write style for screen_home_label_overload_401, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_overload_401, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_overload_401, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_overload_401, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_overload_401, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_overload_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_fire_401
    ui->screen_home_label_fire_401 = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_fire_401, "消防  FIRE");
    lv_label_set_long_mode(ui->screen_home_label_fire_401, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_fire_401, 304, 978);
    lv_obj_set_size(ui->screen_home_label_fire_401, 171, 35);

    // Write style for screen_home_label_fire_401, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_fire_401, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_fire_401, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_fire_401, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_fire, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_fire_401, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_fire_401, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_overload_402
    ui->screen_home_label_overload_402 = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_overload_402, "超载           \nOVERLOAD");
    lv_label_set_long_mode(ui->screen_home_label_overload_402, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_overload_402, 0, 741);
    lv_obj_set_size(ui->screen_home_label_overload_402, 160, 70);

    // Write style for screen_home_label_overload_402, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_overload_402, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_overload_402, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_overload_402, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_overload_402, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_overload_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_fire_402
    ui->screen_home_label_fire_402 = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_fire_402, "消防\nFIRE");
    lv_label_set_long_mode(ui->screen_home_label_fire_402, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_fire_402, 156, 737);
    lv_obj_set_size(ui->screen_home_label_fire_402, 82, 70);

    // Write style for screen_home_label_fire_402, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_fire_402, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_fire_402, &lv_font_Dengb_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_fire_402, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_fire_402, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_fire_402, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Write codes screen_home_label_welecom
    ui->screen_home_label_welecom = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_welecom, "斯沃德杭州西奥本电梯欢迎您");
    lv_label_set_long_mode(ui->screen_home_label_welecom, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_welecom, 86, 982);
    lv_obj_set_size(ui->screen_home_label_welecom, 319, 33);

    // Write style for screen_home_label_welecom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_welecom, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_welecom, &lv_font_YaHei_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_welecom, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_welecom, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_welecom, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_welecom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);


    // Write codes screen_home_img_img
    ui->screen_home_img_img = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_pivot(ui->screen_home_img_img, 50, 50);
    lv_img_set_angle(ui->screen_home_img_img, 0);
    lv_obj_set_pos(ui->screen_home_img_img, 0, 556);
    lv_obj_set_size(ui->screen_home_img_img, 768, 437);

    // Write style for screen_home_img_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_img, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_arrow_swd
    ui->screen_home_img_arrow_swd = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_arrow_swd, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_arrow_swd, &_swd_up_alpha_126x120);
    lv_img_set_pivot(ui->screen_home_img_arrow_swd, 50, 50);
    lv_img_set_angle(ui->screen_home_img_arrow_swd, 0);
    lv_obj_set_pos(ui->screen_home_img_arrow_swd, 42, 53);
    lv_obj_set_size(ui->screen_home_img_arrow_swd, 121, 140);

    // Write style for screen_home_img_arrow_swd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_arrow_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_img_arrow_swd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_arrow_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_arrow_swd, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_arrow_xio
    ui->screen_home_img_arrow_xio = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_arrow_xio, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_arrow_xio, &_xio_up_alpha_104x120);
    lv_img_set_pivot(ui->screen_home_img_arrow_xio, 50, 50);
    lv_img_set_angle(ui->screen_home_img_arrow_xio, 0);
    lv_obj_set_pos(ui->screen_home_img_arrow_xio, 32, 53);
    lv_obj_set_size(ui->screen_home_img_arrow_xio, 146, 140);

    // Write style for screen_home_img_arrow_xio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_arrow_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_img_arrow_xio, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_arrow_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_arrow_xio, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_logo_swd
    ui->screen_home_img_logo_swd = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_logo_swd, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_logo_swd, &_logo_swd_alpha_180x36);
    lv_img_set_pivot(ui->screen_home_img_logo_swd, 50, 50);
    lv_img_set_angle(ui->screen_home_img_logo_swd, 0);
    lv_obj_set_pos(ui->screen_home_img_logo_swd, 586, 9);
    lv_obj_set_size(ui->screen_home_img_logo_swd, 180, 36);

    // Write style for screen_home_img_logo_swd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_img_logo_swd, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_logo_swd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_logo_swd, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_logo_xio
    ui->screen_home_img_logo_xio = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_logo_xio, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_logo_xio, &_logo_xio_alpha_301x33);
    lv_img_set_pivot(ui->screen_home_img_logo_xio, 50, 50);
    lv_img_set_angle(ui->screen_home_img_logo_xio, 0);
    lv_obj_set_pos(ui->screen_home_img_logo_xio, 465, 9);
    lv_obj_set_size(ui->screen_home_img_logo_xio, 301, 33);

    // Write style for screen_home_img_logo_xio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_img_logo_xio, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_logo_xio, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_logo_xio, true, LV_PART_MAIN | LV_STATE_DEFAULT);


    //Write codes screen_home_img_weather
    ui->screen_home_img_weather = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_weather, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_weather, &_duoyun_alpha_118x96);
    lv_img_set_pivot(ui->screen_home_img_weather, 50,50);
    lv_img_set_angle(ui->screen_home_img_weather, 0);
    lv_obj_set_pos(ui->screen_home_img_weather, 24, 454);
    lv_obj_set_size(ui->screen_home_img_weather, 118, 96);

    //Write style for screen_home_img_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_img_weather, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_weather, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_temperature
    ui->screen_home_label_temperature = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_temperature, "");
    lv_label_set_long_mode(ui->screen_home_label_temperature, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_temperature, 43, 540);
    lv_obj_set_size(ui->screen_home_label_temperature, 44, 16);

    //Write style for screen_home_label_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_temperature, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_temperature, &lv_font_Alatsi_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_temperature, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    // Write codes screen_home_label_date
    ui->screen_home_label_date = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_date, "");
    lv_label_set_long_mode(ui->screen_home_label_date, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_date, 445, 982);
    lv_obj_set_size(ui->screen_home_label_date, 319, 33);

    // Write style for screen_home_label_date, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_date, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_date, &lv_font_Dengb_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_date, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_date, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Write codes screen_home_img_num2
    ui->screen_home_img_num2 = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_num2, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_num2, &_num9_alpha_89x140);
    lv_img_set_pivot(ui->screen_home_img_num2, 50, 50);
    lv_img_set_angle(ui->screen_home_img_num2, 0);
    lv_obj_set_pos(ui->screen_home_img_num2, 270, 48);
    lv_obj_set_size(ui->screen_home_img_num2, 89, 140);

    // Write style for screen_home_img_num2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_num2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_img_num2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_num2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_num2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_num2, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_num1
    ui->screen_home_img_num1 = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_num1, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_num1, &_num2_alpha_89x140);
    lv_img_set_pivot(ui->screen_home_img_num1, 50, 50);
    lv_img_set_angle(ui->screen_home_img_num1, 0);
    lv_obj_set_pos(ui->screen_home_img_num1, 177, 48);
    lv_obj_set_size(ui->screen_home_img_num1, 89, 140);

    // Write style for screen_home_img_num1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_num1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_img_num1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_num1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_num1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_num1, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_num3
    ui->screen_home_img_num3 = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_num3, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_num3, &_num9_alpha_89x140);
    lv_img_set_pivot(ui->screen_home_img_num3, 50, 50);
    lv_img_set_angle(ui->screen_home_img_num3, 0);
    lv_obj_set_pos(ui->screen_home_img_num3, 270, 48);
    lv_obj_set_size(ui->screen_home_img_num3, 89, 140);

    // Write style for screen_home_img_num3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_num3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_home_img_num3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_num3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_num3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_num3, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_time
    ui->screen_home_label_time = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_time, "12:30");
    lv_label_set_long_mode(ui->screen_home_label_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_time, 644, 6);
    lv_obj_set_size(ui->screen_home_label_time, 123, 42);

    // Write style for screen_home_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_time, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_label_update
    ui->screen_home_label_update = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_update, "");
    lv_label_set_long_mode(ui->screen_home_label_update, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_update, 438, 182);
    lv_obj_set_size(ui->screen_home_label_update, 768, 150);

    // Write style for screen_home_label_update, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_update, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_update, &lv_font_Dengb_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_update, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_update, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_update, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_smoke
    ui->screen_home_img_smoke = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_smoke, LV_OBJ_FLAG_CLICKABLE);
    // lv_img_set_src(ui->screen_home_img_smoke, &_smoke_alpha_43x43);
    lv_img_set_pivot(ui->screen_home_img_smoke, 50, 50);
    lv_img_set_angle(ui->screen_home_img_smoke, 0);
    lv_obj_set_pos(ui->screen_home_img_smoke, 23, 973);
    lv_obj_set_size(ui->screen_home_img_smoke, 43, 43);

    // Write style for screen_home_img_smoke, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_smoke, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_smoke, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_smoke, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_smoke, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_home_img_init
    ui->screen_home_img_init = lv_img_create(ui->screen_home);
    lv_obj_add_flag(ui->screen_home_img_init, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_home_img_init, MY_LVGL_IMAGE_PATH(picture/background/init_ver.png));
    lv_img_set_pivot(ui->screen_home_img_init, 50, 50);
    lv_img_set_angle(ui->screen_home_img_init, 0);
    lv_obj_set_pos(ui->screen_home_img_init, 0, 0);
    lv_obj_set_size(ui->screen_home_img_init, 768, 1024);

    // Write style for screen_home_img_init, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_home_img_init, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_home_img_init, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_img_init, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_home_img_init, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    if(init_set_img_ok) lv_obj_add_flag(guider_ui.screen_home_img_init, LV_OBJ_FLAG_HIDDEN);

    //Write codes screen_home_label_weather_erro
    ui->screen_home_label_weather_erro = lv_label_create(ui->screen_home);
    lv_label_set_text(ui->screen_home_label_weather_erro, "请检查IP");
    lv_label_set_long_mode(ui->screen_home_label_weather_erro, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_home_label_weather_erro, 10, 508);
    lv_obj_set_size(ui->screen_home_label_weather_erro, 128, 28);

    //Write style for screen_home_label_weather_erro, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_weather_erro, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_weather_erro, &lv_font_Dengb_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_weather_erro, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_weather_erro, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_weather_erro, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_flag(ui->screen_home_label_weather_erro, LV_OBJ_FLAG_HIDDEN);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // The custom code of screen_home.
    lv_obj_clear_flag(ui->screen_home, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->screen_home_img_background, LV_OBJ_FLAG_SCROLLABLE);

    if (MY_SET.play_mode == PLAY_IMAGE || energy_conservation ||
        MY_SET_IMAGE.image == IMAGE_C404_ver)
        lv_obj_set_style_img_opa(ui->screen_home_img_background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_img_opa(ui->screen_home_img_background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    rt_kprintf("image:%s\n", image_filename[MY_SET_IMAGE.image - 1]);
    lv_img_set_src(guider_ui.screen_home_img_background, image_filename[MY_SET_IMAGE.image - 1]);
    lv_img_set_src(ui->screen_home_img_smoke, MY_LVGL_IMAGE_PATH(picture/logo/smoke.png));
    lv_img_set_src(guider_ui.screen_home_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio.png));
    lv_img_set_src(guider_ui.screen_home_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd.png));
    if (elevator_data.dir_arrow)
    {
        if(MY_SET_IMAGE.image == IMAGE_C401_ver)
        {
            lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C401.png));
            lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C401.png));
        }else if(MY_SET_IMAGE.image == IMAGE_C404_ver)
        {
            lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down_C404.png));
            lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down_C404.png));
        }else{
            lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_down.png));
            lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_down.png));
        }
    }else {
        if(MY_SET_IMAGE.image == IMAGE_C401_ver)
        {
            lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C401.png));
            lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C401.png));

        }
        else if(MY_SET_IMAGE.image == IMAGE_C404_ver)
        {
            lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(guider_ui.screen_home_img_arrow_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up_C404.png));
            lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up_C404.png));

        }else {
            lv_img_set_src(guider_ui.screen_home_img_arrow_swd, MY_LVGL_IMAGE_PATH(picture/arrow/swd_up.png));
            lv_img_set_src(guider_ui.screen_home_img_arrow_xio, MY_LVGL_IMAGE_PATH(picture/arrow/xio_up.png));
        }
    }

    lv_img_set_src(ui->screen_home_img_shade_up, MY_LVGL_IMAGE_PATH(picture/shade/up_ver.png));
    lv_img_set_src(ui->screen_home_img_shade_left, MY_LVGL_IMAGE_PATH(picture/shade/left_ver.png));

    uint16_t img_x = 24;
    uint16_t img_y = 454;

    switch(my_weather.weather)
    {
        case 0:
        case 53:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+18, img_y+82);
        break;
        case 1:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+27, img_y+56);
        break;
        case 2:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+23, img_y+52);
        break;
        case 3:
        case 13:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+46);
        break;
        case 4:
        case 12:
        case 17:
        case 25:
        case 28:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+34);
        break;
        case 5:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+34);
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
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+39);
        break;
        case 18:
        case 24:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+29, img_y+37);
        break;
        case 19:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+21, img_y+37);
        break;
        case 20:
        case 29:
        case 30:
        case 31:
            lv_obj_set_pos(guider_ui.screen_home_label_temperature, img_x+16, img_y+75);
        break;
        default: break;
    }
    if(get_lwip_flag && geted_weather_flag)
    {
        lv_label_set_text_fmt(guider_ui.screen_home_label_temperature, "%d°C", my_weather.temperature);
        if(my_weather.weather >= 0 && my_weather.weather <= 31)
        lv_img_set_src(guider_ui.screen_home_img_weather, image_weather_filename[my_weather.weather]);
        else if(my_weather.weather == 53)
        lv_img_set_src(guider_ui.screen_home_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/53.png));
        else if(my_weather.weather == 301)
        lv_img_set_src(guider_ui.screen_home_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/301.png));
        else if(my_weather.weather == 302)
        lv_img_set_src(guider_ui.screen_home_img_weather,  MY_LVGL_IMAGE_PATH(picture/weather/302.png));
    }

    if(!page_image_cnt[update_page_num])
        lv_img_set_src(ui->screen_home_img_img, MY_LVGL_IMAGE_PATH(picture/background/C403_hor2.png));





    lv_obj_add_flag(guider_ui.screen_home_img_shade_up, LV_OBJ_FLAG_HIDDEN);     // 不可见
    lv_obj_add_flag(guider_ui.screen_home_img_shade_left, LV_OBJ_FLAG_HIDDEN);   // 不可见
    lv_obj_add_flag(guider_ui.screen_home_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 不可见
    lv_obj_add_flag(guider_ui.screen_home_label_overload, LV_OBJ_FLAG_HIDDEN);   // 不可见
    lv_obj_add_flag(guider_ui.screen_home_label_overload_401, LV_OBJ_FLAG_HIDDEN);   // 不可见
    lv_obj_add_flag(guider_ui.screen_home_label_overload_402, LV_OBJ_FLAG_HIDDEN);   // 不可见
    lv_obj_add_flag(guider_ui.screen_home_label_fire, LV_OBJ_FLAG_HIDDEN);       // 不可见
    lv_obj_add_flag(guider_ui.screen_home_label_fire_401, LV_OBJ_FLAG_HIDDEN);       // 不可见
    lv_obj_add_flag(guider_ui.screen_home_label_fire_402, LV_OBJ_FLAG_HIDDEN);       // 不可见

    lv_obj_add_flag(guider_ui.screen_home_img_img, LV_OBJ_FLAG_HIDDEN); // 不可见
    if (MY_SET_IMAGE.image == IMAGE_C401_ver)
    {
        lv_obj_set_pos(ui->screen_home_label_fire_401, 304, 978);
        lv_obj_set_pos(ui->screen_home_label_overload_401, 58, 978);

        lv_obj_set_pos(ui->screen_home_label_update, 438, 182);
        lv_obj_set_pos(ui->screen_home_img_smoke, 23, 973);
        lv_obj_set_pos(guider_ui.screen_home_img_arrow_xio, 25, 54);
        lv_obj_set_size(guider_ui.screen_home_img_arrow_xio, 126, 121);
        lv_obj_set_pos(guider_ui.screen_home_img_arrow_swd, 44, 54);
        lv_obj_set_size(guider_ui.screen_home_img_arrow_swd, 85, 121);
        // lv_obj_set_pos(guider_ui.screen_home_img_num1, 183, 48);
        // lv_obj_set_pos(guider_ui.screen_home_img_num2, 276, 48);
        lv_obj_set_pos(guider_ui.screen_home_img_logo_xio, 483, 14);
        lv_obj_set_size(guider_ui.screen_home_img_logo_xio, 280, 30);
        lv_obj_set_pos(guider_ui.screen_home_img_logo_swd, 583, 14);
        lv_obj_set_size(guider_ui.screen_home_img_logo_swd, 180, 30);
        lv_obj_set_pos(ui->screen_home_label_date, 466, 978);
        lv_obj_set_size(ui->screen_home_label_date, 294, 29);
        lv_obj_set_pos(guider_ui.screen_home_label_welecom, 86, 982);
        lv_obj_set_style_text_letter_space(ui->screen_home_label_date, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui->screen_home_label_date, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_label_date, &lv_font_date_29, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C401.png));
        lv_img_set_src(ui->screen_home_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C401.png));

        lv_obj_clear_flag(guider_ui.screen_home_img_shade_up, LV_OBJ_FLAG_HIDDEN);     // 可见
        lv_obj_clear_flag(guider_ui.screen_home_label_shade_down, LV_OBJ_FLAG_HIDDEN); // 可见

        lv_obj_clear_flag(guider_ui.screen_home_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_add_flag(guider_ui.screen_home_label_time, LV_OBJ_FLAG_HIDDEN);   // 不可见

    }
    else if (MY_SET_IMAGE.image == IMAGE_C402_ver)
    {
        lv_obj_set_pos(ui->screen_home_label_overload, 594, 303);
        lv_obj_set_pos(ui->screen_home_label_fire, 630, 185);

        lv_obj_set_pos(ui->screen_home_label_update, 438, 182);
        lv_obj_set_pos(ui->screen_home_img_smoke, 117, 726);
        lv_obj_set_pos(guider_ui.screen_home_img_arrow_xio, 49, 167);
        lv_obj_set_size(guider_ui.screen_home_img_arrow_xio, 179, 172);
        lv_obj_set_pos(guider_ui.screen_home_img_arrow_swd, 75, 167);
        lv_obj_set_size(guider_ui.screen_home_img_arrow_swd, 122, 172);

        lv_obj_set_pos(guider_ui.screen_home_img_logo_xio, 465, 19);
        lv_obj_set_size(guider_ui.screen_home_img_logo_xio, 285, 31);
        lv_obj_set_pos(guider_ui.screen_home_img_logo_swd, 571, 18);
        lv_obj_set_size(guider_ui.screen_home_img_logo_swd, 180, 31);
        lv_obj_set_pos(guider_ui.screen_home_label_welecom, 24, 655);
        lv_obj_set_size(guider_ui.screen_home_label_welecom, 250, 24);

        lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C401.png));
        lv_img_set_src(ui->screen_home_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C401.png));
        if(MY_SET.language == LANGUAGE_CN)
            lv_obj_set_style_text_font(ui->screen_home_label_welecom, &lv_font_medium_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        else
            lv_obj_set_style_text_font(ui->screen_home_label_welecom, &lv_font_YaHei_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui->screen_home_label_welecom, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_clear_flag(guider_ui.screen_home_img_shade_left, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_add_flag(guider_ui.screen_home_label_date, LV_OBJ_FLAG_HIDDEN);           // 不可见
        lv_obj_add_flag(guider_ui.screen_home_label_time, LV_OBJ_FLAG_HIDDEN);           // 不可见
    }
    else if (MY_SET_IMAGE.image == IMAGE_C403_ver)
    {
        lv_obj_set_pos(ui->screen_home_label_fire_401, 322, 974);
        lv_obj_set_pos(ui->screen_home_label_overload_401, 72, 974);

        lv_obj_set_pos(ui->screen_home_label_update, 438, 112);
        lv_obj_set_pos(ui->screen_home_img_smoke, 29, 968);
        lv_obj_set_pos(guider_ui.screen_home_img_arrow_xio, 313, 207);
        lv_obj_set_size(guider_ui.screen_home_img_arrow_xio, 143, 151);
        lv_obj_set_pos(guider_ui.screen_home_img_arrow_swd, 335, 207);
        lv_obj_set_size(guider_ui.screen_home_img_arrow_swd, 96, 151);

        lv_obj_set_pos(guider_ui.screen_home_img_logo_xio, 21, 22);
        lv_obj_set_size(guider_ui.screen_home_img_logo_xio, 305, 33);
        lv_obj_set_pos(guider_ui.screen_home_img_logo_swd, 23, 22);
        lv_obj_set_size(guider_ui.screen_home_img_logo_swd, 180, 33);
        lv_obj_set_pos(ui->screen_home_label_date, 423, 24);
        lv_obj_set_size(ui->screen_home_label_date, 330, 45);

        lv_obj_set_pos(guider_ui.screen_home_label_welecom, 84, 974);
        lv_obj_set_size(guider_ui.screen_home_label_welecom, 295, 33);
        lv_obj_set_style_text_font(ui->screen_home_label_date, &lv_font_date_39, LV_PART_MAIN | LV_STATE_DEFAULT);
        if(MY_SET.language == LANGUAGE_CN)
            lv_obj_set_style_text_font(ui->screen_home_label_welecom, &lv_font_medium_28, LV_PART_MAIN | LV_STATE_DEFAULT);
        else
            lv_obj_set_style_text_font(ui->screen_home_label_welecom, &lv_font_YaHei_28, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_text_align(ui->screen_home_label_date, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui->screen_home_label_welecom, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_clear_flag(guider_ui.screen_home_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_add_flag(guider_ui.screen_home_label_time, LV_OBJ_FLAG_HIDDEN);   // 不可见
        lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C401.png));
        lv_img_set_src(ui->screen_home_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C401.png));
    }
    else if (MY_SET_IMAGE.image == IMAGE_C404_ver)
    {
        lv_obj_set_pos(ui->screen_home_label_overload, 594, 303);
        lv_obj_set_pos(ui->screen_home_label_fire, 630, 185);

        lv_obj_set_pos(ui->screen_home_label_update, 438, 112);
        lv_obj_set_pos(ui->screen_home_img_smoke, 649, 256);
        lv_obj_set_pos(guider_ui.screen_home_img_arrow_xio, 148, 217);
        lv_obj_set_size(guider_ui.screen_home_img_arrow_xio, 165, 159);
        lv_obj_set_pos(guider_ui.screen_home_img_arrow_swd, 174, 217);
        lv_obj_set_size(guider_ui.screen_home_img_arrow_swd, 110, 159);

        lv_obj_set_pos(guider_ui.screen_home_img_logo_xio, 7, 13);
        lv_obj_set_size(guider_ui.screen_home_img_logo_xio, 325, 33);
        lv_obj_set_pos(guider_ui.screen_home_img_logo_swd, 9, 13);
        lv_obj_set_size(guider_ui.screen_home_img_logo_swd, 180, 33);
        lv_obj_set_pos(ui->screen_home_label_date, 516, 17);
        lv_obj_set_size(ui->screen_home_label_date, 138, 26);
        lv_obj_set_pos(ui->screen_home_label_time, 647, 2);
        lv_obj_set_size(ui->screen_home_label_time, 107, 49);
        lv_obj_set_pos(guider_ui.screen_home_label_welecom, 245, 508);
        lv_obj_set_size(guider_ui.screen_home_label_welecom, 403, 35);

        lv_obj_set_style_text_align(ui->screen_home_label_date, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_label_time, &lv_font_date_44, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_label_date, &lv_font_date_26, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->screen_home_label_welecom, &lv_font_YaHei_35, LV_PART_MAIN | LV_STATE_DEFAULT);

        if (MY_SET.play_mode == PLAY_IMAGE)
            lv_obj_clear_flag(guider_ui.screen_home_img_img, LV_OBJ_FLAG_HIDDEN); // 可见
        else
            lv_obj_add_flag(guider_ui.screen_home_img_img, LV_OBJ_FLAG_HIDDEN);  // 不可见
        lv_obj_clear_flag(guider_ui.screen_home_label_date, LV_OBJ_FLAG_HIDDEN); // 可见
        lv_obj_clear_flag(guider_ui.screen_home_label_time, LV_OBJ_FLAG_HIDDEN); // 可见

        lv_obj_set_style_text_color(guider_ui.screen_home_label_date, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.screen_home_label_welecom, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(guider_ui.screen_home_img_logo_swd, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_img_recolor(guider_ui.screen_home_img_logo_xio, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_img_arrow_swd, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_img_arrow_xio, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_img_num1, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_img_num2, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor(guider_ui.screen_home_img_num3, lv_color_hex(0x152C2D), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_swd, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(ui->screen_home_img_logo_xio, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(ui->screen_home_img_logo_swd, MY_LVGL_IMAGE_PATH(picture/logo/logo_swd_C404_ver.png));
        lv_img_set_src(ui->screen_home_img_logo_xio, MY_LVGL_IMAGE_PATH(picture/logo/logo_xio_C404_ver.png));
    }
    // if(MY_SET_IMAGE.image != IMAGE_C404_ver)
    //     lv_obj_set_pos(ui->screen_home_label_update, 240, 450);
    // else
    //     lv_obj_set_pos(ui->screen_home_label_update, 240, 460);
    lv_obj_set_pos(ui->screen_home_label_update, 0, 378);
    lv_image_set_inner_align(ui->screen_home_img_img, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_img_background, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_img_num1, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_img_num2, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_img_num3, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_img_arrow_swd, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_img_arrow_xio, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_img_logo_xio, LV_IMAGE_ALIGN_STRETCH);
    lv_image_set_inner_align(ui->screen_home_img_logo_swd, LV_IMAGE_ALIGN_STRETCH);

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
            // else
            // {
            //     sprintf(temp_path, "%s0.bmp", image_flash_path_prefix[update_page_num]);
            //     if (access(temp_path, 0) == 0)
            //         ext = ".bmp";
            // }
        }
        char show_image_path[30];
        // 构建最新图片路径（假设最后一个导入的图片为当前显示图片）
        sprintf(show_image_path, "L:%s0%s", image_flash_path_prefix[update_page_num], ext);
        rt_kprintf("image_path:%s\n", show_image_path);
        // 刷新图片
        if (MY_SET_IMAGE.image == IMAGE_C404_ver)
        {
            lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_img_img));
            lv_img_set_src(guider_ui.screen_home_img_img, show_image_path);
            lv_image_set_inner_align(guider_ui.screen_home_img_img, LV_IMAGE_ALIGN_STRETCH);
        }
        else
        {
            lv_image_cache_drop(lv_img_get_src(guider_ui.screen_home_img_background));
            lv_img_set_src(guider_ui.screen_home_img_background, show_image_path);
            lv_image_set_inner_align(guider_ui.screen_home_img_background, LV_IMAGE_ALIGN_STRETCH);
        }
    }

    if (MY_SET.language == LANGUAGE_CN)
        lv_label_set_text(ui->screen_home_label_weather_erro, "请检查IP");
    else
        lv_label_set_text(ui->screen_home_label_weather_erro, "check IP");

    lv_obj_set_pos(guider_ui.screen_home_img_num1, image_num_pos[0].x, image_num_pos[0].y);
    lv_obj_set_pos(guider_ui.screen_home_img_num2, image_num_pos[1].x, image_num_pos[1].y);
    lv_obj_set_pos(guider_ui.screen_home_img_num3, image_num_pos[2].x, image_num_pos[2].y);

        lv_label_set_text(ui->screen_home_label_fire_402, "消防\nFIRE");
        lv_label_set_text(ui->screen_home_label_fire_401, "消防  FIRE");
        lv_label_set_text(ui->screen_home_label_fire, "消防\nFIRE");
        lv_label_set_text(ui->screen_home_label_overload_402, "超载           \nOVERLOAD");
        lv_label_set_text(ui->screen_home_label_overload_401, "超载  OVERLOAD");
        lv_label_set_text(ui->screen_home_label_overload, "超载\nOVERLOAD");
    if(MY_SET.language == LANGUAGE_CN)
    {
        lv_label_set_text(ui->screen_home_label_fire_402, "消防");
        lv_label_set_text(ui->screen_home_label_fire_401, "消防      ");
        lv_label_set_text(ui->screen_home_label_fire, "消防");
        lv_label_set_text(ui->screen_home_label_overload_402, "超载");
        lv_label_set_text(ui->screen_home_label_overload_401, "超载                     ");
        lv_label_set_text(ui->screen_home_label_overload, "超载");
    }else{
        lv_label_set_text(ui->screen_home_label_fire_402, "    \nFIRE");
        lv_label_set_text(ui->screen_home_label_fire_401, "      FIRE");
        lv_label_set_text(ui->screen_home_label_fire, "    \nFIRE");
        lv_label_set_text(ui->screen_home_label_overload_402, "               \nOVERLOAD");
        lv_label_set_text(ui->screen_home_label_overload_401, "     OVERLOAD");
        lv_label_set_text(ui->screen_home_label_overload, "    \nOVERLOAD");
    }

    if (MY_SET_IMAGE.logo == LOGO_XIO)
    {
        lv_obj_clear_flag(guider_ui.screen_home_img_logo_xio, LV_OBJ_FLAG_HIDDEN); // 显示
        lv_obj_add_flag(guider_ui.screen_home_img_logo_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        if(MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(ui->screen_home_label_welecom, "杭州西奥电梯欢迎您");
        else
            lv_label_set_text(ui->screen_home_label_welecom, "Welcome to Xiolift!");
    }
    else if (MY_SET_IMAGE.logo == LOGO_SWD)
    {
        lv_obj_clear_flag(guider_ui.screen_home_img_logo_swd, LV_OBJ_FLAG_HIDDEN); // 显示
        lv_obj_add_flag(guider_ui.screen_home_img_logo_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        if(MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(ui->screen_home_label_welecom, "斯沃德欢迎您");
        else
            lv_label_set_text(ui->screen_home_label_welecom, "Welcome to SWORD!");
    }
    else
    {
        lv_obj_add_flag(guider_ui.screen_home_img_logo_xio, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_home_img_logo_swd, LV_OBJ_FLAG_HIDDEN);
        if(MY_SET.language == LANGUAGE_CN)
            lv_label_set_text(ui->screen_home_label_welecom, "本电梯欢迎您");
        else
            lv_label_set_text(ui->screen_home_label_welecom, "Welcome!");
    }

    if(have_txt_flag[txt_update_page_num])
    {
        FILE *flash_txt_file = fopen(txt_path[txt_update_page_num], "rb");
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
            if (MY_SET_IMAGE.image == IMAGE_C402_ver)
            {
                lv_obj_set_pos(guider_ui.screen_home_label_welecom, 24, 651);
                lv_obj_set_size(guider_ui.screen_home_label_welecom, 250, 32);
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                24,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }else if(MY_SET_IMAGE.image == IMAGE_C404_ver){
                lv_obj_set_pos(guider_ui.screen_home_label_welecom, 245, 500);
                lv_obj_set_size(guider_ui.screen_home_label_welecom, 403, 40);
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                34,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }else{
                if(MY_SET_IMAGE.image == IMAGE_C401_ver)
                {
                    lv_obj_set_pos(guider_ui.screen_home_label_welecom, 86, 977);
                    lv_obj_set_size(guider_ui.screen_home_label_welecom, 319, 38);
                }else if(MY_SET_IMAGE.image == IMAGE_C403_ver)
                {
                    lv_obj_set_pos(guider_ui.screen_home_label_welecom, 84, 969);
                    lv_obj_set_size(guider_ui.screen_home_label_welecom, 295, 38);
                }
                font = lv_freetype_font_create(LVGL_PATH_ORI(font/YaHei.ttf),
                LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                28,
                LV_FREETYPE_FONT_STYLE_NORMAL);
            }
            lv_obj_set_style_text_font(guider_ui.screen_home_label_welecom, font, LV_PART_MAIN|LV_STATE_DEFAULT);
            if(txt_mode[txt_update_page_num])
                lv_label_set_long_mode(guider_ui.screen_home_label_welecom, LV_LABEL_LONG_SCROLL_CIRCULAR);
            else
                lv_label_set_long_mode(guider_ui.screen_home_label_welecom, LV_LABEL_LONG_WRAP);
            lv_refr_now(NULL);
            lv_label_set_text_fmt(guider_ui.screen_home_label_welecom, "%s", update_txt_content);
            rt_kprintf("update_txt_content:%s\n",  update_txt_content);
            rt_kprintf("len:%d\n",  strlen(update_txt_content));
            lv_refr_now(NULL);
        }
        if (flash_txt_file)
        {
            fclose(flash_txt_file);
        }
    }

    if(!show_arrow_flag)
    {
        lv_obj_add_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        lv_obj_add_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
    }
    else
    {
        if (MY_SET_IMAGE.arrow == ARROW_XIO)
        {
            lv_obj_clear_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN); // 显示
            lv_obj_add_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        }
        else if (MY_SET_IMAGE.arrow == ARROW_SWD)
        {
            lv_obj_clear_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN); // 显示
            lv_obj_add_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        }
        if(arrow_num == 3)
        {
            lv_obj_add_flag(guider_ui.screen_home_img_arrow_swd, LV_OBJ_FLAG_HIDDEN);   // 隐藏
            lv_obj_add_flag(guider_ui.screen_home_img_arrow_xio, LV_OBJ_FLAG_HIDDEN);   // 隐藏
        }
    }

    // 定时器
    if (home_set_time_timer == NULL)
        home_set_time_timer = lv_timer_create(set_time_callback, 1000, 0);
    else
        lv_timer_resume(home_set_time_timer);
    if (home_move_timer == NULL)
        home_move_timer = lv_timer_create(move_callback, 20, 0);
    else
        lv_timer_resume(home_move_timer);
    if (home_refresh_picture_timer == NULL)
        home_refresh_picture_timer = lv_timer_create(refresh_picture_callback, 200, 0);
    else
        lv_timer_resume(home_refresh_picture_timer);
    if (update_timer == NULL)
        update_timer = lv_timer_create(update_callback, 500, 0);
    else
        lv_timer_resume(update_timer);
    if (home_set_weather_timer == NULL)
        home_set_weather_timer = lv_timer_create(weather_callback, 1000, 0);
    else
        lv_timer_resume(home_set_weather_timer);
    lv_obj_update_layout(guider_ui.screen_home);

    if(frist_timer == NULL)
        frist_timer = lv_timer_create(frist_callback, 50, 0);
    else
        lv_timer_resume(frist_timer);

    my_page = PAGE_HOME;
    up_down_cnt = 0;
    have_door_flag = false;
    show_frist_flag = true;
    last_dir_arrow = 100;
    up_down_cnt = 0;
    in_arr_flag = false;
    memset(&my_cnt, 0, sizeof(my_cnt));
    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    home_video_flag = false;
    have_two_video_flag = false;
    rt_mutex_release(video_mutex);
    // Update current screen layout.
}
