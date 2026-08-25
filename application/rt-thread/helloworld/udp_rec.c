#include "udp_rec.h"
#include "my_uart.h"

#include "string.h"
#include <rtthread.h>
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"
#include "lwip/igmp.h"

#define VOFA_UDP_PORT        1346
#define VOFA_MULTICAST_ADDR  "239.0.0.1"
#define VOFA_RECV_BUF_LEN    4096
#define UART_9BYTE           9

static struct udp_pcb *vofa_upcb = NULL;
static uint8_t vofa_recv_buf[VOFA_RECV_BUF_LEN];
static ip_addr_t vofa_multicast_ip;

// 复用串口环形缓冲区结构
typedef struct {
    uint8_t buf[VOFA_RECV_BUF_LEN];
    uint32_t r_ptr;
    uint32_t w_ptr;
    uint32_t len;
} ring_buffer_t;

static ring_buffer_t udp_ring_buf;

static void ring_buffer_init(ring_buffer_t *rb) {
    memset(rb->buf, 0, VOFA_RECV_BUF_LEN);
    rb->r_ptr = 0;
    rb->w_ptr = 0;
    rb->len = 0;
}

static int ring_buffer_write_byte(ring_buffer_t *rb, uint8_t byte) {
    if (rb->len >= VOFA_RECV_BUF_LEN) {
        rb->r_ptr = (rb->r_ptr + 1) % VOFA_RECV_BUF_LEN;
        rb->len--;
        static uint32_t overflow_count = 0;
        overflow_count++;
        if (overflow_count % 100 == 0) {
            rt_kprintf("⚠️ UDP环形缓冲区溢出，累计次数：%d\n", overflow_count);
        }
    }
    rb->buf[rb->w_ptr] = byte;
    rb->w_ptr = (rb->w_ptr + 1) % VOFA_RECV_BUF_LEN;
    rb->len++;
    return 0;
}

static void ring_buffer_read_at(ring_buffer_t *rb, uint8_t *dst, uint32_t pos, uint32_t len) {
    uint32_t start_idx = (rb->r_ptr + pos) % VOFA_RECV_BUF_LEN;
    uint32_t first_part_len = VOFA_RECV_BUF_LEN - start_idx;
    if (len <= first_part_len) {
        memcpy(dst, &rb->buf[start_idx], len);
    } else {
        memcpy(dst, &rb->buf[start_idx], first_part_len);
        memcpy(dst + first_part_len, rb->buf, len - first_part_len);
    }
}

static void ring_buffer_skip(ring_buffer_t *rb, uint32_t len) {
    if (len > rb->len) len = rb->len;
    rb->r_ptr = (rb->r_ptr + len) % VOFA_RECV_BUF_LEN;
    rb->len -= len;
}

static void ring_buffer_clear(ring_buffer_t *rb) {
    rb->r_ptr = 0;
    rb->w_ptr = 0;
    rb->len = 0;
    memset(rb->buf, 0, VOFA_RECV_BUF_LEN);
}

static void vofa_udp_recv_cb(void *arg, struct udp_pcb *pcb,
                             struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    if (p == NULL)
        return;

    uint16_t recv_len = p->tot_len;
    if (recv_len >= VOFA_RECV_BUF_LEN)
        recv_len = VOFA_RECV_BUF_LEN - 1;

    memset(vofa_recv_buf, 0, VOFA_RECV_BUF_LEN);
    pbuf_copy_partial(p, vofa_recv_buf, recv_len, 0);

    // rt_kprintf("[UDP组播接收] %d.%d.%d.%d:%d 长度:%d\n",
    //            ip4_addr1_16(addr), ip4_addr2_16(addr),
    //            ip4_addr3_16(addr), ip4_addr4_16(addr), port, recv_len);

    for (uint16_t i = 0; i < recv_len; i++)
    {
        ring_buffer_write_byte(&udp_ring_buf, vofa_recv_buf[i]);

        // 9字节 0x00开头 0xE6结尾电梯协议
        if (udp_ring_buf.len >= UART_9BYTE)
        {
            uint8_t valid_frame_buf[UART_9BYTE] = {0};
            ring_buffer_read_at(&udp_ring_buf, valid_frame_buf, udp_ring_buf.len - UART_9BYTE, UART_9BYTE);

            if (valid_frame_buf[0] == 0x00 && valid_frame_buf[8] == 0xE6)
            {
                uint16_t recv_crc = (valid_frame_buf[8] << 8) | valid_frame_buf[7];
                uint16_t calc_crc = crc_chk_value(valid_frame_buf, 7);
#if 0
                rt_kprintf("calc_crc = %04x\n", calc_crc);
                Cmdparsing(valid_frame_buf);
#else
                if (calc_crc == recv_crc)
                {
                    Cmdparsing(valid_frame_buf);
                    if(video_udp_state == 1)
                    {
                        video_init(true, 1);
                    }else if(video_udp_state == 2)
                    {
                        destroy_player(PRINTF_ELEVTOR);
                    }else if(video_udp_state == 3)
                    {
                        if(my_lvgl_player_ctx.player != NULL)
                        {
                            seek_to_start_play_video();
                        }else rt_kprintf("seek失败，播放器为空\n");
                    }
                    // rt_kprintf("[UDP] 9字节电梯协议解析成功\n");
                }
                else
                {
                    // rt_kprintf("[UDP] 9字节协议CRC校验失败\n");
                }
#endif
                ring_buffer_skip(&udp_ring_buf, udp_ring_buf.len);
                continue;
            }
        }

        // 8字节 AA 56 版本协议
        if (udp_ring_buf.len >= UART_8BYTE)
        {
            uint8_t valid_frame_buf[UART_8BYTE] = {0};
            ring_buffer_read_at(&udp_ring_buf, valid_frame_buf, udp_ring_buf.len - UART_8BYTE, UART_8BYTE);

            if (valid_frame_buf[0] == 0xAA && valid_frame_buf[1] == 0x56)
            {
                rt_kprintf("[UDP] 收到版本协议\n");
                process_version_packet(valid_frame_buf);
                ring_buffer_skip(&udp_ring_buf, udp_ring_buf.len);
                continue;
            }
        }
    }

    pbuf_free(p);
}

void udp_rec_thread_entry(void *parameter)
{
    ip_addr_t any_netif_addr;
    ip4_addr_set_any(&any_netif_addr);
    err_t err;

    ring_buffer_init(&udp_ring_buf);
    ipaddr_aton(VOFA_MULTICAST_ADDR, &vofa_multicast_ip);

    while (1)
    {
        if (!get_lwip_flag)
        {
            rt_thread_mdelay(1000);
            rt_kprintf("等待网络就绪...\n");
            continue;
        }

        if (vofa_upcb == NULL)
        {
            vofa_upcb = udp_new();
            if (vofa_upcb == NULL)
            {
                rt_kprintf("UDP PCB创建失败\n");
                rt_thread_mdelay(2000);
                continue;
            }

            err = udp_bind(vofa_upcb, IP_ANY_TYPE, VOFA_UDP_PORT);
            if (err != ERR_OK)
            {
                udp_remove(vofa_upcb);
                vofa_upcb = NULL;
                rt_kprintf("UDP绑定%d端口失败,err:%d\n", VOFA_UDP_PORT, err);
                rt_thread_mdelay(2000);
                continue;
            }

            // 删掉 ip_set_multicast_ttl、ip_set_multicast_loop 两行配置
            err = igmp_joingroup(&any_netif_addr, &vofa_multicast_ip);
            if (err != ERR_OK)
            {
                udp_remove(vofa_upcb);
                vofa_upcb = NULL;
                rt_kprintf("加入组播%s失败,err:%d\n", VOFA_MULTICAST_ADDR, err);
                rt_thread_mdelay(2000);
                continue;
            }

            udp_recv(vofa_upcb, vofa_udp_recv_cb, NULL);
            rt_kprintf("UDP组播就绪 %s:%d，复用串口协议解析\n", VOFA_MULTICAST_ADDR, VOFA_UDP_PORT);
        }

        rt_thread_mdelay(1000);
    }
}

void udp_deinit(void)
{
    ip_addr_t any_netif_addr;
    ip4_addr_set_any(&any_netif_addr);
    if (vofa_upcb != NULL)
    {
        igmp_leavegroup(&any_netif_addr, &vofa_multicast_ip);
        udp_remove(vofa_upcb);
        vofa_upcb = NULL;
        rt_kprintf("UDP组播已退出\n");
    }
}
