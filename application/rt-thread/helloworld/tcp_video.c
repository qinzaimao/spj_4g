#include "tcp_video.h"

void tcp_video_thread_entry(void *parameter)
{
    while (1)
    {
        if (is_tcp_connected && MY_SET_DHCP.host_state == false && !video_in_updating)
        {
            if(tcp_video_des)
            {
                tcp_video_des = false;
                destroy_player(PRINTF_CHANGE);
                rt_thread_mdelay(150); // 每500毫秒检查一次
            }
            else if(tcp_video_init)
            {
                tcp_video_init = false;
                create_player_flag = true;

                if (MY_SET_IMAGE.image == IMAGE_C201_hor || MY_SET_IMAGE.image == IMAGE_C202_hor ||
                    MY_SET_IMAGE.image == IMAGE_C404_hor || MY_SET_IMAGE.image == IMAGE_C404_ver)
                    video_init(true, 1);
                else
                    video_init(true, 0);
            }
            else if (tcp_video_num)
            {
                rt_kprintf("tcp_video_num: %d\n", tcp_video_num);
                while (1)
                {
                    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                    bool wait_elevator_temp = wait_elevtor_flag;
                    rt_mutex_release(elevtor_mutex);

                    if (!wait_elevator_temp)
                        break;
                    rt_thread_mdelay(50);
                }
                int ret = lvgl_stop(&my_lvgl_player_ctx);
                rt_kprintf("stop ok\n");
                if (ret == -1)
                {
                    rt_kprintf("ret == -1 跳过这次\n");
                }else{
                    lvgl_play(&my_lvgl_player_ctx);
                    rt_kprintf("play ok\n");
                }
                rt_mutex_take(video_mutex, RT_WAITING_FOREVER);
                have_two_video_flag = false; // ✅ 新增：重置多视频标志
                rt_mutex_release(video_mutex);
                rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
                need_to_play_video_flag = false;
                rt_mutex_release(elevtor_mutex);
            }
        }
        rt_thread_mdelay(100); // 每500毫秒检查一次
    }
}
