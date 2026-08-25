#include "deal.h"

void deal_thread_entry(void *parameter)
{
    g_tcp_recv_sem = rt_sem_create("tcp_recv_sem", 0, RT_IPC_FLAG_FIFO);
    if (g_tcp_recv_sem == RT_NULL)
    {
        rt_kprintf("[TCP] sem create fail!\n");
    }

    while (1)
    {
        // 阻塞等待TCP接收事件，没有数据这里休眠
        rt_err_t sem_ret = rt_sem_take(g_tcp_recv_sem, RT_WAITING_FOREVER);
        if (sem_ret != RT_EOK)
        {
            continue;
        }

        rt_enter_critical();
        uint16_t rlen = g_tcp_recv_len;
        uint8_t *rbuf = g_tcp_recv_buf;
        // rt_bool_t net_ok = (set_dhcp_info.tcp_connected && set_dhcp_info.get_lwip);
        // 读完立即清空长度，下一次接收可以覆盖
        g_tcp_recv_len = 0;
        rt_exit_critical();

        // 网络断开/未就绪，直接丢弃这一包
        if (rlen == 0)
        {
            continue;
        }

        if (MY_SET_DHCP.host_state == true)
        {
            if (rlen >= 5)
            {
                uint8_t pos;
                for (pos = 0; pos <= rlen - 5; pos++)
                {
                    uint8_t *tmp = rbuf + pos;
                    if (memcmp(tmp, "video", 5) == 0)
                    {
                        seek_to_start_play_video();
                        rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                        need_to_play_video_flag = false;
                        rt_mutex_release(elevtor_mutex);

                        break;
                    }
                }
            }
        }

        if (MY_SET_DHCP.host_state == false)
        {

            // ========== 第一层：6字节二进制短指令（最高优先级） ==========
            if (rlen == 7 || rlen == 14)
            {
                // 增加首字节校验：必须第一个字节是0x00才是合法指令
                if (rbuf[0] == 0x00)
                {
                    uint8_t pkg_9[9] = {0x00};
                    // 原始有效6个字节在recv_buf[1]~rbuf[6]，拷贝到pkg9[1~6]
                    memcpy(pkg_9 + 1, rbuf + 1, 6);
                    pkg_9[7] = 0x00;
                    pkg_9[8] = 0xE6;
                    Cmdparsing(pkg_9);
                    continue;
                }
                // 长度7但首字节不是0x00，属于杂包，不return，继续后续所有指令解析
            }

            // ========== 第二层：4字节视频/图片控制指令（次高频） ==========

            if (my_page == PAGE_HOME || my_page == PAGE_HOME_HOR)
            {
                uint16_t offset = 0;
                while (offset + 4 <= rlen)
                {
                    uint8_t cmd_buf[4];
                    memcpy(cmd_buf, rbuf + offset, 4);

                    if (memcmp(cmd_buf, "v1mp", 4) == 0)
                        tcp_video_num = 1;
                    else if (memcmp(cmd_buf, "v1av", 4) == 0)
                        tcp_video_num = 2;
                    else if (memcmp(cmd_buf, "v2mp", 4) == 0)
                        tcp_video_num = 3;
                    else if (memcmp(cmd_buf, "v2av", 4) == 0)
                        tcp_video_num = 4;
                    else if (memcmp(cmd_buf, "v3mp", 4) == 0)
                        tcp_video_num = 5;
                    else if (memcmp(cmd_buf, "v3av", 4) == 0)
                        tcp_video_num = 6;

                    if (memcmp(cmd_buf, "v1mp", 4) == 0 || memcmp(cmd_buf, "v1av", 4) == 0 ||
                        memcmp(cmd_buf, "v2mp", 4) == 0 || memcmp(cmd_buf, "v2av", 4) == 0 ||
                        memcmp(cmd_buf, "v3mp", 4) == 0 || memcmp(cmd_buf, "v3av", 4) == 0)
                    {
                        if (video_in_updating)
                        {
                            tcp_video_num = 0;
                        }
                        offset += 4;
                        continue;
                    }

                    if (memcmp(cmd_buf, "vdes", 4) == 0)
                    {
                        tcp_video_des = true;
                        offset += 4;
                        continue;
                    }

                    if (memcmp(cmd_buf, "vin1", 4) == 0)
                        tcp_video_num = 1;
                    else if (memcmp(cmd_buf, "vin2", 4) == 0)
                        tcp_video_num = 2;
                    else if (memcmp(cmd_buf, "vin3", 4) == 0)
                        tcp_video_num = 3;
                    else if (memcmp(cmd_buf, "vin4", 4) == 0)
                        tcp_video_num = 4;
                    else if (memcmp(cmd_buf, "vin5", 4) == 0)
                        tcp_video_num = 5;
                    else if (memcmp(cmd_buf, "vin6", 4) == 0)
                        tcp_video_num = 6;
                    else if (memcmp(cmd_buf, "vint", 4) == 0)
                        tcp_video_num = 0;

                    if (memcmp(cmd_buf, "vin1", 4) == 0 || memcmp(cmd_buf, "vin2", 4) == 0 ||
                        memcmp(cmd_buf, "vin3", 4) == 0 || memcmp(cmd_buf, "vin4", 4) == 0 ||
                        memcmp(cmd_buf, "vin5", 4) == 0 || memcmp(cmd_buf, "vin6", 4) == 0 ||
                        memcmp(cmd_buf, "vint", 4) == 0)
                    {
                        tcp_video_init = true;
                        if (video_in_updating)
                        {
                            tcp_video_num = 0;
                            tcp_video_init = false;
                        }
                        offset += 4;
                        continue;
                    }

                    if (memcmp(cmd_buf, "seek", 4) == 0)
                    {
                        if (tcp_return_home_flag)
                        {
                            tcp_return_home_flag = false;
                            create_player_flag = true;

                            if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                                MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
                                video_init(true, 1);
                            else
                                video_init(true, 0);
                            rt_kprintf("从机设置退出等待主机信号\n");
                        }
                        if (my_lvgl_player_ctx.player != NULL)
                            seek_to_start_play_video();
                        else
                        {
                            create_player_flag = true;
                            if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                                MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
                                video_init(true, 1);
                            else
                                video_init(true, 0);
                            rt_kprintf("seek 时没有播放器，创建\n");
                        }
                        rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                        need_to_play_video_flag = false;
                        rt_mutex_release(elevtor_mutex);
                        offset += 4;
                        continue;
                    }

                    if (memcmp(cmd_buf, "img", 3) == 0)
                    {
                        if (cmd_buf[3] >= '0' && cmd_buf[3] <= '9')
                            tcp_img_num = cmd_buf[3] - '0' + 1;
                        offset += 4;
                        continue;
                    }

                    if (memcmp(cmd_buf, "ida", 3) == 0)
                    {
                        if (cmd_buf[3] >= '0' && cmd_buf[3] <= '9')
                            tcp_img_num = cmd_buf[3] - '0' + 1;
                        offset += 4;
                        continue;
                    }

                    offset++;
                }
            }
            // ========== 第三层：8字节版本升级帧（0xAA 0x56） ==========
            if (rlen >= UART_8BYTE)
            {
                for (uint16_t i = 0; i <= rlen - UART_8BYTE; i++)
                {
                    if (rbuf[i] == 0xAA && rbuf[i + 1] == 0x56)
                    {
                        process_version_packet(rbuf + i);
                        break;
                    }
                }
            }

            // ========== 第四层：低频文本指令（背光 + 时间，放最后） ==========
            // 背光匹配
            if (rlen >= 7)
            {
                uint16_t try_pos;
                for (try_pos = 0; try_pos <= 10 && try_pos <= rlen - 6; try_pos++)
                {
                    uint8_t *p = rbuf + try_pos;
                    if (memcmp(p, "light:", 6) == 0 || memcmp(p, "li_en:", 6) == 0)
                    {
                        int val = 0;
                        for (uint16_t i = 6; i < (rlen - try_pos); i++)
                        {
                            if (p[i] >= '0' && p[i] <= '9')
                                val = val * 10 + (p[i] - '0');
                            else
                                break;
                        }
                        if (memcmp(p, "light:", 6) == 0)
                        {
                            MY_SET.backlight = val;
                            backlight_set(MY_SET.backlight);
                            rt_kprintf("[TCP BL] 收到背光设置：%d\n", MY_SET.backlight);
                        }
                        else if (memcmp(p, "li_en:", 6) == 0)
                        {
                            MY_SET.e_con_backlight = val;
                            backlight_set(MY_SET.e_con_backlight);
                            rt_kprintf("[TCP BL] 收到节能背光设置：%d\n", MY_SET.e_con_backlight);
                        }
                        // rt_kprintf("[TCP BL] 收到背光设置：%d\n", MY_SET.backlight);
                        break;
                    }
                }
            }
            // 时间字符串匹配
            if (rlen >= 16)
            {
                uint16_t try_pos;
                for (try_pos = 0; try_pos <= 10 && try_pos <= rlen - 16; try_pos++)
                {
                    uint8_t *p = rbuf + try_pos;
                    if (p[4] == '/' && p[7] == '/' && p[10] == ' ')
                    {
                        char date_str[11] = {0};
                        char time_str[6] = {0};
                        memcpy(date_str, p, 10);
                        memcpy(time_str, p + 11, 5);

                        // rt_kprintf("[TCP TIME SET] 原始接收：%s %s\n", date_str, time_str);
                        int year, mon, day, hour, min;
                        sscanf(date_str, "%d/%d/%d", &year, &mon, &day);
                        sscanf(time_str, "%d:%d", &hour, &min);

                        // rt_kprintf("[TCP TIME SET] 解析结果 年:%d 月:%d 日:%d 时:%d 分:%d\n", year, mon, day, hour, min);
                        rt_err_t ret = RT_EOK;
                        ret = set_date(year, mon, day);
                        if (ret != RT_EOK)
                            rt_kprintf("set RTC date failed, ret:%d\n", ret);
                        rt_thread_mdelay(1);
                        ret = set_time(hour, min, 1);
                        if (ret != RT_EOK)
                            rt_kprintf("set RTC time failed, ret:%d\n", ret);
                        tcp_set_time_flag = true;
                        break;;
                    }
                }
            }
        }
    }
}
