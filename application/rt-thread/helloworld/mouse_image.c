#include "mouse_image.h"



void mouse_image_thread_entry(void *parameter)
{
    // 初始化看门狗
    if (wdt_init() != RT_EOK)
        rt_kprintf("[WDT] Init failed, system halted!");
    else
        rt_kprintf("wDT init success\n");
    // 注册空闲钩子（第一层喂狗机制）
    rt_thread_idle_sethook(idle_hook);
    static uint8_t obs_cnt = 0;
    while (1)
    {

        if(start_obs_flag)
        {
            if(obs_cnt > 15)
                rt_kprintf("obs_cnt:%d\n",obs_cnt);
            #if MY_USE
            if(++obs_cnt >= 10)
            #else
            if(++obs_cnt >= 30)
            #endif
            {
                start_obs_flag = false;
                obs_cnt = 0;
                if(up_or_down == 1)
                {
                    if (voice_language == VOICE_CN)
                        play_elevator.video_up_obs = true;
                    else if (voice_language == VOICE_EN)
                    {
                        if (MY_SET.play_mode == PLAY_VIDEO ||
                            (MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2])))
                        {
                            rt_kprintf("上行遇阻pcm");
                            up_obs_music_pcm_flag = true;
                        }else {
                            rt_kprintf("上行遇阻mp3");
                            up_obs_music_mp3_flag = true;
                        }
                    }
                    else
                    {
                        rt_kprintf("上行遇阻视频加音乐");
                        play_elevator.video_up_obs = true;
                        up_obs_start_flag = true;
                    }
                }
                else if(up_or_down == 2)
                {
                    if (voice_language == VOICE_CN)
                        play_elevator.video_down_obs = true;
                    else if (voice_language == VOICE_EN)
                    {
                        if (MY_SET.play_mode == PLAY_VIDEO ||
                            (MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2])))
                        {
                            rt_kprintf("下行遇阻pcm");
                            down_obs_music_pcm_flag = true;
                        }else {
                            rt_kprintf("下行遇阻mp3");
                            down_obs_music_mp3_flag = true;
                        }
                    }
                    else
                    {
                        rt_kprintf("下行遇阻视频加音乐");
                        play_elevator.video_down_obs = true;
                        down_obs_start_flag = true;
                    }
                }
            }
        }else {
            obs_cnt = 0;
        }
        rt_thread_delay(1000); // 1秒检测一次
    }
}
