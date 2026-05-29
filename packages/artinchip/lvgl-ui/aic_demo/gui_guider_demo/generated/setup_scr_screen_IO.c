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

lv_timer_t *get_state_timer = NULL;

static bool set_io_flag[19] = {false, false, false, false, false, false, false,
                                         false, false, false, false, false, false, false, false, false, false, false, false};

static void get_state_callback(lv_timer_t *timer)
{
    static bool set_state_ok_flag[19] = {false, false, false, false, false, false, false,
                                         false, false, false, false, false, false, false, false, false, false, false, false};
    //
    if (io_state_flag[NUM_POWER_OFF] && !set_state_ok_flag[NUM_POWER_OFF])
    {
        set_state_ok_flag[NUM_POWER_OFF] = true;
        set_io_flag[NUM_POWER_OFF] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_15, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!io_state_flag[NUM_POWER_OFF] && set_state_ok_flag[NUM_POWER_OFF])
    {
        set_state_ok_flag[NUM_POWER_OFF] = false;
        set_io_flag[NUM_POWER_OFF] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_15, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (io_state_flag[NUM_SLEEP] && !set_state_ok_flag[NUM_SLEEP])
    {
        set_state_ok_flag[NUM_SLEEP] = true;
        set_io_flag[NUM_SLEEP] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_16, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!io_state_flag[NUM_SLEEP] && set_state_ok_flag[NUM_SLEEP])
    {
        set_state_ok_flag[NUM_SLEEP] = false;
        set_io_flag[NUM_SLEEP] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_16, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (io_state_flag[NUM_RESET_COMFORT] && !set_state_ok_flag[NUM_RESET_COMFORT])
    {
        set_state_ok_flag[NUM_RESET_COMFORT] = true;
        set_io_flag[NUM_RESET_COMFORT] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_17, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!io_state_flag[NUM_RESET_COMFORT] && set_state_ok_flag[NUM_RESET_COMFORT])
    {
        set_state_ok_flag[NUM_RESET_COMFORT] = false;
        set_io_flag[NUM_RESET_COMFORT] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_17, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (io_state_flag[NUM_RESET_RESCUE_LEVEL] && !set_state_ok_flag[NUM_RESET_RESCUE_LEVEL])
    {
        set_state_ok_flag[NUM_RESET_RESCUE_LEVEL] = true;
        set_io_flag[NUM_RESET_RESCUE_LEVEL] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_18, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!io_state_flag[NUM_RESET_RESCUE_LEVEL] && set_state_ok_flag[NUM_RESET_RESCUE_LEVEL])
    {
        set_state_ok_flag[NUM_RESET_RESCUE_LEVEL] = false;
        set_io_flag[NUM_RESET_RESCUE_LEVEL] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_18, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (io_state_flag[NUM_RESET_END] && !set_state_ok_flag[NUM_RESET_END])
    {
        set_state_ok_flag[NUM_RESET_END] = true;
        set_io_flag[NUM_RESET_END] = true;

        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_20, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!io_state_flag[NUM_RESET_END] && set_state_ok_flag[NUM_RESET_END])
    {
        set_state_ok_flag[NUM_RESET_END] = false;
        set_io_flag[NUM_RESET_END] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_20, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (io_state_flag[NUM_BUZZER_SIGNAL] && !set_state_ok_flag[NUM_BUZZER_SIGNAL])
    {
        set_state_ok_flag[NUM_BUZZER_SIGNAL] = true;
        set_io_flag[NUM_BUZZER_SIGNAL] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_21, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!io_state_flag[NUM_BUZZER_SIGNAL] && set_state_ok_flag[NUM_BUZZER_SIGNAL])
    {
        set_state_ok_flag[NUM_BUZZER_SIGNAL] = false;
        set_io_flag[NUM_BUZZER_SIGNAL] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_21, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (io_state_flag[NUM_TRAP_COMFORT] && !set_state_ok_flag[NUM_TRAP_COMFORT])
    {
        set_state_ok_flag[NUM_TRAP_COMFORT] = true;
        set_io_flag[NUM_TRAP_COMFORT] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_29, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!io_state_flag[NUM_TRAP_COMFORT] && set_state_ok_flag[NUM_TRAP_COMFORT])
    {
        set_state_ok_flag[NUM_TRAP_COMFORT] = false;
        set_io_flag[NUM_TRAP_COMFORT] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_29, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.dir_arrow && !set_state_ok_flag[NUM_UP_DIRECTION] && IO_dir_arrow_value == 0)
    {
        set_state_ok_flag[NUM_UP_DIRECTION] = true;
        set_io_flag[NUM_UP_DIRECTION] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_11, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.dir_arrow && set_state_ok_flag[NUM_UP_DIRECTION])
    {
        set_state_ok_flag[NUM_UP_DIRECTION] = false;
        set_io_flag[NUM_UP_DIRECTION] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_11, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.dir_arrow && !set_state_ok_flag[NUM_DOWN_DIRECTION] && IO_dir_arrow_value == 1)
    {
        set_state_ok_flag[NUM_DOWN_DIRECTION] = true;
        set_io_flag[NUM_DOWN_DIRECTION] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_12, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.dir_arrow && set_state_ok_flag[NUM_DOWN_DIRECTION])
    {
        set_state_ok_flag[NUM_DOWN_DIRECTION] = false;
        set_io_flag[NUM_DOWN_DIRECTION] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_12, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.car_overload && !set_state_ok_flag[NUM_OVERLOAD_SIGNAL])
    {
        set_state_ok_flag[NUM_OVERLOAD_SIGNAL] = true;
        set_io_flag[NUM_OVERLOAD_SIGNAL] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_14, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.car_overload && set_state_ok_flag[NUM_OVERLOAD_SIGNAL])
    {
        set_state_ok_flag[NUM_OVERLOAD_SIGNAL] = false;
        set_io_flag[NUM_OVERLOAD_SIGNAL] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_14, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.fire && !set_state_ok_flag[NUM_FIRE_SIGNAL])
    {
        set_state_ok_flag[NUM_FIRE_SIGNAL] = true;
        set_io_flag[NUM_FIRE_SIGNAL] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_13, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.fire && set_state_ok_flag[NUM_FIRE_SIGNAL])
    {
        set_state_ok_flag[NUM_FIRE_SIGNAL] = false;
        set_io_flag[NUM_FIRE_SIGNAL] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_13, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.full_load && !set_state_ok_flag[NUM_FULL_LOAD_SIGNAL])
    {
        set_state_ok_flag[NUM_FULL_LOAD_SIGNAL] = true;
        set_io_flag[NUM_FULL_LOAD_SIGNAL] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_19, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.full_load && set_state_ok_flag[NUM_FULL_LOAD_SIGNAL])
    {
        set_state_ok_flag[NUM_FULL_LOAD_SIGNAL] = false;
        set_io_flag[NUM_FULL_LOAD_SIGNAL] = false;

        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_19, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.open_door && !set_state_ok_flag[NUM_OPEN_DOOR])
    {
        set_state_ok_flag[NUM_OPEN_DOOR] = true;
        set_io_flag[NUM_OPEN_DOOR] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_22, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.open_door && set_state_ok_flag[NUM_OPEN_DOOR])
    {
        set_state_ok_flag[NUM_OPEN_DOOR] = false;
        set_io_flag[NUM_OPEN_DOOR] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_22, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.dir_arrow && !set_state_ok_flag[NUM_UP_RUNNING] && IO_dir_arrow_value == 0)
    {
        set_state_ok_flag[NUM_UP_RUNNING] = true;
        set_io_flag[NUM_UP_RUNNING] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_23, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.dir_arrow&& set_state_ok_flag[NUM_UP_RUNNING])
    {
        set_state_ok_flag[NUM_UP_RUNNING] = false;
        set_io_flag[NUM_UP_RUNNING] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_23, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.dir_arrow && !set_state_ok_flag[NUM_DOWN_RUNNING] && IO_dir_arrow_value == 1)
    {
        set_state_ok_flag[NUM_DOWN_RUNNING] = true;
        set_io_flag[NUM_DOWN_RUNNING] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_24, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.dir_arrow && set_state_ok_flag[NUM_DOWN_RUNNING])
    {
        set_state_ok_flag[NUM_DOWN_RUNNING] = false;
        set_io_flag[NUM_DOWN_RUNNING] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_24, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if (MY_IO_FLAG.front_up && !set_state_ok_flag[NUM_FRONT_UP_TO_FLOOR])
    {
        set_state_ok_flag[NUM_FRONT_UP_TO_FLOOR] = true;
        set_io_flag[NUM_FRONT_UP_TO_FLOOR] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_25, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.front_up && set_state_ok_flag[NUM_FRONT_UP_TO_FLOOR])
    {
        set_state_ok_flag[NUM_FRONT_UP_TO_FLOOR] = false;
        set_io_flag[NUM_FRONT_UP_TO_FLOOR] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_25, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.front_down && !set_state_ok_flag[NUM_FRONT_DOWN_TO_FLOOR])
    {
        set_state_ok_flag[NUM_FRONT_DOWN_TO_FLOOR] = true;
        set_io_flag[NUM_FRONT_DOWN_TO_FLOOR] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_26, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.front_down && set_state_ok_flag[NUM_FRONT_DOWN_TO_FLOOR])
    {
        set_state_ok_flag[NUM_FRONT_DOWN_TO_FLOOR] = false;
        set_io_flag[NUM_FRONT_DOWN_TO_FLOOR] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_26, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.back_up && !set_state_ok_flag[NUM_BACK_UP_TO_FLOOR])
    {
        set_state_ok_flag[NUM_BACK_UP_TO_FLOOR] = true;
        set_io_flag[NUM_BACK_UP_TO_FLOOR] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_27, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.back_up && set_state_ok_flag[NUM_BACK_UP_TO_FLOOR])
    {
        set_state_ok_flag[NUM_BACK_UP_TO_FLOOR] = false;
        set_io_flag[NUM_BACK_UP_TO_FLOOR] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_27, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    //
    if (MY_IO_FLAG.back_down && !set_state_ok_flag[NUM_BACK_DOWN_TO_FLOOR])
    {
        set_state_ok_flag[NUM_BACK_DOWN_TO_FLOOR] = true;
        set_io_flag[NUM_BACK_DOWN_TO_FLOOR] = true;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_28, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else if (!MY_IO_FLAG.back_down && set_state_ok_flag[NUM_BACK_DOWN_TO_FLOOR])
    {
        set_state_ok_flag[NUM_BACK_DOWN_TO_FLOOR] = false;
        set_io_flag[NUM_BACK_DOWN_TO_FLOOR] = false;
        lv_obj_set_style_bg_color(guider_ui.screen_IO_label_28, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void send_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    char buf[10] = {0};
    if (code == LV_EVENT_CLICKED)
    {
        if (target == guider_ui.screen_IO_btn_1)
        {
            // uart_send_frame_with_bytes(0, IO_value[0], IO_value[1]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_1, buf, sizeof(buf));
            IO_value[0] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_2, buf, sizeof(buf));
            IO_value[1] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_11, "上行方向(CUDL):%d.%d", IO_value[0], IO_value[1]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_3, buf, sizeof(buf));
            IO_value[2] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_4, buf, sizeof(buf));
            IO_value[3] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_12, "下行方向(CDDL):%d.%d", IO_value[2], IO_value[3]);
            need_send_data = 1;
        }
        else if (target == guider_ui.screen_IO_btn_2)
        {
            // uart_send_frame_with_bytes(1, IO_value[4], IO_value[5]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_5, buf, sizeof(buf));
            IO_value[4] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_6, buf, sizeof(buf));
            IO_value[5] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_13, "消防信号(FSL):%d.%d", IO_value[4], IO_value[5]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_7, buf, sizeof(buf));
            IO_value[6] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_8, buf, sizeof(buf));
            IO_value[7] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_14, "超载信号(OLS):%d.%d", IO_value[6], IO_value[7]);
            need_send_data = 2;
        }
        else if (target == guider_ui.screen_IO_btn_3)
        {
            // uart_send_frame_with_bytes(2, IO_value[8], IO_value[9]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_9, buf, sizeof(buf));
            IO_value[8] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_10, buf, sizeof(buf));
            IO_value[9] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_15, "断电(ARD):%d.%d", IO_value[8], IO_value[9]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_11, buf, sizeof(buf));
            IO_value[10] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_12, buf, sizeof(buf));
            IO_value[11] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_16, "节能(LR):%d.%d", IO_value[10], IO_value[11]);
            need_send_data = 3;
        }
        else if (target == guider_ui.screen_IO_btn_4)
        {
            // uart_send_frame_with_bytes(3, IO_value[12], IO_value[13]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_13, buf, sizeof(buf));
            IO_value[12] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_14, buf, sizeof(buf));
            IO_value[13] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_17, "复位安抚信号(WTL):%d.%d", IO_value[12], IO_value[13]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_15, buf, sizeof(buf));
            IO_value[14] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_16, buf, sizeof(buf));
            IO_value[15] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_18, "复位救援平层信号(WTL):%d.%d", IO_value[14], IO_value[15]);
            need_send_data = 4;
        }
        else if (target == guider_ui.screen_IO_btn_5)
        {
            // uart_send_frame_with_bytes(4, IO_value[16], IO_value[17]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_17, buf, sizeof(buf));
            IO_value[16] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_18, buf, sizeof(buf));
            IO_value[17] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_19, "满载(LNSL):%d.%d", IO_value[16], IO_value[17]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_19, buf, sizeof(buf));
            IO_value[18] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_20, buf, sizeof(buf));
            IO_value[19] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_20, "复位结束信号(MD_BTN):%d.%d", IO_value[18], IO_value[19]);
            need_send_data = 5;
        }
        else if (target == guider_ui.screen_IO_btn_6)
        {
            // uart_send_frame_with_bytes(5, IO_value[20], IO_value[21]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_21, buf, sizeof(buf));
            IO_value[20] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_22, buf, sizeof(buf));
            IO_value[21] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_21, "蜂鸣信号(BUZ):%d.%d", IO_value[20], IO_value[21]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_23, buf, sizeof(buf));
            IO_value[22] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_24, buf, sizeof(buf));
            IO_value[23] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_22, "开门信号(DO):%d.%d", IO_value[22], IO_value[23]);
            need_send_data = 6;
        }
        else if (target == guider_ui.screen_IO_btn_7)
        {
            // uart_send_frame_with_bytes(6, IO_value[24], IO_value[25]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_25, buf, sizeof(buf));
            IO_value[24] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_26, buf, sizeof(buf));
            IO_value[25] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_23, "运行中上行(M-CUDL):%d.%d", IO_value[24], IO_value[25]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_27, buf, sizeof(buf));
            IO_value[26] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_28, buf, sizeof(buf));
            IO_value[27] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_24, "运行中下行(M-CDDL):%d.%d", IO_value[26], IO_value[27]);
            need_send_data = 7;
        }
        else if (target == guider_ui.screen_IO_btn_8)
        {
            // uart_send_frame_with_bytes(7, IO_value[28], IO_value[29]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_29, buf, sizeof(buf));
            IO_value[28] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_30, buf, sizeof(buf));
            IO_value[29] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_25, "前门上行到层(CDLU):%d.%d", IO_value[28], IO_value[29]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_31, buf, sizeof(buf));
            IO_value[30] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_32, buf, sizeof(buf));
            IO_value[31] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_26, "前门下行到层(CDLD):%d.%d", IO_value[30], IO_value[31]);
            need_send_data = 8;
        }
        else if (target == guider_ui.screen_IO_btn_9)
        {
            // uart_send_frame_with_bytes(8, IO_value[32], IO_value[33]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_33, buf, sizeof(buf));
            IO_value[32] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_34, buf, sizeof(buf));
            IO_value[33] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_27, "后门上行到层(RCDLU):%d.%d", IO_value[32], IO_value[33]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_35, buf, sizeof(buf));
            IO_value[34] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_36, buf, sizeof(buf));
            IO_value[35] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_28, "后门下行到层(RCDLD):%d.%d", IO_value[34], IO_value[35]);
            need_send_data = 9;
        }
        else if (target == guider_ui.screen_IO_btn_10)
        {
            // uart_send_frame_with_bytes(9, IO_value[36], IO_value[37]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_37, buf, sizeof(buf));
            IO_value[36] = atoi(buf);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_38, buf, sizeof(buf));
            IO_value[37] = atoi(buf);
            lv_label_set_text_fmt(guider_ui.screen_IO_label_29, "困人安抚信号():%d.%d", IO_value[36], IO_value[37]);
            lv_dropdown_get_selected_str(guider_ui.screen_IO_ddlist_39, buf, sizeof(buf));
            need_send_data = 10;
        }
        save_begin();
    }
}

static void default_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        IO_value[0] = 5;
        IO_value[1] = 1;
        IO_value[2] = 5;
        IO_value[3] = 2;
        IO_value[4] = 5;
        IO_value[5] = 4;
        IO_value[6] = 5;
        IO_value[7] = 3;
        IO_value[8] = 59;
        IO_value[9] = 1;
        IO_value[10] = 6;
        IO_value[11] = 3;
        IO_value[12] = 62;
        IO_value[13] = 1;
        IO_value[14] = 62;
        IO_value[15] = 2;
        IO_value[16] = 60;
        IO_value[17] = 3;
        IO_value[18] = 62;
        IO_value[19] = 4;
        IO_value[20] = 4;
        IO_value[21] = 4;
        IO_value[22] = 20;
        IO_value[23] = 3;
        IO_value[24] = 55;
        IO_value[25] = 3;
        IO_value[26] = 55;
        IO_value[27] = 2;
        IO_value[28] = 4;
        IO_value[29] = 3;
        IO_value[30] = 4;
        IO_value[31] = 2;
        IO_value[32] = 20;
        IO_value[33] = 1;
        IO_value[34] = 20;
        IO_value[35] = 2;
        IO_value[36] = 63;
        IO_value[37] = 1;

        lv_label_set_text_fmt(guider_ui.screen_IO_label_11, "上行方向(CUDL):%d.%d", IO_value[0], IO_value[1]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_12, "下行方向(CDDL):%d.%d", IO_value[2], IO_value[3]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_13, "消防信号(FSL):%d.%d", IO_value[4], IO_value[5]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_14, "超载信号(OLS):%d.%d", IO_value[6], IO_value[7]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_15, "断电(ARD):%d.%d", IO_value[8], IO_value[9]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_16, "节能(LR):%d.%d", IO_value[10], IO_value[11]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_17, "复位安抚信号(WTL):%d.%d", IO_value[12], IO_value[13]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_18, "复位救援平层信号(WTL):%d.%d", IO_value[14], IO_value[15]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_19, "满载(LNSL):%d.%d", IO_value[16], IO_value[17]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_20, "复位结束信号(MD_BTN):%d.%d", IO_value[18], IO_value[19]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_21, "蜂鸣信号(BUZ):%d.%d", IO_value[20], IO_value[21]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_22, "开门信号(DO):%d.%d", IO_value[22], IO_value[23]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_23, "运行中上行(M-CUDL):%d.%d", IO_value[24], IO_value[25]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_24, "运行中下行(M-CDDL):%d.%d", IO_value[26], IO_value[27]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_25, "前门上行到层(CDLU):%d.%d", IO_value[28], IO_value[29]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_26, "前门下行到层(CDLD):%d.%d", IO_value[30], IO_value[31]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_27, "后门上行到层(RCDLU):%d.%d", IO_value[32], IO_value[33]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_28, "后门下行到层(RCDLD):%d.%d", IO_value[34], IO_value[35]);
        lv_label_set_text_fmt(guider_ui.screen_IO_label_29, "困人安抚信号():%d.%d", IO_value[36], IO_value[37]);

        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_1, 63 - IO_value[0]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_2, 4 - IO_value[1]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_3, 63 - IO_value[2]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_4, 4 - IO_value[3]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_5, 63 - IO_value[4]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_6, 4 - IO_value[5]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_7, 63 - IO_value[6]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_8, 4 - IO_value[7]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_9, 63 - IO_value[8]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_10, 4 - IO_value[9]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_11, 63 - IO_value[10]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_12, 4 - IO_value[11]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_13, 63 - IO_value[12]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_14, 4 - IO_value[13]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_15, 63 - IO_value[14]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_16, 4 - IO_value[15]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_17, 63 - IO_value[16]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_18, 4 - IO_value[17]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_19, 63 - IO_value[18]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_20, 4 - IO_value[19]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_21, 63 - IO_value[20]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_22, 4 - IO_value[21]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_23, 63 - IO_value[22]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_24, 4 - IO_value[23]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_25, 63 - IO_value[24]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_26, 4 - IO_value[25]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_27, 63 - IO_value[26]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_28, 4 - IO_value[27]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_29, 63 - IO_value[28]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_30, 4 - IO_value[29]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_31, 63 - IO_value[30]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_32, 4 - IO_value[31]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_33, 63 - IO_value[32]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_34, 4 - IO_value[33]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_35, 63 - IO_value[34]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_36, 4 - IO_value[35]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_37, 63 - IO_value[36]);
        lv_dropdown_set_selected(guider_ui.screen_IO_ddlist_38, 4 - IO_value[37]);
        default_set_io_flag = true;
        save_begin();
    }
}

void setup_scr_screen_IO(lv_ui *ui)
{
    // Write codes screen_IO
    ui->screen_IO = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_IO, 768, 1024);
    lv_obj_set_scrollbar_mode(ui->screen_IO, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_IO, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO, lv_color_hex(0xc3c3c3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_1
    ui->screen_IO_label_1 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_1, "");
    lv_label_set_long_mode(ui->screen_IO_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_1, 18, 29);
    lv_obj_set_size(ui->screen_IO_label_1, 617, 71);

    // Write style for screen_IO_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_1, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_2
    ui->screen_IO_label_2 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_2, "");
    lv_label_set_long_mode(ui->screen_IO_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_2, 18, 119);
    lv_obj_set_size(ui->screen_IO_label_2, 617, 70);

    // Write style for screen_IO_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_2, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_3
    ui->screen_IO_label_3 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_3, "");
    lv_label_set_long_mode(ui->screen_IO_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_3, 18, 212);
    lv_obj_set_size(ui->screen_IO_label_3, 508, 70);

    // Write style for screen_IO_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_3, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_4
    ui->screen_IO_label_4 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_4, "");
    lv_label_set_long_mode(ui->screen_IO_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_4, 18, 301);
    lv_obj_set_size(ui->screen_IO_label_4, 750, 70);

    // Write style for screen_IO_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_4, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_4, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_5
    ui->screen_IO_label_5 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_5, "");
    lv_label_set_long_mode(ui->screen_IO_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_5, 18, 390);
    lv_obj_set_size(ui->screen_IO_label_5, 662, 70);

    // Write style for screen_IO_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_5, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_6
    ui->screen_IO_label_6 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_6, "");
    lv_label_set_long_mode(ui->screen_IO_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_6, 18, 755);
    lv_obj_set_size(ui->screen_IO_label_6, 735, 70);

    // Write style for screen_IO_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_6, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_6, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_7
    ui->screen_IO_label_7 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_7, "");
    lv_label_set_long_mode(ui->screen_IO_label_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_7, 18, 481);
    lv_obj_set_size(ui->screen_IO_label_7, 617, 70);

    // Write style for screen_IO_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_7, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_7, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_7, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_8
    ui->screen_IO_label_8 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_8, "");
    lv_label_set_long_mode(ui->screen_IO_label_8, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_8, 18, 845);
    lv_obj_set_size(ui->screen_IO_label_8, 512, 70);

    // Write style for screen_IO_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_8, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_8, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_8, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_9
    ui->screen_IO_label_9 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_9, "");
    lv_label_set_long_mode(ui->screen_IO_label_9, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_9, 18, 662);
    lv_obj_set_size(ui->screen_IO_label_9, 728, 70);

    // Write style for screen_IO_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_9, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_9, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_9, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_10
    ui->screen_IO_label_10 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_10, "");
    lv_label_set_long_mode(ui->screen_IO_label_10, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_10, 17, 572);
    lv_obj_set_size(ui->screen_IO_label_10, 729, 70);

    // Write style for screen_IO_label_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_10, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_10, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_10, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_1
    ui->screen_IO_btn_1 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_1_label = lv_label_create(ui->screen_IO_btn_1);
    lv_label_set_text(ui->screen_IO_btn_1_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_1, 22, 43);
    lv_obj_set_size(ui->screen_IO_btn_1, 110, 40);

    // Write style for screen_IO_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_1, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_1, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_1, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_11
    ui->screen_IO_label_11 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_11, "上行方向(CUDL):");
    lv_label_set_long_mode(ui->screen_IO_label_11, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_11, 137, 31);
    lv_obj_set_size(ui->screen_IO_label_11, 246, 32);

    // Write style for screen_IO_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_11, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_11, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_11, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_11, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_11, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_12
    ui->screen_IO_label_12 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_12, "下行方向(CDDL):");
    lv_label_set_long_mode(ui->screen_IO_label_12, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_12, 387, 31);
    lv_obj_set_size(ui->screen_IO_label_12, 246, 32);

    // Write style for screen_IO_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_12, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_12, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_12, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_12, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_1
    ui->screen_IO_ddlist_1 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_1, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_1, 137, 66);
    lv_obj_set_size(ui->screen_IO_ddlist_1, 121, 32);

    // Write style for screen_IO_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_1, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_1, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_1, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_1_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_1_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_1_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_1_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_1_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_1_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_1_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_1), &style_screen_IO_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_1_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_1_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_1_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_1_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_1_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_1_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_1_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_1_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_1_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_1_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_1), &style_screen_IO_ddlist_1_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_1_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_1_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_1_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_1_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_1_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_1), &style_screen_IO_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_2
    ui->screen_IO_ddlist_2 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_2, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_2, 262, 66);
    lv_obj_set_size(ui->screen_IO_ddlist_2, 121, 32);

    // Write style for screen_IO_ddlist_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_2, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_2, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_2, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_2, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_2, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_2_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_2_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_2_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_2_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_2_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_2_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_2_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_2_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_2_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_2_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_2_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_2), &style_screen_IO_ddlist_2_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_2_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_2_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_2_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_2_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_2_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_2_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_2_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_2_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_2_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_2_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_2_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_2_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_2), &style_screen_IO_ddlist_2_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_2_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_2_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_2_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_2_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_2_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_2), &style_screen_IO_ddlist_2_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_3
    ui->screen_IO_ddlist_3 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_3, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_3, 387, 66);
    lv_obj_set_size(ui->screen_IO_ddlist_3, 121, 32);

    // Write style for screen_IO_ddlist_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_3, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_3, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_3, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_3, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_3, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_3_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_3_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_3_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_3_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_3_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_3_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_3_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_3_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_3_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_3_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_3_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_3), &style_screen_IO_ddlist_3_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_3_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_3_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_3_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_3_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_3_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_3_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_3_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_3_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_3_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_3_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_3_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_3_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_3), &style_screen_IO_ddlist_3_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_3_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_3_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_3_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_3_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_3_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_3), &style_screen_IO_ddlist_3_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_4
    ui->screen_IO_ddlist_4 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_4, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_4, 511, 66);
    lv_obj_set_size(ui->screen_IO_ddlist_4, 121, 32);

    // Write style for screen_IO_ddlist_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_4, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_4, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_4, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_4, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_4, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_4_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_4_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_4_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_4_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_4_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_4_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_4_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_4_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_4_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_4_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_4_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_4), &style_screen_IO_ddlist_4_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_4_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_4_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_4_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_4_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_4_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_4_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_4_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_4_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_4_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_4_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_4_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_4_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_4), &style_screen_IO_ddlist_4_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_4_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_4_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_4_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_4_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_4_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_4), &style_screen_IO_ddlist_4_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_13
    ui->screen_IO_label_13 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_13, "消防信号(FSL):");
    lv_label_set_long_mode(ui->screen_IO_label_13, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_13, 137, 121);
    lv_obj_set_size(ui->screen_IO_label_13, 246, 32);

    // Write style for screen_IO_label_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_13, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_13, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_13, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_13, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_13, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_14
    ui->screen_IO_label_14 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_14, "超载信号(OLS):");
    lv_label_set_long_mode(ui->screen_IO_label_14, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_14, 387, 121);
    lv_obj_set_size(ui->screen_IO_label_14, 246, 32);

    // Write style for screen_IO_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_14, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_14, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_14, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_14, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_14, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_5
    ui->screen_IO_ddlist_5 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_5, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_5, 137, 155);
    lv_obj_set_size(ui->screen_IO_ddlist_5, 121, 32);

    // Write style for screen_IO_ddlist_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_5, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_5, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_5, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_5, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_5, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_5, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_5_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_5_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_5_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_5_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_5_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_5_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_5_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_5_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_5_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_5_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_5_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_5), &style_screen_IO_ddlist_5_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_5_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_5_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_5_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_5_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_5_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_5_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_5_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_5_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_5_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_5_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_5_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_5_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_5), &style_screen_IO_ddlist_5_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_5_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_5_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_5_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_5_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_5_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_5), &style_screen_IO_ddlist_5_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_6
    ui->screen_IO_ddlist_6 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_6, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_6, 262, 155);
    lv_obj_set_size(ui->screen_IO_ddlist_6, 121, 32);

    // Write style for screen_IO_ddlist_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_6, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_6, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_6, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_6, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_6, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_6, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_6, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_6_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_6_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_6_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_6_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_6_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_6_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_6_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_6_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_6_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_6_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_6_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_6), &style_screen_IO_ddlist_6_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_6_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_6_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_6_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_6_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_6_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_6_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_6_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_6_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_6_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_6_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_6_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_6_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_6), &style_screen_IO_ddlist_6_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_6_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_6_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_6_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_6_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_6_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_6), &style_screen_IO_ddlist_6_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_7
    ui->screen_IO_ddlist_7 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_7, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_7, 387, 155);
    lv_obj_set_size(ui->screen_IO_ddlist_7, 121, 32);

    // Write style for screen_IO_ddlist_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_7, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_7, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_7, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_7, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_7, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_7, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_7, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_7, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_7_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_7_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_7_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_7_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_7_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_7_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_7_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_7_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_7_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_7_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_7_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_7), &style_screen_IO_ddlist_7_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_7_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_7_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_7_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_7_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_7_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_7_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_7_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_7_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_7_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_7_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_7_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_7_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_7), &style_screen_IO_ddlist_7_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_7_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_7_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_7_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_7_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_7_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_7), &style_screen_IO_ddlist_7_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_8
    ui->screen_IO_ddlist_8 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_8, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_8, 511, 155);
    lv_obj_set_size(ui->screen_IO_ddlist_8, 121, 32);

    // Write style for screen_IO_ddlist_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_8, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_8, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_8, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_8, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_8, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_8, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_8, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_8, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_8_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_8_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_8_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_8_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_8_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_8_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_8_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_8_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_8_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_8_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_8_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_8), &style_screen_IO_ddlist_8_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_8_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_8_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_8_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_8_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_8_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_8_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_8_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_8_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_8_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_8_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_8_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_8_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_8), &style_screen_IO_ddlist_8_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_8_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_8_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_8_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_8_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_8_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_8), &style_screen_IO_ddlist_8_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_15
    ui->screen_IO_label_15 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_15, "断电(ARD):");
    lv_label_set_long_mode(ui->screen_IO_label_15, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_15, 137, 214);
    lv_obj_set_size(ui->screen_IO_label_15, 190, 32);

    // Write style for screen_IO_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_15, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_15, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_15, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_15, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_15, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_9
    ui->screen_IO_ddlist_9 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_9, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_9, 137, 248);
    lv_obj_set_size(ui->screen_IO_ddlist_9, 93, 32);

    // Write style for screen_IO_ddlist_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_9, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_9, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_9, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_9, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_9, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_9, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_9, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_9, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_9_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_9_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_9_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_9_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_9_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_9_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_9_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_9_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_9_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_9_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_9_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_9), &style_screen_IO_ddlist_9_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_9_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_9_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_9_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_9_extra_list_main_default, 180);
    lv_style_set_text_color(&style_screen_IO_ddlist_9_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_9_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_9_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_9_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_9_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_9_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_9_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_9_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_9), &style_screen_IO_ddlist_9_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_9_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_9_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_9_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_9_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_9_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_9), &style_screen_IO_ddlist_9_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_10
    ui->screen_IO_ddlist_10 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_10, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_10, 234, 248);
    lv_obj_set_size(ui->screen_IO_ddlist_10, 93, 32);

    // Write style for screen_IO_ddlist_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_10, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_10, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_10, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_10, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_10, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_10, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_10, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_10, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_10_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_10_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_10_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_10_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_10_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_10_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_10_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_10_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_10_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_10_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_10_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_10), &style_screen_IO_ddlist_10_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_10_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_10_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_10_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_10_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_10_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_10_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_10_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_10_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_10_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_10_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_10_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_10_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_10), &style_screen_IO_ddlist_10_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_10_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_10_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_10_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_10_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_10_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_10), &style_screen_IO_ddlist_10_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_16
    ui->screen_IO_label_16 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_16, "节能(LR):");
    lv_label_set_long_mode(ui->screen_IO_label_16, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_16, 331, 214);
    lv_obj_set_size(ui->screen_IO_label_16, 193, 32);

    // Write style for screen_IO_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_16, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_16, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_16, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_16, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_16, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_11
    ui->screen_IO_ddlist_11 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_11, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_11, 331, 248);
    lv_obj_set_size(ui->screen_IO_ddlist_11, 94, 32);

    // Write style for screen_IO_ddlist_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_11, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_11, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_11, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_11, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_11, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_11, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_11, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_11, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_11, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_11_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_11_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_11_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_11_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_11_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_11_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_11_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_11_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_11_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_11_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_11_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_11), &style_screen_IO_ddlist_11_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_11_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_11_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_11_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_11_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_11_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_11_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_11_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_11_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_11_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_11_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_11_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_11_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_11), &style_screen_IO_ddlist_11_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_11_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_11_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_11_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_11_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_11_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_11), &style_screen_IO_ddlist_11_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_12
    ui->screen_IO_ddlist_12 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_12, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_12, 429, 248);
    lv_obj_set_size(ui->screen_IO_ddlist_12, 94, 32);

    // Write style for screen_IO_ddlist_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_12, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_12, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_12, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_12, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_12, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_12, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_12, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_12, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_12_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_12_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_12_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_12_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_12_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_12_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_12_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_12_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_12_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_12_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_12_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_12), &style_screen_IO_ddlist_12_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_12_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_12_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_12_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_12_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_12_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_12_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_12_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_12_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_12_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_12_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_12_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_12_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_12), &style_screen_IO_ddlist_12_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_12_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_12_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_12_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_12_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_12_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_12), &style_screen_IO_ddlist_12_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_17
    ui->screen_IO_label_17 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_17, "复位安抚信号(WTL):");
    lv_label_set_long_mode(ui->screen_IO_label_17, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_17, 137, 303);
    lv_obj_set_size(ui->screen_IO_label_17, 301, 32);

    // Write style for screen_IO_label_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_17, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_17, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_17, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_17, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_17, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_18
    ui->screen_IO_label_18 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_18, "复位救援平层信号(WTL):\n");
    lv_label_set_long_mode(ui->screen_IO_label_18, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_18, 442, 303);
    lv_obj_set_size(ui->screen_IO_label_18, 324, 32);

    // Write style for screen_IO_label_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_18, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_18, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_18, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_18, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_18, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_13
    ui->screen_IO_ddlist_13 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_13, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_13, 137, 337);
    lv_obj_set_size(ui->screen_IO_ddlist_13, 147, 32);

    // Write style for screen_IO_ddlist_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_13, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_13, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_13, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_13, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_13, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_13, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_13, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_13, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_13, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_13_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_13_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_13_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_13_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_13_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_13_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_13_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_13_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_13_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_13_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_13_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_13), &style_screen_IO_ddlist_13_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_13_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_13_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_13_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_13_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_13_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_13_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_13_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_13_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_13_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_13_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_13_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_13_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_13), &style_screen_IO_ddlist_13_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_13_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_13_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_13_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_13_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_13_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_13), &style_screen_IO_ddlist_13_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_14
    ui->screen_IO_ddlist_14 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_14, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_14, 288, 337);
    lv_obj_set_size(ui->screen_IO_ddlist_14, 150, 32);

    // Write style for screen_IO_ddlist_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_14, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_14, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_14, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_14, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_14, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_14, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_14, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_14, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_14, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_14_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_14_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_14_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_14_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_14_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_14_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_14_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_14_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_14_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_14_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_14_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_14), &style_screen_IO_ddlist_14_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_14_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_14_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_14_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_14_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_14_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_14_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_14_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_14_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_14_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_14_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_14_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_14_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_14), &style_screen_IO_ddlist_14_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_14_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_14_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_14_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_14_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_14_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_14), &style_screen_IO_ddlist_14_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_15
    ui->screen_IO_ddlist_15 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_15, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_15, 442, 337);
    lv_obj_set_size(ui->screen_IO_ddlist_15, 160, 32);

    // Write style for screen_IO_ddlist_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_15, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_15, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_15, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_15, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_15, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_15, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_15, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_15, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_15, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_15_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_15_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_15_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_15_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_15_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_15_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_15_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_15_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_15_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_15_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_15_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_15), &style_screen_IO_ddlist_15_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_15_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_15_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_15_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_15_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_15_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_15_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_15_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_15_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_15_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_15_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_15_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_15_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_15), &style_screen_IO_ddlist_15_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_15_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_15_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_15_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_15_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_15_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_15), &style_screen_IO_ddlist_15_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_16
    ui->screen_IO_ddlist_16 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_16, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_16, 605, 337);
    lv_obj_set_size(ui->screen_IO_ddlist_16, 160, 32);

    // Write style for screen_IO_ddlist_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_16, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_16, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_16, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_16, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_16, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_16, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_16, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_16, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_16, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_16_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_16_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_16_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_16_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_16_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_16_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_16_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_16_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_16_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_16_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_16_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_16), &style_screen_IO_ddlist_16_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_16_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_16_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_16_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_16_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_16_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_16_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_16_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_16_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_16_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_16_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_16_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_16_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_16), &style_screen_IO_ddlist_16_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_16_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_16_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_16_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_16_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_16_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_16), &style_screen_IO_ddlist_16_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_19
    ui->screen_IO_label_19 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_19, "满载(LNSL):");
    lv_label_set_long_mode(ui->screen_IO_label_19, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_19, 138, 392);
    lv_obj_set_size(ui->screen_IO_label_19, 191, 32);

    // Write style for screen_IO_label_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_19, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_19, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_19, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_19, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_19, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_20
    ui->screen_IO_label_20 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_20, "复位结束信号(MD_BTN):");
    lv_label_set_long_mode(ui->screen_IO_label_20, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_20, 333, 392);
    lv_obj_set_size(ui->screen_IO_label_20, 346, 32);

    // Write style for screen_IO_label_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_20, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_20, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_20, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_20, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_20, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_17
    ui->screen_IO_ddlist_17 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_17, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_17, 138, 426);
    lv_obj_set_size(ui->screen_IO_ddlist_17, 94, 32);

    // Write style for screen_IO_ddlist_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_17, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_17, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_17, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_17, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_17, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_17, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_17, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_17, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_17, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_17_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_17_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_17_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_17_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_17_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_17_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_17_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_17_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_17_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_17_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_17_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_17), &style_screen_IO_ddlist_17_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_17_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_17_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_17_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_17_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_17_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_17_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_17_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_17_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_17_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_17_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_17_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_17_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_17), &style_screen_IO_ddlist_17_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_17_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_17_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_17_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_17_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_17_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_17), &style_screen_IO_ddlist_17_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_18
    ui->screen_IO_ddlist_18 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_18, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_18, 235, 426);
    lv_obj_set_size(ui->screen_IO_ddlist_18, 94, 32);

    // Write style for screen_IO_ddlist_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_18, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_18, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_18, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_18, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_18, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_18, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_18, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_18, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_18, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_18_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_18_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_18_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_18_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_18_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_18_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_18_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_18_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_18_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_18_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_18_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_18), &style_screen_IO_ddlist_18_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_18_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_18_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_18_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_18_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_18_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_18_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_18_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_18_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_18_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_18_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_18_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_18_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_18), &style_screen_IO_ddlist_18_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_18_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_18_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_18_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_18_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_18_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_18), &style_screen_IO_ddlist_18_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_19
    ui->screen_IO_ddlist_19 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_19, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_19, 333, 426);
    lv_obj_set_size(ui->screen_IO_ddlist_19, 169, 32);

    // Write style for screen_IO_ddlist_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_19, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_19, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_19, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_19, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_19, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_19, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_19, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_19, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_19, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_19_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_19_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_19_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_19_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_19_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_19_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_19_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_19_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_19_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_19_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_19_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_19), &style_screen_IO_ddlist_19_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_19_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_19_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_19_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_19_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_19_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_19_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_19_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_19_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_19_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_19_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_19_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_19_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_19), &style_screen_IO_ddlist_19_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_19_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_19_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_19_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_19_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_19_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_19), &style_screen_IO_ddlist_19_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_20
    ui->screen_IO_ddlist_20 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_20, "1                 \n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_20, 508, 426);
    lv_obj_set_size(ui->screen_IO_ddlist_20, 169, 32);

    // Write style for screen_IO_ddlist_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_20, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_20, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_20, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_20, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_20, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_20, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_20, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_20, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_20, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_20_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_20_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_20_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_20_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_20_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_20_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_20_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_20_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_20_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_20_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_20_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_20), &style_screen_IO_ddlist_20_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_20_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_20_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_20_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_20_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_20_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_20_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_20_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_20_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_20_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_20_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_20_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_20_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_20), &style_screen_IO_ddlist_20_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_20_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_20_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_20_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_20_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_20_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_20), &style_screen_IO_ddlist_20_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_21
    ui->screen_IO_label_21 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_21, "蜂鸣信号(BUZ):");
    lv_label_set_long_mode(ui->screen_IO_label_21, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_21, 137, 483);
    lv_obj_set_size(ui->screen_IO_label_21, 246, 32);

    // Write style for screen_IO_label_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_21, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_21, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_21, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_21, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_21, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_22
    ui->screen_IO_label_22 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_22, "开门信号(DO):");
    lv_label_set_long_mode(ui->screen_IO_label_22, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_22, 386, 483);
    lv_obj_set_size(ui->screen_IO_label_22, 248, 32);

    // Write style for screen_IO_label_22, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_22, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_22, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_22, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_22, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_22, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_21
    ui->screen_IO_ddlist_21 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_21, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_21, 137, 517);
    lv_obj_set_size(ui->screen_IO_ddlist_21, 120, 32);

    // Write style for screen_IO_ddlist_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_21, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_21, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_21, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_21, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_21, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_21, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_21, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_21, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_21, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_21_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_21_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_21_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_21_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_21_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_21_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_21_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_21_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_21_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_21_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_21_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_21), &style_screen_IO_ddlist_21_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_21_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_21_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_21_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_21_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_21_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_21_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_21_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_21_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_21_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_21_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_21_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_21_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_21), &style_screen_IO_ddlist_21_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_21_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_21_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_21_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_21_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_21_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_21), &style_screen_IO_ddlist_21_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_22
    ui->screen_IO_ddlist_22 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_22, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_22, 262, 517);
    lv_obj_set_size(ui->screen_IO_ddlist_22, 121, 32);

    // Write style for screen_IO_ddlist_22, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_22, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_22, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_22, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_22, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_22, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_22, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_22, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_22, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_22, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_22_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_22_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_22_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_22_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_22_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_22_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_22_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_22_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_22_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_22_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_22_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_22), &style_screen_IO_ddlist_22_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_22_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_22_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_22_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_22_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_22_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_22_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_22_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_22_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_22_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_22_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_22_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_22_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_22), &style_screen_IO_ddlist_22_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_22_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_22_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_22_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_22_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_22_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_22), &style_screen_IO_ddlist_22_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_23
    ui->screen_IO_ddlist_23 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_23, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_23, 386, 517);
    lv_obj_set_size(ui->screen_IO_ddlist_23, 121, 32);

    // Write style for screen_IO_ddlist_23, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_23, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_23, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_23, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_23, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_23, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_23, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_23, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_23, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_23, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_23_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_23_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_23_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_23_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_23_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_23_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_23_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_23_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_23_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_23_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_23_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_23), &style_screen_IO_ddlist_23_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_23_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_23_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_23_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_23_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_23_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_23_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_23_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_23_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_23_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_23_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_23_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_23_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_23), &style_screen_IO_ddlist_23_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_23_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_23_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_23_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_23_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_23_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_23), &style_screen_IO_ddlist_23_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_24
    ui->screen_IO_ddlist_24 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_24, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_24, 511, 517);
    lv_obj_set_size(ui->screen_IO_ddlist_24, 121, 32);

    // Write style for screen_IO_ddlist_24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_24, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_24, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_24, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_24, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_24, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_24, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_24, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_24, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_24, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_24_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_24_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_24_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_24_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_24_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_24_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_24_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_24_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_24_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_24_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_24_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_24), &style_screen_IO_ddlist_24_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_24_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_24_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_24_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_24_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_24_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_24_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_24_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_24_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_24_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_24_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_24_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_24_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_24), &style_screen_IO_ddlist_24_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_24_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_24_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_24_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_24_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_24_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_24), &style_screen_IO_ddlist_24_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_23
    ui->screen_IO_label_23 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_23, "运行中上行(M-CUDL):");
    lv_label_set_long_mode(ui->screen_IO_label_23, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_23, 137, 574);
    lv_obj_set_size(ui->screen_IO_label_23, 300, 32);

    // Write style for screen_IO_label_23, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_23, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_23, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_23, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_23, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_23, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_24
    ui->screen_IO_label_24 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_24, "运行中下行(M-CDDL):");
    lv_label_set_long_mode(ui->screen_IO_label_24, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_24, 441, 574);
    lv_obj_set_size(ui->screen_IO_label_24, 303, 32);

    // Write style for screen_IO_label_24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_24, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_24, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_24, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_24, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_24, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_25
    ui->screen_IO_ddlist_25 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_25, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_25, 136, 608);
    lv_obj_set_size(ui->screen_IO_ddlist_25, 149, 32);

    // Write style for screen_IO_ddlist_25, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_25, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_25, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_25, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_25, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_25, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_25, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_25, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_25, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_25, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_25_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_25_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_25_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_25_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_25_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_25_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_25_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_25_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_25_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_25_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_25_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_25), &style_screen_IO_ddlist_25_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_25_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_25_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_25_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_25_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_25_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_25_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_25_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_25_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_25_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_25_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_25_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_25_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_25), &style_screen_IO_ddlist_25_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_25_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_25_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_25_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_25_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_25_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_25), &style_screen_IO_ddlist_25_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_26
    ui->screen_IO_ddlist_26 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_26, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_26, 288, 608);
    lv_obj_set_size(ui->screen_IO_ddlist_26, 149, 32);

    // Write style for screen_IO_ddlist_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_26, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_26, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_26, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_26, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_26, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_26, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_26, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_26, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_26, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_26_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_26_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_26_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_26_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_26_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_26_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_26_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_26_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_26_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_26_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_26_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_26), &style_screen_IO_ddlist_26_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_26_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_26_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_26_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_26_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_26_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_26_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_26_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_26_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_26_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_26_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_26_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_26_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_26), &style_screen_IO_ddlist_26_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_26_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_26_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_26_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_26_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_26_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_26), &style_screen_IO_ddlist_26_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_27
    ui->screen_IO_ddlist_27 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_27, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_27, 441, 608);
    lv_obj_set_size(ui->screen_IO_ddlist_27, 149, 32);

    // Write style for screen_IO_ddlist_27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_27, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_27, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_27, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_27, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_27, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_27, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_27, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_27, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_27, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_27_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_27_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_27_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_27_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_27_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_27_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_27_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_27_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_27_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_27_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_27_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_27), &style_screen_IO_ddlist_27_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_27_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_27_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_27_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_27_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_27_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_27_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_27_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_27_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_27_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_27_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_27_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_27_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_27), &style_screen_IO_ddlist_27_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_27_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_27_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_27_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_27_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_27_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_27), &style_screen_IO_ddlist_27_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_28
    ui->screen_IO_ddlist_28 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_28, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_28, 594, 608);
    lv_obj_set_size(ui->screen_IO_ddlist_28, 149, 32);

    // Write style for screen_IO_ddlist_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_28, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_28, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_28, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_28, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_28, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_28, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_28, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_28, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_28, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_28_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_28_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_28_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_28_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_28_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_28_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_28_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_28_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_28_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_28_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_28_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_28), &style_screen_IO_ddlist_28_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_28_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_28_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_28_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_28_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_28_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_28_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_28_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_28_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_28_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_28_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_28_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_28_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_28), &style_screen_IO_ddlist_28_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_28_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_28_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_28_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_28_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_28_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_28), &style_screen_IO_ddlist_28_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_25
    ui->screen_IO_label_25 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_25, "前门上行到层(CDLU):");
    lv_label_set_long_mode(ui->screen_IO_label_25, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_25, 137, 664);
    lv_obj_set_size(ui->screen_IO_label_25, 300, 32);

    // Write style for screen_IO_label_25, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_25, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_25, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_25, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_25, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_25, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_26
    ui->screen_IO_label_26 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_26, "前门下行到层(CDLD):");
    lv_label_set_long_mode(ui->screen_IO_label_26, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_26, 441, 664);
    lv_obj_set_size(ui->screen_IO_label_26, 303, 32);

    // Write style for screen_IO_label_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_26, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_26, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_26, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_26, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_26, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_29
    ui->screen_IO_ddlist_29 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_29, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_29, 137, 698);
    lv_obj_set_size(ui->screen_IO_ddlist_29, 147, 32);

    // Write style for screen_IO_ddlist_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_29, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_29, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_29, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_29, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_29, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_29, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_29, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_29, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_29, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_29_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_29_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_29_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_29_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_29_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_29_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_29_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_29_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_29_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_29_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_29_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_29), &style_screen_IO_ddlist_29_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_29_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_29_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_29_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_29_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_29_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_29_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_29_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_29_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_29_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_29_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_29_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_29_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_29), &style_screen_IO_ddlist_29_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_29_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_29_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_29_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_29_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_29_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_29), &style_screen_IO_ddlist_29_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_30
    ui->screen_IO_ddlist_30 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_30, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_30, 288, 698);
    lv_obj_set_size(ui->screen_IO_ddlist_30, 149, 32);

    // Write style for screen_IO_ddlist_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_30, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_30, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_30, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_30, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_30, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_30, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_30, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_30, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_30, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_30_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_30_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_30_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_30_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_30_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_30_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_30_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_30_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_30_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_30_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_30_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_30), &style_screen_IO_ddlist_30_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_30_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_30_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_30_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_30_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_30_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_30_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_30_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_30_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_30_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_30_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_30_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_30_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_30), &style_screen_IO_ddlist_30_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_30_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_30_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_30_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_30_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_30_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_30), &style_screen_IO_ddlist_30_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_31
    ui->screen_IO_ddlist_31 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_31, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_31, 441, 698);
    lv_obj_set_size(ui->screen_IO_ddlist_31, 149, 32);

    // Write style for screen_IO_ddlist_31, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_31, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_31, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_31, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_31, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_31, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_31, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_31, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_31, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_31, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_31_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_31_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_31_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_31_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_31_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_31_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_31_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_31_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_31_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_31_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_31_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_31), &style_screen_IO_ddlist_31_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_31_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_31_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_31_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_31_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_31_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_31_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_31_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_31_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_31_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_31_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_31_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_31_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_31), &style_screen_IO_ddlist_31_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_31_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_31_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_31_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_31_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_31_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_31), &style_screen_IO_ddlist_31_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_32
    ui->screen_IO_ddlist_32 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_32, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_32, 594, 698);
    lv_obj_set_size(ui->screen_IO_ddlist_32, 149, 32);

    // Write style for screen_IO_ddlist_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_32, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_32, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_32, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_32, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_32, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_32, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_32, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_32, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_32, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_32_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_32_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_32_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_32_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_32_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_32_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_32_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_32_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_32_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_32_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_32_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_32), &style_screen_IO_ddlist_32_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_32_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_32_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_32_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_32_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_32_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_32_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_32_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_32_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_32_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_32_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_32_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_32_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_32), &style_screen_IO_ddlist_32_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_32_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_32_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_32_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_32_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_32_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_32), &style_screen_IO_ddlist_32_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_27
    ui->screen_IO_label_27 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_27, "后门上行到层(RCDLU):");
    lv_label_set_long_mode(ui->screen_IO_label_27, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_27, 137, 757);
    lv_obj_set_size(ui->screen_IO_label_27, 304, 32);

    // Write style for screen_IO_label_27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_27, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_27, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_27, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_27, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_27, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_28
    ui->screen_IO_label_28 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_28, "后门下行到层(RCDLD):");
    lv_label_set_long_mode(ui->screen_IO_label_28, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_28, 445, 757);
    lv_obj_set_size(ui->screen_IO_label_28, 306, 32);

    // Write style for screen_IO_label_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_28, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_28, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_28, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_28, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_28, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_33
    ui->screen_IO_ddlist_33 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_33, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_33, 137, 791);
    lv_obj_set_size(ui->screen_IO_ddlist_33, 151, 32);

    // Write style for screen_IO_ddlist_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_33, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_33, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_33, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_33, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_33, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_33, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_33, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_33, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_33, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_33_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_33_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_33_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_33_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_33_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_33_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_33_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_33_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_33_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_33_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_33_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_33), &style_screen_IO_ddlist_33_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_33_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_33_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_33_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_33_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_33_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_33_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_33_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_33_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_33_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_33_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_33_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_33_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_33), &style_screen_IO_ddlist_33_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_33_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_33_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_33_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_33_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_33_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_33), &style_screen_IO_ddlist_33_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_34
    ui->screen_IO_ddlist_34 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_34, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_34, 290, 791);
    lv_obj_set_size(ui->screen_IO_ddlist_34, 151, 32);

    // Write style for screen_IO_ddlist_34, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_34, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_34, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_34, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_34, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_34, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_34, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_34, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_34, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_34, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_34_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_34_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_34_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_34_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_34_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_34_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_34_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_34_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_34_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_34_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_34_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_34), &style_screen_IO_ddlist_34_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_34_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_34_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_34_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_34_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_34_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_34_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_34_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_34_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_34_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_34_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_34_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_34_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_34), &style_screen_IO_ddlist_34_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_34_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_34_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_34_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_34_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_34_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_34), &style_screen_IO_ddlist_34_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_35
    ui->screen_IO_ddlist_35 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_35, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_35, 445, 791);
    lv_obj_set_size(ui->screen_IO_ddlist_35, 151, 32);

    // Write style for screen_IO_ddlist_35, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_35, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_35, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_35, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_35, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_35, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_35, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_35, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_35, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_35, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_35, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_35, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_35, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_35_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_35_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_35_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_35_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_35_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_35_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_35_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_35_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_35_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_35_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_35_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_35), &style_screen_IO_ddlist_35_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_35_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_35_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_35_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_35_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_35_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_35_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_35_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_35_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_35_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_35_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_35_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_35_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_35), &style_screen_IO_ddlist_35_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_35_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_35_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_35_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_35_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_35_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_35), &style_screen_IO_ddlist_35_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_36
    ui->screen_IO_ddlist_36 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_36, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_36, 599, 791);
    lv_obj_set_size(ui->screen_IO_ddlist_36, 151, 32);

    // Write style for screen_IO_ddlist_36, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_36, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_36, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_36, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_36, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_36, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_36, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_36, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_36, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_36, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_36_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_36_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_36_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_36_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_36_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_36_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_36_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_36_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_36_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_36_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_36_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_36), &style_screen_IO_ddlist_36_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_36_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_36_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_36_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_36_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_36_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_36_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_36_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_36_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_36_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_36_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_36_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_36_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_36), &style_screen_IO_ddlist_36_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_36_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_36_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_36_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_36_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_36_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_36), &style_screen_IO_ddlist_36_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_29
    ui->screen_IO_label_29 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_29, "困人安抚信号():");
    lv_label_set_long_mode(ui->screen_IO_label_29, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_29, 136, 847);
    lv_obj_set_size(ui->screen_IO_label_29, 248, 32);

    // Write style for screen_IO_label_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_29, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_29, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_29, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_29, lv_color_hex(0xf11717), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_29, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_37
    ui->screen_IO_ddlist_37 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_37, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_37, 136, 881);
    lv_obj_set_size(ui->screen_IO_ddlist_37, 122, 32);

    // Write style for screen_IO_ddlist_37, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_37, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_37, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_37, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_37, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_37, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_37, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_37, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_37, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_37, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_37, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_37, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_37, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_37_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_37_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_37_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_37_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_37_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_37_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_37_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_37_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_37_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_37_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_37_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_37), &style_screen_IO_ddlist_37_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_37_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_37_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_37_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_37_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_37_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_37_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_37_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_37_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_37_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_37_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_37_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_37_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_37), &style_screen_IO_ddlist_37_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_37_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_37_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_37_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_37_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_37_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_37), &style_screen_IO_ddlist_37_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_38
    ui->screen_IO_ddlist_38 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_38, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_38, 262, 881);
    lv_obj_set_size(ui->screen_IO_ddlist_38, 122, 32);

    // Write style for screen_IO_ddlist_38, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_38, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_38, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_38, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_38, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_38, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_38, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_38, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_38, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_38, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_38_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_38_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_38_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_38_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_38_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_38_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_38_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_38_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_38_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_38_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_38_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_38), &style_screen_IO_ddlist_38_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_38_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_38_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_38_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_38_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_38_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_38_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_38_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_38_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_38_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_38_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_38_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_38_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_38), &style_screen_IO_ddlist_38_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_38_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_38_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_38_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_38_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_38_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_38), &style_screen_IO_ddlist_38_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_39
    ui->screen_IO_ddlist_39 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_39, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_39, 397, 881);
    lv_obj_set_size(ui->screen_IO_ddlist_39, 64, 32);

    // Write style for screen_IO_ddlist_39, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_39, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_39, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_39, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_39, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_39, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_39, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_39, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_39, lv_color_hex(0x98b5ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_39, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_39_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_39_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_39_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_39_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_39_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_39_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_39_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_39_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_39_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_39_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_39_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_39), &style_screen_IO_ddlist_39_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_39_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_39_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_39_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_39_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_39_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_39_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_39_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_39_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_39_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_39_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_39_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_39_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_39), &style_screen_IO_ddlist_39_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_39_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_39_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_39_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_39_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_39_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_39), &style_screen_IO_ddlist_39_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_ddlist_40
    ui->screen_IO_ddlist_40 = lv_dropdown_create(ui->screen_IO);
    lv_dropdown_set_options(ui->screen_IO_ddlist_40, "1\n2\n3\n4");
    lv_obj_set_pos(ui->screen_IO_ddlist_40, 462, 881);
    lv_obj_set_size(ui->screen_IO_ddlist_40, 64, 32);

    // Write style for screen_IO_ddlist_40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_IO_ddlist_40, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_ddlist_40, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_ddlist_40, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_ddlist_40, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_ddlist_40, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_ddlist_40, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_ddlist_40, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_ddlist_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_ddlist_40, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_ddlist_40, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_ddlist_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_ddlist_40, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_ddlist_40, lv_color_hex(0x98b5ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_ddlist_40, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_ddlist_40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_IO_ddlist_40_extra_list_selected_checked
    static lv_style_t style_screen_IO_ddlist_40_extra_list_selected_checked;
    ui_init_style(&style_screen_IO_ddlist_40_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_IO_ddlist_40_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_IO_ddlist_40_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_IO_ddlist_40_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_IO_ddlist_40_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_IO_ddlist_40_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_40_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_40_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_40_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_40), &style_screen_IO_ddlist_40_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_40_extra_list_main_default
    static lv_style_t style_screen_IO_ddlist_40_extra_list_main_default;
    ui_init_style(&style_screen_IO_ddlist_40_extra_list_main_default);

    lv_style_set_max_height(&style_screen_IO_ddlist_40_extra_list_main_default, 150);
    lv_style_set_text_color(&style_screen_IO_ddlist_40_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_IO_ddlist_40_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_IO_ddlist_40_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_IO_ddlist_40_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_IO_ddlist_40_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_40_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_IO_ddlist_40_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_40_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_40), &style_screen_IO_ddlist_40_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_IO_ddlist_40_extra_list_scrollbar_default
    static lv_style_t style_screen_IO_ddlist_40_extra_list_scrollbar_default;
    ui_init_style(&style_screen_IO_ddlist_40_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_IO_ddlist_40_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_IO_ddlist_40_extra_list_scrollbar_default, 0);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_IO_ddlist_40), &style_screen_IO_ddlist_40_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_return
    ui->screen_IO_btn_return = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_return_label = lv_label_create(ui->screen_IO_btn_return);
    lv_label_set_text(ui->screen_IO_btn_return_label, "返回");
    lv_label_set_long_mode(ui->screen_IO_btn_return_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_return_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_return, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_return_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_return, 234, 950);
    lv_obj_set_size(ui->screen_IO_btn_return, 133, 52);

    // Write style for screen_IO_btn_return, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_return, lv_color_hex(0xff0027), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_return, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_return, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_return, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_return, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_12
    ui->screen_IO_btn_12 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_12_label = lv_label_create(ui->screen_IO_btn_12);
    lv_label_set_text(ui->screen_IO_btn_12_label, "调用默认");
    lv_label_set_long_mode(ui->screen_IO_btn_12_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_12_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_12, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_12_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_12, 376, 950);
    lv_obj_set_size(ui->screen_IO_btn_12, 178, 52);

    // Write style for screen_IO_btn_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_12, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_12, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_12, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_2
    ui->screen_IO_btn_2 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_2_label = lv_label_create(ui->screen_IO_btn_2);
    lv_label_set_text(ui->screen_IO_btn_2_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_2_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_2, 22, 134);
    lv_obj_set_size(ui->screen_IO_btn_2, 110, 40);

    // Write style for screen_IO_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_2, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_2, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_2, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_3
    ui->screen_IO_btn_3 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_3_label = lv_label_create(ui->screen_IO_btn_3);
    lv_label_set_text(ui->screen_IO_btn_3_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_3_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_3, 22, 226);
    lv_obj_set_size(ui->screen_IO_btn_3, 110, 40);

    // Write style for screen_IO_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_3, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_3, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_3, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_4
    ui->screen_IO_btn_4 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_4_label = lv_label_create(ui->screen_IO_btn_4);
    lv_label_set_text(ui->screen_IO_btn_4_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_4_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_4_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_4, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_4_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_4, 22, 316);
    lv_obj_set_size(ui->screen_IO_btn_4, 110, 40);

    // Write style for screen_IO_btn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_4, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_4, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_4, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_5
    ui->screen_IO_btn_5 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_5_label = lv_label_create(ui->screen_IO_btn_5);
    lv_label_set_text(ui->screen_IO_btn_5_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_5_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_5_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_5, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_5_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_5, 22, 405);
    lv_obj_set_size(ui->screen_IO_btn_5, 110, 40);

    // Write style for screen_IO_btn_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_5, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_5, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_5, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_6
    ui->screen_IO_btn_6 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_6_label = lv_label_create(ui->screen_IO_btn_6);
    lv_label_set_text(ui->screen_IO_btn_6_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_6_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_6_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_6, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_6_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_6, 22, 495);
    lv_obj_set_size(ui->screen_IO_btn_6, 110, 40);

    // Write style for screen_IO_btn_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_6, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_6, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_6, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_6, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_7
    ui->screen_IO_btn_7 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_7_label = lv_label_create(ui->screen_IO_btn_7);
    lv_label_set_text(ui->screen_IO_btn_7_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_7_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_7_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_7, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_7_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_7, 22, 586);
    lv_obj_set_size(ui->screen_IO_btn_7, 110, 40);

    // Write style for screen_IO_btn_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_7, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_7, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_7, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_7, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_7, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_8
    ui->screen_IO_btn_8 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_8_label = lv_label_create(ui->screen_IO_btn_8);
    lv_label_set_text(ui->screen_IO_btn_8_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_8_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_8_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_8, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_8_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_8, 22, 676);
    lv_obj_set_size(ui->screen_IO_btn_8, 110, 40);

    // Write style for screen_IO_btn_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_8, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_8, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_8, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_8, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_8, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_9
    ui->screen_IO_btn_9 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_9_label = lv_label_create(ui->screen_IO_btn_9);
    lv_label_set_text(ui->screen_IO_btn_9_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_9_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_9_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_9, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_9_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_9, 22, 769);
    lv_obj_set_size(ui->screen_IO_btn_9, 110, 40);

    // Write style for screen_IO_btn_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_9, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_9, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_9, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_9, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_9, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_btn_10
    ui->screen_IO_btn_10 = lv_btn_create(ui->screen_IO);
    ui->screen_IO_btn_10_label = lv_label_create(ui->screen_IO_btn_10);
    lv_label_set_text(ui->screen_IO_btn_10_label, "发送");
    lv_label_set_long_mode(ui->screen_IO_btn_10_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_IO_btn_10_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_IO_btn_10, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_IO_btn_10_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_IO_btn_10, 22, 860);
    lv_obj_set_size(ui->screen_IO_btn_10, 110, 40);

    // Write style for screen_IO_btn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_IO_btn_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_btn_10, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_btn_10, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_IO_btn_10, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_btn_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_btn_10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_btn_10, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_btn_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_btn_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_btn_10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_btn_10, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_btn_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_btn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c1
    ui->screen_IO_label_c1 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c1, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c1, 227, 67);
    lv_obj_set_size(ui->screen_IO_label_c1, 30, 32);

    // Write style for screen_IO_label_c1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c1, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c1, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c1, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c2
    ui->screen_IO_label_c2 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c2, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c2, 352, 67);
    lv_obj_set_size(ui->screen_IO_label_c2, 30, 32);

    // Write style for screen_IO_label_c2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c2, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c2, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c2, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c2, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c3
    ui->screen_IO_label_c3 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c3, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c3, 477, 67);
    lv_obj_set_size(ui->screen_IO_label_c3, 30, 32);

    // Write style for screen_IO_label_c3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c3, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c3, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c3, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c3, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c3, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c3, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c4
    ui->screen_IO_label_c4 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c4, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c4, 601, 67);
    lv_obj_set_size(ui->screen_IO_label_c4, 30, 32);

    // Write style for screen_IO_label_c4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c4, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c4, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c4, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c4, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c4, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c4, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c5
    ui->screen_IO_label_c5 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c5, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c5, 227, 156);
    lv_obj_set_size(ui->screen_IO_label_c5, 30, 32);

    // Write style for screen_IO_label_c5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c5, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c5, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c5, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c5, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c5, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c5, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c6
    ui->screen_IO_label_c6 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c6, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c6, 352, 156);
    lv_obj_set_size(ui->screen_IO_label_c6, 30, 32);

    // Write style for screen_IO_label_c6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c6, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c6, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c6, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c6, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c6, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c6, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c6, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c7
    ui->screen_IO_label_c7 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c7, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c7, 477, 156);
    lv_obj_set_size(ui->screen_IO_label_c7, 30, 32);

    // Write style for screen_IO_label_c7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c7, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c7, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c7, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c7, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c7, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c7, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c7, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c8
    ui->screen_IO_label_c8 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c8, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c8, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c8, 601, 156);
    lv_obj_set_size(ui->screen_IO_label_c8, 30, 32);

    // Write style for screen_IO_label_c8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c8, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c8, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c8, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c8, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c8, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c8, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c8, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c9
    ui->screen_IO_label_c9 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c9, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c9, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c9, 199, 249);
    lv_obj_set_size(ui->screen_IO_label_c9, 30, 32);

    // Write style for screen_IO_label_c9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c9, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c9, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c9, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c9, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c9, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c9, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c9, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c10
    ui->screen_IO_label_c10 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c10, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c10, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c10, 296, 249);
    lv_obj_set_size(ui->screen_IO_label_c10, 30, 32);

    // Write style for screen_IO_label_c10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c10, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c10, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c10, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c10, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c10, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c10, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c10, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c11
    ui->screen_IO_label_c11 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c11, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c11, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c11, 394, 249);
    lv_obj_set_size(ui->screen_IO_label_c11, 30, 32);

    // Write style for screen_IO_label_c11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c11, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c11, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c11, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c11, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c11, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c11, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c11, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c11, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c11, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c12
    ui->screen_IO_label_c12 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c12, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c12, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c12, 492, 249);
    lv_obj_set_size(ui->screen_IO_label_c12, 30, 32);

    // Write style for screen_IO_label_c12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c12, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c12, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c12, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c12, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c12, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c12, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c12, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c13
    ui->screen_IO_label_c13 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c13, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c13, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c13, 253, 338);
    lv_obj_set_size(ui->screen_IO_label_c13, 30, 32);

    // Write style for screen_IO_label_c13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c13, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c13, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c13, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c13, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c13, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c13, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c13, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c13, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c13, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c14
    ui->screen_IO_label_c14 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c14, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c14, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c14, 407, 338);
    lv_obj_set_size(ui->screen_IO_label_c14, 30, 32);

    // Write style for screen_IO_label_c14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c14, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c14, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c14, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c14, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c14, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c14, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c14, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c14, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c14, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c15
    ui->screen_IO_label_c15 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c15, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c15, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c15, 571, 338);
    lv_obj_set_size(ui->screen_IO_label_c15, 30, 32);

    // Write style for screen_IO_label_c15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c15, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c15, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c15, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c15, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c15, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c15, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c15, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c15, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c15, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c16
    ui->screen_IO_label_c16 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c16, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c16, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c16, 734, 338);
    lv_obj_set_size(ui->screen_IO_label_c16, 30, 32);

    // Write style for screen_IO_label_c16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c16, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c16, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c16, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c16, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c16, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c16, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c16, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c16, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c16, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c17
    ui->screen_IO_label_c17 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c17, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c17, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c17, 201, 427);
    lv_obj_set_size(ui->screen_IO_label_c17, 30, 32);

    // Write style for screen_IO_label_c17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c17, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c17, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c17, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c17, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c17, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c17, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c17, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c17, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c17, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c18
    ui->screen_IO_label_c18 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c18, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c18, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c18, 298, 427);
    lv_obj_set_size(ui->screen_IO_label_c18, 30, 32);

    // Write style for screen_IO_label_c18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c18, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c18, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c18, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c18, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c18, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c18, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c18, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c18, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c18, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c19
    ui->screen_IO_label_c19 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c19, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c19, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c19, 471, 427);
    lv_obj_set_size(ui->screen_IO_label_c19, 30, 32);

    // Write style for screen_IO_label_c19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c19, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c19, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c19, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c19, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c19, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c19, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c19, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c19, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c19, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c20
    ui->screen_IO_label_c20 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c20, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c20, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c20, 646, 427);
    lv_obj_set_size(ui->screen_IO_label_c20, 30, 32);

    // Write style for screen_IO_label_c20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c20, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c20, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c20, LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c20, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c20, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c20, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c20, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c20, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c20, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c21
    ui->screen_IO_label_c21 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c21, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c21, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c21, 226, 518);
    lv_obj_set_size(ui->screen_IO_label_c21, 30, 32);

    // Write style for screen_IO_label_c21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c21, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c21, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c21, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c21, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c21, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c21, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c21, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c21, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c21, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c21, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c22
    ui->screen_IO_label_c22 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c22, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c22, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c22, 352, 518);
    lv_obj_set_size(ui->screen_IO_label_c22, 30, 32);

    // Write style for screen_IO_label_c22, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c22, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c22, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c22, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c22, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c22, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c22, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c22, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c22, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c22, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c22, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c23
    ui->screen_IO_label_c23 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c23, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c23, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c23, 476, 518);
    lv_obj_set_size(ui->screen_IO_label_c23, 30, 32);

    // Write style for screen_IO_label_c23, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c23, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c23, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c23, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c23, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c23, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c23, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c23, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c23, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c23, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c23, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c24
    ui->screen_IO_label_c24 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c24, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c24, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c24, 601, 518);
    lv_obj_set_size(ui->screen_IO_label_c24, 30, 32);

    // Write style for screen_IO_label_c24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c24, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c24, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c24, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c24, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c24, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c24, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c24, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c24, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c24, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c24, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c25
    ui->screen_IO_label_c25 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c25, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c25, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c25, 254, 609);
    lv_obj_set_size(ui->screen_IO_label_c25, 30, 32);

    // Write style for screen_IO_label_c25, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c25, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c25, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c25, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c25, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c25, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c25, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c25, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c25, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c25, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c25, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c26
    ui->screen_IO_label_c26 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c26, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c26, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c26, 406, 609);
    lv_obj_set_size(ui->screen_IO_label_c26, 30, 32);

    // Write style for screen_IO_label_c26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c26, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c26, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c26, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c26, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c26, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c26, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c26, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c26, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c26, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c26, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c27
    ui->screen_IO_label_c27 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c27, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c27, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c27, 559, 609);
    lv_obj_set_size(ui->screen_IO_label_c27, 30, 32);

    // Write style for screen_IO_label_c27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c27, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c27, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c27, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c27, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c27, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c27, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c27, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c27, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c27, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c27, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c28
    ui->screen_IO_label_c28 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c28, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c28, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c28, 712, 609);
    lv_obj_set_size(ui->screen_IO_label_c28, 30, 32);

    // Write style for screen_IO_label_c28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c28, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c28, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c28, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c28, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c28, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c28, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c28, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c28, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c28, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c28, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c29
    ui->screen_IO_label_c29 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c29, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c29, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c29, 253, 699);
    lv_obj_set_size(ui->screen_IO_label_c29, 30, 32);

    // Write style for screen_IO_label_c29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c29, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c29, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c29, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c29, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c29, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c29, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c29, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c29, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c29, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c30
    ui->screen_IO_label_c30 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c30, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c30, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c30, 406, 699);
    lv_obj_set_size(ui->screen_IO_label_c30, 30, 32);

    // Write style for screen_IO_label_c30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c30, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c30, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c30, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c30, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c30, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c30, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c30, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c30, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c30, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c31
    ui->screen_IO_label_c31 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c31, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c31, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c31, 559, 699);
    lv_obj_set_size(ui->screen_IO_label_c31, 30, 32);

    // Write style for screen_IO_label_c31, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c31, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c31, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c31, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c31, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c31, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c31, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c31, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c31, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c31, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c31, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c32
    ui->screen_IO_label_c32 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c32, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c32, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c32, 712, 699);
    lv_obj_set_size(ui->screen_IO_label_c32, 30, 32);

    // Write style for screen_IO_label_c32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c32, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c32, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c32, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c32, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c32, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c32, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c32, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c32, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c32, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c33
    ui->screen_IO_label_c33 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c33, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c33, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c33, 257, 792);
    lv_obj_set_size(ui->screen_IO_label_c33, 30, 32);

    // Write style for screen_IO_label_c33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c33, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c33, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c33, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c33, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c33, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c33, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c33, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c33, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c33, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c33, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c34
    ui->screen_IO_label_c34 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c34, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c34, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c34, 410, 792);
    lv_obj_set_size(ui->screen_IO_label_c34, 30, 32);

    // Write style for screen_IO_label_c34, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c34, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c34, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c34, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c34, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c34, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c34, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c34, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c34, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c34, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c34, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c35
    ui->screen_IO_label_c35 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c35, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c35, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c35, 565, 792);
    lv_obj_set_size(ui->screen_IO_label_c35, 30, 32);

    // Write style for screen_IO_label_c35, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c35, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c35, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c35, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c35, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c35, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c35, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c35, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c35, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c35, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c35, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c35, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c35, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c35, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c36
    ui->screen_IO_label_c36 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c36, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c36, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c36, 719, 792);
    lv_obj_set_size(ui->screen_IO_label_c36, 30, 32);

    // Write style for screen_IO_label_c36, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c36, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c36, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c36, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c36, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c36, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c36, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c36, lv_color_hex(0xebeb6a), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c36, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c36, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c36, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c37
    ui->screen_IO_label_c37 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c37, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c37, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c37, 227, 882);
    lv_obj_set_size(ui->screen_IO_label_c37, 30, 32);

    // Write style for screen_IO_label_c37, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c37, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c37, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c37, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c37, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c37, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c37, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c37, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c37, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c37, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c37, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c37, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c37, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c37, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c38
    ui->screen_IO_label_c38 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c38, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c38, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c38, 353, 882);
    lv_obj_set_size(ui->screen_IO_label_c38, 30, 32);

    // Write style for screen_IO_label_c38, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c38, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c38, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c38, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c38, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c38, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c38, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c38, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c38, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c38, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c38, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c39
    ui->screen_IO_label_c39 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c39, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c39, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c39, 430, 882);
    lv_obj_set_size(ui->screen_IO_label_c39, 30, 32);

    // Write style for screen_IO_label_c39, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c39, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c39, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c39, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c39, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c39, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c39, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c39, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c39, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c39, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c39, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_IO_label_c40
    ui->screen_IO_label_c40 = lv_label_create(ui->screen_IO);
    lv_label_set_text(ui->screen_IO_label_c40, "" LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->screen_IO_label_c40, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_IO_label_c40, 495, 882);
    lv_obj_set_size(ui->screen_IO_label_c40, 30, 32);

    // Write style for screen_IO_label_c40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_IO_label_c40, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_IO_label_c40, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_IO_label_c40, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_IO_label_c40, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_IO_label_c40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_IO_label_c40, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_IO_label_c40, &lv_font_Dengb_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_IO_label_c40, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_IO_label_c40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_IO_label_c40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_IO_label_c40, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_IO_label_c40, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_IO_label_c40, lv_color_hex(0x3f7bda), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_IO_label_c40, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_IO_label_c40, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_IO_label_c40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_IO_label_c40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_IO_label_c40, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_IO_label_c40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // The custom code of screen_IO
    lv_obj_clear_flag(ui->screen_IO, LV_OBJ_FLAG_SCROLLABLE);

    // 2. 创建数组，存储所有下拉列表的指针（根据实际变量名填写）
    lv_obj_t *ddlist_array_io[40] = {
        ui->screen_IO_ddlist_1, ui->screen_IO_ddlist_2, ui->screen_IO_ddlist_3, ui->screen_IO_ddlist_4,
        ui->screen_IO_ddlist_5, ui->screen_IO_ddlist_6, ui->screen_IO_ddlist_7, ui->screen_IO_ddlist_8,
        ui->screen_IO_ddlist_9, ui->screen_IO_ddlist_10, ui->screen_IO_ddlist_11, ui->screen_IO_ddlist_12,
        ui->screen_IO_ddlist_13, ui->screen_IO_ddlist_14, ui->screen_IO_ddlist_15, ui->screen_IO_ddlist_16,
        ui->screen_IO_ddlist_17, ui->screen_IO_ddlist_18, ui->screen_IO_ddlist_19, ui->screen_IO_ddlist_20,
        ui->screen_IO_ddlist_21, ui->screen_IO_ddlist_22, ui->screen_IO_ddlist_23, ui->screen_IO_ddlist_24,
        ui->screen_IO_ddlist_25, ui->screen_IO_ddlist_26, ui->screen_IO_ddlist_27, ui->screen_IO_ddlist_28,
        ui->screen_IO_ddlist_29, ui->screen_IO_ddlist_30, ui->screen_IO_ddlist_31, ui->screen_IO_ddlist_32,
        ui->screen_IO_ddlist_33, ui->screen_IO_ddlist_34, ui->screen_IO_ddlist_35, ui->screen_IO_ddlist_36,
        ui->screen_IO_ddlist_37, ui->screen_IO_ddlist_38, ui->screen_IO_ddlist_39, ui->screen_IO_ddlist_40};

    // 定义两种选项字符串
    // 偶数索引(0,2,4...)使用：4-63（步长1，共60个选项）
    const char *ddlist_options_even = "63\n62\n61\n60\n59\n58\n57\n56\n55\n54\n53\n52\n51\n50\n49\n48\n47\n46\n45\n44\n"
                                      "43\n42\n41\n40\n39\n38\n37\n36\n35\n34\n33\n32\n31\n30\n29\n28\n27\n26\n25\n24\n"
                                      "23\n22\n21\n20\n19\n18\n17\n16\n15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4";

    // 奇数索引(1,3,5...)使用：1-4（步长1，共4个选项）
    const char *ddlist_options_odd = "4\n3\n2\n1";

    // 循环为所有下拉列表设置选项（根据索引奇偶性区分）
    for (int i = 0; i < 40; i++)
    {
        if (ddlist_array_io[i] != NULL)
        {
            // 根据索引奇偶性选择不同选项集
            if (i % 2 == 0) // 偶数索引：0,2,4...
            {
                lv_dropdown_set_options(ddlist_array_io[i], ddlist_options_even);
                // 默认选中值：限制在4-63范围内（取IO_value[i]与63的最小值，且不小于4）
                int default_val = IO_value[i];
                if (default_val < 4)
                    default_val = 4;
                if (default_val > 63)
                    default_val = 63;
                // 计算选项索引（选项是倒序的，63对应索引0，4对应索引59）
                lv_dropdown_set_selected(ddlist_array_io[i], 63 - default_val);
            }
            else // 奇数索引：1,3,5...
            {
                lv_dropdown_set_options(ddlist_array_io[i], ddlist_options_odd);
                // 默认选中值：限制在1-4范围内
                int default_val = IO_value[i];
                if (default_val < 1)
                    default_val = 1;
                if (default_val > 4)
                    default_val = 4;
                // 计算选项索引（选项是倒序的，4对应索引0，1对应索引3）
                lv_dropdown_set_selected(ddlist_array_io[i], 4 - default_val);
            }
            // lv_style_set_bg_opa(&ddlist_array_io[i], 255);
            // lv_style_set_bg_color(&ddlist_array_io[i], lv_color_hex(0x007fff));
            // lv_style_set_bg_grad_dir(&ddlist_array_io[i], LV_GRAD_DIR_NONE);

        }
        static lv_style_t style_screen_IO_ddlist_extra_list_scrollbar_default;
        ui_init_style(&style_screen_IO_ddlist_extra_list_scrollbar_default);

        lv_style_set_radius(&style_screen_IO_ddlist_extra_list_scrollbar_default, 0);
        lv_style_set_bg_opa(&style_screen_IO_ddlist_extra_list_scrollbar_default, 255);
        lv_style_set_bg_color(&style_screen_IO_ddlist_extra_list_scrollbar_default, lv_color_hex(0x007fff));
        lv_style_set_bg_grad_dir(&style_screen_IO_ddlist_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
        lv_obj_add_style(lv_dropdown_get_list(ddlist_array_io[i]), &style_screen_IO_ddlist_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    }
    lv_obj_t *label_array_io[] = {
        ui->screen_IO_label_11, ui->screen_IO_label_12, ui->screen_IO_label_13, ui->screen_IO_label_14,
        ui->screen_IO_label_15, ui->screen_IO_label_16, ui->screen_IO_label_17, ui->screen_IO_label_18,
        ui->screen_IO_label_19, ui->screen_IO_label_20, ui->screen_IO_label_21, ui->screen_IO_label_22,
        ui->screen_IO_label_23, ui->screen_IO_label_24, ui->screen_IO_label_25, ui->screen_IO_label_26,
        ui->screen_IO_label_27, ui->screen_IO_label_28, ui->screen_IO_label_29};
    for (int i = 0; i < sizeof(label_array_io) / sizeof(label_array_io[0]); i++)
    {
        lv_obj_set_style_bg_color(label_array_io[i], lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // lv_dropdown_set_selected(ui->screen_IO_ddlist_1, 50);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_11, "上行方向(CUDL):%d.%d", IO_value[0], IO_value[1]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_12, "下行方向(CDDL):%d.%d", IO_value[2], IO_value[3]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_13, "消防信号(FSL):%d.%d", IO_value[4], IO_value[5]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_14, "超载信号(OLS):%d.%d", IO_value[6], IO_value[7]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_15, "断电(ARD):%d.%d", IO_value[8], IO_value[9]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_16, "节能(LR):%d.%d", IO_value[10], IO_value[11]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_17, "复位安抚信号(WTL):%d.%d", IO_value[12], IO_value[13]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_18, "复位救援平层信号(WTL):%d.%d", IO_value[14], IO_value[15]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_19, "满载(LNSL):%d.%d", IO_value[16], IO_value[17]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_20, "复位结束信号(MD_BTN):%d.%d", IO_value[18], IO_value[19]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_21, "蜂鸣信号(BUZ):%d.%d", IO_value[20], IO_value[21]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_22, "开门信号(DO):%d.%d", IO_value[22], IO_value[23]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_23, "运行中上行(M-CUDL):%d.%d", IO_value[24], IO_value[25]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_24, "运行中下行(M-CDDL):%d.%d", IO_value[26], IO_value[27]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_25, "前门上行到层(CDLU):%d.%d", IO_value[28], IO_value[29]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_26, "前门下行到层(CDLD):%d.%d", IO_value[30], IO_value[31]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_27, "后门上行到层(RCDLU):%d.%d", IO_value[32], IO_value[33]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_28, "后门下行到层(RCDLD):%d.%d", IO_value[34], IO_value[35]);
    lv_label_set_text_fmt(guider_ui.screen_IO_label_29, "困人安抚信号():%d.%d", IO_value[36], IO_value[37]);

    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_label_set_text(ui->screen_IO_btn_1_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_2_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_3_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_4_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_5_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_6_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_7_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_8_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_9_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_10_label, "发送");
        lv_label_set_text(ui->screen_IO_btn_12_label, "调用默认");
        lv_label_set_text(ui->screen_IO_btn_return_label, "返回");
    }
    else if (MY_SET.language == LANGUAGE_EN)
    {
        lv_label_set_text(ui->screen_IO_btn_1_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_2_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_3_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_4_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_5_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_6_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_7_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_8_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_9_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_10_label, "OK");
        lv_label_set_text(ui->screen_IO_btn_12_label, "Load Default");
        lv_label_set_text(ui->screen_IO_btn_return_label, "BACK");
    }

    if(set_io_flag[NUM_POWER_OFF])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_15, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_15, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_SLEEP])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_16, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_16, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_RESET_COMFORT])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_17, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_17, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_RESET_RESCUE_LEVEL])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_18, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_18, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_RESET_END])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_20, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_20, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_BUZZER_SIGNAL])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_21, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_21, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_TRAP_COMFORT])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_29, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_29, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_UP_DIRECTION])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_11, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_11, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_DOWN_DIRECTION])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_12, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_12, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_OVERLOAD_SIGNAL])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_14, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_14, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_FIRE_SIGNAL])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_13, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_13, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_FULL_LOAD_SIGNAL])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_19, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_19, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_OPEN_DOOR])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_22, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_22, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_UP_RUNNING])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_23, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_23, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_DOWN_RUNNING])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_24, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_24, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_FRONT_UP_TO_FLOOR])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_25, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_25, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_FRONT_DOWN_TO_FLOOR])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_26, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_26, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_BACK_UP_TO_FLOOR])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_27, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_27, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
if(set_io_flag[NUM_BACK_DOWN_TO_FLOOR])
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_28, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
else
    lv_obj_set_style_bg_color(guider_ui.screen_IO_label_27, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

    // The custom code of screen_IO.
    lv_obj_add_event_cb(ui->screen_IO_btn_1, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_2, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_3, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_4, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_5, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_6, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_7, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_8, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_9, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_10, send_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_IO_btn_12, default_event_handler, LV_EVENT_ALL, ui);
    // Update current screen layout.
    lv_obj_update_layout(ui->screen_IO);

    // Init events for screen.
    events_init_screen_IO(ui);

    if (get_state_timer == NULL)
        get_state_timer = lv_timer_create(get_state_callback, 20, 0);
    else
        lv_timer_resume(get_state_timer);

    my_page = PAGE_IO;
}
