/*
    新文件avi:h264+aac更新后出现卡顿，修改D:\shipinji\yuanping\packages\artinchip\mpp\middle_media\player\test\player_video_test.c
    avi:h264+mp3出现卡顿修复avi.c
*/

#include "main.h"

rt_thread_t video_thread = RT_NULL;
static rt_thread_t mouse_image_thread = RT_NULL;
static rt_thread_t  music_thread = RT_NULL, rtc_thread = RT_NULL;
static rt_thread_t uart_thread = RT_NULL, elevator_play_thread = RT_NULL, lwip_thread = RT_NULL;
static rt_thread_t media_import_thread = RT_NULL, image_thread = RT_NULL, cfgsave_thread = RT_NULL;
/*****************LVGL部分******************************/
volatile uint8_t hengping = 0; // 1横屏 0竖屏
/*****************rt-thread部分******************************/
rt_mutex_t mouse_key_mutex = RT_NULL, mouse_x_y_mutex = RT_NULL, wheel_mutex = RT_NULL;
rt_mutex_t video_mutex = RT_NULL, audio_mutex = RT_NULL, elevtor_mutex = RT_NULL;
rt_mutex_t num_mutex = RT_NULL, volume_mutex = RT_NULL;
/*********************鼠标部分******************************/
volatile bool mouse_btn_left = false; // 鼠标左键按下标志
volatile bool mouse_btn_right = false; // 鼠标右键按下标志
volatile bool mouse_leave_flag = false;                // 鼠标离开标志
volatile int mouse_current_x = 0, mouse_current_y = 0; // 鼠标移动的坐标
volatile int16_t wheel_diff = 0;                       // 滚轮差异值
/*********************网络天气部分******************************/
char ip_str[20] = "192.168.1.200";
char mask_str[20] = "255.255.255.0";
char gateway_str[20] = "192.168.1.1";
weather_t my_weather = {0, 0}; //温度，天气
volatile bool get_lwip_flag = false;       // 网络有插入
volatile bool geted_weather_flag = false;       //
volatile bool wait_uart_init_flag = false;       //
volatile bool start_set_lwip_flag = false; //
volatile bool change_weather_flag = false; // 更新天气图标标志
volatile bool weather_erro_flag = false; // 更新天气图标标志
/*********************主页面部分******************************/
image_num_pos_t image_num_pos[3] = {{0, 0}, {0, 0}, {0, 0}}; //记录楼层图片位置
volatile bool image_show_flag = false;                      //当视频播放完毕后，图片显示一下标志
volatile bool break_uart_flag = false;                      // 视频正在更新
volatile bool init_set_img_ok = false;                      // 视频正在更新
volatile bool video_in_updating = false;                      // 视频正在更新
volatile bool energy_show_image_flag = false;                //节能模式图片显示标志
volatile bool home_delete_flag = false;
volatile bool home_hor_delete_flag = false;
volatile bool init_video_flag = false;           // 重新初始化视频状态标志
volatile bool have_two_video_flag = false;
volatile bool wait_elevtor_flag = false;
volatile bool need_to_play_next_video = false;           // 重新初始化视频状态标志
volatile bool need_to_play_video_flag = false;           // 重新初始化视频状态标志
volatile rt_tick_t video_wait_time = 0;           // 重新初始化视频状态标志

volatile uint8_t set_vol_cnt = 0;
volatile uint8_t arrow_num = 0;                              // 箭头执行方案
volatile uint8_t set_gif_arrow = 10;                          // 设置gif播放方向
volatile uint8_t play_image_num = 0;                          // 设置gif播放方向
/*********************变量部分******************************/

volatile bool in_update_video_flag = false;            // 设置音量标志
volatile bool update_video_type_erro_flag = false;            // 设置音量标志

volatile bool home_hor_video_flag = false;            // 设置音量标志
volatile bool home_video_flag = false;            // 设置音量标志

volatile bool set_vol_flag = false;            // 设置音量标志
volatile bool video_no_super_flag = false;            // 视频不支持标志
volatile bool show_video_label_flag = false;            // 视频不支持标志
volatile bool in_play_overload_music = false;            // 设置音量标志
volatile bool wait_overload_flag = false;            // 设置音量标志
volatile bool wait_overload_mp3_flag = false;            // 设置音量标志
volatile bool wait_overload_cnt_flag = false;            // 设置音量标志
volatile bool set_video_vol_flag = false;            // 设置音量标志
volatile bool io_state_flag[19] = {false};//IO页面的显示状态
volatile bool save_flag = false;            // 保存数据标志
volatile bool wait_arr_flag = false;            // 保存数据标志
volatile bool in_play_mp3_flag = false;            // 保存数据标志
volatile bool sound_state = false;          // 声音状态（0：低音，1：高音）
volatile bool music_des_flag = false;       // 音乐播放器销毁标志
volatile bool show_floor_flag = false;      //更新楼层显示标志
volatile bool video_defuat_flag = false;    // 视频已经存在更新标志
volatile bool restart_music_flag = false;   // 重新创建音乐播放器标志
volatile bool frist_set_time_flag = true;   // 刚进入设置页面，设置一下时间
volatile bool wait_fire_play_flag = false;            //
volatile bool video_continue_flag = false;  // 视频继续播放
volatile bool delete_over_flag = false;  // 视频继续播放
volatile uint8_t save_img_page = 0;
char *image_flash_path_prefix[11] = {"/data/a", "/data/b", "/data/c","/data/d",
"/data/e", "/data/f","/data/g", "/data/h", "/data/i", "/data/j", "/data/k"};
char *video_mp4_path[3][11] = {
    {"/data/a1.mp4", "/data/b1.mp4", "/data/c1.mp4", "/data/d1.mp4",
     "/data/e1.mp4", "/data/f1.mp4", "/data/g1.mp4", "/data/h1.mp4", "/data/i1.mp4", "/data/j1.mp4", "/data/k1.mp4"},
    {"/data/a2.mp4", "/data/b2.mp4", "/data/c2.mp4", "/data/d2.mp4",
     "/data/e2.mp4", "/data/f2.mp4", "/data/g2.mp4", "/data/h2.mp4", "/data/i2.mp4", "/data/j2.mp4", "/data/k2.mp4"},
    {"/data/a3.mp4", "/data/b3.mp4", "/data/c3.mp4", "/data/d3.mp4",
     "/data/e3.mp4", "/data/f3.mp4", "/data/g3.mp4", "/data/h3.mp4", "/data/i3.mp4", "/data/j3.mp4", "/data/k3.mp4"}
};

char *video_avi_path[3][11] = {
    {"/data/a1.avi", "/data/b1.avi", "/data/c1.avi", "/data/d1.avi","/data/e1.avi",
     "/data/f1.avi", "/data/g1.avi", "/data/h1.avi", "/data/i1.avi", "/data/j1.avi", "/data/k1.avi"},
    {"/data/a2.avi", "/data/b2.avi", "/data/c2.avi", "/data/d2.avi","/data/e2.avi",
     "/data/f2.avi", "/data/g2.avi", "/data/h2.avi", "/data/i2.avi", "/data/j2.avi", "/data/k2.avi"},
    {"/data/a3.avi", "/data/b3.avi", "/data/c3.avi", "/data/d3.avi","/data/e3.avi",
     "/data/f3.avi", "/data/g3.avi", "/data/h3.avi", "/data/i3.avi", "/data/j3.avi", "/data/k3.avi"}
};

volatile bool page_mp4_update_flag[3][11] = {{false}, {false}, {false}};  //代表mp4视频已经更新
volatile bool page_avi_update_flag[3][11] = {{false}, {false}, {false}};  //代表avi视频已经更新

char *txt_path[14] = {"/data/update_1.txt", "/data/update_2.txt", "/data/update_3.txt", "/data/update_4.txt",
"/data/update_5.txt", "/data/update_6.txt", "/data/update_7.txt", "/data/update_8.txt", "/data/update_9.txt", "/data/update_10.txt",
"/data/update_11.txt", "/data/update_12.txt", "/data/update_13.txt", "/data/update_14.txt"};
volatile uint8_t my_page = PAGE_HOME;       // 当前页面
volatile uint8_t video_renew = PRINTF_NONE; // 视频删除
volatile uint8_t video_select = VIDEO_NONE; // 视频选择
volatile uint8_t wait_arr_cnt = 0;            // 保存数据标志
volatile uint8_t show_floor_num[3] = {0, 0, 0};  //显示的楼层号
volatile uint8_t update_page_num = 0;         // 升级页面选择
volatile uint8_t page_image_cnt[11] = {0};  //

volatile bool have_txt_flag[14] = {false};  //
volatile bool txt_mode[14] = {false};  //
volatile bool page_play_mode[14] = {false};  //

volatile bool txt_renew_flag = false;    // 文本更新成功标志
char update_txt_content[800] = {0};
volatile uint8_t txt_update_page_num = 0;
volatile uint8_t image_have_cnt = 0;        // 图片更新数量
volatile uint8_t have_floor_num = 0;        // 楼层数量
volatile uint8_t need_send_data = 0;        // 发送指令选择
volatile int media_count = 0;                      // USB中当前类型文件总数
volatile int video_num = 0; // V1=0, V2=1, V3=2
volatile int usb_image_cnt = 0;                    // USB中图片的数量
volatile int current_media_idx = 0;                // 当前处理的文件索引（0开始）
volatile uint16_t weather_select = 0;
volatile uint32_t accumulated_seconds = 0;  // 累计流逝的秒数

cnt_t my_cnt = {0, 0, 0, 0, 0, 0};
/*******************set界面变量******************************/
volatile bool reset_time_flag = false;             // 每天自动重置时间开启标志
volatile bool uart_v_flag = false;             // 每天自动重置时间开启标志
volatile bool set_volume_flag = false;             //
volatile uint8_t day_reset_time = 0;               // 每天重置时间
volatile uint8_t uart_v_num[3] = {0, 0, 0};
volatile uint16_t password = 6666;                 // 跳转页面的密码
SET_TIME_T MY_SET_TIME = {2025, 6, 30, 12, 10, 5}; // 当前时间
//                  页面语言、    播放模式、  音量、亮度、节能亮度
#if MY_USE
    SET_T MY_SET = {LANGUAGE_CN, PLAY_IMAGE,  5,    60,    5};
#else
    SET_T MY_SET = {LANGUAGE_CN, PLAY_IMAGE, 70,    60,    5};
#endif
//                           选择的图片、     logo、      箭头
#if MY_USE
SET_IMAGE_T MY_SET_IMAGE = {IMAGE_C401_ver, LOGO_XIO, ARROW_XIO};
#else
SET_IMAGE_T MY_SET_IMAGE = {IMAGE_C401_ver, LOGO_XIO, ARROW_XIO};
#endif
//                        dhcp、        IP、              子网掩码、          网关、            DNS
SET_DHCP_T MY_SET_DHCP = {true, {192, 168, 1, 200}, {255, 255, 255, 0}, {192, 168, 1, 1}, {0, 0, 0, 0}};
/*******************update界面变量******************************/
volatile bool update_ok_flag = false; // 升级成功标志
volatile uint8_t udisk_update_state = UPDATE_NONE, read_percent = 0;
/*******************voice界面变量******************************/
VOICE_SWITCH MY_VOICE_SWITCH = {true, true, true, false, true, true, true, true, true, true, true}; // 开关状态
volatile bool btn_music[3] = {false, true, false};                                                // 记录单击的音乐按钮
volatile bool music_state[3] = {false, true, false};                                               // 音乐播放状态
volatile bool music_renew_flag = false;                                                            // 音乐播放器是否需要更新
volatile bool voice_start_work = false;                                                            // 开始工作
volatile bool energy_conservation = false;                                                         // 节能模式
volatile uint8_t voice_language = VOICE_CN;                                                     // 语音语言
volatile uint8_t work_start_time = 6, work_over_time = 20;                                         // 工作时间
/*******************IO界面变量******************************/
IO_FLAG MY_IO_FLAG = {false};
volatile uint8_t IO_value[40] = {5, 1, 5, 2, 5, 4, 5, 3, 59, 1,
                                 6, 3, 62, 1, 62, 2, 60, 3, 62, 4,
                                 4, 4, 20, 3, 55, 3, 55, 2, 4, 3,
                                 4, 2, 20, 1, 20, 2, 63, 1, 4, 4};
volatile bool default_set_io_flag = false;
volatile uint8_t IO_dir_arrow_value = 0;
volatile uint8_t IO_car_overload_value = 0;
/*******************city界面变量******************************/
city_id_t my_city_id = {0, 0, 0, 0};
char city_name_str[32] = {0};
char city_code_str[32] = {0};
uint8_t city2_temp = 0;
uint16_t city3_temp = 0;
volatile bool set_city_ok_flag = false;
/*******************电梯变量******************************/
volatile bool elevator_arrow_flag = false;
volatile bool elevator_change_flag = false;
volatile bool elevator_change_no_lr_flag = false;

volatile bool arr_mp3_flag = false;

volatile bool have_open_door_flag = false;
volatile bool have_close_door_flag = false;

volatile bool full_load_mp3_flag = false;

volatile bool front_up_mp3_flag = false;
volatile bool front_down_mp3_flag = false;
volatile bool back_up_mp3_flag = false;
volatile bool back_down_mp3_flag = false;

volatile bool fire_music_mp3_flag = false;
volatile bool fire_music_pcm_flag = false;
volatile bool fire_start_flag = false;

volatile bool up_obs_music_pcm_flag = false;
volatile bool up_obs_music_mp3_flag = false;
volatile bool down_obs_music_pcm_flag = false;
volatile bool down_obs_music_mp3_flag = false;

volatile bool appease_music_mp3_flag = false;
volatile bool open_door_music_mp3_flag = false;
volatile bool close_door_music_mp3_flag = false;

volatile bool up_music_mp3_flag = false;
volatile bool down_music_mp3_flag = false;

volatile bool reset_appease_start_flag = false;
volatile bool reset_appease_music_pcm_flag = false;
volatile bool reset_appease_music_mp3_flag = false;

volatile bool reset_help_appease_start_flag = false;
volatile bool reset_help_appease_music_mp3_flag = false;
volatile bool reset_help_appease_music_pcm_flag = false;

volatile bool reset_end_appease_start_flag = false;
volatile bool reset_end_appease_music_mp3_flag = false;
volatile bool reset_end_appease_music_pcm_flag = false;

volatile bool up_obs_start_flag = false;
volatile bool down_obs_start_flag = false;
volatile bool overload_start_flag = false;
volatile bool overload_music_pcm_flag = false;
volatile bool overload_music_mp3_flag = false;

volatile bool state_video_end_flag = false;

volatile bool have_door_flag = false;

volatile bool in_reset_flag = false;
volatile bool in_reset_cn_flag = false;
volatile bool in_reset_end_flag = false;
volatile bool in_reset_help_flag = false;
volatile bool in_reset_end_cn_flag = false;
volatile bool in_reset_help_cn_flag = false;

volatile bool energy_flag = false;
volatile bool in_arr_flag = false;
volatile bool break_energy_flag = false;

volatile bool have_fire_flag = false;     // 有消防标志
volatile bool have_buzzer_flag = false;   // 有消防标志
volatile bool have_overload_flag = false; // 有超载标志
volatile bool in_video_state_flag = false;

volatile bool start_obs_flag = false;     // 标志
volatile bool open_door_obs_flag = false; // 标志

volatile bool set_floor_ok_flag = false;
volatile bool set_minu_floor_ok_flag = false;

volatile uint8_t up_or_down = 0;
volatile uint8_t overload_wait_cnt = 0;
volatile uint8_t overload_play_cnt = 0;
volatile uint8_t now_play_video_num = 0;

volatile uint8_t now_floor = 0;  // 处理0 ~ 79楼
volatile uint8_t minu_floor = 0; // 处理-1 ~ -9楼

volatile uint8_t up_down_cnt = 0; // 第一次上楼或下楼计数

volatile uint8_t last_state2_num = 0;

volatile uint8_t last_elevator_state2 = 0;
volatile uint8_t play_floor[20] = {0};
rt_play_elevator_t play_elevator = {false};
rt_elevator_floor_t elevator_floor = {FLOOR_NONE, 0}; // 电梯播放类型和楼层
/*******************结构体部分******************************/
struct mini_audio_player *music_audio_player = NULL; // 音乐播放器

static void init_main(void);
static void create_mutex(void);
static void create_thread(void);
static void set_flash_image(void);

/*
    线程主函数
*/
int main(void)
{
    init_main();
    create_thread();
    create_mutex();

    return 0;
}

static void init_main(void)
{
#ifdef ULOG_USING_FILTER
    ulog_global_filter_lvl_set(ULOG_OUTPUT_LVL);
#endif
    // EMMC_Init(1);
    // 初始化背光
    backlight_OFF();
    // 声音拉低
    Sound_Init(PIN_LOW);
    rt_thread_mdelay(500);
    // 上电获取保存的配置
    cfgRead();
    set_flash_image();
    // 设置屏幕方向
    if (MY_SET_IMAGE.image <= 4)
        hengping = 0;
    else
        hengping = 1;

    if(MY_SET_IMAGE.image == IMAGE_C201_hor) {update_page_num = UP_C201_hor;      txt_update_page_num = UP_C201_hor;}
    else if(MY_SET_IMAGE.image == IMAGE_C202_hor) {update_page_num = UP_C202_hor; txt_update_page_num = UP_C202_hor;}
    else if(MY_SET_IMAGE.image == IMAGE_C401_hor) {update_page_num = UP_C401_hor; txt_update_page_num = UP_C401_hor;}
    else if(MY_SET_IMAGE.image == IMAGE_C402_hor) {update_page_num = UP_C402_hor; txt_update_page_num = UP_C402_hor;}
    else if(MY_SET_IMAGE.image == IMAGE_C403_hor) {update_page_num = UP_C403_hor; txt_update_page_num = UP_C403_hor;}
    else if(MY_SET_IMAGE.image == IMAGE_C404_hor) {update_page_num = UP_C404_hor; txt_update_page_num = UP_C404_hor;}
    else if(MY_SET_IMAGE.image == IMAGE_C401_ver) {update_page_num = UP_C401_ver; txt_update_page_num = UP_C401_ver;}
    else if(MY_SET_IMAGE.image == IMAGE_C402_ver) {update_page_num = UP_C402_ver; txt_update_page_num = UP_C402_ver;}
    else if(MY_SET_IMAGE.image == IMAGE_C403_ver) {update_page_num = UP_C403_ver; txt_update_page_num = UP_C403_ver;}
    else if(MY_SET_IMAGE.image == IMAGE_C404_ver) {update_page_num = UP_C404_ver; txt_update_page_num = UP_C404_ver;}
    else if(MY_SET_IMAGE.image == IMAGE_C203_hor) txt_update_page_num = UP_C203_hor;
    else if(MY_SET_IMAGE.image == IMAGE_C301_hor) txt_update_page_num = UP_C301_hor;
    else if(MY_SET_IMAGE.image == IMAGE_C302_hor) txt_update_page_num = UP_C302_hor;
    else if(MY_SET_IMAGE.image == IMAGE_C303_hor) txt_update_page_num = UP_C303_hor;

    if(!page_play_mode[txt_update_page_num])
    {
        if( MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
        MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor ||
        MY_SET_IMAGE.image == IMAGE_C303_hor ||
        MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
        {
            MY_SET.play_mode = PLAY_VIDEO;

        }else {
            MY_SET.play_mode = PLAY_IMAGE;
        }
    }

    // 初始化看门狗
    if (wdt_init() != RT_EOK)
        rt_kprintf("[WDT] Init failed, system halted!");
    else
        rt_kprintf("wDT init success\n");
    // 注册空闲钩子（第一层喂狗机制）
    rt_thread_idle_sethook(idle_hook);
    rt_thread_mdelay(500);
}

static void set_flash_image(void)
{
    if(save_img_page == MY_SET_IMAGE.image) return;

    save_img_page = MY_SET_IMAGE.image;

    if(MY_SET_IMAGE.image == IMAGE_C401_hor)
    {
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_hor.jpg", 0);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_hor1.png", 1);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_hor2.png", 2);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_hor3.png", 3);
    }else if(MY_SET_IMAGE.image == IMAGE_C402_hor)
    {
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_hor.jpg", 0);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_hor1.png", 1);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_hor2.png", 2);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_hor3.png", 3);
    }
    else if(MY_SET_IMAGE.image == IMAGE_C403_hor)
    {
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_hor.jpg", 0);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_hor1.png", 1);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_hor2.png", 2);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_hor3.png", 3);
    }else if(MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
            MY_SET_IMAGE.image == IMAGE_C404_hor)
    {
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_hor2.png", 1);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_hor3.png", 2);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_hor2.png", 3);
    }else if(MY_SET_IMAGE.image == IMAGE_C401_ver)
    {
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_ver.jpg", 0);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_ver1.png", 1);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_ver2.png", 2);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C401_ver3.png", 3);
    }else if(MY_SET_IMAGE.image == IMAGE_C402_ver)
    {
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_ver.jpg", 0);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_ver1.png", 1);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_ver2.png", 2);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C402_ver3.png", 3);
    }else if(MY_SET_IMAGE.image == IMAGE_C403_ver || MY_SET_IMAGE.image == IMAGE_C404_ver)
    {
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_ver.jpg", 0);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_ver1.png", 1);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_ver2.png", 2);
        cover_image_to_init_file("/rodata/lvgl_data/picture/background/C403_ver3.png", 3);
    }
    save_begin();
}

static void create_thread(void)
{
    video_thread = rt_thread_create("video",            // 线程名字
                                    video_thread_entry, // 线程入口函数
                                    RT_NULL,            // 线程入口参数
                                    1024 * 12,          // 线程堆栈大小
                                    10,                 // 线程优先级
                                    20);                // 时间片参数
    media_import_thread = rt_thread_create("media_import",
                                           media_import_thread_entry,
                                           RT_NULL,
                                           1024 * 24,
                                           16,
                                           20);
    music_thread = rt_thread_create("music",
                                    music_thread_entry,
                                    RT_NULL,
                                    1024 * 9,
                                    17,
                                    20);
    rtc_thread = rt_thread_create("rtc",                                // 线程名字
                                  rtc_thread_entry,                     // 线程入口函数
                                  RT_NULL,                              // 线程入口参数
                                  2048,                                 // 线程堆栈大小
                                  18,                                    // 线程优先级
                                  20);                                  // 时间片参数
    cfgsave_thread = rt_thread_create("cfgsave",                        // 线程名字
                                      cfgsave_thread_entry,             // 线程入口函数
                                      RT_NULL,                          // 线程入口参数
                                      1024 * 4,                         // 线程堆栈大小
                                      20,                                // 线程优先级
                                      20);                              // 时间片参数
    uart_thread = rt_thread_create("uart",                              // 线程名字
                                   uart_thread_entry,                   // 线程入口函数
                                   RT_NULL,                             // 线程入口参数
                                   1024 * 8,                            // 线程堆栈大小
                                   14,                                  // 线程优先级
                                   20);                                 // 时间片参数
    mouse_image_thread = rt_thread_create("mouse",                      // 线程名字
                                          mouse_image_thread_entry,     // 线程入口函数
                                          RT_NULL,                      // 线程入口参数
                                          1024 * 4,                     // 线程堆栈大小
                                          19,                           // 线程优先级
                                          20);                          // 时间片参数
    elevator_play_thread = rt_thread_create("elevator_play",            // 线程名字
                                            elevator_play_thread_entry, // 线程入口函数
                                            RT_NULL,                    // 线程入口参数
                                            1024 * 10,                  // 线程堆栈大小
                                            19,                          // 线程优先级
                                            20);                        // 时间片参数                       // 时间片参数
    lwip_thread = rt_thread_create("lwip",                              // 线程名字
                                   lwip_thread_entry,                   // 线程入口函数
                                   RT_NULL,                             // 线程入口参数
                                   1024 * 10,                           // 线程堆栈大小
                                   20,                                   // 线程优先级
                                   20);                                 // 时间片参数

    if (video_thread)
        rt_thread_startup(video_thread);
    if (media_import_thread)
        rt_thread_startup(media_import_thread);
    if (music_thread)
        rt_thread_startup(music_thread);
    if (rtc_thread != RT_NULL)
        rt_thread_startup(rtc_thread);
    if (cfgsave_thread != RT_NULL)
        rt_thread_startup(cfgsave_thread);
    if (uart_thread != RT_NULL)
        rt_thread_startup(uart_thread);
    if (mouse_image_thread)
        rt_thread_startup(mouse_image_thread);
    if (elevator_play_thread)
        rt_thread_startup(elevator_play_thread);
    if (lwip_thread)
        rt_thread_startup(lwip_thread);
}
/*创建互斥锁*/
static void create_mutex(void)
{
    mouse_key_mutex = rt_mutex_create("mouse_key_mutex", RT_IPC_FLAG_PRIO);
    if (mouse_key_mutex != RT_NULL)
        rt_kprintf("mouse_key_mutex创建成功\n");

    mouse_x_y_mutex = rt_mutex_create("mouse_x_y_mutex", RT_IPC_FLAG_PRIO);
    if (mouse_x_y_mutex != RT_NULL)
        rt_kprintf("mouse_x_y_mutex创建成功\n");

    wheel_mutex = rt_mutex_create("wheel_mutex", RT_IPC_FLAG_PRIO);
    if (wheel_mutex != RT_NULL)
        rt_kprintf("wheel_mutex创建成功\n");

    video_mutex = rt_mutex_create("video_mutex", RT_IPC_FLAG_PRIO);
    if (video_mutex != RT_NULL)
        rt_kprintf("video_mutex创建成功\n");
    volume_mutex = rt_mutex_create("volume_mutex", RT_IPC_FLAG_PRIO);
    if (volume_mutex != RT_NULL)
        rt_kprintf("volume_mutex创建成功\n");
    audio_mutex = rt_mutex_create("audio_mutex", RT_IPC_FLAG_PRIO);
    if (audio_mutex != RT_NULL)
        rt_kprintf("audio_mutex创建成功\n");
    elevtor_mutex = rt_mutex_create("elevtor_mutex", RT_IPC_FLAG_PRIO);
    if (elevtor_mutex != RT_NULL)
        rt_kprintf("elevtor_mutex创建成功\n");
    num_mutex = rt_mutex_create("num_mutex", RT_IPC_FLAG_PRIO);
    if (num_mutex != RT_NULL)
        rt_kprintf("num_mutex创建成功\n");

}

