#ifndef _MUSIC_H_
#define _MUSIC_H_

#include "main.h"

struct my_aic_audio_frame {
    s32  sample_rate;
    s32  bits_per_sample;
    s32  channels;
    s64  pts;
    s32  id;
    void  *data;
    u32  size;
    u32  flag;
};

struct my_aic_audio_decode_config {
    s32 packet_buffer_size;				// video bytestream size
    s32 packet_count;				// packet buffer count
    s32 frame_count;				// packet buffer count
};

// struct aic_audio_decode_params {
//     s32  sample_rate;
//     s32  bits_per_sample;
//     s32  channels;
//     s32  bit_rate;
//     s32  block_align;               // number of bytes per packet
//     void  *extradata;
//     int  extradata_size;
// };

struct music_audio_info {
    s64 file_size;
    s64 duration;
    s32 nb_channel;
    s32 bits_per_sample;
    s32 sample_rate;
};

struct mini_audio_player {
    char uri[128];
    int type;
    int fd;
    int state;
    int force_stop;
    int volume;
    char *wav_buff;
    int wav_buff_size;
    struct my_aic_audio_frame frame_info;
    struct aic_audio_decoder *decoder;
    struct my_aic_audio_decode_config dec_cfg;
    struct aic_audio_render *render;
    struct aic_parser *parser;
    struct music_audio_info audio_info;
    aicos_thread_t tid;
    aicos_queue_t mq;
    aicos_mutex_t lock;
    aicos_sem_t sem_thread_exit;
    aicos_sem_t sem_ack;
    aicos_sem_t stop_ack;
    int loop;
};

#define MP3_FIRE_CN MUSIC_PATH(YM62_8.mp3)        // 消防中文播报
#define MP3_FIRE_EN MUSIC_PATH(YE62_8.mp3)        // 消防英文播报
#define MP3_UP_CN MUSIC_PATH(WMUP.mp3)            // 升降中文播报
#define MP3_UP_EN MUSIC_PATH(WEUP.mp3)            // 升降英文播报
#define MP3_DOWN_CN MUSIC_PATH(WMDN.mp3)          // 升降中文播报
#define MP3_DOWN_EN MUSIC_PATH(WEDN.mp3)          // 升降英文播报
#define MP3_OPEN_CN MUSIC_PATH(YM63_1.mp3)        // 开门中文播报
#define MP3_OPEN_EN MUSIC_PATH(YE63_1.mp3)        // 开门英文播报
#define MP3_CLOSE_CN MUSIC_PATH(YM63_2.mp3)       // 关门中文播报
#define MP3_CLOSE_EN MUSIC_PATH(YE63_2.mp3)       // 关门英文播报
#define MP3_PEAK_CN MUSIC_PATH(YM63_3.mp3)        // 高峰中文播报
#define MP3_PEAK_EN MUSIC_PATH(YE63_3.mp3)        // 高峰英文播报
#define MP3_OVERLOAD_CN MUSIC_PATH(YM62_5.mp3)    // 超载中文播报
#define MP3_OVERLOAD_EN MUSIC_PATH(YE62_5.mp3)    // 超载英文播报
#define MP3_APPEASE_1_CN MUSIC_PATH(YM62_1_3.mp3) // 安抚中文播报
#define MP3_APPEASE_1_EN MUSIC_PATH(YE62_1_3.mp3) // 安抚英文播报
#define MP3_APPEASE_2_CN MUSIC_PATH(YM62_2.mp3)   // 安抚中文播报
#define MP3_APPEASE_2_EN MUSIC_PATH(YE62_2.mp3)   // 安抚英文播报
#define MP3_APPEASE_3_CN MUSIC_PATH(YM62_4.mp3)   // 安抚中文播报
#define MP3_APPEASE_3_EN MUSIC_PATH(YE62_4.mp3)   // 安抚英文播报
#define MP3_UP_OBS_CN MUSIC_PATH(YM62_6.mp3)    // 升降英文播报
#define MP3_UP_OBS_EN MUSIC_PATH(YE62_6.mp3)    // 升降英文播报
#define MP3_DOWN_OBS_CN MUSIC_PATH(YM62_7.mp3)  // 升降英文播报
#define MP3_DOWN_OBS_EN MUSIC_PATH(YE62_7.mp3)  // 升降英文播报

#define MP3_APPEASE_CN   MUSIC_PATH(YM63_1_1.mp3) // 困人安抚中文播报
#define MP3_APPEASE_EN   MUSIC_PATH(YE63_1_1.mp3) // 困人安抚英文播报



#define MP3_BELL MUSIC_PATH(BELL.mp3)   //

int music_set_volume(int vol);
void music_thread_entry(void *parameter);
void delete_video_music(void);

#endif /* _MUSIC_H_ */

