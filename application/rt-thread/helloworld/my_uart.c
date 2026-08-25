#include "my_uart.h"

#define break_check 0

// RS485方向控制宏
#define RS485_READ rt_pin_write(rs485_dir, PIN_LOW)   // 接收模式
#define RS485_WRITE rt_pin_write(rs485_dir, PIN_HIGH) // 发送模式

#define DOOR_OPEN 1
#define DOOR_CLOSE 2

static uint8_t uart_obs_cnt = 0;

volatile bool is_version_protocol = false;

// 状态映射表
const char *elevator_state_map[16] = {
    "检修", "井道自学习", "微动平层", "消防返基站", "消防员运行", "故障",
    "司机", "自动", "锁梯", "泊梯", "返平层",
    "应急运行", "电机自学习", "键盘调测", "基站检测", "VIP状态"};
// 状态映射表
const char *elevator_state_map2[8] = {
    "", "断电", "节能", "复位安抚", "复位救援平层", "复位结束",
    "蜂鸣信号", "困人安抚"};

// 显示映射表(0~63)
const char *display_table[64] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "A", "B", "G", "H", "L", "M", "P", "R", "-", "",
    "12", "13", "23", "C", "D", "E", "F", "I", "J", "K",
    "N", "O", "Q", "S", "T", "U", "V", "W", "X", "Y",
    "Z", "15", "17", "19", "22", "25", "32", "33", "B1", "-1",
    "14", "18", "", "", "", "", "", "", "", "",
    "", "", "", ""};

// 电梯数据结构体
const rt_uint16_t floor_str_len[64] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0};

// 全局变量
static struct rt_semaphore rx_sem;
static rt_device_t serial = RT_NULL;
static rt_base_t rs485_dir;
struct rt_elevator_t elevator_data = {8, 8, NULL};
static rt_uint8_t update_elevator = 0;
static rt_uint8_t pre_eledata[7] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// ==================== 新增：环形缓冲区实现 ====================
#define RING_BUF_SIZE          4096      // 4KB超大缓冲区，应对任何数据量
#define RX_TIMEOUT_TICKS       10        // 接收超时（ms），和原有一致

typedef struct {
    uint8_t buf[RING_BUF_SIZE];
    uint32_t r_ptr;    // 读指针（下一个要读取的位置）
    uint32_t w_ptr;    // 写指针（下一个要写入的位置）
    uint32_t len;      // 当前可读数据长度
} ring_buffer_t;

static ring_buffer_t rx_ring_buf;

/**
 * 初始化环形缓冲区
 */
static void ring_buffer_init(ring_buffer_t *rb) {
    memset(rb->buf, 0, RING_BUF_SIZE);
    rb->r_ptr = 0;
    rb->w_ptr = 0;
    rb->len = 0;
}

/**
 * 向环形缓冲区写入一个字节
 * @return 0成功，-1缓冲区满（自动覆盖旧数据）
 */
static int ring_buffer_write_byte(ring_buffer_t *rb, uint8_t byte) {
    if (rb->len >= RING_BUF_SIZE) {
        // 缓冲区满，覆盖最旧的数据（移动读指针）
        rb->r_ptr = (rb->r_ptr + 1) % RING_BUF_SIZE;
        rb->len--;
        static uint32_t overflow_count = 0;
        overflow_count++;
        if (overflow_count % 100 == 0) { // 每100次溢出打印一次
            rt_kprintf("⚠️  环形缓冲区溢出，累计次数：%d\n", overflow_count);
        }
    }

    rb->buf[rb->w_ptr] = byte;
    rb->w_ptr = (rb->w_ptr + 1) % RING_BUF_SIZE;
    rb->len++;

    return 0;
}

/**
 * 从环形缓冲区指定位置读取连续数据（处理回绕）
 * @param pos 相对于读指针的偏移量（0表示第一个可读字节）
 */
static void ring_buffer_read_at(ring_buffer_t *rb, uint8_t *dst, uint32_t pos, uint32_t len) {
    uint32_t start_idx = (rb->r_ptr + pos) % RING_BUF_SIZE;
    uint32_t first_part_len = RING_BUF_SIZE - start_idx;

    if (len <= first_part_len) {
        // 数据连续，一次读取
        memcpy(dst, &rb->buf[start_idx], len);
    } else {
        // 数据跨越缓冲区末尾，分两次读取
        memcpy(dst, &rb->buf[start_idx], first_part_len);
        memcpy(dst + first_part_len, rb->buf, len - first_part_len);
    }
}

/**
 * 移动环形缓冲区读指针（丢弃已处理的数据）
 */
static void ring_buffer_skip(ring_buffer_t *rb, uint32_t len) {
    if (len > rb->len) {
        len = rb->len;
    }

    rb->r_ptr = (rb->r_ptr + len) % RING_BUF_SIZE;
    rb->len -= len;
}

/**
 * 清空环形缓冲区
 */
static void ring_buffer_clear(ring_buffer_t *rb) {
    rb->r_ptr = 0;
    rb->w_ptr = 0;
    rb->len = 0;
    memset(rb->buf, 0, RING_BUF_SIZE);
}

/**
 * 计算两个 tick 值的差值（处理溢出）
 */
static rt_tick_t rt_tick_diff(rt_tick_t new_tick, rt_tick_t old_tick)
{
    if (new_tick >= old_tick) {
        return new_tick - old_tick;
    } else {
        return (RT_TICK_MAX - old_tick) + new_tick;
    }
}
// ==================== 环形缓冲区实现结束 ====================

// 计算校验和的函数，按照规则：SUM= ~（第3位+第4位+第5位）+ 1
static rt_uint8_t calculate_checksum(rt_uint8_t len, rt_uint8_t io_number, rt_uint8_t io_data_high, rt_uint8_t io_data_low)
{
    rt_uint8_t sum = 0;
    // 仅计算第3、4、5位的和（IO编号 + IO数据高位 + IO数据低位）
    sum = len + io_number + io_data_high + io_data_low;
    return ~sum + 1; // 取反加1得到校验和
}

// 发送版本查询报文（符合图片协议）
void send_version_query(void)
{
    rt_err_t ret = RT_EOK;
    rt_device_t serial = RT_NULL;
    rt_uint8_t packet[8] = {0}; // 新协议总长度 8 字节

    // 查找UART设备
    serial = rt_device_find(SAMPLE_UART_NAME);
    if (serial == RT_NULL)
    {
        rt_kprintf("UART设备查找失败！(设备名: %s)\n", SAMPLE_UART_NAME);
        return;
    }

    // 构造版本查询报文
    packet[0] = 0xAA;    // STX1
    packet[1] = 0x56;    // STX2
    packet[2] = 0x04;    // LEN = 4
    packet[3] = 0x00;    // TYPE = 0（查询）
    packet[4] = 0x00;    // DATA[0] = 00
    packet[5] = 0x00;    // DATA[1] = 00
    packet[6] = 0x00;    // DATA[2] = 00

    // 计算校验和 SUM = ~(LEN+TYPE+DATA) + 1
    rt_uint8_t sum = packet[2] + packet[3] + packet[4] + packet[5] + packet[6];
    packet[7] = (~sum) + 1;

    // 打印调试
    // rt_kprintf("发送版本查询报文: ");
    // for (int i = 0; i < 8; i++)
    // {
    //     rt_kprintf("0x%02X ", packet[i]);
    // }
    // rt_kprintf("\n");

    // 发送数据
    ret = rt_device_write(serial, 0, packet, sizeof(packet));
    if (ret != sizeof(packet))
    {
        rt_kprintf("版本查询报文发送失败，实际发送 %ld 字节\n", ret);
    }
    else
    {
        rt_kprintf("版本查询报文发送成功\n");
    }
}
// 解析版本协议回复报文
void process_version_packet(rt_uint8_t *buf)
{
    if (buf == NULL) return;

    // 1. 校验报文头
    if (buf[0] != 0xAA || buf[1] != 0x56)
    {
        rt_kprintf("版本协议帧头错误\n");
        return;
    }

    // 2. 校验长度
    if (buf[2] != 0x04)
    {
        rt_kprintf("版本协议长度错误: 0x%02X\n", buf[2]);
        return;
    }

    // 3. 校验类型（必须是回复 1）
    if (buf[3] != 0x01)
    {
        rt_kprintf("版本协议类型错误，不是回复帧: 0x%02X\n", buf[3]);
        return;
    }

    // 4. 校验和验证
    rt_uint8_t cal_sum = buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
    cal_sum = (~cal_sum) + 1;
    if (cal_sum != buf[7])
    {
        rt_kprintf("版本协议校验和错误: 计算=0x%02X, 接收=0x%02X\n", cal_sum, buf[7]);
        return;
    }

    // 5. 解析版本号
    rt_uint8_t ver_major = buf[4];
    rt_uint8_t ver_minor = buf[5];
    rt_uint8_t ver_patch = buf[6];
    rt_kprintf("收到版本回复: V%d.%d.%d\n", ver_major, ver_minor, ver_patch);
    uart_v_num[0] = ver_major;
    uart_v_num[1] = ver_minor;
    uart_v_num[2] = ver_patch;
    uart_v_flag = true;
    // 你可以在这里把版本号存到全局变量，供UI显示
    // elevator_version.major = ver_major;
    // elevator_version.minor = ver_minor;
    // elevator_version.patch = ver_patch;
}
// 发送符合特定格式报文的函数
// 报文格式：7字节
// [0] 0xAA (报文头1)
// [1] 0x55 (报文头2)
// [2] 0x03 (数据长度，固定为3)
// [3] IO编号
// [4] IO数据内容高位
// [5] IO数据内容低位
// [6] 校验和
void send_custom_packet(rt_uint8_t io_number, rt_uint8_t io_data_high, rt_uint8_t io_data_low)
{
    rt_err_t ret = RT_EOK;
    rt_device_t serial = RT_NULL;
    rt_uint8_t packet[7]; // 正确的7字节报文

    // 查找UART设备
    serial = rt_device_find(SAMPLE_UART_NAME);
    if (serial == RT_NULL)
    {
        rt_kprintf("UART设备查找失败！(设备名: %s)\n", SAMPLE_UART_NAME);
        return;
    }

    // 切换到发送模式
    // RS485_WRITE;
    rt_thread_mdelay(1); // 等待10ms
    // 构造报文
    // 构建报文
    memset(packet, 0, sizeof(packet));
    packet[0] = 0xAA;                                                           // 报文头1
    packet[1] = 0x55;                                                           // 报文头2
    packet[2] = 0x03;                                                           // 数据长度固定为3
    packet[3] = io_number;                                                      // 第3位：IO编号
    packet[4] = io_data_high;                                                   // 第4位：IO数据高位
    packet[5] = io_data_low;                                                    // 第5位：IO数据低位
    packet[6] = calculate_checksum(packet[2], packet[3], packet[4], packet[5]); // 第6位：校验和

    // 打印发送的报文信息（调试用）
    rt_kprintf("发送报文: ");
    for (int i = 0; i < 7; i++)
    {
        rt_kprintf("0x%02X ", packet[i]);
    }
    rt_kprintf("\n");

    // 发送数据
    ret = rt_device_write(serial, 0, packet, sizeof(packet));
    if (ret != sizeof(packet))
    {
        rt_kprintf("发送失败，实际发送 %ld 字节，期望7字节\n", ret);
    }
    else
    {
        rt_kprintf("7字节报文发送成功\n");
    }
    // 切换回接收模式
    // RS485_READ;
}

void send_test(void)
{
    static uint8_t send_cnt = 0;
    uint8_t send_io = 0;
    uint8_t io_num = 0;
    io_num = 4 * need_send_data;
    rt_kprintf("need_send_data = %d\n", need_send_data);
    if (need_send_data == 1)
    {
        if (send_cnt == 0)
            send_io = IO_UP_DIRECTION;
        else if (send_cnt == 1)
            send_io = IO_DOWN_DIRECTION;
    }
    else if (need_send_data == 2)
    {
        if (send_cnt == 0)
            send_io = IO_FIRE_SIGNAL;
        else if (send_cnt == 1)
            send_io = IO_OVERLOAD_SIGNAL;
    }
    else if (need_send_data == 3)
    {
        if (send_cnt == 0)
            send_io = IO_POWER_OFF;
        else if (send_cnt == 1)
            send_io = IO_SLEEP;
    }
    else if (need_send_data == 4)
    {
        if (send_cnt == 0)
            send_io = IO_RESET_COMFORT;
        else if (send_cnt == 1)
            send_io = IO_RESET_RESCUE_LEVEL;
    }
    else if (need_send_data == 5)
    {
        if (send_cnt == 0)
            send_io = IO_FULL_LOAD_SIGNAL;
        else if (send_cnt == 1)
            send_io = IO_RESET_END;
    }
    else if (need_send_data == 6)
    {
        if (send_cnt == 0)
            send_io = IO_BUZZER_SIGNAL;
        else if (send_cnt == 1)
            send_io = IO_OPEN_DOOR;
    }
    else if (need_send_data == 7)
    {
        if (send_cnt == 0)
            send_io = IO_UP_RUNNING;
        else if (send_cnt == 1)
            send_io = IO_DOWN_RUNNING;
    }
    else if (need_send_data == 8)
    {
        if (send_cnt == 0)
            send_io = IO_FRONT_UP_TO_FLOOR;
        else if (send_cnt == 1)
            send_io = IO_FRONT_DOWN_TO_FLOOR;
    }
    else if (need_send_data == 9)
    {
        if (send_cnt == 0)
            send_io = IO_BACK_UP_TO_FLOOR;
        else if (send_cnt == 1)
            send_io = IO_BACK_DOWN_TO_FLOOR;
    }
    else if (need_send_data == 10)
    {
        send_io = IO_TRAP_COMFORT;
    }
    if (send_cnt == 0)
    {
        send_custom_packet(send_io, IO_value[io_num - 4], IO_value[io_num - 3]);
    }
    else if (send_cnt == 1)
    {
        send_custom_packet(send_io, IO_value[io_num - 2], IO_value[io_num - 1]);
    }

    if (++send_cnt >= 2 || need_send_data == 10)
    {
        send_cnt = 0;
        need_send_data = 0;
    }
    rt_thread_mdelay(50);
}

/**
 * @brief 新增：组合高/中/低代码，生成最终显示字符串
 * @param high 高位代码(0~63)
 * @param mid  中位代码(0~63)
 * @param low  低位代码(0~63)
 * @return 显示字符串长度（不含结束符）
 */
static rt_uint8_t GetEleFloorDisp(rt_uint8_t high, rt_uint8_t mid, rt_uint8_t low)
{
    rt_uint8_t len_h, len_m, len_l, start_cnt = 0;
    rt_uint8_t floor_num[2] = {0, 0};

    // 打印输入参数，用于调试
    // rt_kprintf("【显示调试】输入参数: high=0x%02X, mid=0x%02X, low=0x%02X\n", high, mid, low);

    memset(elevator_data.disp, 0, DISP_BUF_LEN); // 清空显示缓冲区
    // rt_kprintf("【显示调试】清空显示缓冲区，初始状态: %s (长度: %d)\n", elevator_data.disp, start_cnt);

    // 1. 处理高位代码
    len_h = (high < 64) ? floor_str_len[high] : 0;
    // rt_kprintf("【显示调试】处理高位: high=0x%02X, 长度=%d\n", high, len_h);

    if (len_h == 1)
    {
        elevator_data.disp[start_cnt++] = display_table[high][0];
        // rt_kprintf("【显示调试】高位添加字符: '%c', 当前缓冲区: %s (长度: %d)\n",
        //            display_table[high][0], elevator_data.disp, start_cnt);
    }
    // len_h==2 暂未处理（可根据需求补充）
    else if (len_h == 2)
    {
        // rt_kprintf("【显示调试】高位长度为2，暂未处理\n");
    }

    // 2. 处理中位代码
    len_m = (mid < 64) ? floor_str_len[mid] : 0;
    // rt_kprintf("【显示调试】处理中位: mid=0x%02X, 长度=%d\n", mid, len_m);

    if (len_m == 2 && start_cnt + 2 <= DISP_BUF_LEN - 1) // 预留结束符位置
    {
        elevator_data.disp[start_cnt++] = display_table[mid][0];
        elevator_data.disp[start_cnt++] = display_table[mid][1];
        // rt_kprintf("【显示调试】中位添加2字符: '%c%c', 当前缓冲区: %s (长度: %d)\n",
        //            display_table[mid][0], display_table[mid][1], elevator_data.disp, start_cnt);
    }
    else if (len_m == 1 && start_cnt + 1 <= DISP_BUF_LEN - 1)
    {
        elevator_data.disp[start_cnt++] = display_table[mid][0];
        // rt_kprintf("【显示调试】中位添加1字符: '%c', 当前缓冲区: %s (长度: %d)\n",
        //    display_table[mid][0], elevator_data.disp, start_cnt);
    }
    else if (len_m > 0)
    {
        // rt_kprintf("【显示调试】中位长度=%d，但缓冲区空间不足，无法添加\n", len_m);
    }

    // 3. 处理低位代码（核心：特殊场景覆盖高位，只保留中位+低位）
    len_l = (low < 64) ? floor_str_len[low] : 0;
    // rt_kprintf("【显示调试】处理低位: low=0x%02X, 长度=%d\n", low, len_l);

    if (len_l == 2)
    {
        // 特殊场景：高位1字符 + 中位2字符 → 覆盖高位，只显示中位+低位
        if (len_h == 1 && len_m == 2)
        {
            // rt_kprintf("【显示调试】触发特殊场景: 高位1字符 + 中位2字符\n");
            memset(elevator_data.disp, 0, DISP_BUF_LEN);
            floor_num[0] = display_table[mid][0];
            floor_num[1] = display_table[mid][1];
            memcpy(elevator_data.disp, floor_num, 2); // 复制中位2字符
            elevator_data.disp[2] = 0;                // 确保结束符（避免超长）
            start_cnt = 2;
            // rt_kprintf("【显示调试】特殊场景(高1+中2)，显示: %s\n", elevator_data.disp);
        }
    }
    else if (len_l == 1)
    {
        if (len_h == 1 && len_m == 2)
        {
            // rt_kprintf("【显示调试】触发特殊场景: 高位1字符 + 中位2字符 + 低位1字符\n");
            // 特殊场景：高位1字符 + 中位2字符 → 覆盖高位，显示中位2字符+低位1字符
            if (3 <= DISP_BUF_LEN - 1) // 确保总长度≤5（留1位结束符）
            {
                elevator_data.disp[0] = display_table[mid][0];
                elevator_data.disp[1] = display_table[mid][1];
                elevator_data.disp[2] = display_table[low][0];
                start_cnt = 3;
            }
        }
        else if (len_h == 1 && len_m == 0)
        {
            // 场景：高位1字符 + 中位无 → 暂不处理（可根据需求补充）
        }
        else if (start_cnt + 1 <= DISP_BUF_LEN - 1)
        {
            // 普通场景：追加低位字符
            elevator_data.disp[start_cnt++] = display_table[low][0];
        }
    }

    elevator_data.disp[start_cnt] = '\0'; // 强制添加字符串结束符
    return start_cnt;
}

/**
 * @brief 处理并打印解析后的数据（新增显示字符串打印）
 */
static void process_elevator_data(void)
{

    static char floor_num[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                               'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                               'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                               'U', 'V', 'W', 'X', 'Y', 'Z', '-'};
    // if (!update_elevator)
    //     return;
    // update_elevator = 0;

    #if USE_PRI
    rt_kprintf("\n【显示结果】最终楼层显示: %s (长度: %d)\n",
        elevator_data.disp, elevator_data.disp_len);
        #endif
    static uint8_t cnt = 0;
    if (elevator_data.disp_len == 1)
    {
        for (int i = 0; i < sizeof(floor_num); i++)
        {
            if (elevator_data.disp[0] == floor_num[i])
            {
                show_floor_num[0] = i;
                break;
            }
        }
        if (show_floor_num[0] <= 9)
        {
            // 正确的字符转数字方式：单个字符减去'0'
            elevator_floor.floor = elevator_data.disp[0] - '0';
            if (MY_VOICE_SWITCH.floor)
                elevator_floor.state = FLOOR_NUM;
        }
        else if (elevator_data.disp[0] >= 'A' && elevator_data.disp[0] <= 'Z')
        {
            elevator_floor.floor = elevator_data.disp[0] - 'A';
            if (MY_VOICE_SWITCH.floor)
                elevator_floor.state = FLOOR_A_Z;
        }
        else
            rt_kprintf("elevator_data.disp[0] = %c\n", elevator_data.disp[0]);
    }
    else if (elevator_data.disp_len == 2)
    {
        for (int i = 0; i < sizeof(floor_num); i++)
        {
            if (elevator_data.disp[0] == floor_num[i])
            {
                show_floor_num[0] = i;
                cnt++;
            }
            if (elevator_data.disp[1] == floor_num[i])
            {
                show_floor_num[1] = i;
                cnt++;
            }
            if (cnt == 2)
            {
                cnt = 0;
                break;
            }
        }
        if (MY_VOICE_SWITCH.floor)
        {
            if (show_floor_num[0] >= 0 && show_floor_num[0] <= 9 &&
                show_floor_num[1] >= 0 && show_floor_num[1] <= 9)
            {
                // 正确计算两位数楼层
                elevator_floor.floor = (elevator_data.disp[0] - '0') * 10 + (elevator_data.disp[1] - '0');
                // 确保楼层在0-79范围内
                if (elevator_floor.floor <= 79)
                {

                #if USE_PRI
                rt_kprintf("0: %d, 1: %d, 当前楼层 = %d\n",
                    elevator_data.disp[0] - '0', elevator_data.disp[1] - '0', elevator_floor.floor);
                    #endif
                    elevator_floor.state = FLOOR_NUM;
                }
                else
                {
                    rt_kprintf("楼层超出范围 (0-79): %d\n", elevator_floor.floor);
                }
            }
            else if (elevator_data.disp[0] == '-' && show_floor_num[1] >= 1 && show_floor_num[1] <= 9) //-1 ~ -9
            {
                rt_kprintf("楼层-1 ~ -9\n");
                elevator_floor.floor = (elevator_data.disp[1] - '1');
                rt_kprintf("播放类型FLOOR_MINU\n");
                elevator_floor.state = FLOOR_MINU;
            }
            else if (show_floor_num[0] >= 1 && show_floor_num[0] <= 9)
            {
                elevator_floor.floor = (elevator_data.disp[0] - '1');

                if (elevator_data.disp[1] == 'A')
                    elevator_floor.state = FLOOR_1_9_A;
                else if (elevator_data.disp[1] == 'B')
                    elevator_floor.state = FLOOR_1_9_B;
                else if (elevator_data.disp[1] == 'C' && show_floor_num[0] <= 6)
                    elevator_floor.state = FLOOR_1_6_C;
                else if (elevator_data.disp[1] == 'F' && show_floor_num[0] <= 5)
                    elevator_floor.state = FLOOR_1_5_F;
            }
            else if (show_floor_num[1] >= 0 && show_floor_num[1] <= 9)
            {
                elevator_floor.floor = (elevator_data.disp[1] - '0');
                if (elevator_data.disp[0] == 'A')
                    elevator_floor.state = FLOOR_A_0_9;
                else if (elevator_data.disp[0] == 'B')
                    elevator_floor.state = FLOOR_B_0_9;
                else if (elevator_data.disp[0] == 'M')
                    elevator_floor.state = FLOOR_M_0_9;
                else if (elevator_data.disp[0] == 'P')
                    elevator_floor.state = FLOOR_P_0_9;
                else if (elevator_data.disp[0] == 'C' || elevator_data.disp[0] == 'D')
                {
                    if (show_floor_num[1] >= 1 && show_floor_num[1] <= 4)
                    {
                        elevator_floor.floor = (elevator_data.disp[1] - '1');
                        if (elevator_data.disp[0] == 'C')
                            elevator_floor.state = FLOOR_C_1_4;
                        else if (elevator_data.disp[0] == 'D')
                            elevator_floor.state = FLOOR_D_1_4;
                    }
                }
                else if (elevator_data.disp[0] == 'L' || elevator_data.disp[0] == 'R')
                {
                    if (show_floor_num[1] >= 1 && show_floor_num[1] <= 3)
                    {
                        elevator_floor.floor = (elevator_data.disp[1] - '1');
                        if (elevator_data.disp[0] == 'L')
                            elevator_floor.state = FLOOR_L_1_3;
                        else if (elevator_data.disp[0] == 'R')
                            elevator_floor.state = FLOOR_R_1_3;
                    }
                }
                else if (elevator_data.disp[0] == 'G')
                {
                    elevator_floor.floor = (elevator_data.disp[1] - '1');
                    if (show_floor_num[1] >= 1)
                        elevator_floor.state = FLOOR_G_1_9;
                }
            }
            else if (elevator_data.disp[0] == 'A' && elevator_data.disp[1] == 'F')
                elevator_floor.state = FLOOR_AF;
            else if (elevator_data.disp[0] == 'A' && elevator_data.disp[1] == 'G')
                elevator_floor.state = FLOOR_AG;
            else if (elevator_data.disp[0] == 'B' && elevator_data.disp[1] == 'E')
                elevator_floor.state = FLOOR_BE;
            else if (elevator_data.disp[0] == 'C' && elevator_data.disp[1] == 'F')
                elevator_floor.state = FLOOR_CF;
            else if (elevator_data.disp[0] == 'E' && elevator_data.disp[1] == 'G')
                elevator_floor.state = FLOOR_EG;
            else if (elevator_data.disp[0] == 'G' && elevator_data.disp[1] == 'F')
                elevator_floor.state = FLOOR_GF;
            else if (elevator_data.disp[0] == 'H' && elevator_data.disp[1] == 'P')
                elevator_floor.state = FLOOR_HP;
            else if (elevator_data.disp[0] == 'K' && elevator_data.disp[1] == 'G')
                elevator_floor.state = FLOOR_KG;
            else if (elevator_data.disp[0] == 'L' && elevator_data.disp[1] == 'B')
                elevator_floor.state = FLOOR_LB;
            else if (elevator_data.disp[0] == 'L' && elevator_data.disp[1] == 'D')
                elevator_floor.state = FLOOR_LD;
            else if (elevator_data.disp[0] == 'L' && elevator_data.disp[1] == 'F')
                elevator_floor.state = FLOOR_LF;
            else if (elevator_data.disp[0] == 'L' && elevator_data.disp[1] == 'G')
                elevator_floor.state = FLOOR_LG;
            else if (elevator_data.disp[0] == 'L' && elevator_data.disp[1] == 'L')
                elevator_floor.state = FLOOR_LL;
            else if (elevator_data.disp[0] == 'L' && elevator_data.disp[1] == 'P')
                elevator_floor.state = FLOOR_LP;
            else if (elevator_data.disp[0] == 'M' && elevator_data.disp[1] == 'R')
                elevator_floor.state = FLOOR_MR;
            else if (elevator_data.disp[0] == 'M' && elevator_data.disp[1] == 'Z')
                elevator_floor.state = FLOOR_MZ;
            else if (elevator_data.disp[0] == 'P' && elevator_data.disp[1] == 'B')
                elevator_floor.state = FLOOR_PB;
            else if (elevator_data.disp[0] == 'P' && elevator_data.disp[1] == 'C')
                elevator_floor.state = FLOOR_PC;
            else if (elevator_data.disp[0] == 'P' && elevator_data.disp[1] == 'H')
                elevator_floor.state = FLOOR_PH;
            else if (elevator_data.disp[0] == 'P' && elevator_data.disp[1] == 'M')
                elevator_floor.state = FLOOR_PM;
            else if (elevator_data.disp[0] == 'R' && elevator_data.disp[1] == 'F')
                elevator_floor.state = FLOOR_RF;
            else if (elevator_data.disp[0] == 'U' && elevator_data.disp[1] == 'B')
                elevator_floor.state = FLOOR_UB;
            else if (elevator_data.disp[0] == 'U' && elevator_data.disp[1] == 'F')
                elevator_floor.state = FLOOR_UF;
            else if (elevator_data.disp[0] == 'U' && elevator_data.disp[1] == 'G')
                elevator_floor.state = FLOOR_UG;
            else if (elevator_data.disp[0] == 'U' && elevator_data.disp[1] == 'P')
                elevator_floor.state = FLOOR_UP;

    #if USE_PRI
    rt_kprintf("elevator_data.disp[0] = %c\nelevator_data.disp[1] = %c\n",
        elevator_data.disp[0], elevator_data.disp[1]);
        #endif
        }
    }
    else if (elevator_data.disp_len == 3)
    {
        for (int i = 0; i < sizeof(floor_num); i++)
        {
            if (elevator_data.disp[0] == floor_num[i])
            {
                show_floor_num[0] = i;
                cnt++;
            }
            if (elevator_data.disp[1] == floor_num[i])
            {
                show_floor_num[1] = i;
                cnt++;
            }
            if (elevator_data.disp[2] == floor_num[i])
            {
                show_floor_num[2] = i;
                cnt++;
            }
            if (cnt == 3)
            {
                cnt = 0;
                break;
            }
        }
        if (MY_VOICE_SWITCH.floor)
        {
            if (elevator_data.disp[2] == 'A')
            {
                if (show_floor_num[0] == 1 && show_floor_num[1] == 2)
                    elevator_floor.state = FLOOR_12A;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 3)
                    elevator_floor.state = FLOOR_13A;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 4)
                    elevator_floor.state = FLOOR_14A;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 5)
                    elevator_floor.state = FLOOR_15A;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 7)
                    elevator_floor.state = FLOOR_17A;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 8)
                    elevator_floor.state = FLOOR_18A;
                else if (show_floor_num[0] == 2 && show_floor_num[1] == 3)
                    elevator_floor.state = FLOOR_23A;
                else if (show_floor_num[0] == 3 && show_floor_num[1] == 3)
                    elevator_floor.state = FLOOR_33A;
            }
            else if (elevator_data.disp[2] == 'B')
            {
                if (show_floor_num[0] == 1 && show_floor_num[1] == 2)
                    elevator_floor.state = FLOOR_12B;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 3)
                    elevator_floor.state = FLOOR_13B;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 4)
                    elevator_floor.state = FLOOR_14B;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 5)
                    elevator_floor.state = FLOOR_15B;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 7)
                    elevator_floor.state = FLOOR_17B;
                else if (show_floor_num[0] == 1 && show_floor_num[1] == 8)
                    elevator_floor.state = FLOOR_18B;
                else if (show_floor_num[0] == 2 && show_floor_num[1] == 3)
                    elevator_floor.state = FLOOR_23B;
                else if (show_floor_num[0] == 3 && show_floor_num[1] == 3)
                    elevator_floor.state = FLOOR_33B;
            }
        }
    }
    show_floor_flag = true;
    // 数据4解析结果
    // rt_kprintf("\n【数据4】门1: 开=%s/关=%s,运行状态: %d(%s)",
    //            elevator_data.door1_open ? "是" : "否",
    //            elevator_data.door1_close ? "是" : "否",
    //            elevator_data.elevator_state, elevator_state_map[elevator_data.elevator_state]);

    // rt_kprintf("\n【数据5】状态2: %s\n", elevator_state_map2[elevator_data.elevator_state2]);
    // rt_kprintf("\n【数据5】前门上状态: %d", elevator_data.front_up);
    // rt_kprintf("\n【数据5】前门下状态: %d", elevator_data.front_down);
    // rt_kprintf("\n【数据5】后门上状态: %d", elevator_data.back_up);
    // rt_kprintf("\n【数据5】后门下状态: %d", elevator_data.back_down);
    // 数据6解析结果
    // rt_kprintf("\n【数据6】故障代码: %d", elevator_data.fault_code);
    // rt_kprintf("\n====================\n");
}

/**
 * @brief UART接收回调函数
 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    if (size > 0)
        rt_sem_release(&rx_sem);
    return RT_EOK;
}

/**
 * @brief 校验和计算函数
 */
unsigned int crc_chk_value(unsigned char *data_value, unsigned char length)
{
    if (data_value == NULL || length == 0)
        return 0;
    unsigned int crc_value = 0xe67e;
    while (length--)
    {
        crc_value ^= *data_value++;
    }
    return crc_value;
}

/*
 * @brief 协议解析函数（新增调用GetEleFloorDisp生成显示字符串）
 */
void Cmdparsing(rt_uint8_t *buf)
{
    static volatile uint8_t last_open_door_state = 10;
    static volatile uint8_t last_close_door_state = 1;
    static volatile uint8_t last_fire = 0;

    static volatile uint8_t last_arrival = 0;
    static volatile uint8_t last_car_overload = 0;
    static volatile uint8_t last_arrow_ctrl = 0;

    static volatile uint8_t last_full_load = 0;

    static volatile uint8_t last_car_over = 0;

    static volatile bool elevator_up_down_flag = false;
    static volatile bool elevator_arr_flag = false;

    static volatile bool return_energy_flag = false; // 退出节能模式

    static uint8_t last_trap_comfort = 0;
    static uint8_t last_play_3 = 0;
    static uint8_t last_play_4 = 0;
    static uint8_t last_play_5 = 0;

    static bool date_is_renew = false;
    bool tcp_continue = false;
    static uint8_t tcp_need_cnt = 0;

    if (buf == NULL)
    {
        rt_kprintf("buf is NULL\n");
        return;
    }

        // 检查数据是否更新
    rt_uint8_t data_update = 0;

    for (int i = 0; i < sizeof(pre_eledata); i++)
    {
        if (buf[i] != pre_eledata[i])
        {
            data_update = 1;
            break;
        }
    }
    if(is_tcp_connected && MY_SET_DHCP.host_state == true)
    {
        if(tcp_need_cnt == 1 && data_update)
        {
            date_is_renew = false;
            tcp_need_cnt = 0;
            // rt_kprintf("信号一帧,切换继续发送\n");
        }else if((tcp_need_cnt >= 1 && tcp_need_cnt <= 3) && !data_update)
        {
            date_is_renew = false;
            tcp_continue = true;
        }

        if((data_update && !date_is_renew) || tcp_continue)
        {
            tcp_need_cnt ++;
            date_is_renew = true;
            // rt_kprintf("发送帧\n");
            rt_thread_mdelay(1);
            tcp_send_raw(&buf[0], 7);
        }else {
            date_is_renew = false;
            tcp_need_cnt = 0;
        }
    }

    // 1. 解析数据1(buf[1])
    elevator_data.dir_arrow = (buf[1] >> DIR_ARROW_BIT) & 0x01;
    elevator_data.arrow_ctrl = (buf[1] >> ARROW_CTRL_BIT1) & 0x03;
    elevator_data.car_overload = (buf[1] >> CAR_OVERLOAD_BIT) & 0x01;
    elevator_data.mid_code_low4 = (buf[1] & mid_code_low4_MASK) >> 4;

    // 2. 解析数据2(buf[2])
    elevator_data.mid_code_high2 = buf[2] & mid_code_high2_MASK;
    elevator_data.mid_display_code = (elevator_data.mid_code_high2 << 4) | elevator_data.mid_code_low4;
    elevator_data.low_display_code = (buf[2] & LOW_DISPLAY_MASK) >> 2;

    // 3. 解析数据3(buf[3])
    elevator_data.high_display_code = buf[3] & HIGH_DISPLAY_MASK;
    elevator_data.full_load = (buf[3] >> FULL_LOAD_BIT) & 0x01;
    elevator_data.arrival_signal = (buf[3] >> ARRIVAL_SIGNAL_BIT) & 0x01;

    // 4. 解析数据4(buf[4])
    elevator_data.door1_open = (buf[4] >> DOOR1_OPEN_BIT) & 0x01;
    elevator_data.door1_close = (buf[4] >> DOOR1_CLOSE_BIT) & 0x01;
    elevator_data.door2_open = (buf[4] >> DOOR2_OPEN_BIT) & 0x01;
    elevator_data.door2_close = (buf[4] >> DOOR2_CLOSE_BIT) & 0x01;
    elevator_data.elevator_state = (buf[4] & ELEVATOR_STATE_MASK) >> 4;
    // 新增：解析数据5(buf[5])
    // elevator_data.elevator_state2 = (buf[5] & ELEVATOR_STATE2_MASK);
    elevator_data.power_off_state = (buf[5] >> 0) & 0x01;
    elevator_data.energy_state = (buf[5] >> 1) & 0x01;
    elevator_data.reset_appease_state = (buf[5] >> 2) & 0x01;
    elevator_data.reset_help_state = (buf[5] >> 3) & 0x01;
    elevator_data.front_up = (buf[5] >> FRONT_UP_TO_FLOOR_BIT) & 0x01;
    elevator_data.front_down = (buf[5] >> FRONT_DOWN_TO_FLOOR_BIT) & 0x01;
    elevator_data.back_up = (buf[5] >> BACK_UP_TO_FLOOR_BIT) & 0x01;
    elevator_data.back_down = (buf[5] >> BACK_DOWN_TO_FLOOR_BIT) & 0x01;

    // 5. 解析数据6(buf[6])
    elevator_data.reset_end_state = (buf[6] >> 0) & 0x01;
    elevator_data.buzzer_state = (buf[6] >> 1) & 0x01;
    elevator_data.trap_comfort_state = (buf[6] >> 2) & 0x01;
    // 提取 buf[6] 的 bit6、bit5 两位
    elevator_data.video_state = (buf[6] >> 5) & 0x03;
    // elevator_data.fault_code = buf[6] & FAULT_CODE_MASK;
    // 新增：调用函数生成最终显示字符串
    elevator_data.disp_len = GetEleFloorDisp(elevator_data.high_display_code,
                                             elevator_data.mid_display_code,
                                             elevator_data.low_display_code);

    if (elevator_data.energy_state)
        io_state_flag[NUM_SLEEP] = true;
    else
        io_state_flag[NUM_SLEEP] = false;
    if (elevator_data.reset_appease_state)
        io_state_flag[NUM_RESET_COMFORT] = true;
    else
        io_state_flag[NUM_RESET_COMFORT] = false;
    if (elevator_data.reset_help_state)
        io_state_flag[NUM_RESET_RESCUE_LEVEL] = true;
    else
        io_state_flag[NUM_RESET_RESCUE_LEVEL] = false;
    if (elevator_data.reset_end_state)
        io_state_flag[NUM_RESET_END] = true;
    else
        io_state_flag[NUM_RESET_END] = false;
    if (elevator_data.buzzer_state)
        io_state_flag[NUM_BUZZER_SIGNAL] = true;
    else
    {
        io_state_flag[NUM_BUZZER_SIGNAL] = false;
        have_buzzer_flag = false;
    }
    if (elevator_data.power_off_state)
        io_state_flag[NUM_POWER_OFF] = true;
    else
        io_state_flag[NUM_POWER_OFF] = false;
    if (elevator_data.trap_comfort_state)
        io_state_flag[NUM_TRAP_COMFORT] = true;
    else
        io_state_flag[NUM_TRAP_COMFORT] = false;

    if (elevator_data.elevator_state == 4)
        MY_IO_FLAG.fire = true;
    else
        MY_IO_FLAG.fire = false;

    if (elevator_data.arrow_ctrl != 3)
    {
        MY_IO_FLAG.dir_arrow = true;
        IO_dir_arrow_value = elevator_data.dir_arrow;
    }
    else
        MY_IO_FLAG.dir_arrow = false;
    if (elevator_data.car_overload)
        MY_IO_FLAG.car_overload = true;
    else
        MY_IO_FLAG.car_overload = false;
    if (elevator_data.door1_open)
        MY_IO_FLAG.open_door = true; // 为1（在io界面就设置io界面提示）
    else
        MY_IO_FLAG.open_door = false;
    if (elevator_data.front_up)
        MY_IO_FLAG.front_up = true;
    else
        MY_IO_FLAG.front_up = false;
    if (elevator_data.front_down)
        MY_IO_FLAG.front_down = true;
    else
        MY_IO_FLAG.front_down = false;
    if (elevator_data.back_up)
        MY_IO_FLAG.back_up = true;
    else
        MY_IO_FLAG.back_up = false;
    if (elevator_data.back_down)
        MY_IO_FLAG.back_down = true;
    else
        MY_IO_FLAG.back_down = false;
    if (elevator_data.full_load)
        MY_IO_FLAG.full_load = true;
    else
        MY_IO_FLAG.full_load = false;
#if break_check
#else

    if (!data_update)
    {
        // rt_kprintf("数据未更新\n");
        return;
    }


    // if(is_tcp_connected && MY_SET_DHCP.host_state == true)
    //     rt_thread_mdelay(140);
    // rt_kprintf("数据有变化\n");
    memset(pre_eledata, 0, sizeof(pre_eledata));
    memcpy(pre_eledata, buf, sizeof(pre_eledata));
#endif
    process_elevator_data();
    if(elevator_data.video_state)
    {
        video_udp_state = elevator_data.video_state;
        rt_kprintf("视频状态: %d\n", elevator_data.video_state);
    }
    // if (elevator_data.elevator_state2 == VIDEO_BUZZER_SIGNAL && (my_page == PAGE_HOME_HOR || my_page == PAGE_HOME)) // 新增：处理蜂鸣器信号
    if (elevator_data.buzzer_state && (my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.voice) // 新增：处理蜂鸣器信号
    {
        last_state2_num = VIDEO_BUZZER_SIGNAL;
        rt_kprintf("蜂鸣器信号\n");
    }
    else
        last_state2_num = 0;

    // if ((last_play_3 == 1 || last_play_4 == 1 || last_play_5 == 1 || last_trap_comfort == 1) && elevator_data.elevator_state2 == 0)
    // {
    //     last_play_3 = 0;
    //     last_play_4 = 0;
    //     last_play_5 = 0;
    //     last_trap_comfort = 0;
    // }
    if (last_play_3 == 1 && !elevator_data.reset_appease_state)
    {
        last_play_3 = 0;
    }
    if (last_play_4 == 1 && !elevator_data.reset_help_state)
    {
        last_play_4 = 0;
    }
    if (last_play_5 == 1 && !elevator_data.reset_end_state)
    {
        last_play_5 = 0;
    }
    if (last_trap_comfort == 1 && !elevator_data.trap_comfort_state)
    {
        last_trap_comfort = 0;
    }
    // if ((last_play_3 == 1 || last_play_4 == 1 || last_play_5 == 1 || last_trap_comfort == 1) && elevator_data.elevator_state2 == 0)
    // {
    //     last_play_3 = 0;
    //     last_play_4 = 0;
    //     last_play_5 = 0;
    //     last_trap_comfort = 0;
    // }
    // if (energy_flag && elevator_data.elevator_state2 != VIDEO_SLEEP)
    if (energy_flag && !elevator_data.energy_state)
    {
        rt_kprintf("节能模式状态切换，准备退出\n");
        return_energy_flag = true;
        energy_flag = false;
    }
    else if (!MY_VOICE_SWITCH.lr && energy_show_image_flag)
    {
        rt_kprintf("来信号了，正在处理退出节能模式");
        elevator_change_no_lr_flag = true;
    }
    else if (!MY_VOICE_SWITCH.lr)
    {
        accumulated_seconds = 0;
    }
    if (!elevator_data.buzzer_state)
        have_buzzer_flag = false;
    if (now_play_video_num)
    {
        // if (!elevator_data.elevator_state2)
        // {
        // have_buzzer_flag = false;
        // if (now_play_video_num != VIDEO_FIRE && now_play_video_num != VIDEO_OVERLOAD)
        // {
        //     rt_kprintf("清除VIDEO_FIRE\n");
        //     if (play_elevator.video_fire)
        //         play_elevator.video_fire = false;
        //     if (play_elevator.music_fire)
        //         play_elevator.music_fire = false;
        //     if (fire_music_mp3_flag)
        //         fire_music_mp3_flag = false;
        //     if (fire_start_flag)
        //         fire_start_flag = false;
        //     if (play_elevator.video_fire)
        //         play_elevator.video_fire = false;
        // }
        //
        if (!elevator_data.reset_appease_state)
        {
            rt_kprintf("清除VIDEO_RESET_APPEASE\n");
            if (play_elevator.music_appease)
                play_elevator.music_appease = false;
            if (appease_music_mp3_flag)
                appease_music_mp3_flag = false;
        }

        //
        if (!elevator_data.reset_appease_state)
        {
            if (now_play_video_num != VIDEO_RESET_APPEASE)
            {
                rt_kprintf("清除VIDEO_RESET_APPEASE\n");
                if (last_play_3)
                    last_play_3 = 0;
                if (in_reset_flag)
                {
                    rt_kprintf("清除VIDEO_RESET_APPEASE in_reset_flag\n");
                    in_reset_flag = false;
                }
                if (in_reset_cn_flag)
                {
                    rt_kprintf("清除VIDEO_RESET_APPEASE in_reset_cn_flag\n");
                    in_reset_cn_flag = false;
                }
                if (reset_appease_music_mp3_flag)
                {
                    rt_kprintf("清除VIDEO_RESET_APPEASE reset_appease_music_mp3_flag\n");
                    reset_appease_music_mp3_flag = false;
                }
                if (reset_appease_start_flag)
                {
                    rt_kprintf("清除VIDEO_RESET_APPEASE reset_appease_start_flag\n");
                    reset_appease_start_flag = false;
                }
                if (play_elevator.video_reset_appease)
                {
                    rt_kprintf("清除VIDEO_RESET_APPEASE video_reset_appease\n");
                    play_elevator.video_reset_appease = false;
                }
                if (play_elevator.music_reset_appease)
                {
                    rt_kprintf("清除VIDEO_RESET_APPEASE music_reset_appease\n");
                    play_elevator.music_reset_appease = false;
                }
            }
        }
        //
        if (!elevator_data.reset_help_state)
        {
            if (now_play_video_num != VIDEO_RESET_HELP_APPEASE)
            {
                rt_kprintf("清除VIDEO_RESET_HELP_APPEASE\n");
                if (in_reset_help_flag)
                    in_reset_help_flag = false;
                if (last_play_4)
                    last_play_4 = 0;
                if (in_reset_help_cn_flag)
                    in_reset_help_cn_flag = false;

                if (reset_help_appease_start_flag)
                    reset_help_appease_start_flag = false;
                if (play_elevator.video_reset_help_appease)
                    play_elevator.video_reset_help_appease = false;
                if (play_elevator.music_reset_help_appease)
                    play_elevator.music_reset_help_appease = false;
            }
        }
        //
        if (!elevator_data.reset_end_state)
        {
            if (now_play_video_num != VIDEO_RESET_END_APPEASE)
            {
                rt_kprintf("清除VIDEO_RESET_END_APPEASE\n");
                if (in_reset_end_flag)
                    in_reset_end_flag = false;
                if (last_play_5)
                    last_play_5 = 0;
                if (in_reset_end_cn_flag)
                    in_reset_end_cn_flag = false;

                if (reset_end_appease_start_flag)
                    reset_end_appease_start_flag = false;
                if (play_elevator.video_reset_end_appease)
                    play_elevator.video_reset_end_appease = false;
                if (play_elevator.music_reset_end_appease)
                    play_elevator.music_reset_end_appease = false;
            }
        }
        // }
    }
    if (elevator_data.energy_state)
    {
        if (MY_VOICE_SWITCH.lr)
        {
            if (last_elevator_state2 == VIDEO_SLEEP)
            {
                rt_kprintf("进入节能模式,已有\n");
                if (!energy_show_image_flag)
                    energy_show_image_flag = true;
                energy_flag = true;
            }
            else
            {
                if (!energy_show_image_flag)
                    energy_show_image_flag = true;
                rt_kprintf("进入节能模式,第一次\n");
                energy_flag = true;
                up_down_cnt = 0;
                in_arr_flag = false;
                memset(&my_cnt, 0, sizeof(my_cnt));
                if (is_tcp_connected && MY_SET_DHCP.host_state == false)
                {

                }else{
                    backlight_set(MY_SET.e_con_backlight);
                }
                if (is_tcp_connected && MY_SET_DHCP.host_state == true)
                {
                    send_light(true, MY_SET.e_con_backlight);
                }
                music_renew_flag = true;
                energy_conservation = true;
            }
        }
    }
    if (elevator_data.trap_comfort_state)
    {
        if (last_trap_comfort == 1)
        {
        }
        else
        {
            if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.appease && MY_VOICE_SWITCH.voice)
            {
                last_trap_comfort = 1;
                if (MY_SET.play_mode == PLAY_VIDEO)
                {
                    rt_kprintf("困人安抚pcm\n");
                    if (!energy_flag)
                        play_elevator.music_appease = true;
                }
                else
                {
                    if (!energy_flag)
                        appease_music_mp3_flag = true;
                    rt_kprintf("困人安抚mp3\n");
                }
            }
        }
    }
    if (elevator_data.reset_appease_state)
    {
        if (last_play_3 == 1)
        {
        }
        else
        {
            if (!in_reset_flag && !in_reset_cn_flag && MY_VOICE_SWITCH.appease)
            {
                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.voice)
                {
                    if (now_play_video_num == VIDEO_RESET_APPEASE || reset_appease_music_mp3_flag)
                    {
                        rt_kprintf("复位安抚还在播放\n");
                    }
                    else
                    {
                        last_play_3 = 1;
                        if (voice_language == VOICE_CN)
                        {
                            in_reset_cn_flag = true;
                            play_elevator.video_reset_appease = true;
                            rt_kprintf("复位安抚视频");
                        }
                        else if (voice_language == VOICE_EN)
                        {
                            if (MY_SET.play_mode == PLAY_VIDEO)
                            {
                                rt_kprintf("复位安抚pcm");
                                if (!energy_flag)
                                    play_elevator.music_reset_appease = true;
                            }
                            else
                            {
                                if (!energy_flag)
                                    reset_appease_music_mp3_flag = true;
                                rt_kprintf("复位安抚mp3");
                            }
                        }
                        else
                        {
                            rt_kprintf("复位安抚视频和音乐");
                            in_reset_flag = true;
                            play_elevator.video_reset_appease = true;
                            if (!energy_flag)
                                reset_appease_start_flag = true;
                        }
                    }
                }
            }
        }
    }
    if (elevator_data.reset_help_state)
    {
        if (last_play_4 == 1)
        {
        }
        else
        {
            if (!in_reset_help_flag && !in_reset_help_cn_flag)
            {
                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.appease && MY_VOICE_SWITCH.voice)
                {
                    if (now_play_video_num == VIDEO_RESET_HELP_APPEASE || reset_help_appease_music_mp3_flag)
                    {
                        rt_kprintf("复位救援平层安抚还在播放\n");
                    }
                    else
                    {
                        last_play_4 = 1;
                        if (voice_language == VOICE_CN)
                        {
                            in_reset_help_cn_flag = true;
                            play_elevator.video_reset_help_appease = true;
                        }
                        else if (voice_language == VOICE_EN)
                        {
                            if (MY_SET.play_mode == PLAY_VIDEO)
                            {
                                rt_kprintf("复位救援平层安抚pcm");
                                play_elevator.music_reset_help_appease = true;
                            }
                            else
                            {
                                if (!energy_flag)
                                    reset_help_appease_music_mp3_flag = true;
                                rt_kprintf("复位救援平层安抚mp3");
                            }
                        }
                        else
                        {
                            in_reset_help_flag = true;
                            play_elevator.video_reset_help_appease = true;
                            if (!energy_flag)
                                reset_help_appease_start_flag = true;
                        }
                    }
                }
            }
        }
    }
    if (elevator_data.reset_end_state)
    {
        if (last_play_5 == 1)
        {
        }
        else
        {
            if (!in_reset_end_flag && !in_reset_end_cn_flag)
            {
                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.appease && MY_VOICE_SWITCH.voice)
                {
                    if (now_play_video_num == VIDEO_RESET_END_APPEASE || reset_end_appease_music_mp3_flag)
                    {
                        rt_kprintf("复位结束安抚还在播放\n");
                    }
                    else
                    {
                        last_play_5 = 1;
                        if (voice_language == VOICE_CN)
                        {
                            in_reset_end_cn_flag = true;
                            play_elevator.video_reset_end_appease = true;
                        }
                        else if (voice_language == VOICE_EN)
                        {
                            if (MY_SET.play_mode == PLAY_VIDEO)
                            {
                                rt_kprintf("复位结束安抚pcm");
                                if (!energy_flag)
                                    play_elevator.music_reset_end_appease = true;
                            }
                            else
                            {
                                if (!energy_flag)
                                    reset_end_appease_music_mp3_flag = true;
                                rt_kprintf("复位结束安抚mp3");
                            }
                        }
                        else
                        {
                            in_reset_end_flag = true;
                            play_elevator.video_reset_end_appease = true;
                            if (!energy_flag)
                                reset_end_appease_start_flag = true;
                        }
                    }
                }
            }
        }
    }
    if (elevator_data.buzzer_state)
    {
        if (voice_start_work && (my_page == PAGE_HOME_HOR || my_page == PAGE_HOME))
            play_elevator.music_buzzer = true;
    }
    if (elevator_data.elevator_state2)
    {
        // 根据状态值设置对应的标志（这里需要根据实际状态定义修改）
        switch (elevator_data.elevator_state2)
        {
        case VIDEO_SLEEP:
            if (MY_VOICE_SWITCH.lr)
            {
                if (last_elevator_state2 == VIDEO_SLEEP)
                {
                    rt_kprintf("进入节能模式,已有\n");
                    if (!energy_show_image_flag)
                        energy_show_image_flag = true;
                    energy_flag = true;
                }
                else
                {
                    if (!energy_show_image_flag)
                        energy_show_image_flag = true;
                    rt_kprintf("进入节能模式,第一次\n");
                    up_down_cnt  = 0;
                    in_arr_flag = false;
                    memset(&my_cnt, 0, sizeof(my_cnt));
                    energy_flag = true;
                    if (is_tcp_connected && MY_SET_DHCP.host_state == false)
                    {

                    }else{

                        backlight_set(MY_SET.e_con_backlight);
                    }
                    if (is_tcp_connected && MY_SET_DHCP.host_state == true)
                    {
                        send_light(true, MY_SET.e_con_backlight);
                    }
                    music_renew_flag = true;
                    energy_conservation = true;
                }
            }
            break;
        case VIDEO_POWER_OFF:
            break;
        case VIDEO_TRAP_COMFORT:
            if (last_trap_comfort == 1)
            {
                break;
            }
            if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.appease && MY_VOICE_SWITCH.voice)
            {
                last_trap_comfort = 1;
                if (MY_SET.play_mode == PLAY_VIDEO)
                {
                    rt_kprintf("困人安抚pcm\n");
                    play_elevator.music_appease = true;
                }
                else
                {
                    appease_music_mp3_flag = true;
                    rt_kprintf("困人安抚mp3\n");
                }
            }
            break;
        case VIDEO_RESET_COMFORT:
            if (last_play_3 == 1)
            {
                break;
            }
            if (!in_reset_flag && !in_reset_cn_flag && MY_VOICE_SWITCH.appease)
            {
                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.voice)
                {
                    last_play_3 = 1;
                    if (voice_language == VOICE_CN)
                    {
                        in_reset_cn_flag = true;
                        play_elevator.video_reset_appease = true;
                    }
                    else if (voice_language == VOICE_EN)
                    {
                        if (MY_SET.play_mode == PLAY_VIDEO)
                        {
                            rt_kprintf("复位安抚pcm");
                            play_elevator.music_reset_appease = true;
                        }
                        else
                        {
                            reset_appease_music_mp3_flag = true;
                            rt_kprintf("复位安抚mp3");
                        }
                    }
                    else
                    {
                        rt_kprintf("复位安抚视频和音乐");
                        in_reset_flag = true;
                        play_elevator.video_reset_appease = true;
                        reset_appease_start_flag = true;
                    }
                }
            }
            break;
        case VIDEO_RESET_RESCUE_LEVEL:
            if (last_play_4 == 1)
            {
                break;
            }
            if (!in_reset_help_flag && !in_reset_help_cn_flag)
            {
                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.appease && MY_VOICE_SWITCH.voice)
                {
                    last_play_4 = 1;
                    if (voice_language == VOICE_CN)
                    {
                        in_reset_help_cn_flag = true;
                        play_elevator.video_reset_help_appease = true;
                    }
                    else if (voice_language == VOICE_EN)
                    {
                        if (MY_SET.play_mode == PLAY_VIDEO)
                        {
                            rt_kprintf("复位救援平层安抚pcm");
                            play_elevator.music_reset_help_appease = true;
                        }
                        else
                        {
                            reset_help_appease_music_mp3_flag = true;
                            rt_kprintf("复位救援平层安抚mp3");
                        }
                    }
                    else
                    {
                        in_reset_help_flag = true;
                        play_elevator.video_reset_help_appease = true;
                        reset_help_appease_start_flag = true;
                    }
                }
            }
            break;
        case VIDEO_RESET_END:
            if (last_play_5 == 1)
            {
                break;
            }
            if (!in_reset_end_flag && !in_reset_end_cn_flag)
                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.appease && MY_VOICE_SWITCH.voice)
                {
                    last_play_5 = 1;
                    if (voice_language == VOICE_CN)
                    {
                        in_reset_end_cn_flag = true;
                        play_elevator.video_reset_end_appease = true;
                    }
                    else if (voice_language == VOICE_EN)
                    {
                        if (MY_SET.play_mode == PLAY_VIDEO)
                        {
                            rt_kprintf("复位结束安抚pcm");
                            play_elevator.music_reset_end_appease = true;
                        }
                        else
                        {
                            reset_end_appease_music_mp3_flag = true;
                            rt_kprintf("复位结束安抚mp3");
                        }
                    }
                    else
                    {
                        in_reset_end_flag = true;
                        play_elevator.video_reset_end_appease = true;
                        reset_end_appease_start_flag = true;
                    }
                }
            break;
        case VIDEO_BUZZER_SIGNAL:
            if (voice_start_work && (my_page == PAGE_HOME_HOR || my_page == PAGE_HOME))
                play_elevator.music_buzzer = true;
            break;
        // ... 补充其他16个状态的case
        default:
            break;
        }
        last_elevator_state2 = elevator_data.elevator_state2;
        elevator_data.elevator_state2 = 0;
        rt_kprintf("last_elevator_state2 = %d\n", last_elevator_state2);
    }
    // process_elevator_data();
    if (energy_flag)
    {
        elevator_change_flag = false;
        // rt_kprintf("有节能信号，只处理超载和消防\n");
        // return;
    }

    if (last_fire != elevator_data.elevator_state)
    {
        if (last_fire == 4 && elevator_data.elevator_state != 4)
        {
            have_fire_flag = false;
            rt_kprintf("have_fire_flag = false\n");
        }
        last_fire = elevator_data.elevator_state;

        if (elevator_data.elevator_state == 4)
        {
            have_fire_flag = true;
            if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && MY_VOICE_SWITCH.fire && MY_VOICE_SWITCH.voice)
            {
                if (now_play_video_num == VIDEO_FIRE || fire_music_mp3_flag)
                {
                    rt_kprintf("消防还在播放\n");
                }
                else
                {
                    if (voice_language == VOICE_CN)
                        play_elevator.video_fire = true;
                    else if (voice_language == VOICE_EN)
                    {
                        if (MY_SET.play_mode == PLAY_VIDEO)
                        {
                            rt_kprintf("消防pcm\n");
                            if (!energy_flag)
                                play_elevator.music_fire = true;
                        }
                        else
                        {
                            if (!energy_flag)
                                fire_music_mp3_flag = true;
                            rt_kprintf("消防mp3\n");
                        }
                    }
                    else
                    {
                        rt_kprintf("消防视频加音乐\n");
                        play_elevator.video_fire = true;
                        if (!energy_flag)
                            fire_start_flag = true;
                    }
                }
            }
        }
        else
            play_elevator.music_fire = false;
    }
    // last_arrow_ctrl = elevator_data.arrow_ctrl;
    if (elevator_arrow_flag && !energy_flag)
    {
        if (elevator_data.arrow_ctrl == 3)
            elevator_arrow_flag = false;
    }
    if (elevator_data.arrow_ctrl != 3 && (my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && !energy_flag)
    {

    #if USE_PRI
    rt_kprintf("进入判断箭头方向\n");
    rt_kprintf("elevator_data.arrow_ctrl = %d\n", elevator_data.arrow_ctrl);
    rt_kprintf("elevator_data.dir_arrow = %d\n", elevator_data.dir_arrow);
    rt_kprintf("elevator_arr_flag = %d\n", elevator_arr_flag);
    rt_kprintf("up_down_cnt = %d\n", up_down_cnt);
    rt_kprintf("in_arr_flag = %d\n", in_arr_flag);
    rt_kprintf("elevator_arrow_flag = %d\n", elevator_arrow_flag);
    #endif
        if (elevator_data.door1_open)
        {
            uart_obs_cnt = 1;
            static uint8_t temp = 0;
            if (play_elevator.elevator_up || up_music_mp3_flag)
            {
                up_or_down = 1;
                temp = 1;
            }
            else if (play_elevator.elevator_down || down_music_mp3_flag)
            {
                temp = 2;
                up_or_down = 2;
            }
            else if (temp == 1)
            {
                up_or_down = 1;
            }
            else if (temp == 2)
            {
                up_or_down = 2;
            }
            // rt_kprintf("up_or_down = %d\n", up_or_down);

            // play_elevator.elevator_up = 0;
            // play_elevator.elevator_down = 0;
            if (up_or_down)
            {
                // rt_kprintf("start_obs_flag = true1\n");
                start_obs_flag = true;
            }
        }
        if ((elevator_arr_flag || up_down_cnt == 0) && !in_arr_flag && voice_start_work)
        {
            up_down_cnt++;
            if(up_down_cnt > 100) up_down_cnt = 1;
            elevator_arr_flag = false;
            if(uart_obs_cnt == 1)
                uart_obs_cnt = 2;
            else
                uart_obs_cnt = 0;
            #if USE_PRI
            #endif
            // rt_kprintf("IO_dir_arrow_value: %s\n", IO_dir_arrow_value == 0 ? "上行" : "下行");
            up_or_down = (IO_dir_arrow_value == 0 ? 1 : 2);
            if (IO_dir_arrow_value == 0)
                set_gif_arrow = UP_ARROW;
            else if (IO_dir_arrow_value == 1)
                set_gif_arrow = DOWN_ARROW;
            elevator_arrow_flag = true;
            if (MY_VOICE_SWITCH.up)
            {
                if (MY_SET.play_mode == PLAY_VIDEO)
                {
                    play_elevator.elevator_up = (IO_dir_arrow_value == 0);
                    play_elevator.elevator_down = (IO_dir_arrow_value == 1);
                    if(play_elevator.elevator_up) my_cnt.up ++;
                    else if(play_elevator.elevator_down) my_cnt.down ++;
                }
                else
                {
                    up_music_mp3_flag = (IO_dir_arrow_value == 0);
                    down_music_mp3_flag = (IO_dir_arrow_value == 1);
                }
            }
            // elevator_up_down_flag = true;
            #if USE_PRI
            rt_kprintf("play_elevator.elevator_up = %d\n", play_elevator.elevator_up);
            rt_kprintf("play_elevator.elevator_down = %d\n", play_elevator.elevator_down);
            #endif
        }

    }

    if (last_car_over != elevator_data.car_overload || have_overload_flag)
    {
        if (last_car_over == 1 && elevator_data.car_overload == 0)
        {
            if (video_select)
            {
                rt_kprintf("清除VIDEO_OVERLOAD\n");
                if (overload_music_pcm_flag)
                    overload_music_pcm_flag = false;
                if (overload_music_mp3_flag)
                    overload_music_mp3_flag = false;
                if (play_elevator.video_overload)
                    play_elevator.video_overload = false;
                if (play_elevator.music_overload)
                    play_elevator.music_overload = false;
                // if (overload_start_flag)
                //     overload_start_flag = false;
                if (video_select)
                {
                    delete_over_flag = true;
                }
                have_overload_flag = false;
                if(voice_language == VOICE_CN_EN && MY_SET.play_mode == PLAY_IMAGE)
                {
                    rt_kprintf("wait_overload_mp3_flag = true;\n");
                    wait_overload_mp3_flag = true;
                }
            }
            else
            {
                have_overload_flag = false;
                overload_wait_cnt = 0;
                overload_play_cnt = 0;
                rt_kprintf("在播放时收到正常信号 \n");
                if(MY_SET.play_mode == PLAY_IMAGE)
                    wait_overload_cnt_flag = true;
                if(wait_overload_flag)
                {
                    rt_kprintf("wait_overload_flag = false; \n");
                    wait_overload_flag = false;
                }
                if (overload_music_pcm_flag)
                    overload_music_pcm_flag = false;
                if (overload_music_mp3_flag)
                    overload_music_mp3_flag = false;
                if (play_elevator.video_overload)
                    play_elevator.video_overload = false;
            }
        }
        last_car_over = elevator_data.car_overload;
        if (elevator_data.car_overload)
        {
            have_overload_flag = true;
            if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && !have_buzzer_flag && MY_VOICE_SWITCH.ols && voice_start_work)
            {
                if (voice_language == VOICE_CN)
                    play_elevator.video_overload = true;
                else if (voice_language == VOICE_EN)
                {
                    if (MY_SET.play_mode == PLAY_VIDEO)
                    {
                        rt_kprintf("超载pcm\n");
                        if (!energy_flag)
                            overload_music_pcm_flag = true;
                    }
                    else
                    {
                        if (!energy_flag)
                            overload_music_mp3_flag = true;
                        rt_kprintf("超载mp3\n");
                    }
                }
                else
                {
                    if(video_select == VIDEO_OVERLOAD && MY_SET.play_mode == PLAY_IMAGE)
                    {
                        wait_overload_flag = true;
                    }
                    rt_kprintf("超载视频加音乐\n");
                    // if(wait_overload_mp3_flag) wait_overload_mp3_flag = false;
                    rt_kprintf("wait_overload_mp3_flag = %d\n", wait_overload_mp3_flag);
                    rt_kprintf("wait_overload_cnt_flag = %d\n", wait_overload_cnt_flag);
                    rt_kprintf("wait_overload_flag = %d\n", wait_overload_flag);
                    rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
                    rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);

                    play_elevator.video_overload = true;
                    if (!energy_flag)
                        overload_start_flag = true;
                }
            }
            if (last_state2_num == VIDEO_BUZZER_SIGNAL && voice_start_work && (my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && !energy_flag)
            {
                rt_kprintf("有电梯蜂鸣器信号\n");
                have_buzzer_flag = true;
            }
        }
    }

    if (energy_flag)
        return;


    if(!start_obs_flag && uart_obs_cnt == 2)
    {
        rt_kprintf("start_obs_flag = true2\n");
        start_obs_flag = true;
        uart_obs_cnt = 0;
    }
    if (open_door_obs_flag)
    {
        if (!elevator_data.door1_open)
        {
            // rt_kprintf("door1_open = false\n");
            start_obs_flag = false;
            open_door_obs_flag = false;
            up_or_down = 0;
        }
    }
    if (open_door_obs_flag && voice_start_work)
    {
        if (elevator_data.door1_open && elevator_data.arrow_ctrl != 3)
        {
            static uint8_t temp = 0;
            if (play_elevator.elevator_up || up_music_mp3_flag)
            {
                up_or_down = 1;
                temp = 1;
            }
            else if (play_elevator.elevator_down || down_music_mp3_flag)
            {
                temp = 2;
                up_or_down = 2;
            }
            else if (temp == 1)
            {
                up_or_down = 1;
            }
            else if (temp == 2)
            {
                up_or_down = 2;
            }
            // rt_kprintf("up_or_down11 = %d\n", up_or_down);
            // rt_kprintf("up_or_down11 = %d\n", up_or_down);
            // rt_kprintf("up_or_down11 = %d\n", up_or_down);
            // play_elevator.elevator_up = 0;
            // play_elevator.elevator_down = 0;
            if (up_or_down)
            {
                // rt_kprintf("start_obs_flag = true3\n");
                start_obs_flag = true;
            }
        }
    }
    if (last_arrival != elevator_data.arrival_signal)
    {
        last_arrival = elevator_data.arrival_signal;
        if (elevator_data.arrival_signal)
        {
            // rt_kprintf("到站了\n");
            start_obs_flag = false;
            open_door_obs_flag = false;
            if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && voice_start_work)
            {
                // elevator_up_down_flag = false;
                if (voice_start_work)
                {
                    if (MY_SET.play_mode == PLAY_VIDEO)
                    {
                        play_elevator.elevator_arr = true;
                        // rt_kprintf("play_elevator.elevator_arr = true;\n");
                    }
                    else
                    {
                        arr_mp3_flag = true;
                    }
                    elevator_arr_flag = true;
                    in_arr_flag = true;
                    my_cnt.arr ++;
                }
            }
        }
    }
    if (last_open_door_state != elevator_data.door1_open && MY_VOICE_SWITCH.door)
    {
        static rt_uint32_t open_tick = 0;
        last_open_door_state = elevator_data.door1_open;
        // if(rt_tick_get() - open_tick > 8000)
        // {
        //     rt_kprintf("rt_tick_get() - open_tick = %d\n", rt_tick_get() - open_tick);
        //     rt_kprintf("rt_tick_get() = %d\n", rt_tick_get());
        //     rt_kprintf("open_tick = %d\n", open_tick);
        //     open_tick = rt_tick_get();

            if (elevator_data.door1_open)
            {
                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && voice_start_work)
                {
                    rt_kprintf("door1_open = true\n");
                    have_door_flag = true;
                    open_door_obs_flag = true;
                    if (MY_SET.play_mode == PLAY_VIDEO)
                    {
                        rt_kprintf("电梯开门pcm\n");
                        play_elevator.music_open_door = true;
                    }
                    else
                    {
                        rt_kprintf("电梯开门mp3\n");
                        open_door_music_mp3_flag = true;
                    }
                    my_cnt.open ++;
                }
            }
        // }
    }
    if (last_close_door_state != elevator_data.door1_close && MY_VOICE_SWITCH.door)
    {
        static rt_uint32_t close_tick = 0;
        last_close_door_state = elevator_data.door1_close;
        // if(rt_tick_get() - close_tick > 8000)
        // {
        //     rt_kprintf("rt_tick_get() - close_tick = %d\n", rt_tick_get() - close_tick);
        //     rt_kprintf("rt_tick_get() = %d\n", rt_tick_get());
        //     rt_kprintf("close_tick = %d\n", close_tick);
        //     close_tick = rt_tick_get();
            if (elevator_data.door1_close)
            {
                uart_obs_cnt = 0;
                // rt_kprintf("uart 电梯关门\n");
                start_obs_flag = false;
                open_door_obs_flag = false;
                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && voice_start_work)
                {
                    have_door_flag = true;
                    if (MY_SET.play_mode == PLAY_VIDEO)
                    {
                        // rt_kprintf("电梯关门pcm");
                        play_elevator.music_close_door = true;
                    }
                    else
                    {
                        // rt_kprintf("电梯关门mp3");
                        close_door_music_mp3_flag = true;
                    }
                    my_cnt.close ++;
                }
            // }
        }else {
            // rt_kprintf("play_elevator.music_close_door = %d\n", play_elevator.music_close_door);
            // rt_kprintf("close_door_music_mp3_flag = %d\n", close_door_music_mp3_flag);
            // rt_kprintf("have_door_flag = %d\n", have_door_flag);
        }
    }

    if (last_full_load != elevator_data.full_load && MY_VOICE_SWITCH.peak)
    {
        last_full_load = elevator_data.full_load;
        if (elevator_data.full_load)
        {
            if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && voice_start_work)
            {
                if (MY_SET.play_mode == PLAY_VIDEO)
                {
                    rt_kprintf("满载pcm");
                    play_elevator.music_full_load = true;
                }
                else
                {
                    rt_kprintf("满载mp3");
                    full_load_mp3_flag = true;
                }
            }
        }
    }

    if (elevator_data.front_up)
    {
        MY_IO_FLAG.front_up = true;
        if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && voice_start_work)
        {
            if (MY_SET.play_mode == PLAY_VIDEO ||
                (MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2])))
            {
                play_elevator.front_up = true;
            }
            else
            {
                front_up_mp3_flag = true;
            }
        }
    }
    if (elevator_data.front_down)
    {
        MY_IO_FLAG.front_down = true;
        if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && voice_start_work)
        {
            if (MY_SET.play_mode == PLAY_VIDEO ||
                (MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2])))
            {
                play_elevator.front_down = true;
            }
            else
            {
                front_down_mp3_flag = true;
            }
        }
    }
    if (elevator_data.back_up)
    {
        MY_IO_FLAG.back_up = true;
        if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && voice_start_work)
        {
            if (MY_SET.play_mode == PLAY_VIDEO ||
                (MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2])))
            {
                play_elevator.back_up = true;
            }
            else
            {
                back_up_mp3_flag = true;
            }
        }
    }
    if (elevator_data.back_down)
    {
        MY_IO_FLAG.back_down = true;
        if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME) && voice_start_work)
        {
            if (MY_SET.play_mode == PLAY_VIDEO ||
                (MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2])))
            {
                play_elevator.back_down = true;
            }
            else
            {
                back_down_mp3_flag = true;
            }
        }
    }

    if (return_energy_flag)
    {
        return_energy_flag = false;
        elevator_change_flag = true;
        last_elevator_state2 = 0;
    }
    // update_elevator = 1;
}

/**
 * @brief UART接收线程（环形缓冲区终极版）
 * 8字节 ：AA 56 开头 → 版本协议
 * 9字节 ：00 开头 + E6 结尾 → 正常协议（严格双校验）
 * 零数据移动 / 永不丢帧 / 支持任意数据量
 */
void uart_thread_entry(void *parameter)
{
#define SAMPLE_UART_NAME       "uart5"

#define UART_9BYTE             9
#define UART_FIFO_BATCH_LEN    32   // 批量读取串口FIFO最大长度

    wait_uart_init_flag = 1;
    while (!init_set_img_ok) rt_thread_mdelay(10);
#if !MY_USE
    rt_thread_mdelay(12000);
#endif

    uint8_t ch;
    uint8_t batch_buf[UART_FIFO_BATCH_LEN];
    uint8_t valid_frame_buf[UART_9BYTE] = {0};
    rt_tick_t last_tick = rt_tick_get();

    ring_buffer_init(&rx_ring_buf);

    serial = rt_device_find(SAMPLE_UART_NAME);
    if (!serial) {
        rt_kprintf("[UART] 设备查找失败!\n");
        return;
    }
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    rt_device_open(serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);


    rt_device_set_rx_indicate(serial, uart_input);


    while (1)
    {
        if(is_tcp_connected && MY_SET_DHCP.host_state == false)
        {
            rt_thread_mdelay(1000);
            continue;
        }

        // 发送业务：建议后续单独开发送线程，这里先保留原有逻辑
        if (need_send_data) send_test();
        if (default_set_io_flag)
        {
            static uint8_t count = 0;
            static uint8_t send_cnt_temp = 0;
            if (++send_cnt_temp > 2)
            {
                need_send_data = ++count;
                send_cnt_temp = 0;
            }
            if (need_send_data == 11 && send_cnt_temp <= 1)
            {
                need_send_data = 0;
                default_set_io_flag = false;
                count = 0;
            }
        }

        // 信号量永久阻塞，无数据不占用CPU，删除原有的10ms轮询delay
        if (rt_sem_take(&rx_sem, RT_WAITING_FOREVER) != RT_EOK)
            continue;

        // 批量读取串口硬件FIFO，减少系统调用次数
        int read_len = 0;
        while ((read_len = rt_device_read(serial, -1, batch_buf, UART_FIFO_BATCH_LEN)) > 0)
        {
            rt_tick_t cur_tick = rt_tick_get();
            // 超时判断：仅丢弃脏数据指针，不memset整块环形缓冲区
            if (rt_tick_diff(cur_tick, last_tick) > RX_TIMEOUT_TICKS)
            {
                if (rx_ring_buf.len > 0)
                {
                    rt_kprintf("⏱️  接收超时，丢弃缓冲区旧数据 len:%d\n", rx_ring_buf.len);
                    rx_ring_buf.r_ptr = rx_ring_buf.w_ptr;
                    rx_ring_buf.len = 0;
                }
            }
            last_tick = cur_tick;

            // 批量写入环形缓冲
            for(int i = 0; i < read_len; i++)
            {
                ring_buffer_write_byte(&rx_ring_buf, batch_buf[i]);
            }

            // ============ 循环解析缓冲区所有可识别帧，支持粘包多帧 ============
            while(1)
            {
                // 优先解析9字节标准协议帧
                if (rx_ring_buf.len >= UART_9BYTE)
                {
                    ring_buffer_read_at(&rx_ring_buf, valid_frame_buf, 0, UART_9BYTE);
                    if (valid_frame_buf[0] == 0x00 && valid_frame_buf[8] == 0xE6)
                    {
                        uint16_t recv_crc = (valid_frame_buf[8] << 8) | valid_frame_buf[7];
                        uint16_t calc_crc = crc_chk_value(valid_frame_buf,7);
#if break_check
                        rt_kprintf("valid_frame_buf[7] = %02x\n", valid_frame_buf[7]);
                        rt_kprintf("calc_crc = %04x\n", calc_crc);
                        Cmdparsing(valid_frame_buf);
#else
                        if (calc_crc == recv_crc)
                        {

                            Cmdparsing(valid_frame_buf);
                        }
#endif
                        // 只跳过当前这一帧9字节，不清空整个缓冲区
                        ring_buffer_skip(&rx_ring_buf, UART_9BYTE);
                        continue;
                    }
                }

                // 再解析8字节版本协议帧
                if (rx_ring_buf.len >= UART_8BYTE)
                {
                    ring_buffer_read_at(&rx_ring_buf, valid_frame_buf, 0, UART_8BYTE);
                    if (valid_frame_buf[0] == 0xAA && valid_frame_buf[1] == 0x56)
                    {
                        process_version_packet(valid_frame_buf);
                        if(is_tcp_connected)
                        {
                            tcp_send_raw(valid_frame_buf, 8);
                        }
                        // 跳过8字节版本帧
                        ring_buffer_skip(&rx_ring_buf, UART_8BYTE);
                        continue;
                    }
                }

                // 无完整帧可解析，退出内层循环，等待下一批数据
                break;
            }
        }
    }
}
