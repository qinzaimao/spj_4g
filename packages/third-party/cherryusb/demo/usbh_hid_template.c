/*
 * Copyright (c) 2024-2025, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: Senye Liang <senye.liang@artinchip.com>
 */

#include <rtconfig.h>
#include "usbh_core.h"
#include "usbh_hid.h"
#include "../../../../application/rt-thread/helloworld/main.h"

struct usbh_hid_func_t {
    uint8_t *buf;

    usb_osal_thread_t hid_tid;
    struct usbh_hid *hid_class;

    uint8_t interval;
    uint8_t recv_flag;
} g_usbh_hid_func[CONFIG_USBHOST_MAX_HID_CLASS];

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t hid_buffer[CONFIG_USBHOST_MAX_HID_CLASS][128];

/* 处理鼠标滚轮事件 */
static void mouse_handle_wheel(int16_t delta)
{
    if (wheel_mutex) {
        // rt_mutex_take(wheel_mutex, RT_WAITING_FOREVER);
        wheel_diff += delta;  // 累积滚轮变化量
        // rt_mutex_release(wheel_mutex);
    }
}

static struct usbh_hid_func_t *get_hid_func(struct usbh_hid *hid_class)
{
    if (hid_class->intf > CONFIG_USBHOST_MAX_HID_CLASS) {
        USB_LOG_ERR("USB Host does not support so many interface functions. ");
        return NULL;
    }

    g_usbh_hid_func[hid_class->intf].hid_class = hid_class;
    g_usbh_hid_func[hid_class->intf].buf = hid_buffer[hid_class->intf];

    return &g_usbh_hid_func[hid_class->intf];
}

/*
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
***************************************************************************
*/
int8_t calculate_mode_similarity_average(int8_t *arr, uint8_t len, uint8_t threshold, uint8_t *count);
int8_t calculate_mode_similarity_average_multistage(int8_t *arr, uint8_t len, uint8_t *count);
int8_t calculate_mode_similarity_average_dynamic(int8_t *arr, uint8_t len, uint8_t *count);
/*
    现在使用联想鼠标的HID设备usbh_hid_callback，没有z轴数据，比较稳定
    注释的那个是比较差的鼠标的HID设备usbh_hid_callback，有z轴数据，鼠标移动较快有些抖动
*/
static bool x_invert = false; //补偿x轴方向
static enum { DIR_NONE, DIR_LEFT, DIR_RIGHT };
static enum { X_DIR_NONE, DIR_UP, DIR_DOWN };
void usbh_hid_callback(void *arg, int nbytes) {
    struct usbh_hid *hid_class = (struct usbh_hid *)arg;
    struct usbh_hid_func_t *hid_func = get_hid_func(hid_class);

    static uint8_t buttons = 0x00;
    static bool left_btn_flag = false;
    static bool right_btn_flag = false;
    static uint8_t last_x = 0x80;  // 新增X轴历史记录
    static uint8_t last_y = 0x80;
    int y_dir = DIR_NONE;
    int x_dir = X_DIR_NONE;

    // 速度检测相关变量
    static int16_t x_speed = 0, y_speed = 0;  // X/Y轴移动速度
    static uint8_t speed_counter = 0;         // 速度计算计数器
    static int32_t x_accumulator = 0;         // X轴移动量累加器
    static int32_t y_accumulator = 0;         // Y轴移动量累加器

    // 方向和速度阈值优化
    #define UP_RANGE_LOW       0xe0    // 上移区域下限
    #define DOWN_RANGE_HIGH    0x20    // 下移区域下限
    #define LEFT_RANGE_HIGH    0xe0    // 左移区域上限
    #define RIGHT_RANGE_LOW    0x20    // 右移区域下限

    // 灵敏度配置（针对不同方向和速度）
    #define BASE_SENSITIVITY   1.0f
    #define FAST_SPEED_THRESH  12      // 快速移动阈值
    #define SLOW_SPEED_THRESH  3       // 慢速移动阈值
    #define FAST_SENSITIVITY   1.8f    // 快速移动灵敏度
    #define SLOW_SENSITIVITY   0.6f    // 慢速移动灵敏度
    #define X_SPEED_FACTOR     4       // X轴速度因子
    #define Y_SPEED_FACTOR     3       // Y轴速度因子（Y轴通常需要稍低灵敏度）

        // 滚轮相关变量
    static int8_t wheel_deltas[4] = {0};      // 滚轮数据缓冲区
    static uint8_t wheel_idx = 0;             // 缓冲区索引

    if (hid_func == NULL)
        return;

    if (nbytes > 0) {
        buttons = hid_func->buf[0];
        uint8_t x_data = hid_func->buf[1];  // X轴数据
        uint8_t y_data = hid_func->buf[2];  // Y轴数据
        uint8_t z_data = hid_func->buf[3];

        int16_t dx = 0, dy = 0;
        float x_sensitivity = BASE_SENSITIVITY;
        float y_sensitivity = BASE_SENSITIVITY;

        // 计算X/Y轴变化量（带符号）
        int8_t x_diff = (int8_t)(x_data - last_x);
        int8_t y_diff = (int8_t)(y_data - last_y);
        last_x = x_data;
        last_y = y_data;

        // 累积移动量并计算平均速度（每4次采样更新）
        x_accumulator += abs(x_diff);
        y_accumulator += abs(y_diff);

        if (++speed_counter >= 4) {
            // 计算平均速度（除以采样次数）
            x_speed = x_accumulator / 4;
            y_speed = y_accumulator / 4;

            // 根据速度动态调整X轴灵敏度
            if (x_speed > FAST_SPEED_THRESH) {
                x_sensitivity = FAST_SENSITIVITY;
            } else if (x_speed < SLOW_SPEED_THRESH) {
                x_sensitivity = SLOW_SENSITIVITY;
            }

            // 根据速度动态调整Y轴灵敏度
            if (y_speed > FAST_SPEED_THRESH) {
                y_sensitivity = FAST_SENSITIVITY;
            } else if (y_speed < SLOW_SPEED_THRESH) {
                y_sensitivity = SLOW_SENSITIVITY;
            }

            // 重置累加器和计数器
            x_accumulator = 0;
            y_accumulator = 0;
            speed_counter = 0;
        }

        /* 解析X轴移动 - 左右方向 */
        if (x_data >= LEFT_RANGE_HIGH) {
            x_dir = DIR_LEFT;
            dx = -(0x100 - x_data) * x_sensitivity;
        } else if (x_data <= RIGHT_RANGE_LOW) {
            x_dir = DIR_RIGHT;
            dx = x_data * x_sensitivity;
        } else {
            // 中间区域微小移动处理
            if (abs(x_diff) > 2) {
                x_dir = (x_diff > 0) ? DIR_RIGHT : DIR_LEFT;
                dx = x_diff * x_sensitivity;
            } else {
                x_dir = X_DIR_NONE;
                dx = 0;
            }
        }

        /* 解析Y轴移动 - 上下方向 */
        if (y_data >= UP_RANGE_LOW) {
            y_dir = DIR_UP;
            dy = -(0xff - y_data + 1) * y_sensitivity;
        } else if (y_data <= DOWN_RANGE_HIGH) {
            y_dir = DIR_DOWN;
            dy = (y_data + 1) * y_sensitivity;
        } else {
            // 中间区域微小移动处理
            if (abs(y_diff) > 2) {
                y_dir = (y_diff > 0) ? DIR_DOWN : DIR_UP;
                dy = y_diff * y_sensitivity;
            } else {
                y_dir = DIR_NONE;
                dy = 0;
            }
        }

        /* 应用速度因子并确保最小移动量 */
        if (dx != 0) {
            dx = (dx * X_SPEED_FACTOR) / 2;
            // 确保至少有1单位移动，避免无效值
            if (dx == 0) dx = (x_dir == DIR_RIGHT) ? 1 : -1;
        }

        if (dy != 0) {
            dy = (dy * Y_SPEED_FACTOR) / 2;
            // 确保至少有1单位移动，避免无效值
            if (dy == 0) dy = (y_dir == DIR_DOWN) ? 1 : -1;
        }

         /* 处理滚轮事件 - 根据规则：dy=1时，z=1上滑，z=255下滑 */
        if (dy == 1) {  // 符合滚轮操作的Y轴条件
            if (z_data == 1) {
                // 上滑：使用正值
                wheel_deltas[wheel_idx++] = -1;
            } else if (z_data == 255) {
                // 下滑：使用负值
                wheel_deltas[wheel_idx++] = 1;
            }
            dy -= 1;
            // 每4次采样处理一次滚轮数据
            if (wheel_idx >= 1) {
                int16_t total = 0;
                for (int i = 0; i < 1; i++) {
                    total += wheel_deltas[i];
                    wheel_deltas[i] = 0;
                }
                mouse_handle_wheel(total);  // 传递滚轮数据
                wheel_idx = 0;
            }
        }
        /* 调试输出 - 包含更多状态信息 */
        #if MOUSE_DEBUG
        rt_kprintf("X:0x%02x Y:0x%02x z:%d | Dx:%d Dy:%d |Xd:%d Yd:%d | Spd:%d/%d\n",
                   x_data, y_data, z_data, dx, dy, x_diff, y_diff, x_speed, y_speed);
        rt_kprintf("buttons:0x%02x | \n", buttons);
        #endif

        /* 左键处理 */
        static bool set_x_invert = false;
        if (!left_btn_flag && (buttons & 0x01)) {
            // rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
            left_btn_flag = true;
            mouse_btn_left = true;
            #if MOUSE_DEBUG
            rt_kprintf("左键按下\n");
            #endif
            if (!x_invert && !set_x_invert) {
                x_invert = true;
                set_x_invert = true;
            }
            // rt_mutex_release(mouse_key_mutex);
        } else if (left_btn_flag && !(buttons & 0x01)) {
            // rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
            mouse_btn_left = false;
            set_x_invert = false;
            left_btn_flag = false;
            // rt_mutex_release(mouse_key_mutex);
        }
        if ((buttons & 0x02) && !right_btn_flag) {
            // rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
            #if MOUSE_DEBUG
            rt_kprintf("右键按下\n");
            #endif
            right_btn_flag = true;
            mouse_btn_right = true;
            // rt_mutex_release(mouse_key_mutex);
        } else if (right_btn_flag && !(buttons & 0x02)) {
            // rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
            mouse_btn_right = false;
            right_btn_flag = false;
            // rt_mutex_release(mouse_key_mutex);
        }
        /* 更新鼠标位置 */
        mouse_update_position(dx, dy);
    }

    hid_func->recv_flag = 1;
}


// void usbh_hid_callback(void *arg, int nbytes)
// {
//     struct usbh_hid *hid_class = (struct usbh_hid *)arg;
//     struct usbh_hid_func_t *hid_func = get_hid_func(hid_class);

//     static uint8_t buttons = 0x00;

//     static bool left_btn_flag = false;
//     static uint8_t last_y = 0x80;
//     static enum { DIR_NONE,
//                   DIR_UP,
//                   DIR_DOWN } move_dir = DIR_NONE;

// // Y轴范围阈值 - 根据你的数据调整
// #define UP_RANGE_LOW    0xe0 // 上移区域下限
// #define DOWN_RANGE_HIGH 0x20 // 下移区域上限
// #define SENSITIVITY_X   0.9  // 灵敏度系数
// #define SENSITIVITY_Y   1.2  // 灵敏度系数

//     if (hid_func == NULL)
//         return;

//     if (nbytes > 0) {
//         buttons = hid_func->buf[0];
//         uint8_t x_data = hid_func->buf[1]; // X轴数据
//         uint8_t y_data = hid_func->buf[2]; // Y轴数据

//         int8_t dx = 0, dy = 0;

//         // 计算Y轴变化量
//         int8_t y_diff = y_data - last_y;
//         last_y = y_data;

//         /* 解析X轴移动 - 左右方向 */
//         if (x_data > 0xe0) {
//             dx = -(0x100 - x_data) * SENSITIVITY_X; // 左滑
//         } else if (x_data < 0x50) {
//             dx = x_data * SENSITIVITY_X; // 右滑
//         }

//         /* 解析Y轴移动 - 上下方向 - 基于Y值范围和变化量 */
//         if (y_data >= UP_RANGE_LOW) {
//             move_dir = DIR_UP;
//             // 根据Y值距离上限的远近来计算速度
//             dy = -(0xff - y_data + 1) * SENSITIVITY_Y;
//         } else if (y_data <= DOWN_RANGE_HIGH) {
//             move_dir = DIR_DOWN;
//             // 根据Y值距离下限的远近来计算速度
//             dy = (y_data + 1) * SENSITIVITY_Y;
//         } else {
//             move_dir = DIR_NONE;
//         }

// /* 速度调整 */
// #define X_SPEED_FACTOR 4

//         if (dx != 0) {
//             dx = (dx * X_SPEED_FACTOR) / 2;
//             dx = dx == 0 ? (dx > 0 ? 1 : -1) : dx;
//         }

// /* 调试输出 */
// #if 1
//         rt_kprintf("X:0x%02x Y:0x%02x | Dx:%d Dy:%d | Y_diff:%d\n",
//                    x_data, y_data, dx, dy, y_diff);
// #endif

//         /* 左键处理 */
//         static bool set_x_invert = false;
//         if (buttons & 0x01) {
//             rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
//             left_btn_flag = true;
//             mouse_btn_left = true;
//             if (!x_invert) {
//                 if(!set_x_invert)
//                 {
//                     x_invert = true;
//                     set_x_invert = true;
//                 }
//             }
//             rt_mutex_release(mouse_key_mutex);
//         } else if (left_btn_flag && !(buttons & 0x01)) {
//             rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
//             mouse_btn_left = false;
//             set_x_invert = false;
//             left_btn_flag = false;
//             rt_mutex_release(mouse_key_mutex);
//         }
//         /* 更新鼠标位置 */
//         mouse_update_position(dx, dy);
//     }

//     hid_func->recv_flag = 1;
// }

// void usbh_hid_callback(void *arg, int nbytes)
// {
//     struct usbh_hid *hid_class = (struct usbh_hid *)arg;
//     struct usbh_hid_func_t *hid_func = get_hid_func(hid_class);

//     static uint8_t buttons = 0x00;
//     static bool left_btn_flag = false;

//     if (hid_func == NULL)
//         return;

//     if (nbytes > 0) {
//         buttons = hid_func->buf[0];
//         uint8_t x_data = hid_func->buf[1]; // X轴数据
//         uint8_t y_data = hid_func->buf[2]; // Y轴数据
//         uint8_t z_data = hid_func->buf[3]; // Z轴数据(用于判断上下移动)

//         int8_t dx = 0, dy = 0;

//         if (z_data == 0x00 || z_data == 0xFF) {
//             /* 解析X轴移动 - 左右方向 */
//             if (x_data > 0xe0) {
//                 dx = -(0x100 - x_data); // 左滑
//             } else if (x_data < 0x50) {
//                 dx = x_data; // 右滑
//             }

//             /* 解析Y轴移动 - 上下方向优化 */
//             if (y_data >= 0x60 && y_data <= 0xf0 && z_data >= 0xf0) {
//                 /* 上滑 - 精准条件：y在0x60-0xf0且wheel接近0xff */
//                 dy = -(0x100 - y_data);
//             } else if (y_data >= 0x10 && y_data <= 0xe0 && z_data <= 0x0a) {
//                 /* 下滑 - 精准条件：y在0x10-0xe0且wheel接近0x00 */
//                 dy = y_data;
//             }

//             /* 多方向滑动优化 */
//             if (x_data > 0xe0 && y_data >= 0x10 && y_data <= 0xe0 && z_data <= 0x06) {
//                 dx = -(0x100 - x_data); // 左下滑
//                 dy = y_data;
//             } else if (x_data < 0x37 && y_data >= 0x60 && y_data <= 0xf0 && z_data >= 0xf0) {
//                 dx = x_data; // 右上滑
//                 dy = -(0x100 - y_data);
//             } else if (x_data < 0x10 && y_data >= 0x10 && y_data <= 0xe0 && z_data <= 0x06) {
//                 dx = x_data; // 右下滑
//                 dy = y_data;
//             } else if (x_data > 0xe0 && y_data >= 0x60 && y_data <= 0xf0 && z_data >= 0xf0) {
//                 dx = -(0x100 - x_data); // 左上滑
//                 dy = -(0x100 - y_data);
//             }

// /* 速度调整 - 直接使用当前计算的dx/dy值 */
// #define X_SPEED_FACTOR 4
// #define Y_SPEED_FACTOR 2

//             if (dx != 0) {
//                 dx = (dx * X_SPEED_FACTOR) / 2;
//                 dx = dx == 0 ? (dx > 0 ? 1 : -1) : dx;
//             }
//             if (dy != 0) {
//                 dy = (dy * Y_SPEED_FACTOR) / 12; // 进一步减慢Y轴
//                 dy = dy == 0 ? (dy > 0 ? 1 : -1) : dy;
//             }
//         } else {
//             /* 解析X轴移动 - 左右方向 */
//             if (x_data > 0xE0) {
//                 dx = -(0x100 - x_data); // 左滑
//             } else if (x_data < 0x20) {
//                 dx = x_data; // 右滑
//             }

//             /* 解析Y轴移动 - 上下方向 (使用Z轴数据) */
//             if (z_data > 0xE0) {
//                 dy = -(0x100 - z_data); // 上移
//             } else if (z_data < 0x20) {
//                 dy = z_data; // 下移
//             }

// /* 速度调整 */
// #define X_SPEED_FACTOR 3
// #define Y_SPEED_FACTOR 30

//             dx = (dx * X_SPEED_FACTOR) / 2;
//             dy = (dy * Y_SPEED_FACTOR) / 2;

//             /* 确保最小移动单位 */
//             if (dx != 0 && abs(dx) < 1)
//                 dx = dx > 0 ? 1 : -1;
//             if (dy != 0 && abs(dy) < 1)
//                 dy = dy > 0 ? 1 : -1;
//         }
//         /* 更新鼠标位置 */
//         mouse_update_position(dx, dy);

// /* 调试输出 */
// #if 0
//         rt_kprintf("X:0x%02x Y:0x%02x Z:0x%02x | Dx:%d Dy:%d\n",
//                    x_data, y_data, z_data, dx, dy);
// #endif
//         static bool onec = false;
//         /* 左键处理 */
//         if (buttons & 0x01) {
//             rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
//             left_btn_flag = true;
//             mouse_btn_left = true;
//             if(!onec){
//                 onec = true;
//                 video_init(false);
//             }
//             rt_mutex_release(mouse_key_mutex);
//         } else if (left_btn_flag && !(buttons & 0x01)) {
//             rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
//             mouse_btn_left = false;
//             onec = false;
//             left_btn_flag = false;
//             rt_mutex_release(mouse_key_mutex);
//         }
//     }
//     hid_func->recv_flag = 1;
// }

/* 方案1: 动态阈值 - 根据众数大小自动调整阈值 */
int8_t calculate_mode_similarity_average_dynamic(int8_t *arr, uint8_t len, uint8_t *count)
{
    if (len == 0) {
        *count = 0;
        return 0;
    }

    uint8_t count_table[256] = { 0 };
    for (int i = 0; i < len; i++)
        count_table[arr[i] + 128]++;

    uint8_t max_count = 0;
    int8_t mode_value = 0;
    for (int i = 0; i < 256; i++) {
        if (count_table[i] > max_count) {
            max_count = count_table[i];
            mode_value = i - 128;
        }
    }

    /* 动态计算阈值 - 根据众数的绝对值大小调整 */
    uint8_t threshold;
    int16_t abs_mode = abs(mode_value);

    if (abs_mode < 10) {
        threshold = 0x05; // 小值使用小阈值
    } else if (abs_mode < 30) {
        threshold = 0x10; // 中值使用中等阈值
    } else {
        threshold = 0x20; // 大值使用大阈值
    }

    int16_t sum = 0;
    *count = 0;
    for (int i = 0; i < len; i++) {
        if (abs(arr[i] - mode_value) <= threshold) {
            sum += arr[i];
            (*count)++;
        }
    }

    return (*count == 0) ? mode_value : sum / (*count);
}

/* 方案2: 多阶段滤波 - 先粗过滤再精过滤 */
int8_t calculate_mode_similarity_average_multistage(int8_t *arr, uint8_t len, uint8_t *count)
{
    if (len == 0) {
        *count = 0;
        return 0;
    }

    /* 第一阶段: 粗过滤 - 使用较大阈值找出初步众数 */
    uint8_t count_table[256] = { 0 };
    for (int i = 0; i < len; i++)
        count_table[arr[i] + 128]++;

    uint8_t max_count = 0;
    int8_t mode_value = 0;
    for (int i = 0; i < 256; i++) {
        if (count_table[i] > max_count) {
            max_count = count_table[i];
            mode_value = i - 128;
        }
    }

    /* 收集所有与初步众数相近的数据点 */
    int8_t filtered_data[256] = { 0 };
    uint8_t filtered_count = 0;

    for (int i = 0; i < len; i++) {
        if (abs(arr[i] - mode_value) <= 0x20) { // 粗过滤阈值
            filtered_data[filtered_count++] = arr[i];
        }
    }

    /* 第二阶段: 精过滤 - 对过滤后的数据再次计算众数 */
    if (filtered_count > 0) {
        uint8_t refined_count_table[256] = { 0 };
        for (int i = 0; i < filtered_count; i++) {
            refined_count_table[filtered_data[i] + 128]++;
        }

        uint8_t refined_max_count = 0;
        int8_t refined_mode_value = 0;

        for (int i = 0; i < 256; i++) {
            if (refined_count_table[i] > refined_max_count) {
                refined_max_count = refined_count_table[i];
                refined_mode_value = i - 128;
            }
        }

        /* 使用精过滤后的众数和较小阈值计算最终平均值 */
        int16_t sum = 0;
        *count = 0;

        for (int i = 0; i < len; i++) {
            if (abs(arr[i] - refined_mode_value) <= 0x0a) { // 精过滤阈值
                sum += arr[i];
                (*count)++;
            }
        }

        return (*count == 0) ? refined_mode_value : sum / (*count);
    }

    /* 没有有效数据，返回原始众数 */
    *count = 1;
    return mode_value;
}

/* 计算众数相似度平均值 - 先找众数再计算相似数据均值 */
int8_t calculate_mode_similarity_average(int8_t *arr, uint8_t len, uint8_t threshold, uint8_t *count)
{
    if (len == 0) {
        *count = 0;
        return 0;
    }

    /* 统计每个值的出现次数 */
    uint8_t count_table[256] = { 0 };

    for (int i = 0; i < len; i++) {
        count_table[arr[i] + 128]++; // 偏移128以支持负数
    }

    /* 找到出现次数最多的众数 */
    uint8_t max_count = 0;
    int8_t mode_value = 0;

    for (int i = 0; i < 256; i++) {
        if (count_table[i] > max_count) {
            max_count = count_table[i];
            mode_value = i - 128;
        }
    }

    /* 计算与众数相差不超过threshold的数据点的平均值 */
    int16_t sum = 0;
    *count = 0;

    for (int i = 0; i < len; i++) {
        if (abs(arr[i] - mode_value) <= threshold) {
            sum += arr[i];
            (*count)++;
        }
    }

    /* 无相似数据时返回众数 */
    if (*count == 0) {
        *count = 1;
        return mode_value;
    }

    return sum / (*count);
}

/* 鼠标坐标更新函数 */
void mouse_update_position(int8_t dx, int8_t dy)
{
    static int32_t mouse_x = 0, mouse_y = 0;

    mouse_x += dx;
    mouse_y += dy;

    /* 边界检查 */
    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_y < 0)
        mouse_y = 0;
    if (mouse_x >= LV_HOR_RES - 16)
        mouse_x = LV_HOR_RES - 16;
    if (mouse_y >= LV_VER_RES - 22)
        mouse_y = LV_VER_RES - 22;

    /* 保存坐标 */
    // rt_mutex_take(mouse_key_mutex, RT_WAITING_FOREVER);
    mouse_current_x = mouse_x;
    if(x_invert)
    {
        // mouse_y -= 1;
        x_invert = false;
    }
    else
        mouse_current_y = mouse_y;
    // rt_mutex_release(mouse_key_mutex);
}

static void usbh_hid_thread(void *argument)
{
    struct usbh_hid *hid_class = (struct usbh_hid *)argument;
    struct usbh_hid_func_t *hid_func = get_hid_func(hid_class);

    usbh_hubport_enumerate_wait(hid_class->hport);

    if (hid_func == NULL)
        return;

    usbh_int_urb_fill(&hid_class->intin_urb, hid_class->hport, hid_class->intin, hid_func->buf, hid_class->intin->wMaxPacketSize, 0, usbh_hid_callback, hid_class);
    usbh_submit_urb(&hid_class->intin_urb);

    /* Suggest you to use timer for int transfer and use ep interval */
    while (1) {
        if (hid_func->recv_flag == 1) {
            usbh_int_urb_fill(&hid_class->intin_urb, hid_class->hport, hid_class->intin, hid_func->buf, hid_class->intin->wMaxPacketSize, 0, usbh_hid_callback, hid_class);
            usbh_submit_urb(&hid_class->intin_urb);
            hid_func->recv_flag = 0;
        }

        rt_thread_mdelay(hid_class->bInterval);
    }
}

void usbh_hid_run(struct usbh_hid *hid_class)
{
    struct usbh_hid_func_t *hid_func = get_hid_func(hid_class);

    if (hid_func == NULL) {
        USB_LOG_WRN("The interface function cannot be created.\n");
        return;
    }

    hid_func->hid_tid = usb_osal_thread_create(hid_class->hport->config.intf[hid_class->intf].devname,
                                               2048, CONFIG_USBHOST_PSC_PRIO + 1, usbh_hid_thread, hid_class);
}

void usbh_hid_stop(struct usbh_hid *hid_class)
{
    struct usbh_hid_func_t *hid_func = get_hid_func(hid_class);

    if (hid_func == NULL) {
        USB_LOG_WRN("The interface function cannot be delete.\n");
        return;
    }

    if (hid_func->hid_tid)
        usb_osal_thread_delete(hid_func->hid_tid);

    memset(hid_func, 0, sizeof(struct usbh_hid_func_t));
}
