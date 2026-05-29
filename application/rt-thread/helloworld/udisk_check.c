#include "udisk_check.h"

// 支持的视频文件扩展名列表
char* video_ext[video_ext_num] = {"mp4", "MP4", "AVI", "avi"};
// 支持的图片文件扩展名列表
char* image_ext[image_ext_num] = {"jpg", "JPG", "png", "PNG"};

// 检查文件是否为视频文件（根据扩展名判断）
bool GetVideoFile(const char *filename) {
    const char *dot = strrchr(filename, '.'); // 从后查找最后一个点
    if (!dot || dot == filename) // 无扩展名或隐藏文件
        return false;

    const char *ext = dot + 1;
    for (int i = 0; i < video_ext_num; i++) {
        if (strcasecmp(ext, video_ext[i]) == 0) // 不区分大小写比较
            return true;
    }
    return false;
}

// 检查文件是否为图片文件（根据扩展名判断）
bool GetImageFile(const char *filename) {
    const char *dot = strrchr(filename, '.'); // 从后查找最后一个点
    if (!dot || dot == filename) // 无扩展名或隐藏文件
        return false;

    const char *ext = dot + 1;
    for (int i = 0; i < image_ext_num; i++) {
        if (strcasecmp(ext, image_ext[i]) == 0) // 不区分大小写比较
            return true;
    }
    return false;
}

// 检查USB设备是否已插入（通过访问/udisk/video/目录判断）
int check_usb_connected() {
    if (access(video_usb_path, 0) == 0) {
        return 1;
    }
    if (access(image_usb_path, 0) == 0) {
        return 1;
    }
    if (access("/udisk/update.txt", 0) == 0) {
        return 1;
    }
    // 全部不存在则返回0
    return 0;
}

/**
 * 扫描USB设备中的视频文件
 */
int scan_usb_video_files(char files[MAX_VIDEO][MAX_NAMELEN], int max_files) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    if ((dir = opendir(video_usb_path)) == NULL) {
        printf("无法打开USB视频目录\n");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL && count < max_files) {
        if (entry->d_type == DT_REG && GetVideoFile(entry->d_name)) {
            snprintf(files[count], MAX_NAMELEN, "%s%s",video_usb_path, entry->d_name);
            count++;
        }
    }

    closedir(dir);
    return count;
}

/**
 * 扫描USB设备中的图片文件
 */
int scan_usb_image_files(char files[MAX_IMAGES][MAX_NAMELEN], int max_files) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    char full_path[256];
    char usb_path[MAX_PATH_LEN];

    // 复制并规范化路径
    strncpy(usb_path, image_usb_path, MAX_PATH_LEN - 1);
    usb_path[MAX_PATH_LEN - 1] = '\0';

    // 确保路径以'/'结尾
    size_t len = strlen(usb_path);
    if (len > 0 && usb_path[len - 1] != '/') {
        strncat(usb_path, "/", MAX_PATH_LEN - len - 1);
    }

    if ((dir = opendir(usb_path)) == NULL) {
        printf("无法打开USB图片目录: %s\n", usb_path);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL && count < max_files) {
        if (entry->d_type == DT_REG && GetImageFile(entry->d_name)) {
            // 正确拼接路径并限制长度
            snprintf(full_path, sizeof(full_path), "%s%s", usb_path, entry->d_name);
            // 确保不超过MAX_NAMELEN
            snprintf(files[count], MAX_NAMELEN, "%.255s", full_path);
            files[count][MAX_NAMELEN - 1] = '\0';  // 额外添加终止符
            count++;

            // 调试输出
            printf("找到图片: %s, 完整路径: %s\n", entry->d_name, files[count-1]);
        }
    }

    closedir(dir);
    return count;
}

