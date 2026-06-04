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

// 处理返回按钮点击事件
static void return_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED)
    {
        setup_scr_screen_set(&guider_ui);
        lv_scr_load_anim(guider_ui.screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
    }
}

static void select_city_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        uint16_t city_3_value = 0;
        uint16_t id = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_1);
        for (int i = 0; i < id; i++)
            city_3_value += city_cnt[i];

        if (target == guider_ui.screen_city_ddlist_1)
        {
            if (MY_SET.language == LANGUAGE_CN)
            {
                lv_dropdown_set_options(guider_ui.screen_city_ddlist_2, my_city_2[id].city_name);
                lv_dropdown_set_options(guider_ui.screen_city_ddlist_3, my_city_3[city_3_value].city_name);
            }else{
                lv_dropdown_set_options(guider_ui.screen_city_ddlist_2, my_city_2_en[id].city_name);
                lv_dropdown_set_options(guider_ui.screen_city_ddlist_3, my_city_3_en[city_3_value].city_name);
            }
        }
        else if (target == guider_ui.screen_city_ddlist_2)
        {
            uint16_t city_2_value = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_2);
            if (MY_SET.language == LANGUAGE_CN)
                lv_dropdown_set_options(guider_ui.screen_city_ddlist_3, my_city_3[city_3_value + city_2_value].city_name);
            else
                lv_dropdown_set_options(guider_ui.screen_city_ddlist_3, my_city_3_en[city_3_value + city_2_value].city_name);
        }
    }
}

static void ok_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    uint8_t city_id[3] = {0};
    char buf[32];
    if (code == LV_EVENT_CLICKED)
    {
        my_city_id.city_1 = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_1);
        my_city_id.city_2 = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_2);
        my_city_id.city_3 = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_3);
        if(city_id[0] != my_city_id.city_1 || city_id[1] != my_city_id.city_2 || city_id[2] != my_city_id.city_3)
        {
            start_set_lwip_flag = true;
            city_id[0] = my_city_id.city_1;
            city_id[1] = my_city_id.city_2;
            city_id[2] = my_city_id.city_3;
        }
        rt_kprintf("my_city_id.city_3 = %d\n", my_city_id.city_3);
        uint16_t city_value = 0;
        for (int i = 0; i < my_city_id.city_1; i++)
            city_value += city_cnt[i];
        my_city_id.city_value = city_value;
        lv_dropdown_get_selected_str(guider_ui.screen_city_ddlist_1, buf, sizeof(buf));
        rt_kprintf("选择的城市是：%s", buf);
        lv_dropdown_get_selected_str(guider_ui.screen_city_ddlist_2, buf, sizeof(buf));
        rt_kprintf("-%s", buf);
        lv_dropdown_get_selected_str(guider_ui.screen_city_ddlist_3, buf, sizeof(buf));
        rt_kprintf("-%s\n", buf);
        rt_kprintf("city_code : %s\n", city_code[my_city_id.city_value +  my_city_id.city_2][my_city_id.city_3]);
        memset(city_name_str, 0, sizeof(city_name_str));
        sprintf(city_name_str,"%s",buf);

        city3_temp = 0;
        uint16_t id = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_1);
        city2_temp = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_2);
        for (int i = 0; i < id; i++)
            city3_temp += city_cnt[i];

        setup_scr_screen_set(&guider_ui);
        lv_scr_load_anim(guider_ui.screen_set, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        save_begin();
    }
}

void setup_scr_screen_city(lv_ui *ui)
{
    //Write codes screen_city
    ui->screen_city = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_city, 768, 1024);
    lv_obj_set_scrollbar_mode(ui->screen_city, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_city, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city, lv_color_hex(0xc3c3c3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_city_label_1
    ui->screen_city_label_1 = lv_label_create(ui->screen_city);
    lv_label_set_text(ui->screen_city_label_1, "选择城市");
    lv_label_set_long_mode(ui->screen_city_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_city_label_1, 232, 14);
    lv_obj_set_size(ui->screen_city_label_1, 283, 56);

    //Write style for screen_city_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_city_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_label_1, &lv_font_Dengb_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_city_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_city_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_city_btn_city
    ui->screen_city_btn_city = lv_btn_create(ui->screen_city);
    ui->screen_city_btn_city_label = lv_label_create(ui->screen_city_btn_city);
    lv_label_set_text(ui->screen_city_btn_city_label, "OK");
    lv_label_set_long_mode(ui->screen_city_btn_city_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_city_btn_city_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_city_btn_city, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_city_btn_city_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_city_btn_city, 634, 2);
    lv_obj_set_size(ui->screen_city_btn_city, 132, 94);

    //Write style for screen_city_btn_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_city_btn_city, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_btn_city, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_btn_city, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_city_btn_city, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_btn_city, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_btn_city, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_btn_city, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_btn_city, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_btn_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_btn_city, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_btn_city, lv_color_hex(0x3453af), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_btn_city, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_btn_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_city_ddlist_1
    ui->screen_city_ddlist_1 = lv_dropdown_create(ui->screen_city);
    lv_dropdown_set_options(ui->screen_city_ddlist_1, "");
    lv_obj_set_pos(ui->screen_city_ddlist_1, 35, 153);
    lv_obj_set_size(ui->screen_city_ddlist_1, 220, 57);

    //Write style for screen_city_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_city_ddlist_1, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_ddlist_1, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_ddlist_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_ddlist_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_city_ddlist_1, 13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_city_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_city_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_city_ddlist_1_extra_list_selected_checked
    static lv_style_t style_screen_city_ddlist_1_extra_list_selected_checked;
    ui_init_style(&style_screen_city_ddlist_1_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_city_ddlist_1_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_city_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_city_ddlist_1_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_city_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_city_ddlist_1_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_city_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_1_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_1), &style_screen_city_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_city_ddlist_1_extra_list_main_default
    static lv_style_t style_screen_city_ddlist_1_extra_list_main_default;
    ui_init_style(&style_screen_city_ddlist_1_extra_list_main_default);

    lv_style_set_max_height(&style_screen_city_ddlist_1_extra_list_main_default, 235);
    lv_style_set_text_color(&style_screen_city_ddlist_1_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_city_ddlist_1_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_city_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_city_ddlist_1_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_city_ddlist_1_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_city_ddlist_1_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_1_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_1), &style_screen_city_ddlist_1_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_city_ddlist_1_extra_list_scrollbar_default
    static lv_style_t style_screen_city_ddlist_1_extra_list_scrollbar_default;
    ui_init_style(&style_screen_city_ddlist_1_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_city_ddlist_1_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_city_ddlist_1_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_1_extra_list_scrollbar_default, lv_color_hex(0xa5b0a5));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_1_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_1), &style_screen_city_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_city_ddlist_2
    ui->screen_city_ddlist_2 = lv_dropdown_create(ui->screen_city);
    lv_dropdown_set_options(ui->screen_city_ddlist_2, "");
    lv_obj_set_pos(ui->screen_city_ddlist_2, 278, 153);
    lv_obj_set_size(ui->screen_city_ddlist_2, 220, 57);

    //Write style for screen_city_ddlist_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_city_ddlist_2, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_ddlist_2, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_ddlist_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_ddlist_2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_ddlist_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_ddlist_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_ddlist_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_city_ddlist_2, 13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_city_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_city_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_ddlist_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_ddlist_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_ddlist_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_ddlist_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_ddlist_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_city_ddlist_2_extra_list_selected_checked
    static lv_style_t style_screen_city_ddlist_2_extra_list_selected_checked;
    ui_init_style(&style_screen_city_ddlist_2_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_city_ddlist_2_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_city_ddlist_2_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_city_ddlist_2_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_city_ddlist_2_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_city_ddlist_2_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_city_ddlist_2_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_2_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_2_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_2), &style_screen_city_ddlist_2_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_city_ddlist_2_extra_list_main_default
    static lv_style_t style_screen_city_ddlist_2_extra_list_main_default;
    ui_init_style(&style_screen_city_ddlist_2_extra_list_main_default);

    lv_style_set_max_height(&style_screen_city_ddlist_2_extra_list_main_default, 235);
    lv_style_set_text_color(&style_screen_city_ddlist_2_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_city_ddlist_2_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_city_ddlist_2_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_city_ddlist_2_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_city_ddlist_2_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_city_ddlist_2_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_2_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_2_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_2), &style_screen_city_ddlist_2_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_city_ddlist_2_extra_list_scrollbar_default
    static lv_style_t style_screen_city_ddlist_2_extra_list_scrollbar_default;
    ui_init_style(&style_screen_city_ddlist_2_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_city_ddlist_2_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_city_ddlist_2_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_2_extra_list_scrollbar_default, lv_color_hex(0xa5b0a5));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_2_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_2), &style_screen_city_ddlist_2_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_city_ddlist_3
    ui->screen_city_ddlist_3 = lv_dropdown_create(ui->screen_city);
    lv_dropdown_set_options(ui->screen_city_ddlist_3, "");
    lv_obj_set_pos(ui->screen_city_ddlist_3, 520, 153);
    lv_obj_set_size(ui->screen_city_ddlist_3, 220, 57);

    //Write style for screen_city_ddlist_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_city_ddlist_3, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_ddlist_3, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_ddlist_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_ddlist_3, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_ddlist_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_ddlist_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_ddlist_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_city_ddlist_3, 13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_city_ddlist_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_city_ddlist_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_ddlist_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_ddlist_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_ddlist_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_ddlist_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_ddlist_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_city_ddlist_3_extra_list_selected_checked
    static lv_style_t style_screen_city_ddlist_3_extra_list_selected_checked;
    ui_init_style(&style_screen_city_ddlist_3_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_city_ddlist_3_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_city_ddlist_3_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_city_ddlist_3_extra_list_selected_checked, lv_color_hex(0x0040ff));
    lv_style_set_border_side(&style_screen_city_ddlist_3_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_city_ddlist_3_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_city_ddlist_3_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_3_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_3_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_3), &style_screen_city_ddlist_3_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_city_ddlist_3_extra_list_main_default
    static lv_style_t style_screen_city_ddlist_3_extra_list_main_default;
    ui_init_style(&style_screen_city_ddlist_3_extra_list_main_default);

    lv_style_set_max_height(&style_screen_city_ddlist_3_extra_list_main_default, 235);
    lv_style_set_text_color(&style_screen_city_ddlist_3_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_city_ddlist_3_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_city_ddlist_3_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_city_ddlist_3_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_city_ddlist_3_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_city_ddlist_3_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_3_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_3_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_3), &style_screen_city_ddlist_3_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_city_ddlist_3_extra_list_scrollbar_default
    static lv_style_t style_screen_city_ddlist_3_extra_list_scrollbar_default;
    ui_init_style(&style_screen_city_ddlist_3_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_city_ddlist_3_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_city_ddlist_3_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_city_ddlist_3_extra_list_scrollbar_default, lv_color_hex(0xa5b0a5));
    lv_style_set_bg_grad_dir(&style_screen_city_ddlist_3_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_ddlist_3), &style_screen_city_ddlist_3_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_city_btn_return
    ui->screen_city_btn_return = lv_btn_create(ui->screen_city);
    ui->screen_city_btn_return_label = lv_label_create(ui->screen_city_btn_return);
    lv_label_set_text(ui->screen_city_btn_return_label, "返回");
    lv_label_set_long_mode(ui->screen_city_btn_return_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_city_btn_return_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_city_btn_return, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_city_btn_return_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_city_btn_return, 302, 959);
    lv_obj_set_size(ui->screen_city_btn_return, 154, 53);

    //Write style for screen_city_btn_return, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_city_btn_return, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_btn_return, lv_color_hex(0xff0027), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_btn_return, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_btn_return, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_btn_return, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_btn_return, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_city_btn_return, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_btn_return, &lv_font_Dengb_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_btn_return, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_city_btn_return, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_city.
    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_label_set_text(ui->screen_city_btn_city_label, "确定");
        lv_label_set_text(ui->screen_city_label_1, "选择城市");
    }
    else
    {
        lv_label_set_text(ui->screen_city_btn_city_label, "OK");
        lv_label_set_text(ui->screen_city_label_1, "SELECT CITY");
    }
    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_dropdown_set_options(ui->screen_city_ddlist_1, city_province);
        lv_dropdown_set_options(ui->screen_city_ddlist_2, my_city_2[my_city_id.city_1].city_name);
    }else{
        lv_dropdown_set_options(ui->screen_city_ddlist_1, city_province_en);
        lv_dropdown_set_options(ui->screen_city_ddlist_2, my_city_2_en[my_city_id.city_1].city_name);
    }

    lv_dropdown_set_selected(guider_ui.screen_city_ddlist_1, my_city_id.city_1);
    lv_dropdown_set_selected(guider_ui.screen_city_ddlist_2, my_city_id.city_2);

    uint16_t city_3_value = 0;
    uint16_t id = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_1);
    uint16_t city_2_value = lv_dropdown_get_selected(guider_ui.screen_city_ddlist_2);
    for (int i = 0; i < id; i++)
        city_3_value += city_cnt[i];
    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_dropdown_set_options(ui->screen_city_ddlist_3, my_city_3[city_3_value + city_2_value].city_name);
    }else
        lv_dropdown_set_options(ui->screen_city_ddlist_3, my_city_3_en[city_3_value + city_2_value].city_name);
    lv_dropdown_set_selected(guider_ui.screen_city_ddlist_3, my_city_id.city_3);

    //
    lv_obj_clear_flag(ui->screen_city, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui->screen_city_ddlist_1, select_city_handler, LV_EVENT_VALUE_CHANGED, ui);
    lv_obj_add_event_cb(ui->screen_city_ddlist_2, select_city_handler, LV_EVENT_VALUE_CHANGED, ui);
    lv_obj_add_event_cb(ui->screen_city_btn_return, return_handler, LV_EVENT_CLICKED, ui);
    lv_obj_add_event_cb(ui->screen_city_btn_city, ok_handler, LV_EVENT_CLICKED, ui);
    //Update current screen layout.
    lv_obj_update_layout(ui->screen_city);
    my_page = PAGE_CITY;
}
