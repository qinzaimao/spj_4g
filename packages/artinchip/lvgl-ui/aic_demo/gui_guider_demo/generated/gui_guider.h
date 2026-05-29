/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

	typedef struct
	{

		lv_obj_t *screen_home;
		bool screen_home_del;
		lv_obj_t *screen_home_img_background;
		lv_obj_t *screen_home_img_init;
		lv_obj_t *screen_home_label_shade_down;
		lv_obj_t *screen_home_img_shade_left;
		lv_obj_t *screen_home_img_shade_up;
		lv_obj_t *screen_home_label_fire;
		lv_obj_t *screen_home_label_fire_401;
		lv_obj_t *screen_home_label_fire_402;
		lv_obj_t *screen_home_label_overload;
		lv_obj_t *screen_home_label_overload_401;
		lv_obj_t *screen_home_label_overload_402;
		lv_obj_t *screen_home_cont_1;
		lv_obj_t *screen_home_img_img;
		lv_obj_t *screen_home_img_arrow_xio;
		lv_obj_t *screen_home_img_arrow_swd;
		lv_obj_t *screen_home_img_logo_swd;
		lv_obj_t *screen_home_img_logo_xio;
		lv_obj_t *screen_home_label_welecom;
		lv_obj_t *screen_home_label_date;
		lv_obj_t *screen_home_label_update;
		lv_obj_t *screen_home_label_time;
		lv_obj_t *screen_home_img_num1;
		lv_obj_t *screen_home_img_num2;
		lv_obj_t *screen_home_img_num3;
		lv_obj_t *screen_home_img_smoke;
		lv_obj_t *screen_home_tileview_1;
		lv_obj_t *screen_home_tileview_1_tile;
		lv_obj_t *screen_home_label_temperature;
		lv_obj_t *screen_home_img_weather;
		lv_obj_t *screen_home_label_weather_erro;

		lv_obj_t *screen_set;
		bool screen_set_del;
		lv_obj_t *screen_set_label_1;
		lv_obj_t *screen_set_label_2;
		lv_obj_t *screen_set_label_3;
		lv_obj_t *screen_set_label_4;
		lv_obj_t *screen_set_label_5;
		lv_obj_t *screen_set_label_6;
		lv_obj_t *screen_set_label_7;
		lv_obj_t *screen_set_label_8;
		lv_obj_t *screen_set_label_9;
		lv_obj_t *screen_set_label_11;
		lv_obj_t *screen_set_label_12;
		lv_obj_t *screen_set_btn_IO;
		lv_obj_t *screen_set_btn_IO_label;
		lv_obj_t *screen_set_btn_voice;
		lv_obj_t *screen_set_btn_voice_label;
		lv_obj_t *screen_set_btn_return;
		lv_obj_t *screen_set_btn_return_label;
		lv_obj_t *screen_set_btn_restart_program;
		lv_obj_t *screen_set_btn_restart_program_label;
		lv_obj_t *screen_set_btn_restart_system;
		lv_obj_t *screen_set_btn_restart_system_label;
		lv_obj_t *screen_set_ddlist_language;
		lv_obj_t *screen_set_btn_language;
		lv_obj_t *screen_set_btn_language_label;
		lv_obj_t *screen_set_btn_play;
		lv_obj_t *screen_set_btn_play_label;
		lv_obj_t *screen_set_ddlist_mode;
		lv_obj_t *screen_set_slider_normal_brightness;
		lv_obj_t *screen_set_btn_interface;
		lv_obj_t *screen_set_btn_interface_label;
		lv_obj_t *screen_set_btn_arrowhead;
		lv_obj_t *screen_set_btn_arrowhead_label;
		lv_obj_t *screen_set_btn_time;
		lv_obj_t *screen_set_btn_time_label;
		lv_obj_t *screen_set_btn_city;
		lv_obj_t *screen_set_btn_city_label;
		lv_obj_t *screen_set_btn_restart_time;
		lv_obj_t *screen_set_btn_restart_time_label;
		lv_obj_t *screen_set_ddlist_restart_time;
		lv_obj_t *screen_set_ddlist_city;
		lv_obj_t *screen_set_ddlist_interface;
		lv_obj_t *screen_set_ddlist_arrowhead;
		lv_obj_t *screen_set_ddlist_logo;
		lv_obj_t *screen_set_ddlist_time_year;
		lv_obj_t *screen_set_ddlist_time_hour;
		lv_obj_t *screen_set_ddlist_time_month;
		lv_obj_t *screen_set_ddlist_time_minute;
		lv_obj_t *screen_set_ddlist_time_day;
		lv_obj_t *screen_set_slider_volume;
		lv_obj_t *screen_set_slider_saving_brightness;
		lv_obj_t *screen_set_ddlist_time_second;
		lv_obj_t *screen_set_label_13;
		lv_obj_t *screen_set_btn_ip;
		lv_obj_t *screen_set_btn_ip_label;
		lv_obj_t *screen_set_btn_dhcp;
		lv_obj_t *screen_set_btn_dhcp_label;
		lv_obj_t *screen_set_ddlist_ip1;
		lv_obj_t *screen_set_ddlist_ip2;
		lv_obj_t *screen_set_ddlist_ip3;
		lv_obj_t *screen_set_ddlist_ip4;
		lv_obj_t *screen_set_ddlist_ip5;
		lv_obj_t *screen_set_ddlist_ip6;
		lv_obj_t *screen_set_ddlist_ip7;
		lv_obj_t *screen_set_ddlist_ip8;
		lv_obj_t *screen_set_ddlist_ip9;
		lv_obj_t *screen_set_ddlist_ip10;
		lv_obj_t *screen_set_ddlist_ip11;
		lv_obj_t *screen_set_ddlist_ip12;
		lv_obj_t *screen_set_ddlist_ip13;
		lv_obj_t *screen_set_ddlist_ip14;
		lv_obj_t *screen_set_ddlist_ip15;
		lv_obj_t *screen_set_ddlist_ip16;
		lv_obj_t *screen_set_btn_select_city;
		lv_obj_t *screen_set_btn_select_city_label;
		lv_obj_t *screen_set_tileview_ip;
		lv_obj_t *screen_set_tileview_ip_tile_ip;
		lv_obj_t *screen_set_label_save_ip;

		lv_obj_t *screen_set_tileview_key;
		lv_obj_t *screen_set_tileview_key_tile_key;
		lv_obj_t *screen_set_label_key;
		lv_obj_t *screen_set_btnm_key;
		lv_obj_t *screen_set_ta_key;
		lv_obj_t *screen_set_btn_back;
		lv_obj_t *screen_set_btn_back_label;
		lv_obj_t *screen_set_label_uart_v;

		lv_obj_t *screen_voice;
		bool screen_voice_del;
		lv_obj_t *screen_voice_label_1;
		lv_obj_t *screen_voice_label_2;
		lv_obj_t *screen_voice_label_3;
		lv_obj_t *screen_voice_label_4;
		lv_obj_t *screen_voice_label_5;
		lv_obj_t *screen_voice_label_6;
		lv_obj_t *screen_voice_btn_return;
		lv_obj_t *screen_voice_btn_return_label;
		lv_obj_t *screen_voice_btn_password;
		lv_obj_t *screen_voice_btn_password_label;
		lv_obj_t *screen_voice_btn_music;
		lv_obj_t *screen_voice_btn_music_label;
		lv_obj_t *screen_voice_btn_music1;
		lv_obj_t *screen_voice_btn_music1_label;
		lv_obj_t *screen_voice_btn_music3;
		lv_obj_t *screen_voice_btn_music3_label;
		lv_obj_t *screen_voice_btn_music2;
		lv_obj_t *screen_voice_btn_music2_label;
		lv_obj_t *screen_voice_ddlist_language;
		lv_obj_t *screen_voice_ddlist_work_over;
		lv_obj_t *screen_voice_ddlist_work_start;
		lv_obj_t *screen_voice_btn_language;
		lv_obj_t *screen_voice_btn_language_label;
		lv_obj_t *screen_voice_btn_work_time;
		lv_obj_t *screen_voice_btn_work_time_label;
		lv_obj_t *screen_voice_slider_voice;
		lv_obj_t *screen_voice_slider_lr;
		lv_obj_t *screen_voice_slider_arrival;
		lv_obj_t *screen_voice_slider_close;
		lv_obj_t *screen_voice_slider_broadcast;
		lv_obj_t *screen_voice_slider_firefighting;
		lv_obj_t *screen_voice_slider_overload;
		lv_obj_t *screen_voice_slider_door;
		lv_obj_t *screen_voice_slider_peak;
		lv_obj_t *screen_voice_slider_link;
		lv_obj_t *screen_voice_slider_appease;
		lv_obj_t *screen_voice_btn_voice;
		lv_obj_t *screen_voice_btn_voice_label;
		lv_obj_t *screen_voice_btn_lr;
		lv_obj_t *screen_voice_btn_lr_label;
		lv_obj_t *screen_voice_btn_close;
		lv_obj_t *screen_voice_btn_close_label;
		lv_obj_t *screen_voice_btn_arrival;
		lv_obj_t *screen_voice_btn_arrival_label;
		lv_obj_t *screen_voice_btn_broadcast;
		lv_obj_t *screen_voice_btn_broadcast_label;
		lv_obj_t *screen_voice_btn_firefighting;
		lv_obj_t *screen_voice_btn_firefighting_label;
		lv_obj_t *screen_voice_btn_overload;
		lv_obj_t *screen_voice_btn_overload_label;
		lv_obj_t *screen_voice_btn_door;
		lv_obj_t *screen_voice_btn_door_label;
		lv_obj_t *screen_voice_btn_link;
		lv_obj_t *screen_voice_btn_link_label;
		lv_obj_t *screen_voice_btn_peak;
		lv_obj_t *screen_voice_btn_peak_label;
		lv_obj_t *screen_voice_btn_appease;
		lv_obj_t *screen_voice_btn_appease_label;
		lv_obj_t *screen_voice_tileview_key;
		lv_obj_t *screen_voice_tileview_key_tile_key;
		lv_obj_t *screen_voice_label_key;
		lv_obj_t *screen_voice_btn_back;
		lv_obj_t *screen_voice_btn_back_label;
		lv_obj_t *screen_voice_btnm_key;
		lv_obj_t *screen_voice_ta_key;
		lv_obj_t *screen_IO;
		bool screen_IO_del;
		lv_obj_t *screen_IO_label_1;
		lv_obj_t *screen_IO_label_2;
		lv_obj_t *screen_IO_label_3;
		lv_obj_t *screen_IO_label_4;
		lv_obj_t *screen_IO_label_5;
		lv_obj_t *screen_IO_label_6;
		lv_obj_t *screen_IO_label_7;
		lv_obj_t *screen_IO_label_8;
		lv_obj_t *screen_IO_label_9;
		lv_obj_t *screen_IO_label_10;
		lv_obj_t *screen_IO_btn_1;
		lv_obj_t *screen_IO_btn_1_label;
		lv_obj_t *screen_IO_label_11;
		lv_obj_t *screen_IO_label_12;
		lv_obj_t *screen_IO_ddlist_1;
		lv_obj_t *screen_IO_ddlist_2;
		lv_obj_t *screen_IO_ddlist_3;
		lv_obj_t *screen_IO_ddlist_4;
		lv_obj_t *screen_IO_btn_2;
		lv_obj_t *screen_IO_btn_2_label;
		lv_obj_t *screen_IO_label_13;
		lv_obj_t *screen_IO_label_14;
		lv_obj_t *screen_IO_ddlist_5;
		lv_obj_t *screen_IO_ddlist_6;
		lv_obj_t *screen_IO_ddlist_7;
		lv_obj_t *screen_IO_ddlist_8;
		lv_obj_t *screen_IO_btn_3;
		lv_obj_t *screen_IO_btn_3_label;
		lv_obj_t *screen_IO_label_15;
		lv_obj_t *screen_IO_ddlist_9;
		lv_obj_t *screen_IO_ddlist_10;
		lv_obj_t *screen_IO_label_16;
		lv_obj_t *screen_IO_ddlist_11;
		lv_obj_t *screen_IO_ddlist_12;
		lv_obj_t *screen_IO_btn_4;
		lv_obj_t *screen_IO_btn_4_label;
		lv_obj_t *screen_IO_label_17;
		lv_obj_t *screen_IO_label_18;
		lv_obj_t *screen_IO_ddlist_13;
		lv_obj_t *screen_IO_ddlist_14;
		lv_obj_t *screen_IO_ddlist_15;
		lv_obj_t *screen_IO_ddlist_16;
		lv_obj_t *screen_IO_label_19;
		lv_obj_t *screen_IO_btn_5;
		lv_obj_t *screen_IO_btn_5_label;
		lv_obj_t *screen_IO_label_20;
		lv_obj_t *screen_IO_ddlist_17;
		lv_obj_t *screen_IO_ddlist_18;
		lv_obj_t *screen_IO_ddlist_19;
		lv_obj_t *screen_IO_ddlist_20;
		lv_obj_t *screen_IO_btn_6;
		lv_obj_t *screen_IO_btn_6_label;
		lv_obj_t *screen_IO_label_21;
		lv_obj_t *screen_IO_label_22;
		lv_obj_t *screen_IO_ddlist_21;
		lv_obj_t *screen_IO_ddlist_22;
		lv_obj_t *screen_IO_ddlist_23;
		lv_obj_t *screen_IO_ddlist_24;
		lv_obj_t *screen_IO_btn_7;
		lv_obj_t *screen_IO_btn_7_label;
		lv_obj_t *screen_IO_label_23;
		lv_obj_t *screen_IO_label_24;
		lv_obj_t *screen_IO_ddlist_25;
		lv_obj_t *screen_IO_ddlist_26;
		lv_obj_t *screen_IO_ddlist_27;
		lv_obj_t *screen_IO_ddlist_28;
		lv_obj_t *screen_IO_label_25;
		lv_obj_t *screen_IO_btn_8;
		lv_obj_t *screen_IO_btn_8_label;
		lv_obj_t *screen_IO_label_26;
		lv_obj_t *screen_IO_ddlist_29;
		lv_obj_t *screen_IO_ddlist_30;
		lv_obj_t *screen_IO_ddlist_31;
		lv_obj_t *screen_IO_ddlist_32;
		lv_obj_t *screen_IO_btn_9;
		lv_obj_t *screen_IO_btn_9_label;
		lv_obj_t *screen_IO_label_27;
		lv_obj_t *screen_IO_label_28;
		lv_obj_t *screen_IO_ddlist_33;
		lv_obj_t *screen_IO_ddlist_34;
		lv_obj_t *screen_IO_ddlist_35;
		lv_obj_t *screen_IO_ddlist_36;
		lv_obj_t *screen_IO_btn_10;
		lv_obj_t *screen_IO_btn_10_label;
		lv_obj_t *screen_IO_label_29;
		lv_obj_t *screen_IO_ddlist_37;
		lv_obj_t *screen_IO_ddlist_38;
		lv_obj_t *screen_IO_ddlist_39;
		lv_obj_t *screen_IO_ddlist_40;
		lv_obj_t *screen_IO_btn_return;
		lv_obj_t *screen_IO_btn_return_label;
		lv_obj_t *screen_IO_btn_12;
		lv_obj_t *screen_IO_btn_12_label;
		lv_obj_t *screen_IO_label_c1;
		lv_obj_t *screen_IO_label_c2;
		lv_obj_t *screen_IO_label_c3;
		lv_obj_t *screen_IO_label_c4;
		lv_obj_t *screen_IO_label_c5;
		lv_obj_t *screen_IO_label_c6;
		lv_obj_t *screen_IO_label_c7;
		lv_obj_t *screen_IO_label_c8;
		lv_obj_t *screen_IO_label_c9;
		lv_obj_t *screen_IO_label_c10;
		lv_obj_t *screen_IO_label_c11;
		lv_obj_t *screen_IO_label_c12;
		lv_obj_t *screen_IO_label_c13;
		lv_obj_t *screen_IO_label_c14;
		lv_obj_t *screen_IO_label_c15;
		lv_obj_t *screen_IO_label_c16;
		lv_obj_t *screen_IO_label_c17;
		lv_obj_t *screen_IO_label_c18;
		lv_obj_t *screen_IO_label_c19;
		lv_obj_t *screen_IO_label_c20;
		lv_obj_t *screen_IO_label_c21;
		lv_obj_t *screen_IO_label_c22;
		lv_obj_t *screen_IO_label_c23;
		lv_obj_t *screen_IO_label_c24;
		lv_obj_t *screen_IO_label_c25;
		lv_obj_t *screen_IO_label_c26;
		lv_obj_t *screen_IO_label_c27;
		lv_obj_t *screen_IO_label_c28;
		lv_obj_t *screen_IO_label_c29;
		lv_obj_t *screen_IO_label_c30;
		lv_obj_t *screen_IO_label_c31;
		lv_obj_t *screen_IO_label_c32;
		lv_obj_t *screen_IO_label_c33;
		lv_obj_t *screen_IO_label_c34;
		lv_obj_t *screen_IO_label_c35;
		lv_obj_t *screen_IO_label_c36;
		lv_obj_t *screen_IO_label_c37;
		lv_obj_t *screen_IO_label_c38;
		lv_obj_t *screen_IO_label_c39;
		lv_obj_t *screen_IO_label_c40;

		lv_obj_t *screen_city;
		bool screen_city_del;
		lv_obj_t *screen_city_label_1;
		lv_obj_t *screen_city_btn_city;
		lv_obj_t *screen_city_btn_city_label;
		lv_obj_t *screen_city_ddlist_1;
		lv_obj_t *screen_city_ddlist_2;
		lv_obj_t *screen_city_ddlist_3;
		lv_obj_t *screen_city_btn_return;
		lv_obj_t *screen_city_btn_return_label;

		lv_obj_t *screen_home_hor;
		bool screen_home_hor_del;
		lv_obj_t *screen_home_hor_img_gif_swd;
		lv_obj_t *screen_home_hor_img_init;
		lv_obj_t *screen_home_hor_img_init_logo;
		lv_obj_t *screen_home_hor_img_weather;
		lv_obj_t *screen_home_hor_label_overload;
		lv_obj_t *screen_home_hor_img_background;
		lv_obj_t *screen_home_hor_label_overload_401;
		lv_obj_t *screen_home_hor_label_overload_403;
		lv_obj_t *screen_home_hor_label_temperature;
		lv_obj_t *screen_home_hor_label_fire;
		lv_obj_t *screen_home_hor_label_fire_401;
		lv_obj_t *screen_home_hor_label_fire_403;
		lv_obj_t *screen_home_hor_label_square;
		lv_obj_t *screen_home_hor_label_video;

		lv_obj_t *screen_home_hor_img_arrow_xio;
		lv_obj_t *screen_home_hor_img_shade_up;
		lv_obj_t *screen_home_hor_img_shade_left;
		lv_obj_t *screen_home_hor_label_shade_down;
		lv_obj_t *screen_home_hor_img_arrow_swd;
		lv_obj_t *screen_home_hor_img_num1;
		lv_obj_t *screen_home_hor_img_num2;
		lv_obj_t *screen_home_hor_img_num3;
		lv_obj_t *screen_home_hor_label_date;
		lv_obj_t *screen_home_hor_label_C201;
		lv_obj_t *screen_home_hor_label_date_c3;
		lv_obj_t *screen_home_hor_label_welecom;
		lv_obj_t *screen_home_hor_label_201_welcome;
		lv_obj_t *screen_home_hor_label_time;
		lv_obj_t *screen_home_hor_label_update;
		lv_obj_t *screen_home_hor_img_logo_xio;
		lv_obj_t *screen_home_hor_img_logo_swd;
		lv_obj_t *screen_home_hor_img_smoke;
		lv_obj_t *screen_home_hor_img_img;
		lv_obj_t *screen_home_hor_img_gif;
		lv_obj_t *screen_home_hor_label_weather_erro;
		lv_obj_t *screen_set_hor;
		bool screen_set_hor_del;
		lv_obj_t *screen_set_hor_label_1;
		lv_obj_t *screen_set_hor_label_2;
		lv_obj_t *screen_set_hor_label_3;
		lv_obj_t *screen_set_hor_label_4;
		lv_obj_t *screen_set_hor_label_5;
		lv_obj_t *screen_set_hor_label_6;
		lv_obj_t *screen_set_hor_label_7;
		lv_obj_t *screen_set_hor_label_8;
		lv_obj_t *screen_set_hor_label_9;
		lv_obj_t *screen_set_hor_label_10;
		lv_obj_t *screen_set_hor_label_11;
		lv_obj_t *screen_set_hor_label_12;
		lv_obj_t *screen_set_hor_btn_language;
		lv_obj_t *screen_set_hor_btn_select_city;
		lv_obj_t *screen_set_hor_btn_select_city_label;
		lv_obj_t *screen_set_hor_btn_language_label;
		lv_obj_t *screen_set_hor_ddlist_language;
		lv_obj_t *screen_set_hor_slider_volume;
		lv_obj_t *screen_set_hor_btn_play;
		lv_obj_t *screen_set_hor_btn_play_label;
		lv_obj_t *screen_set_hor_ddlist_mode;
		lv_obj_t *screen_set_hor_slider_normal_brightness;
		lv_obj_t *screen_set_hor_slider_saving_brightness;
		lv_obj_t *screen_set_hor_btn_interface;
		lv_obj_t *screen_set_hor_btn_interface_label;
		lv_obj_t *screen_set_hor_ddlist_interface;
		lv_obj_t *screen_set_hor_btn_arrowhead;
		lv_obj_t *screen_set_hor_btn_arrowhead_label;
		lv_obj_t *screen_set_hor_ddlist_arrowhead;
		lv_obj_t *screen_set_hor_ddlist_logo;
		lv_obj_t *screen_set_hor_btn_time;
		lv_obj_t *screen_set_hor_btn_time_label;
		lv_obj_t *screen_set_hor_ddlist_time_year;
		lv_obj_t *screen_set_hor_ddlist_time_hour;
		lv_obj_t *screen_set_hor_ddlist_time_month;
		lv_obj_t *screen_set_hor_ddlist_time_minute;
		lv_obj_t *screen_set_hor_ddlist_time_day;
		lv_obj_t *screen_set_hor_ddlist_time_second;
		lv_obj_t *screen_set_hor_btn_city;
		lv_obj_t *screen_set_hor_btn_city_label;
		lv_obj_t *screen_set_hor_ddlist_city;
		lv_obj_t *screen_set_hor_btn_restart_time;
		lv_obj_t *screen_set_hor_btn_restart_time_label;
		lv_obj_t *screen_set_hor_ddlist_restart_time;
		lv_obj_t *screen_set_hor_btn_IO;
		lv_obj_t *screen_set_hor_btn_IO_label;
		lv_obj_t *screen_set_hor_btn_voice;
		lv_obj_t *screen_set_hor_btn_voice_label;
		lv_obj_t *screen_set_hor_btn_retur;
		lv_obj_t *screen_set_hor_btn_retur_label;
		lv_obj_t *screen_set_hor_btn_restart_program;
		lv_obj_t *screen_set_hor_btn_restart_program_label;
		lv_obj_t *screen_set_hor_btn_restart_system;
		lv_obj_t *screen_set_hor_btn_restart_system_label;
		lv_obj_t *screen_set_hor_ddlist_ip1;
		lv_obj_t *screen_set_hor_ddlist_ip2;
		lv_obj_t *screen_set_hor_ddlist_ip3;
		lv_obj_t *screen_set_hor_ddlist_ip4;
		lv_obj_t *screen_set_hor_ddlist_ip5;
		lv_obj_t *screen_set_hor_ddlist_ip6;
		lv_obj_t *screen_set_hor_ddlist_ip7;
		lv_obj_t *screen_set_hor_ddlist_ip8;
		lv_obj_t *screen_set_hor_ddlist_ip9;
		lv_obj_t *screen_set_hor_ddlist_ip10;
		lv_obj_t *screen_set_hor_ddlist_ip11;
		lv_obj_t *screen_set_hor_ddlist_ip12;
		lv_obj_t *screen_set_hor_ddlist_ip13;
		lv_obj_t *screen_set_hor_ddlist_ip14;
		lv_obj_t *screen_set_hor_ddlist_ip15;
		lv_obj_t *screen_set_hor_ddlist_ip16;
		lv_obj_t *screen_set_hor_btn_dhcp;
		lv_obj_t *screen_set_hor_btn_dhcp_label;
		lv_obj_t *screen_set_hor_btn_ip;
		lv_obj_t *screen_set_hor_btn_ip_label;
		lv_obj_t *screen_set_hor_tileview_key;
		lv_obj_t *screen_set_hor_tileview_key_tile_key;
		lv_obj_t *screen_set_hor_label_key;
		lv_obj_t *screen_set_hor_btnm_key;
		lv_obj_t *screen_set_hor_btn_back_label;
		lv_obj_t *screen_set_hor_btn_back;
		lv_obj_t *screen_set_hor_ta_key;
		lv_obj_t *screen_set_hor_label_uart_v;
		lv_obj_t *screen_set_hor_tileview_ip;
		lv_obj_t *screen_set_hor_tileview_ip_tile_ip;
		lv_obj_t *screen_set_hor_label_save_ip;

		lv_obj_t *screen_voice_hor;
		bool screen_voice_hor_del;
		lv_obj_t *screen_voice_hor_label_1;
		lv_obj_t *screen_voice_hor_label_2;
		lv_obj_t *screen_voice_hor_label_3;
		lv_obj_t *screen_voice_hor_label_4;
		lv_obj_t *screen_voice_hor_label_5;
		lv_obj_t *screen_voice_hor_label_6;
		lv_obj_t *screen_voice_hor_ddlist_work_start;
		lv_obj_t *screen_voice_hor_ddlist_work_over;
		lv_obj_t *screen_voice_hor_ddlist_language;
		lv_obj_t *screen_voice_hor_slider_voice;
		lv_obj_t *screen_voice_hor_btn_voice;
		lv_obj_t *screen_voice_hor_btn_voice_label;
		lv_obj_t *screen_voice_hor_slider_lr;
		lv_obj_t *screen_voice_hor_btn_lr;
		lv_obj_t *screen_voice_hor_btn_lr_label;
		lv_obj_t *screen_voice_hor_slider_close;
		lv_obj_t *screen_voice_hor_btn_close;
		lv_obj_t *screen_voice_hor_btn_close_label;
		lv_obj_t *screen_voice_hor_slider_arrival;
		lv_obj_t *screen_voice_hor_btn_arrival;
		lv_obj_t *screen_voice_hor_btn_arrival_label;
		lv_obj_t *screen_voice_hor_slider_broadcast;
		lv_obj_t *screen_voice_hor_btn_broadcast;
		lv_obj_t *screen_voice_hor_btn_broadcast_label;
		lv_obj_t *screen_voice_hor_slider_firefighting;
		lv_obj_t *screen_voice_hor_btn_firefighting;
		lv_obj_t *screen_voice_hor_btn_firefighting_label;
		lv_obj_t *screen_voice_hor_slider_overload;
		lv_obj_t *screen_voice_hor_btn_overload;
		lv_obj_t *screen_voice_hor_btn_overload_label;
		lv_obj_t *screen_voice_hor_slider_door;
		lv_obj_t *screen_voice_hor_btn_door;
		lv_obj_t *screen_voice_hor_btn_door_label;
		lv_obj_t *screen_voice_hor_slider_link;
		lv_obj_t *screen_voice_hor_btn_link;
		lv_obj_t *screen_voice_hor_btn_link_label;
		lv_obj_t *screen_voice_hor_slider_peak;
		lv_obj_t *screen_voice_hor_btn_peak;
		lv_obj_t *screen_voice_hor_btn_peak_label;
		lv_obj_t *screen_voice_hor_slider_appease;
		lv_obj_t *screen_voice_hor_btn_appease;
		lv_obj_t *screen_voice_hor_btn_appease_label;
		lv_obj_t *screen_voice_hor_btn_work_time;
		lv_obj_t *screen_voice_hor_btn_work_time_label;
		lv_obj_t *screen_voice_hor_btn_language;
		lv_obj_t *screen_voice_hor_btn_language_label;
		lv_obj_t *screen_voice_hor_btn_music;
		lv_obj_t *screen_voice_hor_btn_music_label;
		lv_obj_t *screen_voice_hor_btn_music1;
		lv_obj_t *screen_voice_hor_btn_music1_label;
		lv_obj_t *screen_voice_hor_btn_music2;
		lv_obj_t *screen_voice_hor_btn_music2_label;
		lv_obj_t *screen_voice_hor_btn_music3;
		lv_obj_t *screen_voice_hor_btn_music3_label;
		lv_obj_t *screen_voice_hor_btn_return;
		lv_obj_t *screen_voice_hor_btn_return_label;
		lv_obj_t *screen_voice_hor_btn_password;
		lv_obj_t *screen_voice_hor_btn_password_label;
		lv_obj_t *screen_voice_hor_tileview_key;
		lv_obj_t *screen_voice_hor_tileview_key_tile_key;
		lv_obj_t *screen_voice_hor_btn_back;
		lv_obj_t *screen_voice_hor_btn_back_label;
		lv_obj_t *screen_voice_hor_label_key;
		lv_obj_t *screen_voice_hor_btnm_key;
		lv_obj_t *screen_voice_hor_ta_key;
		lv_obj_t *screen_IO_hor;
		bool screen_IO_hor_del;
		lv_obj_t *screen_IO_hor_label_1;
		lv_obj_t *screen_IO_hor_label_2;
		lv_obj_t *screen_IO_hor_label_3;
		lv_obj_t *screen_IO_hor_label_4;
		lv_obj_t *screen_IO_hor_label_5;
		lv_obj_t *screen_IO_hor_label_6;
		lv_obj_t *screen_IO_hor_label_7;
		lv_obj_t *screen_IO_hor_label_8;
		lv_obj_t *screen_IO_hor_label_9;
		lv_obj_t *screen_IO_hor_label_10;
		lv_obj_t *screen_IO_hor_label_11;
		lv_obj_t *screen_IO_hor_label_12;
		lv_obj_t *screen_IO_hor_label_13;
		lv_obj_t *screen_IO_hor_label_14;
		lv_obj_t *screen_IO_hor_label_15;
		lv_obj_t *screen_IO_hor_label_16;
		lv_obj_t *screen_IO_hor_label_17;
		lv_obj_t *screen_IO_hor_label_18;
		lv_obj_t *screen_IO_hor_label_19;
		lv_obj_t *screen_IO_hor_label_20;
		lv_obj_t *screen_IO_hor_label_21;
		lv_obj_t *screen_IO_hor_label_22;
		lv_obj_t *screen_IO_hor_label_23;
		lv_obj_t *screen_IO_hor_label_24;
		lv_obj_t *screen_IO_hor_label_25;
		lv_obj_t *screen_IO_hor_label_26;
		lv_obj_t *screen_IO_hor_label_27;
		lv_obj_t *screen_IO_hor_label_28;
		lv_obj_t *screen_IO_hor_label_29;
		lv_obj_t *screen_IO_hor_btn_1;
		lv_obj_t *screen_IO_hor_btn_1_label;
		lv_obj_t *screen_IO_hor_btn_2;
		lv_obj_t *screen_IO_hor_btn_2_label;
		lv_obj_t *screen_IO_hor_btn_3;
		lv_obj_t *screen_IO_hor_btn_3_label;
		lv_obj_t *screen_IO_hor_btn_4;
		lv_obj_t *screen_IO_hor_btn_4_label;
		lv_obj_t *screen_IO_hor_btn_5;
		lv_obj_t *screen_IO_hor_btn_5_label;
		lv_obj_t *screen_IO_hor_btn_6;
		lv_obj_t *screen_IO_hor_btn_6_label;
		lv_obj_t *screen_IO_hor_btn_7;
		lv_obj_t *screen_IO_hor_btn_7_label;
		lv_obj_t *screen_IO_hor_btn_8;
		lv_obj_t *screen_IO_hor_btn_8_label;
		lv_obj_t *screen_IO_hor_btn_9;
		lv_obj_t *screen_IO_hor_btn_9_label;
		lv_obj_t *screen_IO_hor_btn_10;
		lv_obj_t *screen_IO_hor_btn_10_label;
		lv_obj_t *screen_IO_hor_btn_return;
		lv_obj_t *screen_IO_hor_btn_return_label;
		lv_obj_t *screen_IO_hor_btn_12;
		lv_obj_t *screen_IO_hor_btn_12_label;
		lv_obj_t *screen_IO_hor_ddlist_1;
		lv_obj_t *screen_IO_hor_ddlist_2;
		lv_obj_t *screen_IO_hor_ddlist_3;
		lv_obj_t *screen_IO_hor_ddlist_4;
		lv_obj_t *screen_IO_hor_ddlist_5;
		lv_obj_t *screen_IO_hor_ddlist_6;
		lv_obj_t *screen_IO_hor_ddlist_7;
		lv_obj_t *screen_IO_hor_ddlist_8;
		lv_obj_t *screen_IO_hor_ddlist_9;
		lv_obj_t *screen_IO_hor_ddlist_10;
		lv_obj_t *screen_IO_hor_ddlist_11;
		lv_obj_t *screen_IO_hor_ddlist_12;
		lv_obj_t *screen_IO_hor_ddlist_13;
		lv_obj_t *screen_IO_hor_ddlist_14;
		lv_obj_t *screen_IO_hor_ddlist_15;
		lv_obj_t *screen_IO_hor_ddlist_16;
		lv_obj_t *screen_IO_hor_ddlist_17;
		lv_obj_t *screen_IO_hor_ddlist_18;
		lv_obj_t *screen_IO_hor_ddlist_19;
		lv_obj_t *screen_IO_hor_ddlist_20;
		lv_obj_t *screen_IO_hor_ddlist_21;
		lv_obj_t *screen_IO_hor_ddlist_22;
		lv_obj_t *screen_IO_hor_ddlist_23;
		lv_obj_t *screen_IO_hor_ddlist_24;
		lv_obj_t *screen_IO_hor_ddlist_25;
		lv_obj_t *screen_IO_hor_ddlist_26;
		lv_obj_t *screen_IO_hor_ddlist_27;
		lv_obj_t *screen_IO_hor_ddlist_28;
		lv_obj_t *screen_IO_hor_ddlist_29;
		lv_obj_t *screen_IO_hor_ddlist_30;
		lv_obj_t *screen_IO_hor_ddlist_31;
		lv_obj_t *screen_IO_hor_ddlist_32;
		lv_obj_t *screen_IO_hor_ddlist_33;
		lv_obj_t *screen_IO_hor_ddlist_34;
		lv_obj_t *screen_IO_hor_ddlist_35;
		lv_obj_t *screen_IO_hor_ddlist_36;
		lv_obj_t *screen_IO_hor_ddlist_37;
		lv_obj_t *screen_IO_hor_ddlist_38;
		lv_obj_t *screen_IO_hor_ddlist_39;
		lv_obj_t *screen_IO_hor_ddlist_40;
		lv_obj_t *screen_IO_hor_label_c1;
		lv_obj_t *screen_IO_hor_label_c2;
		lv_obj_t *screen_IO_hor_label_c3;
		lv_obj_t *screen_IO_hor_label_c4;
		lv_obj_t *screen_IO_hor_label_c5;
		lv_obj_t *screen_IO_hor_label_c6;
		lv_obj_t *screen_IO_hor_label_c7;
		lv_obj_t *screen_IO_hor_label_c8;
		lv_obj_t *screen_IO_hor_label_c9;
		lv_obj_t *screen_IO_hor_label_c10;
		lv_obj_t *screen_IO_hor_label_c11;
		lv_obj_t *screen_IO_hor_label_c12;
		lv_obj_t *screen_IO_hor_label_c13;
		lv_obj_t *screen_IO_hor_label_c14;
		lv_obj_t *screen_IO_hor_label_c15;
		lv_obj_t *screen_IO_hor_label_c16;
		lv_obj_t *screen_IO_hor_label_c17;
		lv_obj_t *screen_IO_hor_label_c18;
		lv_obj_t *screen_IO_hor_label_c19;
		lv_obj_t *screen_IO_hor_label_c20;
		lv_obj_t *screen_IO_hor_label_c21;
		lv_obj_t *screen_IO_hor_label_c22;
		lv_obj_t *screen_IO_hor_label_c23;
		lv_obj_t *screen_IO_hor_label_c24;
		lv_obj_t *screen_IO_hor_label_c25;
		lv_obj_t *screen_IO_hor_label_c26;
		lv_obj_t *screen_IO_hor_label_c27;
		lv_obj_t *screen_IO_hor_label_c28;
		lv_obj_t *screen_IO_hor_label_c29;
		lv_obj_t *screen_IO_hor_label_c30;
		lv_obj_t *screen_IO_hor_label_c31;
		lv_obj_t *screen_IO_hor_label_c32;
		lv_obj_t *screen_IO_hor_label_c33;
		lv_obj_t *screen_IO_hor_label_c34;
		lv_obj_t *screen_IO_hor_label_c35;
		lv_obj_t *screen_IO_hor_label_c36;
		lv_obj_t *screen_IO_hor_label_c37;
		lv_obj_t *screen_IO_hor_label_c38;
		lv_obj_t *screen_IO_hor_label_c39;
		lv_obj_t *screen_IO_hor_label_c40;
		lv_obj_t *screen_city_hor;
		bool screen_city_hor_del;
		lv_obj_t *screen_city_hor_btn_return;
		lv_obj_t *screen_city_hor_btn_return_label;
		lv_obj_t *screen_city_hor_ddlist_1;
		lv_obj_t *screen_city_hor_ddlist_2;
		lv_obj_t *screen_city_hor_ddlist_3;
		lv_obj_t *screen_city_hor_label_1;
		lv_obj_t *screen_city_hor_btn_city;
		lv_obj_t *screen_city_hor_btn_city_label;
	} lv_ui;

	typedef void (*ui_setup_scr_t)(lv_ui *ui);

	void ui_init_style(lv_style_t *style);

	void ui_load_scr_animation(lv_ui *ui, lv_obj_t **new_scr, bool new_scr_del, bool *old_scr_del, ui_setup_scr_t setup_scr,
							   lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

	void ui_animation(void *var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
					  uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
					  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

	void init_scr_del_flag(lv_ui *ui);

	void setup_ui(lv_ui *ui);

	void init_keyboard(lv_ui *ui);

	extern lv_ui guider_ui;

	void setup_scr_screen_home_hor(lv_ui *ui);
	void setup_scr_screen_set_hor(lv_ui *ui);
	void setup_scr_screen_voice_hor(lv_ui *ui);
	void setup_scr_screen_IO_hor(lv_ui *ui);
	void setup_scr_screen_city_hor(lv_ui *ui);

	void setup_scr_screen_home(lv_ui *ui);
	void setup_scr_screen_set(lv_ui *ui);
	void setup_scr_screen_voice(lv_ui *ui);
	void setup_scr_screen_IO(lv_ui *ui);
	void setup_scr_screen_city(lv_ui *ui);

	LV_FONT_DECLARE(lv_font_Alatsi_Regular_18)
	LV_FONT_DECLARE(lv_font_Deng_12)
	LV_FONT_DECLARE(lv_font_Dengb_16)
	LV_FONT_DECLARE(lv_font_Dengb_22)
	LV_FONT_DECLARE(lv_font_Dengb_24)
	LV_FONT_DECLARE(lv_font_Dengb_25)
	LV_FONT_DECLARE(lv_font_Dengb_26)
	LV_FONT_DECLARE(lv_font_Dengb_27)
	LV_FONT_DECLARE(lv_font_Dengb_28)
	LV_FONT_DECLARE(lv_font_Dengb_30)
	LV_FONT_DECLARE(lv_font_Dengb_36)
	LV_FONT_DECLARE(lv_font_Dengb_38)
	LV_FONT_DECLARE(lv_font_Dengb_40)
	LV_FONT_DECLARE(lv_font_Dengb_45)
	LV_FONT_DECLARE(lv_font_Dengb_50)
	LV_FONT_DECLARE(lv_font_Dengb_60)

	LV_FONT_DECLARE(lv_font_date_26)
	LV_FONT_DECLARE(lv_font_date_28)
	LV_FONT_DECLARE(lv_font_date_29)
	LV_FONT_DECLARE(lv_font_date_32)
	LV_FONT_DECLARE(lv_font_date_34)
	LV_FONT_DECLARE(lv_font_date_39)
	LV_FONT_DECLARE(lv_font_date_41)
	LV_FONT_DECLARE(lv_font_date_44)
	LV_FONT_DECLARE(lv_font_date_82)

	LV_FONT_DECLARE(lv_font_YaHei_24)
	LV_FONT_DECLARE(lv_font_YaHei_26)
	LV_FONT_DECLARE(lv_font_YaHei_28)
	LV_FONT_DECLARE(lv_font_YaHei_32)
	LV_FONT_DECLARE(lv_font_YaHei_35)
	LV_FONT_DECLARE(lv_font_YaHei_52)

	LV_FONT_DECLARE(lv_font_medium_24)
	LV_FONT_DECLARE(lv_font_medium_28)

#ifdef __cplusplus
}
#endif
#endif
