#include "video.h"
#define VIDEO_TIMEOUT_TICK    (RT_TICK_PER_SECOND * 2)
static rt_tick_t last_play_tick = 0;

volatile static bool break_seek_flag = false;
volatile static bool delete_video_flag = false;
volatile static bool play_ok_delete_video_flag = false;
volatile static bool elevator_video_flag = false;


volatile static bool stop_video_flag = false;
volatile static bool update_energy_flag = false;

volatile static uint8_t have_video_num = 0;

static uint8_t set_state = 0;


struct lvgl_player_context my_lvgl_player_ctx;

static char g_filename[][256] = {
    LVGL_FILE_LIST_PATH(v1.mp4),
    LVGL_FILE_LIST_PATH(v2.mp4),
    LVGL_FILE_LIST_PATH(v3.mp4),
    LVGL_FILE_LIST_PATH(xihu.mp4),
    LVGL_FILE_LIST_PATH(v1.avi),
};

int destroy_player(uint8_t printf_text);
void destroy_video(void);

// 多视频模式：不销毁播放器
static bool is_multi_video_resident(void)
{
    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    bool ret = have_two_video_flag;
    rt_mutex_release(video_mutex);
    return ret;
}
// 新增：安全获取update_page_num的本地副本
static uint8_t get_update_page_num_safe(void)
{
    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    uint8_t page = update_page_num;
    rt_mutex_release(video_mutex);
    return page;
}

// 新增：安全计算当前页面的视频总数
static uint8_t get_video_count_safe(void)
{
    uint8_t page = get_update_page_num_safe();
    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    uint8_t count = page_mp4_update_flag[0][page] + page_mp4_update_flag[1][page] + page_mp4_update_flag[2][page] +
                    page_avi_update_flag[0][page] + page_avi_update_flag[1][page] + page_avi_update_flag[2][page];
    rt_mutex_release(video_mutex);
    return count;
}
// 重置标志
void reset_video_flag(void)
{
    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);

    break_seek_flag        = false;
    delete_video_flag      = false;
    play_ok_delete_video_flag = false;
    elevator_video_flag    = false;
    create_player_flag     = true;
    stop_video_flag        = false;
    have_two_video_flag    = false;

    play_num               = 0;
    last_play_tick         = 0;
    rt_mutex_release(video_mutex);

    memset(&my_lvgl_player_ctx, 0x00, sizeof(struct lvgl_player_context));
}

// 安全重启线程
void restart_video_thread(void)
{
    rt_thread_t tid = rt_thread_find("video");

    if (tid != RT_NULL)
    {
        video_renew = PRINTF_RENEW;
        destroy_video();
        rt_thread_mdelay(100);

        rt_thread_suspend(tid);
        rt_thread_delete(tid);
        rt_thread_mdelay(100);
    }

    reset_video_flag();

    video_thread = rt_thread_create(
        "video",
        video_thread_entry,
        RT_NULL,
        1024 * 24,
        10,
        20
    );

    if (video_thread != RT_NULL)
    {
        rt_thread_startup(video_thread);
        rt_kprintf("[视频] 线程重启成功\n");
    }
    else
    {
        rt_kprintf("[视频] 线程重启失败\n");
    }
}

static int set_disp_rect(struct lvgl_player_context *ctx)
{
    int ret = 0;

    if (!ctx->media_info.has_video) return 0;
    aic_player_get_screen_size(ctx->player, &ctx->screen_size);

    if (ctx->media_info.video_stream.width > VIDEO_HEIGHT)
    {
        ctx->disp_rect.x = 0;
        ctx->disp_rect.width = VIDEO_HEIGHT;
    }
    else
    {
        ctx->disp_rect.x = (ctx->screen_size.width - ctx->media_info.video_stream.width) / 2;
        ctx->disp_rect.width = ctx->media_info.video_stream.width;
    }

    if (ctx->media_info.video_stream.height > VIDEO_WIDTH)
    {
        ctx->disp_rect.y = 0;
        ctx->disp_rect.height = VIDEO_WIDTH;
    }
    else
    {
        ctx->disp_rect.y = (ctx->screen_size.height - ctx->media_info.video_stream.height) / 2;
        ctx->disp_rect.height = ctx->media_info.video_stream.height;
    }

    if (hengping)
    {
        ctx->disp_rect.x = 0;
        ctx->disp_rect.y = 0;
        ctx->disp_rect.width = 1024;
        ctx->disp_rect.height = 768;

        if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor)
        {
            ctx->disp_rect.x = 306;
            ctx->disp_rect.y = 125;
            ctx->disp_rect.width = 700;
            ctx->disp_rect.height = 520;
        }
        else if (MY_SET_IMAGE.image == IMAGE_C404_hor)
        {
            ctx->disp_rect.x = 257;
            ctx->disp_rect.y = 165;
            ctx->disp_rect.width = 777;
            ctx->disp_rect.height = 440;
        }
    }
    else
    {
        ctx->disp_rect.x = 0;
        ctx->disp_rect.y = 0;
        ctx->disp_rect.width = 1024;
        ctx->disp_rect.height = 768;

        if (MY_SET_IMAGE.image == IMAGE_C404_ver)
        {
            ctx->disp_rect.x = 31;
            ctx->disp_rect.y = 0;
            ctx->disp_rect.width = 437;
            ctx->disp_rect.height = 768;
        }
        if(video_udp_state == 1)
        {
            ctx->disp_rect.x = 150;
            ctx->disp_rect.y = 20;
            ctx->disp_rect.width = 400;
            ctx->disp_rect.height = 400;
        }
        aic_player_set_rotation(ctx->player, MPP_ROTATION_90);
    }


    ret = aic_player_set_disp_rect(ctx->player, &ctx->disp_rect);
    if (ret != 0)
    {
        printf("aic_player_set_disp_rect error\n");
        return -1;
    }
    return ret;
}

static s32 event_handle(void *app_data, s32 event, s32 data1, s32 data2)
{
    int ret = 0;
    struct lvgl_player_context *ctx = (struct lvgl_player_context *)app_data;

    switch (event)
    {
    case AIC_PLAYER_EVENT_PLAY_END:
        rt_kprintf("Play end\n");
        ctx->player_end = 1;
        ctx->player_state = LVGL_PLAYER_STATE_STOP;

        if (elevator_video_flag)
        {
            rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
            elevator_video_flag = false;
            play_ok_delete_video_flag = true;
            video_continue_flag = true;
            rt_mutex_release(video_mutex);
        }
        else
        {
            uint8_t video_count = get_video_count_safe();
            rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
            have_video_num = video_count;
            rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
            need_to_play_video_flag = true;
            rt_mutex_release(elevtor_mutex);
            if (have_video_num > 1)
            {
                have_two_video_flag = true;
            }
            else have_two_video_flag = false; // 明确设置为false

            video_continue_flag = true;
            break_seek_flag = false;
            last_play_tick = rt_tick_get(); // ✅ 播放结束立即更新时间，避免超时误触发
            rt_mutex_release(video_mutex);
        }
        break;

    case AIC_PLAYER_EVENT_PLAY_TIME:
        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        last_play_tick = rt_tick_get();
        rt_mutex_release(video_mutex);
        break;

    default:
        break;
    }
    return ret;
}

void video_set_volume(rt_uint16_t vol)
{
    // 🔴 修复：播放器为空 或 未播放，绝对不设置音量
    if (my_lvgl_player_ctx.player == NULL ||
        my_lvgl_player_ctx.player_state != LVGL_PLAYER_STATE_PLAY)
    {
        return;
    }

    // 正常播放时才调用底层API
    aic_player_set_volum(my_lvgl_player_ctx.player, vol);
}

int lvgl_play(struct lvgl_player_context *ctx)
{
    // aic_player_reset_sync(ctx->player);
    int ret = 0;
    // if (ctx->player_state == LVGL_PLAYER_STATE_PLAY)
    // {
    //     lvgl_stop(ctx);
    // }
    // ✅ 新增：重置播放结束标志
    ctx->player_end = 0;

    if (video_select == VIDEO_FIRE)
    {
        elevator_video_flag = true;
        const char *file = hengping ? LVGL_FILE_LIST_PATH(fire_hor.mp4) : LVGL_FILE_LIST_PATH(fire_ver.mp4);
        aic_player_set_uri(ctx->player, file);
    }
    else if (video_select == VIDEO_RESET_APPEASE)
    {
        elevator_video_flag = true;
        const char *file = hengping ? LVGL_FILE_LIST_PATH(anfu_help_hor.mp4) : LVGL_FILE_LIST_PATH(anfu_help_ver.mp4);
        aic_player_set_uri(ctx->player, file);
    }
    else if (video_select == VIDEO_RESET_HELP_APPEASE)
    {
        elevator_video_flag = true;
        const char *file = hengping ? LVGL_FILE_LIST_PATH(anfu_hor.mp4) : LVGL_FILE_LIST_PATH(anfu_ver.mp4);
        aic_player_set_uri(ctx->player, file);
    }
    else if (video_select == VIDEO_RESET_END_APPEASE)
    {
        elevator_video_flag = true;
        const char *file = hengping ? LVGL_FILE_LIST_PATH(anfu_end_hor.mp4) : LVGL_FILE_LIST_PATH(anfu_end_ver.mp4);
        aic_player_set_uri(ctx->player, file);
    }
    else if (video_select == VIDEO_OVERLOAD)
    {
        elevator_video_flag = true;
        const char *file = hengping ? LVGL_FILE_LIST_PATH(overload_hor.mp4) : LVGL_FILE_LIST_PATH(overload_ver.mp4);
        aic_player_set_uri(ctx->player, file);
    }
    else if (video_select == VIDEO_UP_OBS)
    {
        elevator_video_flag = true;
        const char *file = hengping ? LVGL_FILE_LIST_PATH(up_hor.mp4) : LVGL_FILE_LIST_PATH(up_ver.mp4);
        aic_player_set_uri(ctx->player, file);
    }
    else if (video_select == VIDEO_DOWN_OBS)
    {
        elevator_video_flag = true;
        const char *file = hengping ? LVGL_FILE_LIST_PATH(down_hor.mp4) : LVGL_FILE_LIST_PATH(down_ver.mp4);
        aic_player_set_uri(ctx->player, file);
    }
    else if (MY_SET_IMAGE.image == IMAGE_C301_hor)
        aic_player_set_uri(ctx->player, g_filename[0]);
    else if (MY_SET_IMAGE.image == IMAGE_C302_hor)
        aic_player_set_uri(ctx->player, g_filename[1]);
    else if (MY_SET_IMAGE.image == IMAGE_C303_hor)
        aic_player_set_uri(ctx->player, g_filename[2]);
    else if (page_mp4_update_flag[0][update_page_num] || page_mp4_update_flag[1][update_page_num] || page_mp4_update_flag[2][update_page_num] ||
             page_avi_update_flag[0][update_page_num] || page_avi_update_flag[1][update_page_num] || page_avi_update_flag[2][update_page_num])
    {
        uint8_t num = page_mp4_update_flag[0][update_page_num] + page_mp4_update_flag[1][update_page_num] + page_mp4_update_flag[2][update_page_num] +
                      page_avi_update_flag[0][update_page_num] + page_avi_update_flag[1][update_page_num] + page_avi_update_flag[2][update_page_num];

        if (num == 1)
        {
            if (page_mp4_update_flag[0][update_page_num]) aic_player_set_uri(ctx->player, video_mp4_path[0][update_page_num]);
            else if (page_mp4_update_flag[1][update_page_num]) aic_player_set_uri(ctx->player, video_mp4_path[1][update_page_num]);
            else if (page_mp4_update_flag[2][update_page_num]) aic_player_set_uri(ctx->player, video_mp4_path[2][update_page_num]);
            else if (page_avi_update_flag[0][update_page_num]) aic_player_set_uri(ctx->player, video_avi_path[0][update_page_num]);
            else if (page_avi_update_flag[1][update_page_num]) aic_player_set_uri(ctx->player, video_avi_path[1][update_page_num]);
            else if (page_avi_update_flag[2][update_page_num]) aic_player_set_uri(ctx->player, video_avi_path[2][update_page_num]);
            tcp_video_num = 0;
        }
        else if (num > 1)
        {
            if(is_tcp_connected && MY_SET_DHCP.host_state == false)
            {
                if(tcp_video_num == 0 && tcp_last_video_num != 0)
                {
                    tcp_video_num = tcp_last_video_num;
                    rt_kprintf("使用tcp_last_video_num:%d\n", tcp_last_video_num);
                }

                if(tcp_video_num == 1)
                {
                    if(page_mp4_update_flag[0][update_page_num])
                        aic_player_set_uri(ctx->player, video_mp4_path[0][update_page_num]);
                    else rt_kprintf("tcp_video_num == 1, 视频源不一致\n");
                }else if(tcp_video_num == 2)
                {
                    if(page_avi_update_flag[0][update_page_num])
                        aic_player_set_uri(ctx->player, video_avi_path[0][update_page_num]);
                    else rt_kprintf("tcp_video_num == 2, 视频源不一致\n");
                }else if(tcp_video_num == 3)
                {
                    if(page_mp4_update_flag[1][update_page_num])
                        aic_player_set_uri(ctx->player, video_mp4_path[1][update_page_num]);
                    else rt_kprintf("tcp_video_num == 2, 视频源不一致\n");
                }else if(tcp_video_num == 4)
                {
                    if(page_avi_update_flag[1][update_page_num])
                        aic_player_set_uri(ctx->player, video_avi_path[1][update_page_num]);
                    else rt_kprintf("tcp_video_num == 4, 视频源不一致\n");
                }else if(tcp_video_num == 5)
                {
                    if(page_mp4_update_flag[2][update_page_num])
                    aic_player_set_uri(ctx->player, video_mp4_path[2][update_page_num]);
                    else rt_kprintf("tcp_video_num == 5, 视频源不一致\n");
                }else if(tcp_video_num == 6)
                {
                    if(page_avi_update_flag[2][update_page_num])
                        aic_player_set_uri(ctx->player, video_avi_path[2][update_page_num]);
                    else rt_kprintf("tcp_video_num == 6, 视频源不一致\n");
                }
                tcp_last_video_num = tcp_video_num;
                tcp_video_num = 0;
            }else{
                if (page_mp4_update_flag[0][update_page_num] && play_num != 1 && play_num != 2)
                {
                    play_num = 1;
                    aic_player_set_uri(ctx->player, video_mp4_path[0][update_page_num]);
                }
                else if (page_avi_update_flag[0][update_page_num] && play_num != 1 && play_num != 2)
                {
                    play_num = 1;
                    aic_player_set_uri(ctx->player, video_avi_path[0][update_page_num]);
                }
                else if (page_mp4_update_flag[1][update_page_num] && play_num != 2)
                {
                    play_num = 2;
                    if (num == 2) play_num = 3;

                    aic_player_set_uri(ctx->player, video_mp4_path[1][update_page_num]);
                }
                else if (page_avi_update_flag[1][update_page_num] && play_num != 2)
                {
                    play_num = 2;
                    if (num == 2) play_num = 3;
                    aic_player_set_uri(ctx->player, video_avi_path[1][update_page_num]);
                }
                else if (page_mp4_update_flag[2][update_page_num] && play_num != 3)
                {
                    play_num = 3;
                    aic_player_set_uri(ctx->player, video_mp4_path[2][update_page_num]);
                }
                else if (page_avi_update_flag[2][update_page_num] && play_num != 3)
                {
                    play_num = 3;
                    aic_player_set_uri(ctx->player, video_avi_path[2][update_page_num]);
                }
            }
        }
    }
    else
    {
        aic_player_set_uri(ctx->player, g_filename[3]);
    }


    ctx->sync_flag = AIC_PLAYER_PREPARE_SYNC;
    ret = aic_player_prepare_sync(ctx->player);
    if (ret != 0)
    {
        rt_kprintf("prepare_sync 失败，错误码：%d\n", ret);
        return -1;
    }

    ret = aic_player_get_media_info(ctx->player, &ctx->media_info);
    if (ret != 0)
    {
        rt_kprintf("aic_player_get_media_info error\n");
        return -1;
    }
    // rt_kprintf("media_info.duration = %ld us", ctx->media_info.duration);
    ret = aic_player_start(ctx->player);
    if (ret != 0)
    {
        rt_kprintf("aic_player_start error\n");
        return -1;
    }

    ret = set_disp_rect(ctx);
    if (ret != 0)
    {
        rt_kprintf("set_disp_rect error\n");
        return -1;
    }

    ret = aic_player_play(ctx->player);
    if (ret != 0)
    {
        rt_kprintf("aic_player_play error\n");
        return -1;
    }
    return 0;
}

static int lvgl_pause(struct lvgl_player_context *ctx)
{
    return aic_player_pause(ctx->player);
}

int lvgl_stop(struct lvgl_player_context *ctx)
{
    if (ctx->player == NULL)
    {
        rt_kprintf("ctx->player == NULL\n");
        return -1;
    }

    // 仅停止视频，不操作音频销毁
    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
    bool wait_elevator_temp = wait_elevtor_flag;
    rt_mutex_release(elevtor_mutex);
    if(wait_elevator_temp) return -1;

    int ret = aic_player_stop(ctx->player);

    // ✅ 新增：重置播放状态和结束标志
    ctx->player_state = LVGL_PLAYER_STATE_STOP;
    ctx->player_end = 0;

    return ret;
}
int do_seek(struct lvgl_player_context *player_ctx,int forward)
{
    s64 pos;
    struct lvgl_player_context *ctx = player_ctx;
    pos = aic_player_get_play_time(ctx->player);
    if (pos == -1) {
        loge("aic_player_get_play_time error!!!!\n");
        return -1;
    }
    if (forward == 1) {
        pos += 8*1000*1000;//+8s
    } else {
        pos -= 8*1000*1000;//-8s
    }

    if (pos < 0) {
        pos = 0;
    } else if (pos < ctx->media_info.duration) {

    } else {
        pos = ctx->media_info.duration;
    }

    if (aic_player_seek(ctx->player,pos) != 0) {
        loge("aic_player_seek error!!!!\n");
        return -1;
    }
    logd("aic_player_seek ok\n");
    return 0;
}

int set_play_time(struct lvgl_player_context *player_ctx,int second)
{
    s64 pos;
    struct lvgl_player_context *ctx = player_ctx;
    // pos = aic_player_get_play_time(ctx->player);
    // if (pos == -1) {
    //     loge("aic_player_get_play_time error!!!!\n");
    //     return -1;
    // }
    // if (forward == 1) {
    //     pos += 8*1000*1000;//+8s
    // } else {
    //     pos -= 8*1000*1000;//-8s
    // }
    pos = (second * 1000 * 1000) + 50000;
    if (pos < 0) {
        pos = 0;
    } else if (pos < ctx->media_info.duration) {

    } else {
        pos = ctx->media_info.duration;
    }

    if (aic_player_seek(ctx->player,pos) != 0) {
        loge("aic_player_seek error!!!!\n");
        return -1;
    }
    logd("aic_player_seek ok\n");
    return 0;
}

int lvgl_play_next(struct lvgl_player_context *ctx)
{
    if (ctx->file_cnt == 0) ctx->file_cnt = 1;

    ctx->file_index++;
    if (ctx->file_index >= ctx->file_cnt) ctx->file_index = 0;

    lvgl_stop(ctx);
    return lvgl_play(ctx);
}

void create_player()
{
    if (is_tcp_connected && MY_SET_DHCP.host_state == false)
    {
        if(my_lvgl_player_ctx.player != NULL)
        {
            rt_kprintf("视频播放器已存在，无需重新创建\n");
            return;
        }
    }
    memset(&my_lvgl_player_ctx, 0x00, sizeof(struct lvgl_player_context));
    my_lvgl_player_ctx.player = aic_player_create(NULL);

    if (my_lvgl_player_ctx.player == NULL)
    {
        rt_kprintf("视频创建失败，重启系统\n");
        wdt_immediate_reset();
        return;
    }
    rt_kprintf("视频播放器创建成功\n");

    my_lvgl_player_ctx.file_cnt = 1;
    my_lvgl_player_ctx.file_index = 0;
    my_lvgl_player_ctx.player_state = LVGL_PLAYER_STATE_STOP;
    my_lvgl_player_ctx.player_end = 0; // ✅ 新增：初始化播放结束标志

    aic_player_set_event_callback(my_lvgl_player_ctx.player, &my_lvgl_player_ctx, event_handle);

    if (lvgl_play(&my_lvgl_player_ctx) != 0)
    {
        my_lvgl_player_ctx.player_state = LVGL_PLAYER_STATE_STOP;
        my_lvgl_player_ctx.player_end = 1;
    }
    else
    {
        my_lvgl_player_ctx.player_state = LVGL_PLAYER_STATE_PLAY;
        my_lvgl_player_ctx.player_end = 0;
    }
}

static struct aicfb_alpha_config ui_alpha_bak = {0};
static struct aicfb_alpha_config video_alpha_bak = {0};
static bool alpha_saved = false;

void video_init(bool play, uint8_t select)
{
    rt_device_t render_dev = rt_device_find("aicfb");
    if (!render_dev)
    {
        rt_kprintf("rt_device_find aicfb failed!\n");
        return;
    }

    if (!alpha_saved)
    {
        ui_alpha_bak.layer_id = AICFB_LAYER_TYPE_UI;
        rt_device_control(render_dev, AICFB_GET_ALPHA_CONFIG, &ui_alpha_bak);

        video_alpha_bak.layer_id = AICFB_LAYER_TYPE_VIDEO;
        rt_device_control(render_dev, AICFB_GET_ALPHA_CONFIG, &video_alpha_bak);
        alpha_saved = true;
    }

    if (select == 0)
    {
        rt_device_control(render_dev, AICFB_UPDATE_ALPHA_CONFIG, &ui_alpha_bak);
        struct aicfb_alpha_config alpha = {0};
        alpha.layer_id = AICFB_LAYER_TYPE_VIDEO;
        alpha.enable = 1;
        alpha.mode = 1;
        alpha.value = play ? 255 : 0;
        rt_device_control(render_dev, AICFB_UPDATE_ALPHA_CONFIG, &alpha);
    }
    else if (select == 1)
    {
        rt_device_control(render_dev, AICFB_UPDATE_ALPHA_CONFIG, &video_alpha_bak);
        struct aicfb_alpha_config alpha = {0};
        alpha.layer_id = AICFB_LAYER_TYPE_UI;
        alpha.enable = 1;
        alpha.mode = 1;
        alpha.value = play ? 0 : 255;
        rt_device_control(render_dev, AICFB_UPDATE_ALPHA_CONFIG, &alpha);
    }

    bool need_create;
    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    need_create = create_player_flag;
    rt_mutex_release(video_mutex);

    if (need_create)
    {
        create_player();
        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        create_player_flag = false;
        rt_mutex_release(video_mutex);
    }
}

int destroy_player(uint8_t printf_text)
{
    printf_text--;
    const char *log_text[] = {"切换模式", "更新视频", "电梯状态", "定位失败", "节能模式", "无视频文件"};
    struct lvgl_player_context *ctx = &my_lvgl_player_ctx;

    if (ctx->player == NULL)
    {
        rt_kprintf("播放器已空，无需销毁\n");
        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        init_video_flag = true;
        create_player_flag = true;
        rt_mutex_release(video_mutex);
        return 0;
    }
    while(1)
    {
        rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
        bool wait_elevator_temp = wait_elevtor_flag;
        rt_mutex_release(elevtor_mutex);

        if(!wait_elevator_temp) break;
        rt_thread_mdelay(50);
    }
    int state = aic_player_destroy(ctx->player);
    if (state == 0)
    {
        if (printf_text < 6)
            rt_kprintf("播放器销毁成功[%s]\n", log_text[printf_text]);
        else
            rt_kprintf("播放器销毁成功[未知]\n");

        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        ctx->player = NULL;
        init_video_flag = true;
        create_player_flag = true;
        video_continue_flag = true;
        rt_mutex_release(video_mutex);
        return 0;
    }
    else
    {
        rt_kprintf("播放器销毁失败！\n");
        return -1;
    }
}

void destroy_video(void)
{
    // 多视频模式：不销毁播放器
    if (is_multi_video_resident() && video_renew != PRINTF_RENEW)
    {
        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        video_renew = PRINTF_NONE;
        break_seek_flag = true;
        rt_mutex_release(video_mutex);
        // rt_kprintf("[多视频] 跳过销毁\n");
        return;
    }

    uint8_t current_renew;
    bool audio_render_active;

    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    current_renew = video_renew;
    rt_mutex_release(video_mutex);

    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
    audio_render_active = wait_elevtor_flag;
    rt_mutex_release(elevtor_mutex);

    if (current_renew != PRINTF_NONE && !audio_render_active)
    {
        destroy_player(current_renew);

        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        break_seek_flag = true;
        video_renew = PRINTF_NONE;
        have_two_video_flag = false;  // ✅ 强制重置多视频标志（核心修复）
        init_video_flag = true;       // ✅ 强制触发重新初始化
        video_continue_flag = true;   // ✅ 强制触发继续播放
        create_player_flag = true;
        rt_mutex_release(video_mutex);
        rt_kprintf("video_renew 复位\n");
    }
}

void set_volume_in_video(uint8_t vol)
{
    video_set_volume(vol);
    if (vol != 0 && !energy_conservation)
    {
        sound_state = PIN_HIGH;
        Sound_Init(PIN_HIGH);
    }
    else
    {
        sound_state = PIN_LOW;
        Sound_Init(PIN_LOW);
    }
}

void state_video_end(void)
{
    rt_kprintf("状态视频播放结束\n");
    if (fire_start_flag || overload_start_flag || reset_appease_start_flag || reset_end_appease_start_flag ||
        reset_help_appease_start_flag || up_obs_start_flag || down_obs_start_flag)
    {
        if (MY_SET.play_mode == PLAY_VIDEO)
        {
            if (fire_start_flag && video_select != VIDEO_OVERLOAD)
            { fire_start_flag = false; if(!energy_conservation) fire_music_pcm_flag = true; }
            else if (reset_appease_start_flag)
            { reset_appease_start_flag = false; if(!energy_conservation) reset_appease_music_pcm_flag = true; }
            else if (reset_help_appease_start_flag)
            { reset_help_appease_start_flag = false; if(!energy_conservation) reset_help_appease_music_pcm_flag = true; }
            else if (reset_end_appease_start_flag)
            { reset_end_appease_start_flag = false; if(!energy_conservation) reset_end_appease_music_pcm_flag = true; }
            else if (overload_start_flag)
            { overload_start_flag = false; if(!energy_conservation) overload_music_pcm_flag = true; }
            else if (up_obs_start_flag)
            { up_obs_start_flag = false; up_obs_music_pcm_flag = true; }
            else if (down_obs_start_flag)
            { down_obs_start_flag = false; down_obs_music_pcm_flag = true; }
        }
        else
        {
            if (fire_start_flag && video_select != VIDEO_OVERLOAD)
            { fire_start_flag = false; if(!energy_conservation) fire_music_mp3_flag = true; }
            else if (reset_appease_start_flag)
            { reset_appease_start_flag = false; if(!energy_conservation) reset_appease_music_mp3_flag = true; }
            else if (reset_help_appease_start_flag)
            { reset_help_appease_start_flag = false; if(!energy_conservation) reset_help_appease_music_mp3_flag = true; }
            else if (reset_end_appease_start_flag)
            { reset_end_appease_start_flag = false; if(!energy_conservation) reset_end_appease_music_mp3_flag = true; }
            else if (overload_start_flag)
            { overload_start_flag = false; if(!energy_conservation) overload_music_mp3_flag = true; }
            else if (up_obs_start_flag)
            { up_obs_start_flag = false; up_obs_music_mp3_flag = true; }
            else if (down_obs_start_flag)
            { down_obs_start_flag = false; down_obs_music_mp3_flag = true; }
        }
    }

    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    play_ok_delete_video_flag = false;
    now_play_video_num = 0;
    delete_video_flag = false;
    state_video_end_flag = true;
    video_continue_flag = true;
    init_video_flag = true;
    create_player_flag = true;
    have_two_video_flag = false;
    rt_mutex_release(video_mutex);

    destroy_player(PRINTF_ELEVTOR);
    video_select = VIDEO_NONE;

    in_reset_cn_flag = false;
    in_reset_help_cn_flag = false;
    in_reset_end_cn_flag = false;
    in_video_state_flag = false;
}

void deal_in_update(void)
{
    if (video_in_updating && !stop_video_flag)
    {
        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        last_play_tick = 0;
        stop_video_flag = true;
        video_renew = PRINTF_RENEW;
        rt_mutex_release(video_mutex);
        destroy_video();
    }
    else if (stop_video_flag && !video_in_updating)
    {
        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        last_play_tick = 0;
        stop_video_flag = false;
        video_continue_flag = true;
        rt_mutex_release(video_mutex);
    }
}

void destroy_player_after_video(bool state)
{
    if (state)
    {
        // 多视频：不销毁
        if (is_multi_video_resident())
        {
            rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
            video_continue_flag = true;
            break_seek_flag = true;
            delete_video_flag = false;
            rt_mutex_release(video_mutex);
            rt_kprintf("[多视频] 不销毁，切下一个\n");
            return;
        }

        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        delete_video_flag = true;
        init_video_flag = true;
        create_player_flag = true;
        video_continue_flag = true;
        break_seek_flag = true;
        rt_mutex_release(video_mutex);

        if (MY_SET.play_mode == PLAY_VIDEO)
        {
            destroy_player(PRINTF_ELEVTOR);
        }
        else if (MY_SET.play_mode == PLAY_IMAGE)
        {
            music_renew_flag = true;
            rt_thread_mdelay(1000);
        }
    }
}

void continue_play_video(bool state)
{
    if (!state) return;

    rt_kprintf("继续播放视频\n");
    bool need_init;
    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    need_init = init_video_flag;
    rt_mutex_release(video_mutex);

    if (need_init)
    {
        if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
            MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
            video_init(true, 1);
        else
            video_init(true, 0);

        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        init_video_flag = false;
        rt_mutex_release(video_mutex);
    }

    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
    video_continue_flag = false;
    rt_mutex_release(video_mutex);
}

void user_play_video(void)
{
    bool need_init;
    rt_mutex_take(audio_mutex, RT_WAITING_FOREVER);
    need_init = init_video_flag;
    rt_mutex_release(audio_mutex);
    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
    bool user_wait_elevator_temp = wait_elevtor_flag;
    rt_mutex_release(elevtor_mutex);

    if (need_init && !user_wait_elevator_temp)
    {
        if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
        {
            uint8_t num = page_mp4_update_flag[0][update_page_num] + page_mp4_update_flag[1][update_page_num] + page_mp4_update_flag[2][update_page_num] +
                      page_avi_update_flag[0][update_page_num] + page_avi_update_flag[1][update_page_num] + page_avi_update_flag[2][update_page_num];
            if(num)
            {
                if (page_mp4_update_flag[0][update_page_num] && play_num != 1 && play_num != 2)
                {
                    // tcp_video_num = 1;
                    tcp_send_raw("vin1", 4);
                }
                else if (page_avi_update_flag[0][update_page_num] && play_num != 1 && play_num != 2)
                {
                    // tcp_video_num = 2;
                    tcp_send_raw("vin2", 4);
                }
                else if (page_mp4_update_flag[1][update_page_num] && play_num != 2)
                {
                    // tcp_video_num = 3;
                    tcp_send_raw("vin3", 4);
                }
                else if (page_avi_update_flag[1][update_page_num] && play_num != 2)
                {
                    // tcp_video_num = 4;
                    tcp_send_raw("vin4", 4);
                }
                else if (page_mp4_update_flag[2][update_page_num] && play_num != 3)
                {
                    // tcp_video_num = 5;
                    tcp_send_raw("vin5", 4);
                }
                else if (page_avi_update_flag[2][update_page_num] && play_num != 3)
                {
                    // tcp_video_num = 6;
                    tcp_send_raw("vin6", 4);
                }
            }else{
                tcp_send_raw("vint", 4);
            }

            last_recv_tick = rt_tick_get();
            rt_kprintf("vint\n");
            rt_thread_mdelay(100);
        }
        if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
            MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
            video_init(true, 1);
        else
            video_init(true, 0);

        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        init_video_flag = false;
        rt_mutex_release(video_mutex);
    }
}

void seek_to_start_play_video(void)
{
    struct lvgl_player_context *ctx = &my_lvgl_player_ctx;
    if (ctx->player == NULL)
    {
        rt_kprintf("seek 失败：播放器为空\n");
        destroy_player(PRINTF_SEEK);
        return;
    }

    ctx->player_end = 0;
    int ret = aic_player_seek(ctx->player, 0);

    if (ret == 0)
    {
        rt_kprintf("seek 成功\n");
    }
    else
    {
        rt_kprintf("seek 失败，销毁重建\n");
        destroy_player(PRINTF_SEEK);
    }
}

void delete_player_after_home(void)
{
    bool need_destroy;
    bool handle_home_hor, handle_home;

    rt_mutex_take(audio_mutex, RT_WAITING_FOREVER);
    // 仅判断主页退出标志
    need_destroy = (home_hor_delete_flag || home_delete_flag);
    // 允许销毁的条件：未初始化 + 音频渲染器空闲

    handle_home_hor = home_hor_delete_flag;
    handle_home = home_delete_flag;

    // 复位标志位
    home_hor_delete_flag = false;
    home_delete_flag = false;
    rt_mutex_release(audio_mutex);

    // ============== 核心修改：退出主页，强制销毁 ==============
    if (need_destroy)
    {
        int ret, retry = 0;
        // 重试5次，确保播放器销毁成功
        do {
            ret = destroy_player(PRINTF_ELEVTOR);
            if (ret)
                rt_thread_mdelay(100);
        } while (ret && retry++ < 5);

        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        alpha_saved = false;
        break_seek_flag = true;
        // 退出主页，清空计时
        last_play_tick = 0;
        // 强制复位多视频标志，彻底重置
        have_two_video_flag = false;

        // ========== 核心：打开重启开关（退回主页必重播） ==========
        init_video_flag = true;       // 允许重新初始化
        create_player_flag = true;    // 允许重新创建播放器
        video_continue_flag = true;   // 允许重新播放

        if (handle_home_hor) {
            home_hor_video_flag = true;
        }
        if (handle_home) {
            home_video_flag = true;
        }
        rt_mutex_release(video_mutex);

        rt_kprintf("[退出主页] 视频播放器已强制销毁\n");
    }
}

void video_thread_init(void)
{
    while (!init_set_img_ok) rt_thread_mdelay(200);
    rt_thread_mdelay(200);

    while (!music_des_flag)
    {
        if (MY_SET.play_mode == PLAY_VIDEO || video_select) break;
        rt_thread_mdelay(50);
    }

    while ((MY_SET.play_mode != PLAY_VIDEO && !video_select))
    {
        rt_thread_mdelay(50);
    }

    if (!video_select && !video_in_updating)
    {
        while (my_page != PAGE_HOME && my_page != PAGE_HOME_HOR) rt_thread_mdelay(50);

        if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
            MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
            video_init(true, 1);
        else
            video_init(true, 0);

        rt_thread_mdelay(1000);
    }
    else
    {
        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        break_seek_flag = true;
        rt_mutex_release(video_mutex);
    }
}


void video_thread_entry(void *parameter)
{
    static bool set_volum_level_flag = false;
    video_thread_init();

    while (1)
    {
        bool need_set_vol;
        if (rt_mutex_take(volume_mutex, RT_TICK_PER_SECOND / 10) == RT_EOK)
        {
            need_set_vol = set_video_vol_flag;
            rt_mutex_release(volume_mutex);
        }

        if (need_set_vol)
        {
            bool need_update;
            set_volum_level_flag = true;
            rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
            need_update = !update_energy_flag;
            rt_mutex_release(video_mutex);

            rt_thread_mdelay(100);
            if (need_update) set_volume_in_video(MY_SET.sound);

            uint8_t set_vol_cnt_temp = 0;
            if (rt_mutex_take(volume_mutex, RT_TICK_PER_SECOND / 10) == RT_EOK)
            {
                set_vol_cnt_temp = (++set_vol_cnt);
                rt_mutex_release(volume_mutex);
            }
            if (set_vol_cnt_temp > 5)
            {
                if (rt_mutex_take(volume_mutex, RT_TICK_PER_SECOND / 10) == RT_EOK)
                {
                    set_vol_cnt = 0;
                    set_video_vol_flag = false;
                    rt_mutex_release(volume_mutex);
                }
                set_volum_level_flag = false;
                rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                update_energy_flag = false;
                rt_mutex_release(video_mutex);
            }
        }

        if ((MY_SET.play_mode == PLAY_VIDEO || video_select) && !video_in_updating &&
            !play_elevator.elevator_arr && !play_elevator.elevator_up && !play_elevator.elevator_down)
        {
            if (!energy_conservation)
            {
                if (set_state != 2 || set_vol_flag)
                {
                    set_vol_flag = false;
                    set_state = 2;
                    set_volume_in_video(MY_SET.sound);
                }
            }
            else
            {
                if (set_state != 1)
                {
                    set_state = 1;
                    rt_kprintf("[节能模式] 视频静音\n");
                    sound_state = PIN_LOW;
                    Sound_Init(PIN_LOW);
                }
            }
        }

        if (!video_select && !video_in_updating && !have_overload_flag && !home_hor_video_flag && !home_video_flag)
        {
            uint8_t play_en;
            bool continue_play, is_multi_video;
            rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
            continue_play = video_continue_flag;
            is_multi_video = have_two_video_flag; // 先获取是否是多视频
            rt_mutex_release(video_mutex);
            // 用音频锁检查音频状态
            rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);



            // ======================
            if (is_multi_video) {
                // 多视频：等待电梯音频播放完毕
                play_en = (continue_play && MY_SET.play_mode == PLAY_VIDEO &&
                    !wait_elevtor_flag &&
                    (my_page == PAGE_HOME || my_page == PAGE_HOME_HOR));
            } else {
                // 单个视频：直接播放，不等待任何音频
                play_en = (continue_play && MY_SET.play_mode == PLAY_VIDEO &&
                    (my_page == PAGE_HOME || my_page == PAGE_HOME_HOR));
            }
            if(is_tcp_connected && MY_SET_DHCP.host_state == false)
                play_en = false;
            rt_mutex_release(elevtor_mutex);

            if (play_en)
            {
                user_play_video();
                rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                video_continue_flag = false;
                bool have_two_temp = have_two_video_flag;
                rt_mutex_release(video_mutex);

                // 多视频直接切下一个，不结束、不销毁
                if (have_two_temp)
                {
                    rt_kprintf("[多视频] 等待电梯音频播放完毕\n");
                    // 🔴 修复：循环检查电梯标志，直到音频播放结束
                    while(1)
                    {
                        rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                        bool wait_elevator_temp = wait_elevtor_flag;
                        rt_mutex_release(elevtor_mutex);

                        if(!wait_elevator_temp) break;
                        rt_thread_mdelay(50);
                    }

                    if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
                    {
                        if (page_mp4_update_flag[0][update_page_num] && play_num != 1 && play_num != 2)
                        {
                            tcp_send_raw("v1mp", 4);
                            rt_kprintf("发送 v1mp\n");
                        }
                        else if (page_avi_update_flag[0][update_page_num] && play_num != 1 && play_num != 2)
                        {
                            tcp_send_raw("v1av", 4);
                            rt_kprintf("发送 v1av\n");
                        }
                        else if (page_mp4_update_flag[1][update_page_num] && play_num != 2)
                        {
                            tcp_send_raw("v2mp", 4);
                            rt_kprintf("发送 v2mp\n");
                        }
                        else if (page_avi_update_flag[1][update_page_num] && play_num != 2)
                        {
                           tcp_send_raw("v2av", 4);
                           rt_kprintf("发送 v2av\n");
                        }
                        else if (page_mp4_update_flag[2][update_page_num] && play_num != 3)
                        {
                            tcp_send_raw("v3mp", 4);
                            rt_kprintf("发送 v3mp\n");
                        }
                        else if (page_avi_update_flag[2][update_page_num] && play_num != 3)
                        {
                            tcp_send_raw("v3av", 4);
                            rt_kprintf("发送 v3av\n");
                        }
                        last_recv_tick = rt_tick_get();
                        rt_thread_mdelay(100);
                    }
                    rt_kprintf("[多视频] 电梯音频播放完毕，开始切换\n");
                    lvgl_stop(&my_lvgl_player_ctx);
                    rt_kprintf("stop ok\n");
                    // rt_thread_mdelay(100);
                    // 新增：切视频前重置同步
                    // ✅ 官方API：seek到0，重置音视频同步（替代reset_sync）
                    // aic_player_seek(my_lvgl_player_ctx.player, 0);
                    lvgl_play(&my_lvgl_player_ctx);
                    rt_kprintf("play ok\n");

                    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                    last_play_tick = rt_tick_get();
                    have_two_video_flag = false;    // ✅ 新增：重置多视频标志
                    rt_mutex_release(video_mutex);
                    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                    need_to_play_video_flag = false;
                    rt_mutex_release(elevtor_mutex);
                    continue;
                }

                bool skip_seek;
                rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                skip_seek = break_seek_flag;
                rt_mutex_release(video_mutex);

                if (skip_seek)
                {
                    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                    break_seek_flag = false;
                    if (energy_conservation) update_energy_flag = true;
                    rt_mutex_release(video_mutex);
                    rt_kprintf("首次播放,跳过seek\n");
                    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                    need_to_play_video_flag = false;
                    rt_mutex_release(elevtor_mutex);
                }
                else
                {
                    uint8_t num = page_mp4_update_flag[0][update_page_num] + page_mp4_update_flag[1][update_page_num] + page_mp4_update_flag[2][update_page_num] +
                                  page_avi_update_flag[0][update_page_num] + page_avi_update_flag[1][update_page_num] + page_avi_update_flag[2][update_page_num];
                    if (num <= 1)
                    {
                        if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
                        {
                            tcp_send_raw("seek", 4);
                            last_recv_tick = rt_tick_get();
                            if(tcp_return_home_flag)
                                rt_thread_mdelay(150);
                            else
                                rt_thread_mdelay(100);
                            rt_kprintf("发送seek 1\n");
                        }
                        if(is_tcp_connected && MY_SET_DHCP.host_state == false)
                        {

                        }else{
                            seek_to_start_play_video();
                            rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                            need_to_play_video_flag = false;
                            rt_mutex_release(elevtor_mutex);
                        }
                    }
                }
            }
        }

        // ========== 关键修复：多视频不走state_video_end，不销毁播放器 ==========
        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
        video_continue_flag = false;
        bool have_two_video_temp = have_two_video_flag;
        rt_mutex_release(video_mutex);
        if (play_ok_delete_video_flag && !have_two_video_temp)
        {
            state_video_end();
        }

        else if (video_select && (!energy_conservation ||
                                 video_select == VIDEO_FIRE || video_select == VIDEO_OVERLOAD ||
                                 video_select == VIDEO_RESET_END_APPEASE || video_select == VIDEO_RESET_HELP_APPEASE ||
                                 video_select == VIDEO_RESET_APPEASE))
        {
            destroy_player_after_video(!delete_video_flag);
            continue_play_video(video_continue_flag);
        }

        if (video_select && my_page != PAGE_HOME && my_page != PAGE_HOME_HOR)
        {
            destroy_player(PRINTF_ELEVTOR);
            rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
            break_seek_flag = true;
            video_renew = PRINTF_NONE;
            rt_mutex_release(video_mutex);
        }
        deal_in_update();
        delete_player_after_home();
        destroy_video();

        // ========== 关键修复：多视频直接屏蔽5秒超时重启 ==========

        if (is_tcp_connected && MY_SET_DHCP.host_state == false)
        {

        }else{

            if (!video_select && MY_SET.play_mode == PLAY_VIDEO && !video_in_updating)
            {
                bool audio_occupied;
                rt_tick_t current_tick;
                rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                current_tick = last_play_tick;
                rt_mutex_release(video_mutex);

                rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                audio_occupied = wait_elevtor_flag;
                rt_mutex_release(elevtor_mutex);
                // 无任何限制：单视频/多视频，只要卡住5秒，强制重启
                // 🔴 新增：音频被占用时，更新last_play_tick，避免超时
                if(audio_occupied)
                {
                    rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                    last_play_tick = rt_tick_get();
                    rt_mutex_release(video_mutex);
                }
                // 只有音频空闲时才检查超时
                else if (current_tick != 0)
                {
                    rt_tick_t now = rt_tick_get();
                    if (now - current_tick > VIDEO_TIMEOUT_TICK)
                    {
                        rt_kprintf("\n!!! 视频卡住2秒,强制重启播放器(多视频生效) !!!\n");
                        tcp_wait_des_flag = true;
                        if(is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
                        {
                            tcp_send_raw("vdes", 4);
                            last_recv_tick = rt_tick_get();
                            rt_kprintf("vdes\n");
                        }
                        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                        video_renew = PRINTF_RENEW;
                        last_play_tick = 0;
                        have_two_video_flag = false;  // 强制清空多视频状态
                        rt_mutex_release(video_mutex);
                    }
                }
            }
        }
        static bool frist_tcp_flag = false;
        if(is_tcp_connected && !frist_tcp_flag)
        {
            rt_kprintf("tcp连接成功,开始同步\n");
            frist_tcp_flag = true;
            if (is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
            {
                send_light(false, MY_SET.backlight);
            }
            if(my_weather.weather != 0 || my_weather.temperature != 0)
            {
                rt_thread_mdelay(100);
                char tcp_temp_str[16] = {0};
                int len = snprintf(tcp_temp_str, sizeof(tcp_temp_str), "we:%d,%d", my_weather.temperature, my_weather.weather);
                tcp_send_raw(tcp_temp_str, len);
            }

            if(MY_SET.play_mode == PLAY_VIDEO && (my_page == PAGE_HOME || my_page == PAGE_HOME_HOR))
            {
                rt_thread_mdelay(100);
                tcp_raw_time_flag = true;

                if(MY_SET_DHCP.host_state == true)
                {
                    uint8_t num = page_mp4_update_flag[0][update_page_num] + page_mp4_update_flag[1][update_page_num] + page_mp4_update_flag[2][update_page_num] +
                                        page_avi_update_flag[0][update_page_num] + page_avi_update_flag[1][update_page_num] + page_avi_update_flag[2][update_page_num];
                    if (num <= 1)
                    {
                        tcp_send_raw("seek", 4);
                        last_recv_tick = rt_tick_get();
                        rt_thread_mdelay(100);
                        seek_to_start_play_video();
                        rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                        need_to_play_video_flag = false;
                        rt_mutex_release(elevtor_mutex);
                    }else
                    {
                        rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                        need_to_play_video_flag = true;
                        rt_mutex_release(elevtor_mutex);
                        while(1)
                        {
                            rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                            bool wait_elevator_temp = wait_elevtor_flag;
                            rt_mutex_release(elevtor_mutex);

                            if(!wait_elevator_temp) break;
                            rt_thread_mdelay(50);
                        }

                        if (page_mp4_update_flag[0][update_page_num] && play_num != 1 && play_num != 2)
                        {
                            tcp_send_raw("v1mp", 4);
                            rt_kprintf("发送 v1mp\n");
                        }
                        else if (page_avi_update_flag[0][update_page_num] && play_num != 1 && play_num != 2)
                        {
                            tcp_send_raw("v1av", 4);
                            rt_kprintf("发送 v1av\n");
                        }
                        else if (page_mp4_update_flag[1][update_page_num] && play_num != 2)
                        {
                            tcp_send_raw("v2mp", 4);
                            rt_kprintf("发送 v2mp\n");
                        }
                        else if (page_avi_update_flag[1][update_page_num] && play_num != 2)
                        {
                            tcp_send_raw("v2av", 4);
                            rt_kprintf("发送 v2av\n");
                        }
                        else if (page_mp4_update_flag[2][update_page_num] && play_num != 3)
                        {
                            tcp_send_raw("v3mp", 4);
                            rt_kprintf("发送 v3mp\n");
                        }
                        else if (page_avi_update_flag[2][update_page_num] && play_num != 3)
                        {
                            tcp_send_raw("v3av", 4);
                            rt_kprintf("发送 v3av\n");
                        }
                        last_recv_tick = rt_tick_get();
                        rt_thread_mdelay(100);
                        rt_kprintf("[多视频] 电梯音频播放完毕，开始切换\n");
                        lvgl_stop(&my_lvgl_player_ctx);
                        rt_kprintf("stop ok\n");
                        lvgl_play(&my_lvgl_player_ctx);
                        rt_kprintf("play ok\n");
                        rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                        last_play_tick = rt_tick_get();
                        have_two_video_flag = false;    // ✅ 新增：重置多视频标志
                        rt_mutex_release(video_mutex);
                        rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                        need_to_play_video_flag = false;
                        rt_mutex_release(elevtor_mutex);
                        continue;
                    }
                }
            }
        }else if(!is_tcp_connected && frist_tcp_flag)
        {
            frist_tcp_flag = false;
        }

        rt_thread_mdelay(80);
    }
}
