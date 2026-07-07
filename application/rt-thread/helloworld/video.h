#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "main.h"

#define LVGL_PLAYER_STATE_PLAY 1
#define LVGL_PLAYER_STATE_PAUSE 2
#define LVGL_PLAYER_STATE_STOP 3

#define VIDEO_X_START 0
#define VIDEO_Y_START 0

/*竖屏时的分辨率*/
#define VIDEO_WIDTH 768
#define VIDEO_HEIGHT 1024

extern struct lvgl_player_context my_lvgl_player_ctx;

// 播放器上下文结构体，用于存储播放器相关的各种状态和参数信息
struct lvgl_player_context
{
    int file_cnt;
    int file_index;
    int player_state;
    struct aic_player *player;
    int sync_flag;
    struct av_media_info media_info;
    int demuxer_detected_flag;
    int player_end;
    struct mpp_size screen_size;
    struct mpp_rect disp_rect;
};

typedef enum
{
    PRINTF_NONE = 0, // 不打印任何信息
    PRINTF_CHANGE,  // 模式切换
    PRINTF_RENEW,   // 视频更新
    PRINTF_ELEVTOR, //电梯状态
    PRINTF_SEEK,    //视频跳转失败
    PRINTF_ENERGY, // 节能模式
    PRINTF_NO_SUPER, // 无视频文件
} SELECT_PRINTF_ID;

void video_init(bool play, uint8_t select);
void video_thread_entry(void *parameter);
void destroy_video(void);
void restart_video_thread(void);
void seek_to_start_play_video(void);
void create_player();
void video_set_volume(rt_uint16_t vol);
int lvgl_play(struct lvgl_player_context *ctx);
int lvgl_stop(struct lvgl_player_context *ctx);
int lvgl_play_next(struct lvgl_player_context *ctx);

#endif
