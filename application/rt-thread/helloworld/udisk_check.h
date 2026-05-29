#ifndef __UDISK_CHECK_H__
#define __UDISK_CHECK_H__

#include "main.h"

#define MAX_VIDEO 8
#define MAX_IMAGES 10
#define MAX_MEDIA 10
#define MAX_NAMELEN 256
#define video_ext_num 4
#define image_ext_num 4
#define MAX_PATH_LEN 100

// 检查USB设备是否已插入（通过访问/udisk/video/目录判断）
int check_usb_connected();
// 扫描USB设备中的视频文件
int scan_usb_video_files(char files[MAX_VIDEO][MAX_NAMELEN], int max_files) ;
int scan_usb_image_files(char files[MAX_IMAGES][MAX_NAMELEN], int max_files);
#endif
