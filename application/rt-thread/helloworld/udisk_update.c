#include "udisk_update.h"
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <rtthread.h>
#include <dfs_posix.h>
#include <ctype.h>

// ========== 新增：播放器头文件（根据你的实际路径调整） ==========
#include "video.h"

// 增大的I/O缓冲区，用于提升文件拷贝速度 (8MB)
#define LARGE_TRANSFER_BUFFER_SIZE (8 * 1024 * 1024)
static uint8_t g_large_transfer_buffer[LARGE_TRANSFER_BUFFER_SIZE] __attribute__((aligned(4))); // 内存对齐


#define VIDEO_WIDEO_ERROR 1
#define VIDEO_TYPE_ERROR 2

static uint8_t update_video_error_type = 0;//视频错误类型

// ========== 新增：分辨率限制配置 ==========
#define MAX_VIDEO_WIDTH  1024  // 最大宽度限制
#define MAX_VIDEO_HEIGHT 1024  // 补充定义（原代码中使用但未定义）
#define MAX_VIDEO_COUNT 3      // 最多处理3个视频（V1/V2/V3）

char g_display_text[1024] = {0};
char g_display_mode[32] = {0};

// 媒体类型枚举
typedef enum
{
    MEDIA_TYPE_VIDEO,
    MEDIA_TYPE_IMAGE
} MediaType;

// 媒体导入状态
uint8_t readmedia_en = 0;
bool get_txt_flag = false;
// 媒体处理状态
typedef enum
{
    MEDIA_STATE_VIDEO,
    MEDIA_STATE_IMAGE,
    MEDIA_STATE_ERROR
} MediaProcessState;
static MediaProcessState media_state = MEDIA_STATE_VIDEO;

static bool image_update_start = false;
static struct lvgl_player_context g_lvgl_video_ctx;

// 函数声明
static void safe_file_close(FILE **fp);
static void write_video_error_to_udisk(int v_num, const char *error_msg);
static int get_vnum_from_video_filename(const char *filename);
static void sort_image_files_by_number(char files[MAX_MEDIA][MAX_NAMELEN], int count);
static int get_number_from_filename(const char *filename);
static ImageFormat get_image_format(const char *filename);
static int read_usb_update_txt(void);
static bool check_flash_free_space(const char *path, long need_size);
static bool retry_write(FILE *fp, const uint8_t *buf, size_t need_write);
// ========== 新增：视频按V1/V2/V3排序函数 ==========
static void sort_video_files_by_vnum(char files[MAX_VIDEO_COUNT][MAX_NAMELEN], int count);
// ========== 新增：批量校验视频分辨率函数 ==========
static bool check_all_video_resolution(char valid_video_files[MAX_VIDEO_COUNT][MAX_NAMELEN], int valid_count);

// ========== 新增：清空当前页面所有旧视频 ==========
static void clear_all_old_videos_on_current_page(void)
{
    rt_kprintf("【视频清理】开始清空当前页面所有旧视频...\n");

    for (int i = 0; i < 3; i++)
    {
        // 删除 MP4
        if (access(video_mp4_path[i][update_page_num], 0) == 0)
        {
            remove(video_mp4_path[i][update_page_num]);
            rt_kprintf("【视频清理】删除：%s\n", video_mp4_path[i][update_page_num]);
        }
        // 删除 AVI
        if (access(video_avi_path[i][update_page_num], 0) == 0)
        {
            remove(video_avi_path[i][update_page_num]);
            rt_kprintf("【视频清理】删除：%s\n", video_avi_path[i][update_page_num]);
        }

        // 清空标志
        page_mp4_update_flag[i][update_page_num] = false;
        page_avi_update_flag[i][update_page_num] = false;
    }

    rt_kprintf("【视频清理】当前页面旧视频已全部清空！\n");
}

// ========== 核心优化：使用播放器获取分辨率（替代原有文件解析） ==========
/**
 * @brief 使用播放器SDK获取视频分辨率（100%准确，支持moov在末尾的MP4）
 * @param file_path 视频文件路径
 * @param width 输出宽度
 * @param height 输出高度
 * @return true=获取成功，false=获取失败
 */
/**
 * @brief 使用播放器SDK获取视频分辨率（仅返回数据，不写日志）
 * @param file_path 视频文件路径
 * @param width 输出宽度
 * @param height 输出高度
 * @return true=获取成功，false=获取失败
 */
static bool get_video_resolution_by_player(const char *file_path, int *width, int *height)
{
    if (!file_path || !width || !height)
    {
        rt_kprintf("【分辨率解析】参数错误\n");
        return false;
    }

    *width = 0;
    *height = 0;

    // 1. 创建临时播放器实例
    struct lvgl_player_context temp_ctx;
    memset(&temp_ctx, 0, sizeof(struct lvgl_player_context));
    temp_ctx.player = aic_player_create(NULL);
    if (temp_ctx.player == NULL)
    {
        rt_kprintf("【分辨率解析】创建临时播放器失败\n");
        return false; // 只返回失败，不写日志
    }

    // 2. 设置播放URI
    int ret = aic_player_set_uri(temp_ctx.player, file_path);
    if (ret != 0)
    {
        rt_kprintf("【分辨率解析】设置URI失败\n");
        aic_player_destroy(temp_ctx.player);
        return false; // 只返回失败，不写日志
    }
    in_update_video_flag = true;
    rt_kprintf("here 1\n");
    // 3. 同步prepare（解析媒体信息）
    ret = aic_player_prepare_sync(temp_ctx.player);
    if (ret != 0)
    {
        rt_kprintf("【分辨率解析】Prepare失败\n");
        aic_player_destroy(temp_ctx.player);
        return false; // 只返回失败，不写日志
    }
    in_update_video_flag = false;
    if(update_video_type_erro_flag)
    {
        update_video_type_erro_flag = false;
        rt_kprintf("【分辨率解析】无视频流\n");
        aic_player_destroy(temp_ctx.player);
        update_video_error_type = VIDEO_TYPE_ERROR;
        return false; // 只返回失败，不写日志
    }
    rt_kprintf("here 2\n");
    // 4. 获取媒体信息（分辨率核心）
    ret = aic_player_get_media_info(temp_ctx.player, &temp_ctx.media_info);
    if (ret != 0)
    {
        rt_kprintf("【分辨率解析】获取媒体信息失败\n");
        aic_player_destroy(temp_ctx.player);
        return false; // 只返回失败，不写日志
    }

    rt_kprintf("has_audio:%d, has_video:%d,"
        "width:%d, height:%d,\n"
        "bits_per_sample:%d, nb_channel:%d, sample_rate:%d\n"
        ,temp_ctx.media_info.has_audio
        ,temp_ctx.media_info.has_video
        ,temp_ctx.media_info.video_stream.width
        ,temp_ctx.media_info.video_stream.height
        ,temp_ctx.media_info.audio_stream.bits_per_sample
        ,temp_ctx.media_info.audio_stream.nb_channel
        ,temp_ctx.media_info.audio_stream.sample_rate);

    // 5. 提取分辨率
    if (temp_ctx.media_info.has_video
        // && temp_ctx.media_info.audio_stream.bits_per_sample != 0
        && temp_ctx.media_info.audio_stream.nb_channel != 0
        && temp_ctx.media_info.audio_stream.sample_rate != 0
        )
    {
        *width = temp_ctx.media_info.video_stream.width;
        *height = temp_ctx.media_info.video_stream.height;
        rt_kprintf("【分辨率解析】从播放器获取：%dx%d\n", *width, *height);
    }
    else
    {
        rt_kprintf("【分辨率解析】无视频流\n");
        aic_player_destroy(temp_ctx.player);
        update_video_error_type = VIDEO_TYPE_ERROR;
        return false; // 只返回失败，不写日志
    }

    // 6. 销毁临时播放器
    aic_player_destroy(temp_ctx.player);

    return (*width > 0 && *height > 0);
}

// ========== 新增：错误日志写入函数 ==========
/**
 * @brief 向U盘error.txt写入视频错误信息
 * @param v_num 视频编号（0=V1,1=V2,2=V3）
 * @param error_msg 错误描述（如"格式错误"、"分辨率超标"）
 */
static void write_video_error_to_udisk(int v_num, const char *error_msg)
{
    if (v_num < 0 || v_num > 2 || !error_msg) return;

    char error_file_path[64] = "/udisk/error.txt";
    FILE *fp = fopen(error_file_path, "a+");  // 追加模式，不存在则创建
    if (!fp)
    {
        rt_kprintf("【错误日志】打开error.txt失败：%s\n", strerror(errno));
        return;
    }

    // 写入格式：V1：格式错误\n
    fprintf(fp, "V%d：%s\n", v_num + 1, error_msg);
    fflush(fp);
    fsync(fileno(fp));
    safe_file_close(&fp);

    rt_kprintf("【错误日志】已写入：V%d：%s → %s\n", v_num + 1, error_msg, error_file_path);
}

/**
 * @brief 清空U盘error.txt文件（USB拔出/重新扫描时调用）
 */
// ========== 优化：强制删除error.txt文件 ==========
static void clear_udisk_error_file(void)
{
    char error_file_path[64] = "/udisk/error.txt";
    // 强制删除，忽略不存在的错误（access检查可选，直接remove更简洁）
    if (remove(error_file_path) == 0)
    {
        rt_kprintf("【错误日志】已删除旧的error.txt\n");
    }
    else if (errno != ENOENT) // 仅打印非"文件不存在"的错误
    {
        rt_kprintf("【错误日志】删除error.txt失败：%s\n", strerror(errno));
    }
    // 文件不存在时不打印日志，避免冗余输出
}

/**
 * @brief 批量校验所有视频分辨率（完整日志写入，不中断校验）
 * @param valid_video_files 已筛选的有效视频文件列表
 * @param valid_count 有效视频数量
 * @return true=所有视频分辨率合规，false=有至少一个视频超标/格式错误
 */
static bool check_all_video_resolution(char valid_video_files[MAX_VIDEO_COUNT][MAX_NAMELEN], int valid_count)
{
    if (valid_count <= 0) return true;

    rt_kprintf("【分辨率批量校验】开始校验%d个视频...\n", valid_count);
    bool all_valid = true;
    for (int i = 0; i < valid_count; i++)
    {
        if (strlen(valid_video_files[i]) == 0) continue;

        int video_width = 0, video_height = 0;
        const char *filename = strrchr(valid_video_files[i], '/');
        filename = filename ? filename + 1 : valid_video_files[i];
        int v_num = get_vnum_from_video_filename(filename);  // 获取V1/V2/V3编号

        // 1. 获取当前视频分辨率（失败则记录格式错误）
        if (!get_video_resolution_by_player(valid_video_files[i], &video_width, &video_height))
        {
            rt_kprintf("【分辨率批量校验】%s 解析失败，视为格式错误！\n", filename);
            all_valid = false;
            if (v_num >= 0)
            {
                write_video_error_to_udisk(v_num, "格式错误");  // 统一写日志
            }
            continue; // 继续校验下一个，不中断
        }

        // 2. 校验分辨率（超标则记录分辨率错误）
        if (video_width > MAX_VIDEO_WIDTH || video_height > MAX_VIDEO_HEIGHT)
        {
            rt_kprintf("\n/******************/\n");
            rt_kprintf("ERROR: %s 分辨率超标！\n", filename);
            rt_kprintf("当前分辨率：%dx%d，限制：%dx%d\n", video_width, video_height, MAX_VIDEO_WIDTH, MAX_VIDEO_HEIGHT);
            rt_kprintf("/******************/\n");
            update_video_error_type = VIDEO_WIDEO_ERROR;
            all_valid = false;
            if (v_num >= 0)
            {
                write_video_error_to_udisk(v_num, "分辨率超标");  // 补全V2的日志写入
            }
        }
        else
        {
            rt_kprintf("【分辨率批量校验】%s：%dx%d（合规）\n", filename, video_width, video_height);
        }
    }

    rt_kprintf("【分辨率批量校验】完成，所有视频是否合规：%s\n", all_valid ? "是" : "否");
    return all_valid;
}

// 安全关闭文件（避免野指针）
static void safe_file_close(FILE **fp)
{
    if (*fp)
    {
        fclose(*fp);
        *fp = NULL;
    }
}

// 安全获取文件大小（避免空指针访问）
long safe_get_file_size(FILE *fp)
{
    if (!fp)
        return -1L;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

// 从文件名提取数字（核心辅助函数）
static int get_number_from_filename(const char *filename)
{
    if (filename == NULL)
        return -1;

    const char *pure_filename = strrchr(filename, '/');
    pure_filename = (pure_filename != NULL) ? pure_filename + 1 : filename;

    if (pure_filename[0] != 'p')
        return -1;

    const char *num_str = pure_filename + 1;
    if (!isdigit(num_str[0]))
        return -1;

    return atoi(num_str);
}

// ========== 新增：从视频文件名提取V编号（V1=0, V2=1, V3=2） ==========
static int get_vnum_from_video_filename(const char *filename)
{
    if (filename == NULL)
        return -1;

    const char *pure_filename = strrchr(filename, '/');
    pure_filename = (pure_filename != NULL) ? pure_filename + 1 : filename;

    // 忽略大小写匹配V1/V2/V3
    if (strncasecmp(pure_filename, "V1.", 3) == 0)
        return 0;
    else if (strncasecmp(pure_filename, "V2.", 3) == 0)
        return 1;
    else if (strncasecmp(pure_filename, "V3.", 3) == 0)
        return 2;
    else
        return -1;
}

// ========== 新增：视频文件按V1→V2→V3排序 ==========
static void sort_video_files_by_vnum(char files[MAX_VIDEO_COUNT][MAX_NAMELEN], int count)
{
    if (count <= 1)
        return;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int vnum_j = get_vnum_from_video_filename(files[j]);
            int vnum_j1 = get_vnum_from_video_filename(files[j + 1]);

            // 排序规则：V1(0) < V2(1) < V3(2)，无效编号放最后
            bool need_swap = false;
            if (vnum_j == -1 && vnum_j1 != -1)
                need_swap = true;
            else if (vnum_j != -1 && vnum_j1 != -1 && vnum_j > vnum_j1)
                need_swap = true;

            if (need_swap)
            {
                char temp[MAX_NAMELEN];
                strncpy(temp, files[j], MAX_NAMELEN - 1);
                temp[MAX_NAMELEN - 1] = '\0';
                strncpy(files[j], files[j + 1], MAX_NAMELEN - 1);
                files[j][MAX_NAMELEN - 1] = '\0';
                strncpy(files[j + 1], temp, MAX_NAMELEN - 1);
                files[j + 1][MAX_NAMELEN - 1] = '\0';
            }
        }
    }
}

#define FLASH_BUFFER_SIZE (16 * 1024)  // 16KB缓冲区（平衡内存/效率）
static char flash_buffer[FLASH_BUFFER_SIZE] = {0};

/**
 * @brief 将USB图片（JPG/PNG）二进制覆盖写入/data/initX.后缀
 * @param usb_img_path USB图片路径（如"/udisk/C401_hor.jpg""/udisk/C401_hor1.png"）
 * @param init_idx 目标init索引（0=init0,1=init1...）
 * @return true=覆盖成功，false=失败
 */
void build_init_target_path(int init_idx, const char *suffix, char *target_path) {
    if (!target_path || !suffix) return;
    snprintf(target_path, 64, "/data/init%d.%s", init_idx, suffix);
}

bool get_file_suffix(const char *file_path, char *suffix) {
    if (!file_path || !suffix) return false;
    const char *dot = strrchr(file_path, '.'); // 找最后一个.的位置
    if (!dot || dot == file_path) return false; // 无后缀或.在开头
    strncpy(suffix, dot + 1, 3); // 只取3位（jpg/png）
    suffix[3] = '\0';
    // 统一转为小写（兼容XXX.JPG/XXX.PNG）
    for (int i = 0; i < 3; i++) {
        if (suffix[i] >= 'A' && suffix[i] <= 'Z') {
            suffix[i] += 32;
        }
    }
    // 仅支持jpg/png
    if (strcmp(suffix, "jpg") != 0 && strcmp(suffix, "png") != 0) {
        rt_kprintf("【不支持的格式】仅支持jpg/png，当前：%s\n", suffix);
        return false;
    }
    return true;
}

bool ensure_data_dir_exists(void) {
    if (access("/data", 0) == 0) return true;
    if (mkdir("/data", 0777) != 0) {
        rt_kprintf("【创建目录失败】/data → %s\n", strerror(errno));
        return false;
    }
    rt_kprintf("【提示】已自动创建/data目录\n");
    return true;
}

bool cover_image_to_init_file(const char *src_img_path, int init_idx) {
    // 1. 极简前置检查
    if (!src_img_path || init_idx < 0 || init_idx >= 4) return false;
    if (!ensure_data_dir_exists()) return false;

    // 2. 提取后缀+拼接目标路径
    char suffix[4] = {0};
    if (!get_file_suffix(src_img_path, suffix)) return false;
    char target_path[64] = {0};
    build_init_target_path(init_idx, suffix, target_path);

    // 3. 核心变量（仅保留必要项）
    FILE *src_file = NULL;
    FILE *dst_file = NULL;
    int fd = -1;
    long src_size = 0;
    long total_written = 0;
    bool ret = false;

    // 4. 打开源文件（二进制读）
    src_file = fopen(src_img_path, "rb");
    if (!src_file) {
        rt_kprintf("[IMG_INIT] 打开源文件失败：%s\n", src_img_path);
        goto cleanup;
    }

    // 5. 检查文件大小
    src_size = safe_get_file_size(src_file);
    if (src_size <= 0) {
        rt_kprintf("[IMG_INIT] 无效源文件：%s\n", src_img_path);
        goto cleanup;
    }

    // 6. 打开目标文件（覆盖模式）
    fd = open(target_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd < 0) {
        rt_kprintf("[IMG_INIT] 打开目标文件失败：%s\n", target_path);
        goto cleanup;
    }
    dst_file = fdopen(fd, "wb");
    if (!dst_file) {
        close(fd);
        fd = -1;
        rt_kprintf("[IMG_INIT] fdopen失败：%s\n", target_path);
        goto cleanup;
    }

    // 7. 批量二进制写入（核心，无冗余打印）
    size_t bytes_read;
    while ((bytes_read = fread(flash_buffer, 1, FLASH_BUFFER_SIZE, src_file)) > 0) {
        size_t bytes_written = fwrite(flash_buffer, 1, bytes_read, dst_file);
        if (bytes_written != bytes_read) goto copy_end;
        total_written += bytes_written;
    }

    // 8. 校验完整性
    ret = (total_written == src_size);

copy_end:
    // 9. 刷写+关闭文件
    if (dst_file) {
        fflush(dst_file);
        safe_file_close(&dst_file);
    } else if (fd >= 0) {
        fsync(fd);
        close(fd);
    }

    // 10. 失败清理
    if (!ret && access(target_path, 0) == 0) {
        remove(target_path);
        rt_kprintf("[IMG_INIT] 覆盖失败，删除不完整文件：%s\n", target_path);
    } else if (ret) {
        rt_kprintf("[IMG_INIT] 覆盖成功：%s → %s\n", src_img_path, target_path);
    }

    // 11. 资源清理
cleanup:
    safe_file_close(&src_file);
    if (fd >= 0) close(fd);

    return ret;
}

// 图片按p0→p1→p2排序（核心排序函数）
static void sort_image_files_by_number(char files[MAX_MEDIA][MAX_NAMELEN], int count)
{
    if (count <= 1)
        return;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int num_j = get_number_from_filename(files[j]);
            int num_j1 = get_number_from_filename(files[j + 1]);

            bool need_swap = false;
            if (num_j == -1 && num_j1 != -1)
                need_swap = true;
            else if (num_j != -1 && num_j1 != -1 && num_j > num_j1)
                need_swap = true;

            if (need_swap)
            {
                char temp[MAX_NAMELEN];
                strncpy(temp, files[j], MAX_NAMELEN - 1);
                temp[MAX_NAMELEN - 1] = '\0';
                strncpy(files[j], files[j + 1], MAX_NAMELEN - 1);
                files[j][MAX_NAMELEN - 1] = '\0';
                strncpy(files[j + 1], temp, MAX_NAMELEN - 1);
                files[j + 1][MAX_NAMELEN - 1] = '\0';
            }
        }
    }
}

// 图片格式判断
ImageFormat get_image_format(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    if (!ext)
        return FORMAT_UNKNOWN;

    ext++;
    if (strcasecmp(ext, "jpg") == 0 || strcasecmp(ext, "jpeg") == 0)
        return FORMAT_JPG;
    else if (strcasecmp(ext, "png") == 0)
        return FORMAT_PNG;
    else if (strcasecmp(ext, "bmp") == 0)
        return FORMAT_BMP;

    return FORMAT_UNKNOWN;
}

// ==============================================================================
// 【修改功能】读取并解析USB中的update.txt文件
// 功能：读取文件内容，解析出text和mode，并存储到相应的全局变量中
// 返回：成功读取并解析返回 0，否则返回 -1
// 新增规则：必须同时找到text:和mode:字段才视为成功，任一缺失都不更新
// ==============================================================================
static int read_usb_update_txt(void)
{
    FILE *txt_file = NULL;
    char txt_usb_path[100] = "/udisk/update.txt";
    char line_buffer[1024];  // 用于逐行读取文件
    bool text_found = false; // 标记是否找到 text: 字段
    bool mode_found = false; // 新增：标记是否找到 mode: 字段

    // 初始化 mode 为默认值 0
    txt_mode[txt_update_page_num] = 0;

    // 1. 打开文件
    txt_file = fopen(txt_usb_path, "r");
    if (!txt_file)
    {
        rt_kprintf("【update.txt】错误：无法打开文件 %s\n", txt_usb_path);
        goto exit_func;
    }

    rt_kprintf("【update.txt】开始尝试读取...\n");
    get_txt_flag = true;
    // 2. 逐行读取并解析
    while (fgets(line_buffer, sizeof(line_buffer), txt_file) != NULL)
    {
        // 去除每行末尾的换行符和回车符
        char *newline = strchr(line_buffer, '\n');
        if (newline)
            *newline = '\0';
        char *carriage = strchr(line_buffer, '\r');
        if (carriage)
            *carriage = '\0';

        // 解析以 "text:" 或 "text：" 开头的行
        int colon_offset = -1; // 初始化偏移量为无效值
        if (strncmp(line_buffer, "text:", 5) == 0)
        {
            colon_offset = 5; // 英文冒号，偏移5
        }
        else if (strncmp(line_buffer, "text：", 7) == 0)
        {
            colon_offset = 7; // 中文冒号，偏移7（text4字节+中文冒号3字节）
        }

        if (colon_offset != -1)
        {
            // 将冒号后面的内容复制到 g_display_text
            strncpy(g_display_text, line_buffer + colon_offset, sizeof(g_display_text) - 1);
            g_display_text[sizeof(g_display_text) - 1] = '\0'; // 确保字符串结尾
            rt_kprintf("【update.txt】解析到文本: %s\n", g_display_text);
            text_found = true;
            continue; // 解析完text，跳过后续mode解析逻辑
        }

        // 解析以 "mode:" 或 "mode：" 开头的行
        colon_offset = -1;
        if (strncmp(line_buffer, "mode:", 5) == 0)
        {
            colon_offset = 5; // 英文冒号，偏移5
        }
        else if (strncmp(line_buffer, "mode：", 7) == 0)
        {
            colon_offset = 7; // 中文冒号，偏移7（mode4字节+中文冒号3字节）
        }

        if (colon_offset != -1)
        {
            // 检查冒号后面是否是 "1"
            if (strcmp(line_buffer + colon_offset, "1") == 0)
            {
                txt_mode[txt_update_page_num] = 1;
                rt_kprintf("【update.txt】解析到模式: 滚动\n");
            }
            else
            {
                // 如果是其他任何值，都设置为 0
                txt_mode[txt_update_page_num] = 0;
                rt_kprintf("【update.txt】解析到模式: %s，设置为默认模式 0\n", line_buffer + colon_offset);
            }
            mode_found = true; // 标记找到mode字段
        }
    }

    // 3. 检查是否同时找到text和mode字段（核心修改）
    if (!text_found || !mode_found)
    {
        rt_kprintf("【update.txt】错误：未找到 'text:' | 'mode:' 字段，不更新文件！\n");
        udisk_update_state = TEXT_ERROR;
        rt_thread_mdelay(1500);
        goto exit_func;
    }

    // 4. 读取和解析成功
    rt_kprintf("【update.txt】读取并解析成功！\n");
    rt_kprintf("  - 文本内容: %s\n", g_display_text);
    rt_kprintf("  - 当前页面模式 (txt_mode[%d]): %d\n", txt_update_page_num, txt_mode[txt_update_page_num]);

exit_func:
    // 5. 关闭文件
    safe_file_close(&txt_file);

    // 6. 只有同时找到text和mode才返回0，否则返回-1
    return (text_found && mode_found) ? 0 : -1;
}

/**
 * @brief 带重试机制的写入函数 (此版本保留，但在优化后的拷贝逻辑中未使用)
 * @param fp 文件指针
 * @param buf 数据缓冲区
 * @param need_write 需要写入的字节数
 * @return 全部写入成功返回true，否则返回false
 */
static bool retry_write(FILE *fp, const uint8_t *buf, size_t need_write)
{
    size_t total_written = 0;
    int retry_count = 3;

    while (total_written < need_write && retry_count > 0)
    {
        size_t written = fwrite(buf + total_written, 1, need_write - total_written, fp);
        if (written > 0)
        {
            total_written += written;
            fflush(fp);
        }
        else
        {
            rt_kprintf("【写入重试】失败，剩余重试次数：%d, 错误: %s\n", retry_count, strerror(errno));
            retry_count--;
            rt_thread_mdelay(500);
        }
    }
    return total_written == need_write;
}

/**
 * @brief 检查指定路径的文件系统剩余空间是否足够
 * @param path 文件系统路径
 * @param need_size 需要的空间大小（字节）
 * @return 空间足够返回true，否则返回false
 */
#define DEBUG_STORAGE 1 // 设置为1开启存储调试信息

static bool check_flash_free_space(const char *path, long need_size)
{
    struct statfs fs_info;
    int ret;

#if DEBUG_STORAGE
    rt_kprintf("[STORAGE] Checking free space for path: %s, need: %ld bytes\n", path, need_size);
#endif

    ret = dfs_statfs(path, &fs_info);
    if (ret != 0)
    {
        rt_kprintf("[ERROR] dfs_statfs(%s) failed! errno: %d, error: %s\n", path, errno, strerror(errno));
        return false;
    }

#if DEBUG_STORAGE
    rt_kprintf("[STORAGE] FileSystem Info for %s:\n", path);
    rt_kprintf("[STORAGE]   Block size (f_bsize): %ld\n", fs_info.f_bsize);
    rt_kprintf("[STORAGE]   Total blocks (f_blocks): %ld\n", fs_info.f_blocks);
    rt_kprintf("[STORAGE]   Free blocks (f_bfree): %ld\n", fs_info.f_bfree);
#endif

    // 计算总空间和可用空间
    unsigned long long total_space = (unsigned long long)fs_info.f_bsize * fs_info.f_blocks;
    unsigned long long free_space = (unsigned long long)fs_info.f_bsize * fs_info.f_bfree;

#if DEBUG_STORAGE
    rt_kprintf("[STORAGE]   Total Space: %ld MB\n", total_space / 1024 / 1024);
    rt_kprintf("[STORAGE]   Free Space: %ld MB\n", free_space / 1024 / 1024);
    rt_kprintf("[STORAGE]   Need Space: %ld B (%ld MB)\n", need_size, need_size / 1024 / 1024);
#endif

    /* 计算剩余空间（单位：字节），并预留10%的安全空间 */
    unsigned long long safe_free_space = free_space * 0.9;

    if (safe_free_space < (unsigned long long)need_size)
    {
        rt_kprintf("[ERROR] Flash space is NOT enough! Need: %ld MB, Safe Free: %ld MB\n",
                   need_size / 1024 / 1024, safe_free_space / 1024 / 1024);
        return false;
    }

#if DEBUG_STORAGE
    rt_kprintf("[STORAGE] Flash space is sufficient.\n");
#endif
    return true;
}

/**
 * 媒体导入线程 - 完整版本
 * 适配三维视频路径数组 + V1/V2/V3.mp4/avi 文件名规则
 * 支持最多3个视频导入，严格按V1→V2→V3顺序更新
 * 新增：批量校验分辨率，有一个超标则终止视频更新
 */
void media_import_thread_entry(void *parameter)
{
    bool usb_prev_connected = false;
    char media_files[MAX_MEDIA][MAX_NAMELEN];
    bool need_rescan_images = false;
    bool is_avi_format = false;
    int video_processed_count = 0; // 已处理视频计数

    // ========================= 初始化阶段 =========================
    rt_kprintf("【媒体线程】启动，等待系统初始化...\n");
    while (!init_set_img_ok)
    {
        rt_thread_mdelay(200);
    }
    // rt_thread_mdelay(4000);
    while (MY_SET_IMAGE.image == IMAGE_C203_hor)
    {
        rt_thread_mdelay(10000);
    };

    // ========================= 主循环 =========================
    while (1)
    {
        bool usb_connected = check_usb_connected();

        // 1. USB状态变化处理
        if (usb_connected != usb_prev_connected)
        {
            usb_prev_connected = usb_connected;

            if (usb_connected)
            {
                // video_in_updating = true;
                // ========== 优先删除旧的error.txt（核心修改） ==========
                clear_udisk_error_file();
                // =======================================================
                rt_kprintf("\n【USB状态】已插入，开始初始化...\n");

                // 检查/data可写性
                if (access("/data", 0) != 0)
                {
                    rt_kprintf("【权限错误】/data目录不可写，尝试重新挂载...\n");
                    system("mount -o remount,rw /data 2>/dev/null");
                    rt_thread_mdelay(1500);
                    if (access("/data", 0) != 0)
                    {
                        rt_kprintf("【严重错误】/data重新挂载失败，退出媒体导入！\n");
                        goto media_delay;
                    }
                }

                // 横屏/竖屏页面切换
                if (hengping)
                {
                    if (my_page == PAGE_SET_HOR)
                    {
                        lv_timer_pause(hor_set_time_timer);
                        lv_timer_pause(hor_get_time_timer);
                    }
                    else if (my_page == PAGE_IO_HOR)
                    {
                        lv_timer_pause(get_state_hor_timer);
                    }
                    if (my_page != PAGE_HOME_HOR)
                    {
                        rt_kprintf("【页面切换】跳转到横屏主页\n");
                        setup_scr_screen_home_hor(&guider_ui);
                        lv_scr_load_anim(guider_ui.screen_home_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                    }
                }
                else
                {
                    if (my_page == PAGE_SET)
                    {
                        lv_timer_pause(set_time_timer);
                        lv_timer_pause(get_time_timer);
                    }
                    else if (my_page == PAGE_IO)
                    {
                        lv_timer_pause(get_state_timer);
                    }
                    if (my_page != PAGE_HOME)
                    {
                        rt_kprintf("【页面切换】跳转到竖屏主页\n");
                        setup_scr_screen_home(&guider_ui);
                        lv_scr_load_anim(guider_ui.screen_home, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                    }
                }

                // 读取并解析update.txt
                int bytes_read = read_usb_update_txt();
                if (bytes_read == 0)
                {
                    rt_kprintf("【update.txt】准备保存到：%s\n", txt_path[txt_update_page_num]);
                    FILE *flash_txt_file = fopen(txt_path[txt_update_page_num], "wb");
                    if (flash_txt_file)
                    {
                        fprintf(flash_txt_file, "%s\n", g_display_text);
                        fflush(flash_txt_file);
                        fsync(fileno(flash_txt_file));
                        safe_file_close(&flash_txt_file);
                        rt_kprintf("【update.txt】保存成功\n");
                        udisk_update_state = TEXT_OK;
                        have_txt_flag[txt_update_page_num] = true;
                        txt_renew_flag = true;
                        rt_thread_mdelay(1500);
                    }
                    else
                    {
                        rt_kprintf("【update.txt】保存失败：%s\n", strerror(errno));
                    }
                }
                else
                {
                    rt_kprintf("【update.txt】读取或解析失败，不保存。\n");
                }
                save_begin();

                // 跳过特定图片模式
                if (MY_SET_IMAGE.image == IMAGE_C301_hor || MY_SET_IMAGE.image == IMAGE_C302_hor || MY_SET_IMAGE.image == IMAGE_C303_hor)
                {
                    goto media_delay;
                }

                // 重置所有状态
                readmedia_en = 0;
                media_count = 0;
                current_media_idx = 0;
                media_state = MEDIA_STATE_VIDEO;
                need_rescan_images = false;
                image_update_start = false;
                usb_image_cnt = 0;
                video_in_updating = true;
                read_percent = 0;
                video_processed_count = 0;
                is_avi_format = false;
                if(MY_SET.play_mode == PLAY_IMAGE)
                    music_renew_flag = true;
                if(show_video_label_flag) show_video_label_flag = false;

                // 扫描视频文件并筛选V1/V2/V3
                rt_kprintf("【媒体扫描】开始扫描视频文件...\n");
                media_count = scan_usb_video_files(media_files, MAX_MEDIA);

                // ====================== 核心逻辑：1个不限制，多个限制V1/V2/V3 ======================
                char valid_video_files[MAX_VIDEO_COUNT][MAX_NAMELEN] = {0};
                int valid_count = 0;

                int total_video_files = media_count;
                media_count = 0;

                if (total_video_files == 1)
                {
                    // 只有1个视频 → 不限制文件名，直接用
                    rt_kprintf("【视频规则】仅检测到1个视频，不限制文件名\n");
                    strncpy(valid_video_files[0], media_files[0], MAX_NAMELEN-1);
                    valid_count = 1;
                }
                else if (total_video_files > 1)
                {
                    // 多个视频 → 必须 V1/V2/V3
                    rt_kprintf("【视频规则】检测到多个视频，仅识别 V1/V2/V3.mp4/avi\n");
                    for (int i = 0; i < total_video_files; i++)
                    {
                        const char *filename = strrchr(media_files[i], '/');
                        filename = filename ? filename + 1 : media_files[i];

                        if ((strncasecmp(filename, "V1.", 3) == 0 ||
                             strncasecmp(filename, "V2.", 3) == 0 ||
                             strncasecmp(filename, "V3.", 3) == 0) &&
                            (strstr(filename, ".mp4") || strstr(filename, ".avi")))
                        {
                            strncpy(valid_video_files[valid_count++], media_files[i], MAX_NAMELEN-1);
                            if (valid_count >= MAX_VIDEO_COUNT) break;
                        }
                    }
                }

                // 排序
                sort_video_files_by_vnum(valid_video_files, valid_count);

                // ====================== 每次更新视频前清空旧视频 ======================
                if (valid_count > 0)
                {
                    clear_all_old_videos_on_current_page();
                }

                // 分辨率检查
                if (valid_count > 0)
                {
                    if (!check_all_video_resolution(valid_video_files, valid_count))
                    {
                        rt_kprintf("【视频更新终止】存在分辨率超标的视频，取消所有视频更新！\n");
                        if(update_video_error_type == VIDEO_WIDEO_ERROR)
                            udisk_update_state = VIDEO_OVER_WIGHT;
                        else if(update_video_error_type == VIDEO_TYPE_ERROR)
                            udisk_update_state = VIDEO_TYPE_NONE;
                        rt_thread_mdelay(1500);
                        media_state = MEDIA_STATE_ERROR;
                        need_rescan_images = true;
                        goto media_delay;
                    }
                }

                // 生效
                media_count = valid_count;
                for (int i = 0; i < media_count; i++)
                {
                    strncpy(media_files[i], valid_video_files[i], MAX_NAMELEN - 1);
                }

                if (media_count > 0)
                {
                    rt_kprintf("【视频扫描结果】找到%d个有效视频：\n", media_count);
                    for (int i = 0; i < media_count; i++)
                    {
                        const char *filename = strrchr(media_files[i], '/');
                        filename = filename ? filename + 1 : media_files[i];
                        rt_kprintf("  索引%d: %s\n", i, filename);
                    }

                    readmedia_en = 1;
                    current_media_idx = 0;
                    video_processed_count = 0;
                }
                else
                {
                    // 无有效视频，扫描图片
                    rt_kprintf("【视频扫描结果】未找到有效视频，切换扫描图片\n");
                    media_count = scan_usb_image_files(media_files, MAX_MEDIA);
                    if (media_count > 0)
                    {
                        rt_kprintf("【图片扫描结果】原始顺序（未排序）：\n");
                        for (int i = 0; i < media_count; i++)
                        {
                            const char *filename = strrchr(media_files[i], '/');
                            filename = filename ? filename + 1 : media_files[i];
                            rt_kprintf("  原始索引%d: %s\n", i, filename);
                        }

                        sort_image_files_by_number(media_files, media_count);
                        usb_image_cnt = media_count;
                        rt_kprintf("【图片排序结果】按p0→p1→p2顺序：\n");
                        for (int i = 0; i < media_count; i++)
                        {
                            const char *filename = strrchr(media_files[i], '/');
                            filename = filename ? filename + 1 : media_files[i];
                            ImageFormat format = get_image_format(media_files[i]);
                            const char *format_str = format == FORMAT_JPG ? "JPG" : format == FORMAT_PNG ? "PNG"
                                                                                                         : "BMP";
                            rt_kprintf("  索引%d: %s （格式：%s）\n", i, filename, format_str);
                        }

                        readmedia_en = 1;
                        media_state = MEDIA_STATE_IMAGE;
                        current_media_idx = 0;
                    }
                    else
                    {
                        rt_kprintf("【扫描结果】未找到任何媒体文件\n");
                        image_have_cnt = 0;
                        page_image_cnt[update_page_num] = 0;
                        if(get_txt_flag);
                        // else udisk_update_state = IMAGE_UPDATE_FINISH;
                        else  udisk_update_state = VIDEO_TYPE_NONE;
                        readmedia_en = 0;
                    }
                }
            }
            else
            {
                // USB拔出：重置所有状态
                rt_kprintf("【USB状态】已拔出\n");
                update_ok_flag = true;
                readmedia_en = 0;
                media_count = 0;
                current_media_idx = 0;
                get_txt_flag = false;
                udisk_update_state = 0;
                update_video_error_type = 0;
                media_state = MEDIA_STATE_VIDEO;
                usb_image_cnt = 0;
                video_in_updating = false;
                read_percent = 0;
                video_processed_count = 0;
                is_avi_format = false;
            }
        }

        // 2. 媒体导入核心逻辑
        if (readmedia_en)
        {
            // 2.1 处理视频（V1/V2/V3，适配三维路径，已按顺序处理）
            if (media_state == MEDIA_STATE_VIDEO && current_media_idx < media_count)
            {
                FILE *usb_file = NULL;
                FILE *flash_file = NULL;
                long usb_filelen = 0;
                size_t total_written = 0;
                bool update_ok = false;
                char current_usb_path[256] = {0};
                char current_flash_path[256] = {0};
                int fd = -1;

                strncpy(current_usb_path, media_files[current_media_idx], sizeof(current_usb_path) - 1);
                const char *filename = strrchr(current_usb_path, '/');
                filename = filename ? filename + 1 : current_usb_path;

                // ====================== 单个视频直接写入 V1 ======================
                if (media_count == 1)
                {
                    video_num = 0;
                }
                else
                {
                    if (strncasecmp(filename, "V1.", 3) == 0) video_num = 0;
                    else if (strncasecmp(filename, "V2.", 3) == 0) video_num = 1;
                    else if (strncasecmp(filename, "V3.", 3) == 0) video_num = 2;
                    else
                    {
                        rt_kprintf("【视频格式错误】非V1/V2/V3文件，跳过\n");
                        current_media_idx++;
                        video_processed_count++;
                        goto media_delay;
                    }
                }

                // 解析格式并映射到三维路径
                is_avi_format = (strstr(filename, ".avi") != NULL);
                if (is_avi_format)
                {
                    rt_kprintf("【视频格式识别】V%d为AVI格式\n", video_num + 1);
                    strncpy(current_flash_path, video_avi_path[video_num][update_page_num], sizeof(current_flash_path) - 1);
                }
                else
                {
                    rt_kprintf("【视频格式识别】V%d为MP4格式\n", video_num + 1);
                    strncpy(current_flash_path, video_mp4_path[video_num][update_page_num], sizeof(current_flash_path) - 1);
                }

                // 打印导入信息
                rt_kprintf("\n========================================\n");
                rt_kprintf("【视频导入】开始处理V%d：%s\n", video_num + 1, filename);
                rt_kprintf("  USB路径：%s\n  Flash路径：%s\n", current_usb_path, current_flash_path);

                // 打开USB文件
                usb_file = fopen(current_usb_path, "rb");
                if (!usb_file)
                {
                    rt_kprintf("【视频导入错误】USB文件打开失败：%s\n", strerror(errno));
                    current_media_idx++;
                    video_processed_count++;
                    goto media_delay;
                }

                // 获取文件大小
                usb_filelen = safe_get_file_size(usb_file);
                flash_file = fopen(current_flash_path, "rb");
                long flash_filelen = safe_get_file_size(flash_file);
                safe_file_close(&flash_file);

                // 大小检查（单个文件≤500MB）
                if (usb_filelen > 524288000)
                {
                    rt_kprintf("【视频导入错误】V%d文件过大（>500MB），跳过\n", video_num + 1);
                    udisk_update_state = VIDEO_UPDATE_ERROR;

                    rt_thread_mdelay(1500);
                    // udisk_update_state = VIDEO_UPDATE_FINISH;
                    current_media_idx++;
                    video_processed_count++;
                    need_rescan_images = (video_processed_count >= media_count);
                    safe_file_close(&usb_file);
                    goto media_delay;
                }
                if (usb_filelen <= 0)
                {
                    rt_kprintf("【视频导入错误】V%d无效文件（大小≤0）\n", video_num + 1);
                    safe_file_close(&usb_file);
                    current_media_idx++;
                    video_processed_count++;
                    goto media_delay;
                }

                // 总大小检查（≤3GB）
                long long all_size = 0;
                if (page_mp4_update_flag[0][0] || page_mp4_update_flag[0][1] || page_mp4_update_flag[0][2] ||
                    page_avi_update_flag[0][0] || page_avi_update_flag[0][1] || page_avi_update_flag[0][2])
                {
                    // 统计已存在的MP4视频大小
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 11; j++)
                        {
                            if (page_mp4_update_flag[i][j])
                            {
                                FILE *temp_file = fopen(video_mp4_path[i][j], "rb");
                                if (temp_file)
                                {
                                    long size = safe_get_file_size(temp_file);
                                    if (size > 0) all_size += size;
                                    safe_file_close(&temp_file);
                                }
                            }
                        }
                    }
                    // 统计已存在的AVI视频大小
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 11; j++)
                        {
                            if (page_avi_update_flag[i][j])
                            {
                                FILE *temp_file = fopen(video_avi_path[i][j], "rb");
                                if (temp_file)
                                {
                                    long size = safe_get_file_size(temp_file);
                                    if (size > 0) all_size += size;
                                    safe_file_close(&temp_file);
                                }
                            }
                        }
                    }
                }
                #if USE_8G
                    const long long MAX_TOTAL_SIZE = 4LL * 1024 * 1024 * 1024; // 4GB
                #else
                    const long long MAX_TOTAL_SIZE = 2LL * 1024 * 1024 * 1024; // 2GB
                #endif
                if (all_size + usb_filelen > MAX_TOTAL_SIZE)
                {
                    udisk_update_state = VIDEO_UPDATE_ALL_ERROR;
                    rt_kprintf("\n/******************/\n");
                    #if USE_8G
                        rt_kprintf("ERROR:视频总量超过4G\n");
                    #else
                        rt_kprintf("ERROR:视频总量超过2G\n");
                    #endif
                    rt_kprintf("/******************/\n");
                    rt_thread_mdelay(1500);
                    safe_file_close(&usb_file);
                    current_media_idx++;
                    video_processed_count++;
                    need_rescan_images = (video_processed_count >= media_count);
                    goto media_delay;
                }

                // 检查Flash剩余空间
                if (!check_flash_free_space("/data", usb_filelen))
                {
                    udisk_update_state = VIDEO_UPDATE_FLASH_ERROR;
                    rt_thread_mdelay(1000);
                    // udisk_update_state = VIDEO_UPDATE_FINISH;
                    safe_file_close(&usb_file);
                    current_media_idx++;
                    video_processed_count++;
                    need_rescan_images = (video_processed_count >= media_count);
                    goto media_delay;
                }

                // 开始拷贝文件（已提前校验分辨率，此处无需重复校验）
                if (flash_filelen == -1 || usb_filelen)
                {
                    uint8_t *buffer = g_large_transfer_buffer;
                    size_t buffer_size = LARGE_TRANSFER_BUFFER_SIZE;

                    // 打开Flash文件
                    fd = open(current_flash_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0)
                    {
                        rt_kprintf("【视频导入错误】Flash文件创建失败：%s\n", strerror(errno));
                        safe_file_close(&usb_file);
                        current_media_idx++;
                        video_processed_count++;
                        goto media_delay;
                    }
                    flash_file = fdopen(fd, "wb");
                    if (!flash_file)
                    {
                        rt_kprintf("【视频导入错误】fdopen失败：%s\n", strerror(errno));
                        close(fd);
                        safe_file_close(&usb_file);
                        current_media_idx++;
                        video_processed_count++;
                        goto media_delay;
                    }

                    // 进度更新
                    rt_kprintf("【视频导入中】V%d，大小：%ldB，加速拷贝...\n", video_num + 1, usb_filelen);
                    udisk_update_state = VIDEO_UPDATING;
                    read_percent = 0;

                    // 循环拷贝
                    size_t bytes_read;
                    while ((bytes_read = fread(buffer, 1, buffer_size, usb_file)) > 0)
                    {
                        size_t written = fwrite(buffer, 1, bytes_read, flash_file);
                        if (written != bytes_read)
                        {
                            rt_kprintf("【视频导入错误】V%d写入失败: %s\n", video_num + 1, strerror(errno));
                            udisk_update_state = VIDEO_ERROR;
                            rt_thread_mdelay(1500);
                            udisk_update_state = VIDEO_CLEAR;
                            update_ok = false;
                            break;
                        }

                        total_written += written;

                        // 进度更新（减少频率）
                        int new_percent = (total_written * 100.0) / usb_filelen;
                        if (new_percent != read_percent && (new_percent % 2) == 0)
                        {
                            read_percent = new_percent;
                            rt_kprintf("【视频导入】V%d，进度：%d%%\n", video_num + 1, read_percent);
                        }
                    }

                    // 刷写缓冲区
                    if (flash_file) fflush(flash_file);

                    // 校验拷贝结果
                    update_ok = (total_written == usb_filelen);
                    if (update_ok)
                    {
                        rt_kprintf("【视频拷贝】V%d完成，写入：%u字节\n", video_num + 1, total_written);
                    }
                    else
                    {
                        rt_kprintf("【视频拷贝】V%d未完成：期望%ld，实际%u\n", video_num + 1, usb_filelen, total_written);
                    }

                    safe_file_close(&flash_file);
                }
                else
                {
                    rt_kprintf("【视频导入】V%d文件已是最新\n", video_num + 1);
                    update_ok = true;
                    udisk_update_state = VIDEO_IS_UPDATED;
                    image_update_start = true;
                    rt_thread_mdelay(1500);
                    udisk_update_state = VIDEO_UPDATE_FINISH;
                }

                // 二次校验并更新标记
                if (update_ok)
                {
                    FILE *verify_file = fopen(current_flash_path, "rb");
                    if (verify_file)
                    {
                        long verify_len = safe_get_file_size(verify_file);
                        safe_file_close(&verify_file);
                        if (verify_len == usb_filelen)
                        {
                            rt_kprintf("【视频导入成功】V%d校验通过！\n", video_num + 1);
                            udisk_update_state = VIDEO_UPDATE_FINISH;


                            // 更新三维标记数组（互斥）
                            if (is_avi_format)
                            {
                                page_mp4_update_flag[video_num][update_page_num] = false;
                                page_avi_update_flag[video_num][update_page_num] = true;
                                rt_kprintf("【AVI标记】page_avi_update_flag[%d][%d] = true\n", video_num, update_page_num);
                            }
                            else
                            {
                                page_avi_update_flag[video_num][update_page_num] = false;
                                page_mp4_update_flag[video_num][update_page_num] = true;
                                rt_kprintf("【MP4标记】page_mp4_update_flag[%d][%d] = true\n", video_num, update_page_num);
                            }
                            save_begin();
                            image_update_start = true;
                        }
                        else
                        {
                            rt_kprintf("【视频导入失败】V%d校验失败！\n", video_num + 1);
                            remove(current_flash_path);
                        }
                    }
                    else
                    {
                        rt_kprintf("【视频导入失败】V%d校验文件打开失败！\n", video_num + 1);
                    }
                }

                // 清理资源
                safe_file_close(&usb_file);
                current_media_idx++;
                video_processed_count++;
                need_rescan_images = (video_processed_count >= media_count);
                if (need_rescan_images)
                {
                    media_state = MEDIA_STATE_ERROR; // 触发图片扫描
                }
            }

            // 2.2 处理图片（原有逻辑）
            else if (media_state == MEDIA_STATE_IMAGE && current_media_idx < media_count)
            {
                FILE *usb_file = NULL;
                FILE *flash_file = NULL;
                long file_read_len = 0;
                bool update_ok = false;
                char current_usb_path[256] = {0};
                char current_flash_path[256] = {0};
                int fd = -1;

                strncpy(current_usb_path, media_files[current_media_idx], sizeof(current_usb_path) - 1);
                const char *usb_filename = strrchr(current_usb_path, '/');
                usb_filename = usb_filename ? usb_filename + 1 : current_usb_path;

                // 获取图片格式并拼接目标路径
                ImageFormat format = get_image_format(current_usb_path);
                const char *ext = format == FORMAT_JPG ? ".jpg" : format == FORMAT_PNG ? ".png"
                                                              : format == FORMAT_BMP   ? ".bmp"
                                                                                       : ".unknown";
                sprintf(current_flash_path, "%s%d%s", image_flash_path_prefix[update_page_num], current_media_idx, ext);

                rt_kprintf("\n========================================\n");
                rt_kprintf("【图片导入】开始处理：%s\n", usb_filename);
                rt_kprintf("  USB路径：%s\n  Flash路径：%s\n", current_usb_path, current_flash_path);

                // 打开USB文件
                usb_file = fopen(current_usb_path, "rb");
                if (!usb_file)
                {
                    rt_kprintf("【图片导入错误】USB文件打开失败：%s\n", strerror(errno));
                    goto image_cleanup;
                }

                // 获取文件大小
                long usb_filelen = safe_get_file_size(usb_file);
                flash_file = fopen(current_flash_path, "rb");
                long flash_filelen = safe_get_file_size(flash_file);
                safe_file_close(&flash_file);

                if (usb_filelen <= 0)
                {
                    rt_kprintf("【图片导入错误】无效文件\n");
                    goto image_cleanup;
                }

                // 检查剩余空间
                if (!check_flash_free_space("/data", usb_filelen))
                {
                    udisk_update_state = IMAGE_UPDATE_FINISH;
                    goto image_cleanup;
                }

                // 拷贝图片文件
                if (flash_filelen == -1 || usb_filelen)
                {
                    // 删除旧文件
                    char temp_path[256] = {0};
                    const char *old_exts[] = {".jpg", ".png", ".bmp"};
                    for (int i = 0; i < 3; i++)
                    {
                        sprintf(temp_path, "%s%d%s", image_flash_path_prefix[update_page_num], current_media_idx, old_exts[i]);
                        if (access(temp_path, 0) == 0)
                        {
                            if (remove(temp_path) == 0)
                                rt_kprintf("【图片清理】已删除旧文件：%s\n", temp_path);
                        }
                    }

                    // 打开Flash文件
                    fd = open(current_flash_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0)
                    {
                        rt_kprintf("【图片导入错误】Flash文件创建失败：%s\n", strerror(errno));
                        goto image_cleanup;
                    }
                    flash_file = fdopen(fd, "wb");
                    if (!flash_file)
                    {
                        rt_kprintf("【图片导入错误】fdopen失败：%s\n", strerror(errno));
                        close(fd);
                        goto image_cleanup;
                    }

                    // 进度更新
                    rt_kprintf("【图片导入中】大小：%ld B，加速拷贝...\n", usb_filelen);
                    udisk_update_state = IMAGE_UPDATING;
                    read_percent = 0;

                    // 循环拷贝
                    uint8_t *buffer = g_large_transfer_buffer;
                    size_t buffer_size = LARGE_TRANSFER_BUFFER_SIZE;
                    size_t bytes_read;
                    while ((bytes_read = fread(buffer, 1, buffer_size, usb_file)) > 0)
                    {
                        size_t written = fwrite(buffer, 1, bytes_read, flash_file);
                        if (written != bytes_read)
                        {
                            rt_kprintf("【图片导入错误】写入失败: %s\n", strerror(errno));
                            update_ok = false;
                            goto image_copy_end;
                        }

                        file_read_len += written;
                        int new_percent = (file_read_len * 100.0) / usb_filelen;
                        if (new_percent > read_percent)
                        {
                            read_percent = new_percent;
                        }
                    }
                    update_ok = (file_read_len == usb_filelen);

                image_copy_end:
                    if (flash_file) fflush(flash_file);
                    safe_file_close(&flash_file);
                }
                else
                {
                    rt_kprintf("【图片导入】文件已是最新\n");
                    update_ok = true;
                }

                // 校验结果
                if (update_ok)
                {
                    rt_kprintf("【图片导入成功】\n");
                    image_have_cnt = current_media_idx + 1;
                    page_image_cnt[update_page_num] = current_media_idx + 1;
                }
                else
                {
                    rt_kprintf("【图片导入失败】\n");
                    remove(current_flash_path);
                }

            image_cleanup:
                safe_file_close(&usb_file);
                if (fd >= 0) close(fd);

                current_media_idx++;
                if (current_media_idx >= media_count)
                {
                    rt_kprintf("\n========================================\n");
                    rt_kprintf("【图片导入完成】所有图片处理完毕\n");
                    rt_kprintf("========================================\n");
                    udisk_update_state = IMAGE_UPDATE_FINISH;
                    readmedia_en = 0;
                    save_begin();
                }
            }

            // 2.3 视频处理后扫描图片
            if (media_state == MEDIA_STATE_ERROR && need_rescan_images)
            {
                rt_thread_mdelay(100);
                rt_kprintf("\n【视频后处理】开始扫描USB图片文件\n");
                media_count = scan_usb_image_files(media_files, MAX_MEDIA);
                if (media_count > 0)
                {
                    rt_kprintf("【图片扫描结果】原始顺序：\n");
                    for (int i = 0; i < media_count; i++)
                    {
                        const char *filename = strrchr(media_files[i], '/');
                        filename = filename ? filename + 1 : media_files[i];
                        rt_kprintf("  原始索引%d: %s\n", i, filename);
                    }

                    sort_image_files_by_number(media_files, media_count);
                    usb_image_cnt = media_count;
                    rt_kprintf("【图片排序结果】按p0→p1→p2顺序：\n");
                    for (int i = 0; i < media_count; i++)
                    {
                        const char *filename = strrchr(media_files[i], '/');
                        filename = filename ? filename + 1 : media_files[i];
                        ImageFormat format = get_image_format(media_files[i]);
                        const char *format_str = format == FORMAT_JPG ? "JPG" : format == FORMAT_PNG ? "PNG"
                                                                                                     : "BMP";
                        rt_kprintf("  索引%d: %s （格式：%s）\n", i, filename, format_str);
                    }

                    current_media_idx = 0;
                    media_state = MEDIA_STATE_IMAGE;
                    readmedia_en = 1;
                }
                else
                {
                    rt_kprintf("【图片扫描结果】未找到图片\n");
                    if(udisk_update_state != VIDEO_OVER_WIGHT
                    && udisk_update_state != VIDEO_TYPE_NONE
                    && udisk_update_state != VIDEO_UPDATE_FLASH_ERROR
                    && udisk_update_state != VIDEO_ERROR
                    && udisk_update_state != VIDEO_UPDATE_ERROR)
                    {
                        udisk_update_state = IMAGE_UPDATE_FINISH;
                    }
                    readmedia_en = 0;
                }
                need_rescan_images = false;
            }
        }

    media_delay:
        rt_thread_mdelay(500);
    }
}
