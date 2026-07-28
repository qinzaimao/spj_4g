#include "tcp_video.h"

void tcp_video_thread_entry(void *parameter)
{
    while (1)
    {
        if (is_tcp_connected && MY_SET_DHCP.host_state == false && !video_in_updating)
        {
            if(tcp_video_erro_flag)
            {
                tcp_video_erro_flag = false;
                rt_kprintf("视频出错\n");
                if(is_tcp_connected && MY_SET_DHCP.host_state == false)
                {
                    tcp_send_raw("video", 5);
                }
                rt_thread_mdelay(100);
                seek_to_start_play_video();
                rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                need_to_play_video_flag = false;
                rt_mutex_release(elevtor_mutex);
                // tcp_video_erro_seek_flag = true;

            }else if(tcp_video_des)
            {
                tcp_video_des = false;
                if(tcp_wait_des_flag)
                {
                    tcp_wait_des_flag = false;
                    rt_thread_mdelay(300); // 每500毫秒检查一次
                }
                destroy_player(PRINTF_CHANGE);
                rt_thread_mdelay(150); // 每500毫秒检查一次
            }
            else if(tcp_video_init)
            {
                rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                need_to_play_video_flag = true;
                rt_mutex_release(elevtor_mutex);
                if(tcp_return_home_flag) tcp_return_home_flag = false;
                tcp_video_init = false;
                create_player_flag = true;

                if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                    MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
                    video_init(true, 1);
                else
                    video_init(true, 0);
                rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                need_to_play_video_flag = false;
                rt_mutex_release(elevtor_mutex);
            }
            else if (tcp_video_num)
            {
                rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                need_to_play_video_flag = true;
                rt_mutex_release(elevtor_mutex);
                rt_kprintf("tcp_video_num: %d\n", tcp_video_num);
                if(tcp_return_home_flag)
                {
                    tcp_return_home_flag = false;
                    create_player_flag = true;

                    if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                        MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
                        video_init(true, 1);
                    else
                        video_init(true, 0);
                    rt_kprintf("从机设置退出等待主机信号\n");
                    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                    need_to_play_video_flag = false;
                    rt_mutex_release(elevtor_mutex);
                    continue;
                }
                destroy_player(PRINTF_CHANGE);
                create_player_flag = true;

                if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                    MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
                    video_init(true, 1);
                else
                    video_init(true, 0);
                tcp_video_num = 0;
                rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                need_to_play_video_flag = false;
                rt_mutex_release(elevtor_mutex);

            }
        }
        rt_thread_mdelay(100); // 每500毫秒检查一次
    }
}
