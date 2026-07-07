#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef RT_USING_ULOG
#include <ulog.h>
#endif

#include <board.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>   // 包含文件权限定义
#include <sys/types.h>  // 包含一些类型定义
#include <rtthread.h>
#include <rtdevice.h>

#include <aic_drv_wdt.h>
#include <drivers/pin.h>

#include "../../packages/artinchip/mpp/middle_media/component/include/mm_component.h"
#include "../../packages/artinchip/mpp/middle_media/component/include/mm_index.h"
#include "../../packages/artinchip/mpp/middle_media/base/include/aic_audio_render_manager.h"
#include "mini_audio_player.h"
#include "mpp_log.h"



#include <dirent.h>

#include "aic_ui.h"
#include "lv_port_disp.h"
#include "mpp_fb.h"
#include "aic_player.h"

#include "gui_guider.h"
#include "backlight.h"
#include "video.h"
#include "udisk_update.h"
#include "udisk_check.h"
#include "cfgsave.h"
#include "sound.h"
#include "music.h"
#include "my_rtc.h"
#include "my_uart.h"
#include "wdt.h"
#include "mouse_image.h"
#include "elevator_play.h"
#include "weather.h"
#include "udp_rec.h"

#define CONN(x, y) x#y
#define LVGL_DIRR "L:" LVGL_STORAGE_PATH "/"
#define MY_LVGL_IMAGE_PATH(y) CONN(LVGL_DIRR, y)

#define LVGL_MUSIC_PATH LVGL_STORAGE_PATH"/music/"
#define MUSIC_PATH(y) CONN(LVGL_MUSIC_PATH, y)

#define LVGL_PCM_PATH LVGL_STORAGE_PATH"/pcm/"
#define PCM_PATH(y) CONN(LVGL_PCM_PATH, y)

#define VERSION_DATE "Ver: 1.0 (20260701)"

//我用 1 ，客户 0
#define MY_USE 1

#define USE_PRI 0
#define MOUSE_DEBUG 0

#if MY_USE
    #define DELAY_LOGO 2
#else
    #define DELAY_LOGO 12
#endif
#if 1
    #define DEBUG_LOG(fmt,...) \
        rt_kprintf("[DEBUG]%s:%d: " fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)
#else
    #define DEBUG_LOG(...)  do{}while(0)
#endif
/*****************lvgl部分******************************/
extern lv_ui guider_ui;
extern volatile uint8_t hengping;
extern lv_timer_t *home_set_time_timer,     *home_move_timer,     *home_refresh_picture_timer,     *update_timer, *home_set_weather_timer, *frist_timer;
extern lv_timer_t *home_hor_set_time_timer, *home_hor_move_timer, *home_hor_refresh_picture_timer, *update_hor_timer, *home_hor_set_weather_timer, *frist_hor_timer;
extern lv_timer_t *set_time_timer,     *get_time_timer;
extern lv_timer_t *voice_hor_password_timer;
extern lv_timer_t *voice_password_timer;
extern lv_timer_t *hor_set_time_timer, *hor_get_time_timer;
extern lv_timer_t *get_state_timer, *get_state_hor_timer;
extern lv_obj_t *cursor_img;
/*****************rt-thread部分******************************/

extern rt_thread_t video_thread;
extern rt_mutex_t mouse_key_mutex, mouse_x_y_mutex, wheel_mutex, video_mutex, audio_mutex, elevtor_mutex;
extern rt_mutex_t num_mutex, volume_mutex;
/***********************鼠标部分******************************/
/*********************鼠标部分******************************/
extern volatile bool mouse_btn_left, mouse_btn_right, mouse_plugged, mouse_leave_flag;
extern volatile int mouse_current_x, mouse_current_y;//鼠标移动的坐标
extern volatile int16_t wheel_diff;          // 滚轮差异值
/*********************网络天气部分******************************/
extern char ip_str[20];
extern char mask_str[20];
extern char gateway_str[20];
extern volatile bool wait_uart_init_flag, weather_erro_flag;
extern volatile bool get_lwip_flag, start_set_lwip_flag, change_weather_flag, geted_weather_flag;
/*********************主页面部分******************************/
typedef struct {
    int32_t x;
    int32_t y;
}image_num_pos_t;

extern image_num_pos_t image_num_pos[3];
extern volatile bool energy_show_image_flag, video_in_updating;
extern volatile bool init_set_img_ok;
extern volatile bool home_hor_delete_flag, home_delete_flag;
extern volatile bool init_video_flag;
extern volatile bool have_two_video_flag;
extern volatile bool wait_elevtor_flag;
extern volatile bool need_to_play_video_flag;

extern volatile bool need_to_play_next_video;           // 重新初始化视频状态标志
extern volatile rt_tick_t video_wait_time;           // 重新初始化视频状态标志

#define UP_ARROW 2
#define DOWN_ARROW 1
extern volatile uint8_t arrow_num, set_gif_arrow;
/*********************变量部分******************************/
#define SOUND_HIGH true
#define SOUND_LOW false
typedef struct {
    int temperature;  //温度
    uint16_t weather;     //天气
}weather_t;
typedef struct {
    uint8_t arr;  //温度
    uint8_t up;     //天气
    uint8_t down;     //天气
    uint8_t open;     //天气
    uint8_t close;     //天气
    uint8_t last_play_state;     //天气
}cnt_t;
extern cnt_t my_cnt;
extern weather_t my_weather;
extern volatile bool in_update_video_flag,update_video_type_erro_flag;
extern volatile bool home_hor_video_flag, home_video_flag;
extern volatile bool show_video_label_flag;
extern volatile bool in_play_overload_music, wait_overload_mp3_flag;
extern volatile bool set_vol_flag, set_video_vol_flag, wait_overload_flag, wait_overload_cnt_flag;
extern volatile bool video_continue_flag, show_floor_flag;     //视频继续播放
extern volatile bool wait_fire_play_flag;
extern volatile bool delete_over_flag;     //视频继续播放
extern volatile bool save_flag, frist_set_time_flag, sound_state, restart_music_flag, music_des_flag, io_state_flag[19];
extern volatile uint8_t save_img_page, video_udp_state;
extern volatile uint8_t set_vol_cnt;
extern char *image_flash_path_prefix[11],  *txt_path[14];
extern char *video_mp4_path[3][11], *video_avi_path[3][11];
extern volatile uint8_t image_have_cnt, my_page, video_select, video_renew;
extern volatile uint8_t show_floor_num[3], have_floor_num, need_send_data;
extern volatile uint8_t update_page_num, txt_update_page_num;
extern volatile uint8_t page_image_cnt[11];
extern volatile bool have_txt_flag[14], txt_mode[14], page_play_mode[14];
extern volatile uint8_t page_set_play_mode[14];
extern volatile bool page_avi_update_flag[3][11], page_mp4_update_flag[3][11];
extern volatile bool txt_renew_flag;
extern volatile bool wait_arr_flag;
extern volatile uint8_t wait_arr_cnt;
extern char update_txt_content[800];
extern volatile int current_media_idx, media_count, usb_image_cnt;
extern volatile int video_num;
extern volatile uint32_t accumulated_seconds;


struct user_audio_render
{
    struct aic_audio_render *render;
    struct aic_audio_render_attr ao_attr;
    struct aic_audio_render_attr original_ao_attr;  // 保存初始属性用于重置
    int user_audio_play_enable;
    uint8_t init_flag;  // 初始化标志，0:未初始化 1:已初始化
};

extern struct user_audio_render g_user_audio_render;
/*******************set界面变量******************************/
#define LANGUAGE_CN 1
#define LANGUAGE_EN 2
#define PLAY_IMAGE 1
#define PLAY_VIDEO 2

typedef struct {
    volatile uint8_t image; // 界面选择的图片序号
    volatile uint8_t logo; // 界面选择的logo序号
    volatile uint8_t arrow; // 界面选择的箭头序号
} SET_IMAGE_T;
typedef struct {
    volatile uint8_t language; //语言选择
    volatile uint8_t play_mode; //选择播放模式
    volatile uint8_t sound; //声音大小
    volatile uint8_t backlight; //正常亮度
    volatile uint8_t e_con_backlight; //节能亮度
} SET_T;
typedef struct {
    volatile uint16_t year;
    volatile uint8_t month;
    volatile uint8_t day;
    volatile uint8_t hour;
    volatile uint8_t minute;
    volatile uint8_t second;
} SET_TIME_T;

typedef struct {
    volatile bool dhcp_state;        // 是否开启dhcp
    volatile uint16_t ip[4];         //ip
    volatile uint16_t mask[4];        //子网掩码
    volatile uint16_t gateway[4];     //网关
    volatile uint16_t dns[4];         //DNS
} SET_DHCP_T;


extern SET_TIME_T MY_SET_TIME;
extern SET_T MY_SET;
extern SET_IMAGE_T MY_SET_IMAGE;
extern SET_DHCP_T MY_SET_DHCP;

extern volatile bool reset_time_flag, uart_v_flag, set_volume_flag;
extern volatile uint8_t day_reset_time;
extern volatile uint8_t uart_v_num[3];
extern volatile uint16_t password;//跳转页面的密码
/*******************update界面变量******************************/
extern volatile bool update_ok_flag;
extern volatile uint8_t udisk_update_state, read_percent;
/*******************voice界面变量******************************/
#define VOICE_CN 1
#define VOICE_EN 2
#define VOICE_CN_EN 3

typedef struct {
    volatile bool voice;   //语音报站
    volatile bool lr;      //检测LR位
    volatile bool block;   //关门受阻语音
    volatile bool dzz;     //到站钟
    volatile bool floor;   //楼层播报
    volatile bool fire;    //消防语音
    volatile bool door;    //开关门语音
    volatile bool peak;    //高峰安抚语音
    volatile bool ols;     //超载语音
    volatile bool up;      //上下行语音
    volatile bool appease; //安抚语音
} VOICE_SWITCH;
extern VOICE_SWITCH MY_VOICE_SWITCH;

extern volatile bool voice_start_work, music_renew_flag, energy_conservation;
extern volatile bool music_state[3], btn_music[3];
extern volatile uint8_t voice_language;
extern volatile uint8_t work_start_time, work_over_time;
/*******************IO界面变量***********************/
typedef struct {
    volatile bool fire;        //语音报站
    volatile bool back_up;     //语音报站
    volatile bool front_up;    //语音报站
    volatile bool full_load;   //语音报站
    volatile bool dir_arrow;   //语音报站
    volatile bool open_door;   //语音报站+
    volatile bool back_down;   //语音报站
    volatile bool front_down;  //语音报站
    volatile bool car_overload;//语音报站
} IO_FLAG;
extern IO_FLAG MY_IO_FLAG;
extern volatile bool default_set_io_flag;
extern volatile uint8_t IO_value[40];
extern volatile uint8_t IO_dir_arrow_value;

/*******************city界面变量******************************/
typedef struct
{
    char *city_name;
}city_t;

typedef struct
{
    uint16_t city_1;
    uint16_t city_2;
    uint16_t city_3;
    uint16_t city_value;
}city_id_t;
extern city_id_t my_city_id;
extern city_t my_city_2[34];
extern city_t my_city_2_en[34];
extern city_t my_city_3[400];
extern city_t my_city_3_en[400];

extern char *city_code[][25];

extern char city_name_str[32];
extern char city_code_str[32];
extern char *city_province;
extern char *city_province_en;
extern uint8_t city_cnt[34];
extern uint8_t city2_temp;
extern uint16_t city3_temp;
/*******************电梯变量******************************/
typedef struct{

    volatile bool music_appease;    //音乐困人安抚

    volatile bool video_reset_appease; //复位安抚
    volatile bool music_reset_appease; //复位安抚

    volatile bool video_reset_help_appease; //复位救援平层安抚
    volatile bool music_reset_help_appease; //复位救援平层安抚

    volatile bool video_reset_end_appease; //复位结束安抚
    volatile bool music_reset_end_appease; //复位结束安抚

    volatile bool video_fire;        //视频消防
    volatile bool music_fire;        //音乐消防

    volatile bool video_overload;    //视频超载
    volatile bool music_overload;    //音乐超载

    volatile bool music_full_load;  //满载

    volatile bool music_open_door;  //音乐开门
    volatile bool music_close_door; //音乐关门
    volatile bool music_buzzer;     //音乐蜂鸣器

    volatile bool elevator_up;       //电梯上行
    volatile bool elevator_down;     //电梯下行

    volatile bool video_up_obs;       //上行遇阻
    volatile bool music_up_obs;       //上行遇阻

    volatile bool video_down_obs;     //下行遇阻
    volatile bool music_down_obs;     //下行遇阻

    volatile bool front_up;          //前门上行
    volatile bool front_down;        //前门下行
    volatile bool back_up;           //后门上行
    volatile bool back_down;         //后门下行

    volatile bool elevator_arr;
}rt_play_elevator_t ;

typedef struct{
    volatile uint8_t state;
    volatile uint8_t floor;
}rt_elevator_floor_t ;

extern rt_play_elevator_t play_elevator;
extern rt_elevator_floor_t elevator_floor;
extern volatile bool elevator_arrow_flag;
extern volatile bool full_load_mp3_flag;
extern volatile bool front_up_mp3_flag;
extern volatile bool front_down_mp3_flag;
extern volatile bool back_up_mp3_flag ;
extern volatile bool back_down_mp3_flag;

extern volatile bool in_reset_flag, in_reset_cn_flag;
extern volatile bool in_reset_help_flag, in_reset_help_cn_flag;
extern volatile bool in_reset_end_flag, in_reset_end_cn_flag;

extern volatile bool have_open_door_flag, have_open_close_flag;

extern volatile bool state_video_end_flag;
extern volatile bool arr_mp3_flag;
extern volatile bool elevator_change_flag;
extern volatile bool elevator_change_no_lr_flag;
extern volatile bool fire_start_flag;
extern volatile bool appease_music_mp3_flag;
extern volatile bool open_door_music_mp3_flag, close_door_music_mp3_flag;
extern volatile bool up_music_mp3_flag, down_music_mp3_flag;
extern volatile bool reset_appease_start_flag, reset_end_appease_start_flag, reset_help_appease_start_flag;
extern volatile bool reset_appease_music_mp3_flag, reset_appease_music_pcm_flag;//复位安抚
extern volatile bool reset_end_appease_music_mp3_flag, reset_end_appease_music_pcm_flag;//复位结束安抚

extern volatile bool reset_help_appease_music_mp3_flag, reset_help_appease_music_pcm_flag;//复位救援平层安抚
extern volatile bool fire_music_mp3_flag, fire_music_pcm_flag;
extern volatile bool overload_music_mp3_flag, overload_music_pcm_flag, overload_start_flag;
extern volatile bool set_floor_ok_flag, set_minu_floor_ok_flag;
extern volatile bool have_door_flag;
extern volatile bool up_obs_start_flag, down_obs_start_flag;
extern volatile bool up_obs_music_pcm_flag, down_obs_music_pcm_flag;
extern volatile bool up_obs_music_mp3_flag, down_obs_music_mp3_flag;
extern volatile bool break_energy_flag;
extern volatile bool energy_flag;
extern volatile bool in_video_state_flag;
extern volatile bool have_overload_flag;//超载
extern volatile bool have_buzzer_flag;//蜂鸣器
extern volatile bool have_fire_flag;//消防
extern volatile bool in_arr_flag;//在播放到站

extern volatile bool start_obs_flag;//
extern volatile bool open_door_obs_flag;//


extern volatile uint8_t up_or_down ;
extern volatile uint8_t overload_wait_cnt ;
extern volatile uint8_t now_floor ,minu_floor ; //电梯播放种类
extern volatile uint8_t last_state2_num ;
extern volatile uint8_t overload_play_cnt ;
extern volatile uint8_t now_play_video_num ;
extern volatile uint8_t last_elevator_state2 ,up_down_cnt;
extern volatile uint8_t play_floor[20] ;
/*******************结构体部分******************************/
extern struct mini_audio_player *music_audio_player;
extern struct rt_elevator_t elevator_data;

extern char image_filename[][100];
extern char image_num_filename[][100];
extern char image_num_c4_filename[][100];
extern char image_num_big_filename[][100];
extern char image_weather_filename[][100];
extern char image_filename_c201_hor[][100];
typedef enum{
    PLAY_MP3_0 = 0,
    PLAY_MP3_1,
    PLAY_MP3_2,
    PLAY_MP3_3,
    PLAY_MP3_4,
    PLAY_MP3_5,
    PLAY_MP3_6,
    PLAY_MP3_7,
    PLAY_MP3_8,
    PLAY_MP3_9,
    PLAY_MP3_10,
    PLAY_MP3_A,
    PLAY_MP3_B,
    PLAY_MP3_C,
    PLAY_MP3_D,
    PLAY_MP3_E,
    PLAY_MP3_FIRE = 240,
    PLAY_MP3_OVERLOAD,
    PLAY_MP3_APPEASE,
    PLAY_MP3_RESET_APPEASE,
    PLAY_MP3_RESET_HELP_APPEASE,
    PLAY_MP3_RESET_END_APPEASE,
    PLAY_MP3_BELL,
    PLAY_MP3_FULL_LOAD,
    PLAY_MP3_UP_OBS,
    PLAY_MP3_DOWN_OBS,
} MP3_ID;
typedef enum{
    NUM_UP_DIRECTION = 0,
    NUM_DOWN_DIRECTION,
    NUM_FIRE_SIGNAL,
    NUM_OVERLOAD_SIGNAL,
    NUM_POWER_OFF,
    NUM_SLEEP,
    NUM_RESET_COMFORT,
    NUM_RESET_RESCUE_LEVEL,
    NUM_FULL_LOAD_SIGNAL,
    NUM_RESET_END,
    NUM_BUZZER_SIGNAL,
    NUM_OPEN_DOOR,
    NUM_UP_RUNNING,
    NUM_DOWN_RUNNING,
    NUM_FRONT_UP_TO_FLOOR,
    NUM_FRONT_DOWN_TO_FLOOR,
    NUM_BACK_UP_TO_FLOOR,
    NUM_BACK_DOWN_TO_FLOOR,
    NUM_TRAP_COMFORT,
} STATE_NUM_ID;

typedef enum{
    VIDEO_NONE = 0,
    VIDEO_FIRE,
    VIDEO_RESET_APPEASE,
    VIDEO_RESET_HELP_APPEASE,
    VIDEO_RESET_END_APPEASE,
    VIDEO_OVERLOAD,
    VIDEO_UP_OBS,
    VIDEO_DOWN_OBS,
}SELECT_VIDEO_ID;

typedef enum{
    FLOOR_NONE = 0,
    FLOOR_NUM,
    FLOOR_MINU,
    FLOOR_A_Z,
    FLOOR_1_9_A,
    FLOOR_1_9_B,
    FLOOR_1_6_C,
    FLOOR_1_5_F,
    FLOOR_A_0_9,
    FLOOR_B_0_9,
    FLOOR_C_1_4,
    FLOOR_D_1_4,
    FLOOR_G_1_9,
    FLOOR_L_1_3,
    FLOOR_M_0_9,
    FLOOR_P_0_9,
    FLOOR_R_1_3,
    FLOOR_AF,
    FLOOR_AG,
    FLOOR_BE,
    FLOOR_CF,
    FLOOR_EG,
    FLOOR_GF,
    FLOOR_HP,
    FLOOR_KG,
    FLOOR_LB,
    FLOOR_LD,
    FLOOR_LF,
    FLOOR_LG,
    FLOOR_LL,
    FLOOR_LP,
    FLOOR_MR,
    FLOOR_MZ,
    FLOOR_PB,
    FLOOR_PC,
    FLOOR_PH,
    FLOOR_PM,
    FLOOR_RF,
    FLOOR_UB,
    FLOOR_UF,
    FLOOR_UG,
    FLOOR_UP,
    FLOOR_12A,
    FLOOR_12B,
    FLOOR_13A,
    FLOOR_13B,
    FLOOR_14A,
    FLOOR_14B,
    FLOOR_15A,
    FLOOR_15B,
    FLOOR_17A,
    FLOOR_17B,
    FLOOR_18A,
    FLOOR_18B,
    FLOOR_23A,
    FLOOR_23B,
    FLOOR_33A,
    FLOOR_33B,
}FLOOR_SELECT_ID;


typedef enum{
    VIDEO_POWER_OFF = 1,
    VIDEO_SLEEP,
    VIDEO_RESET_COMFORT,
    VIDEO_RESET_RESCUE_LEVEL,
    VIDEO_RESET_END,
    VIDEO_BUZZER_SIGNAL,
    VIDEO_TRAP_COMFORT,
} STATE_2_ID;

typedef enum{
    duoyun_zhao = 1,
    duoyun_wang,
    xiaoyu,
    ying,
    qing,

} WEATHER_ID;

typedef enum{
    PAGE_HOME = 1,
    PAGE_SET,
    PAGE_VOICE,
    PAGE_UPDATE,
    PAGE_IO,
    PAGE_CITY,
    PAGE_HOME_HOR,
    PAGE_SET_HOR,
    PAGE_VOICE_HOR,
    PAGE_UPDATE_HOR,
    PAGE_IO_HOR,
    PAGE_CITY_HOR,
} PAGE_ID;

typedef enum{
    IMAGE_C401_ver = 1,//竖屏
    IMAGE_C402_ver,
    IMAGE_C403_ver,
    IMAGE_C404_ver,
    IMAGE_C201_hor,
    IMAGE_C202_hor,
    IMAGE_C203_hor,
    IMAGE_C301_hor,
    IMAGE_C302_hor,
    IMAGE_C303_hor,
    IMAGE_C401_hor,
    IMAGE_C402_hor,
    IMAGE_C403_hor,
    IMAGE_C404_hor,
}SELECT_IMAGE_ID;

typedef enum{
    LOGO_XIO = 1,
    LOGO_SWD,
    LOGO_NONE,
} SELECT_LOGO_ID;

typedef enum{
    ARROW_SWD = 1,
    ARROW_XIO,
} SELECT_ARROW_ID;

typedef enum{
    IMAGE_NONE = 0,
    IMAGE_INVISIBLE, //不可见
    IMAGE_VISIBLE, //可见
} SELECT_IMAGE_STATE_ID;

typedef enum{
    UP_C201_hor = 0,
    UP_C202_hor,
    UP_C203_hor,
    UP_C401_hor,
    UP_C402_hor,
    UP_C403_hor,
    UP_C404_hor,
    UP_C401_ver,
    UP_C402_ver,
    UP_C403_ver,
    UP_C404_ver,
    UP_C301_hor,
    UP_C302_hor,
    UP_C303_hor,
} SELECT_UPDATE_ID;

#endif /* __MAIN_H__ */
