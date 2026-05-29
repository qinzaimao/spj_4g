#ifndef _UDISK_UPDATE_H_
#define _UDISK_UPDATE_H_

#include "main.h"

#define MAX_VIDEO_WIDTH  1024
#define MAX_VIDEO_HEIGHT 768

// #define image_path       "L:/data/1.jpg"
#define image_usb_path   "/udisk/top/"

#define video_usb_path   "/udisk/file/"

#define MAX_IMAGES 10     // 最大支持的图片数量
#define IMAGE_NAME_LEN 64 // 图片名称最大长度

// 图片格式枚举
typedef enum
{
    FORMAT_UNKNOWN,
    FORMAT_JPG,
    FORMAT_PNG,
    FORMAT_BMP
} ImageFormat;

enum{
    UPDATE_NONE = 0,
    VIDEO_UPDATING,
    IMAGE_UPDATING,
    VIDEO_UPDATE_FINISH,
    VIDEO_OVER_WIGHT,
    VIDEO_TYPE_NONE,
    VIDEO_IS_UPDATED,
    VIDEO_OVER_SIZE,
    VIDEO_NO_FIDE,
    VIDEO_UPDATE_ERROR,
    VIDEO_ERROR,
    VIDEO_CLEAR,
    VIDEO_UPDATE_FLASH_ERROR,
    VIDEO_UPDATE_ALL_ERROR,
    VIDEO_UPDATE_NO_MP4,
    IMAGE_UPDATE_FINISH,
    IMAGE_NO_FIDE,
    TEXT_ERROR,
    TEXT_OK,
};

enum
{
    // 图片状态
    IMAGE_READ_START = 10,
    IMAGE_READ_ING,
    IMAGE_READ_FINISH,
    IMAGE_READ_IDLE,
    IMAGE_UPDATA_OK
};

struct elevator
{
    lv_obj_t *label;
    uint8_t auto_num;
    uint8_t floor_min;
    uint8_t floor_max;
    uint8_t language;
    uint8_t media_enable;
    uint8_t arrow_scroll_speed;
    uint8_t volume;
    uint8_t brightness;
    uint8_t standby_brightness;
    uint8_t standby_timeout;
    uint8_t page_flag;
    uint8_t page_cursor;
    uint8_t flag;       // 正在设置的功能标志，停止处理接收串口
    uint8_t media_flag; // 正在播放的媒体文件标志
    uint8_t standby_flag;
    uint32_t timeout_cnt;
    uint16_t settimeout;        // 设置的超时时间
    uint8_t hidden_state;       // 隐藏状态获取
    uint8_t video_destroy_flag; // 定时删除视频标志
    uint8_t bg_img_dif;         // 使用默认背景图片
};


// ImageFormat get_image_format(const char *filename);
bool cover_image_to_init_file(const char *usb_img_path, int init_idx);
void video_import_thread_entry(void *parameter);
void image_import_thread_entry(void *parameter);
void media_import_thread_entry(void *parameter);
long get_file_size(FILE *fp);
#endif
