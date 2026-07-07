#ifndef __MY_UART_H__
#define __MY_UART_H__

#include "main.h"

// 1. 统一宏定义：删除冗余，明确用途
#define SAMPLE_UART_NAME     "uart5"
#define UART_PROTOCOL_LEN    9       // 协议固定长度（原UART_MAX_COUNT/UART_FIXED_LENGTH）
#define PROTOCOL_ADDR        0x00    // 协议首字节地址（原ADDRESS/ADDRESS_FILTER）
#define CHECKSUM_LEN         2       // 校验和长度（2字节）
#define CHECKSUM_HIGH_FIXED  0xE6    // 校验码高位固定值
// #define RS485_DIR_PIN        "PE.6"  // RS485方向引脚（集中管理硬件配置）

#define UART_PROTOCOL_LEN_VER 8     // 新版本协议长度
#define PROTOCOL_STX1 0xAA          // 新协议帧头1
#define PROTOCOL_STX2 0x56          // 新协议帧头2
#define PROTOCOL_ADDR_OLD 0x00      // 原协议帧头

// 数据1（buf[1]）位定义
#define DIR_ARROW_BIT          0   // Bit0: 方向箭头指示(0=上,1=下)
#define ARROW_CTRL_BIT1        1   // Bit1: 箭头控制位1
#define ARROW_CTRL_BIT2        2   // Bit2: 箭头控制位2
#define CAR_OVERLOAD_BIT       3   // Bit3: 轿厢超载(0=正常,1=超载)
#define mid_code_low4_MASK     0xF0  // Bit4~7: 中间位的低4位（二进制低位部分）

// 数据2（buf[2]）位定义
#define mid_code_high2_MASK    0x03  // Bit0~1: 中间位的高2位（二进制高位部分）
#define LOW_DISPLAY_MASK       0xFC  // Bit2~7: 低位显示(右移2位)

// 数据3（buf[3]）位定义
#define HIGH_DISPLAY_MASK      0x3F  // Bit0~5: 高位显示
#define FULL_LOAD_BIT          6     // Bit6: 满载(0=否,1=是)
#define ARRIVAL_SIGNAL_BIT     7     // Bit7: 到站(0=否,1=是)

// 数据4（buf[4]）位定义
#define DOOR1_OPEN_BIT         0     // Bit0: 门1开(1=开)
#define DOOR1_CLOSE_BIT        1     // Bit1: 门1关(1=关)
#define DOOR2_OPEN_BIT         2     // Bit2: 门2开(1=开)
#define DOOR2_CLOSE_BIT        3     // Bit3: 门2关(1=关)
#define ELEVATOR_STATE_MASK    0xF0  // Bit4~7: 电梯状态(右移4位)

// 数据5（buf[5]）位定义
#define ELEVATOR_STATE2_MASK    0x0F  // Bit0~3: 保留位
#define FRONT_UP_TO_FLOOR_BIT   4     // Bit4: 前门上行到层(1=到达)
#define FRONT_DOWN_TO_FLOOR_BIT 5     // Bit5: 前门下行到层(1=到达)
#define BACK_UP_TO_FLOOR_BIT    6     // Bit6: 后门上行到层(1=到达)
#define BACK_DOWN_TO_FLOOR_BIT  7     // Bit7: 后门下行到层(1=到达)
// 数据6（buf[6]）位定义
#define FAULT_CODE_MASK        0xFF  // Bit0~7: 故障代码

// 其他配置参数
#define RX_TIMEOUT_MS          100  // 接收超时时间(ms)
#define DISP_BUF_LEN           6     // 显示字符串缓冲区长度（含结束符）

// IO编号定义
#define IO_TRAP_COMFORT       1
#define IO_FRONT_UP_TO_FLOOR  2
#define IO_FRONT_DOWN_TO_FLOOR 3
#define IO_BACK_UP_TO_FLOOR   4
#define IO_BACK_DOWN_TO_FLOOR 5
#define IO_OPEN_DOOR          6
#define IO_UP_DIRECTION       7
#define IO_DOWN_DIRECTION     8
#define IO_UP_RUNNING         9
#define IO_DOWN_RUNNING       10
#define IO_OVERLOAD_SIGNAL    11
#define IO_BUZZER_SIGNAL      12
#define IO_FIRE_SIGNAL        13
#define IO_FULL_LOAD_SIGNAL   14
#define IO_RESET_COMFORT      15
#define IO_RESET_RESCUE_LEVEL 16
#define IO_RESET_END          17
#define IO_SLEEP              18
#define IO_POWER_OFF          19
#define IO_RESTORE_DEFAULT    255

// 报文结构定义
struct received_command {
    unsigned char header[2];     // 0xAA 0x55
    unsigned char data_length;   // 固定为3
    unsigned char io_number;     // IO编号
    unsigned char io_data_high;  // 数据高位
    unsigned char io_data_low;   // 数据低位
    unsigned char checksum;      // 校验和
};// 电梯数据结构体(包含所有解析结果)

// 电梯数据结构体（新增disp成员存储最终显示字符串）
struct rt_elevator_t{
    // 数据1解析
    uint8_t dir_arrow;         // 方向箭头(0=上,1=下)
    uint8_t arrow_ctrl;        // 箭头控制(0~3)
    uint8_t car_overload;      // 轿厢超载(0=正常)
    uint8_t mid_code_low4;     // 中间位低4位

    // 数据2解析
    uint8_t mid_code_high2;    // 中间位高2位
    uint8_t mid_display_code;  // 中间位完整代码(0~63)
    uint8_t low_display_code;  // 低位显示代码(0~63)

    // 数据3解析
    uint8_t high_display_code; // 高位显示代码(0~63)
    uint8_t full_load;         // 满载(0=否)
    uint8_t arrival_signal;    // 到站(0=否)

    // 数据4解析
    uint8_t door1_open;        // 门1开
    uint8_t door1_close;       // 门1关
    uint8_t door2_open;        // 门2开
    uint8_t door2_close;       // 门2关
    uint8_t elevator_state;    // 电梯状态(0~15)

    // 数据5解析

    uint8_t energy_state;    // 电梯状态(1)
    uint8_t reset_appease_state;    // 电梯状态(2)
    uint8_t reset_help_state;    // 电梯状态(3)
    uint8_t reset_end_state;    // 电梯状态(4)
    uint8_t elevator_state2;    // 电梯状态(1~7)
    uint8_t front_up ;         // 前门上行到层
    uint8_t front_down ;         // 前门下行到层
    uint8_t back_up ;         // 后门上行到层
    uint8_t back_down ;         // 后门下行到层

    // 数据6解析
    uint8_t buzzer_state;      // 蜂鸣器状态(0=否,1=是)
    uint8_t power_off_state;    // 断电状态(0=否,1=是)
    uint8_t trap_comfort_state;    // 困人状态(0=否,1=是)
    uint8_t fault_code;        // 故障代码
    uint8_t video_state;      // 视频状态

    // 新增：显示相关
    char disp[DISP_BUF_LEN];   // 最终楼层显示字符串（如"B1"、"12"）
    uint8_t disp_len;          // 显示字符串长度（不含结束符）
} ;


// 函数声明
void uart_thread_entry(void *parameter);
void send_version_query(void);
void Cmdparsing(rt_uint8_t *buf);
unsigned int crc_chk_value(unsigned char *data_value, unsigned char length);
void send_custom_packet(rt_uint8_t io_number, rt_uint8_t io_data_high, rt_uint8_t io_data_low);

#endif /* __MY_UART_H__ */
