#include "elevator_play.h"



volatile bool begin_to_play_pcm = false;
volatile bool two_play_music = false; // 双语音模式标志
volatile bool arr_ok_flag = false; // 双语音模式标志
volatile static uint8_t open_or_close_door_cnt = 0;
// 加在 elevator_play_thread_entry 函数外面
static uint8_t last_door_playing = 0;  // 关门语音正在播放标志
static bool up_played = false;  // 关门语音正在播放标志
static bool down_played = false;  // 关门语音正在播放标志

#define LAST_DOOR_CLOSE 1
#define LAST_DOOR_OPEN 2

void deal_arr(void);

void elevator_play_thread_entry(void *parameter)
{

    while (1)
    {
        // 如果在语音工作时间
        if (MY_VOICE_SWITCH.voice && (!energy_conservation || (play_elevator.video_overload && MY_VOICE_SWITCH.ols) ||
        (play_elevator.video_fire && MY_VOICE_SWITCH.fire) || (play_elevator.video_reset_appease && MY_VOICE_SWITCH.appease) ||
    (play_elevator.video_reset_help_appease && MY_VOICE_SWITCH.appease) ||(play_elevator.video_reset_end_appease && MY_VOICE_SWITCH.appease)))
        {
            /*
                处理电梯状态相关的语音播放逻辑
                第一层判断：有视频状态直接进第二层
                第一层判断: 有音乐状态，在图片模式下要有音乐播放，在视频模式下进入第二层判断
            */
            // 视频状态
            uint8_t video_en = (play_elevator.video_fire && MY_VOICE_SWITCH.fire) ||
                               (play_elevator.video_reset_appease && MY_VOICE_SWITCH.appease) ||
                                (play_elevator.video_reset_help_appease && MY_VOICE_SWITCH.appease) ||
                                (play_elevator.video_reset_end_appease && MY_VOICE_SWITCH.appease) ||
                                (play_elevator.video_overload && MY_VOICE_SWITCH.ols) ||
                                (play_elevator.video_down_obs || play_elevator.video_up_obs);
            // 音乐状态
            uint8_t music_en = (play_elevator.music_close_door || play_elevator.music_open_door ||
                                play_elevator.music_appease || play_elevator.music_fire ||
                                play_elevator.music_reset_appease || play_elevator.music_reset_end_appease ||
                                play_elevator.music_reset_help_appease ||
                                play_elevator.music_overload ||
                                (last_state2_num == 6 && MY_SET.play_mode == PLAY_VIDEO) ||
                                play_elevator.elevator_arr || play_elevator.music_full_load ||
                                play_elevator.elevator_up || play_elevator.elevator_down ||
                                overload_music_pcm_flag || up_obs_music_pcm_flag || down_obs_music_pcm_flag ||
                                fire_music_pcm_flag || reset_appease_music_pcm_flag ||
                                reset_help_appease_music_pcm_flag ||
                                reset_end_appease_music_pcm_flag);
            // 在视频模式下进入，图片模式下要有音乐才进入
            uint8_t enter_en = (MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2]))
                             || MY_SET.play_mode == PLAY_VIDEO;
            rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
            bool need   =  need_to_play_video_flag && (!wait_elevtor_flag);
            rt_mutex_release(elevtor_mutex);
                        // rt_kprintf("play_elevator.elevator_up = %d\n", play_elevator.elevator_up);
            if (((enter_en && music_en) || video_en) && !in_video_state_flag && !need)
            {
                static uint8_t buzz_cnt = 0;

                if ((my_page == PAGE_HOME_HOR || my_page == PAGE_HOME))
                {
                    if (break_energy_flag)
                    {
                        rt_thread_mdelay(1500);
                        break_energy_flag = false;
                    }

                    // rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
                    if (voice_language == VOICE_CN_EN)
                    {
                        two_play_music = true;
                    }
                    else
                        two_play_music = false;
                    // 执行语音播放（如果语音工作模式已开启）
                    // rt_kprintf("overload_music_pcm_flag = %d\n", overload_music_pcm_flag);
                    // rt_kprintf("overload_music_mp3_flag = %d\n", overload_music_mp3_flag);
                    // 设置视频播放的
                    if (play_elevator.music_appease && MY_VOICE_SWITCH.appease)
                    {
                        rt_kprintf("困人安抚EN\n");
                        player_audio_render_share_play(MY_SET.sound, 11, two_play_music);
                    }
                    else if (play_elevator.video_fire && MY_VOICE_SWITCH.fire &&
                            !appease_music_mp3_flag && !fire_music_mp3_flag && !overload_music_pcm_flag &&
                            !reset_appease_music_mp3_flag && !reset_help_appease_music_mp3_flag && !overload_music_mp3_flag &&
                            !reset_end_appease_music_mp3_flag && !up_obs_music_mp3_flag &&
                            !down_obs_music_mp3_flag)
                    {
                        rt_kprintf("消防信号\n");
                        in_video_state_flag = true;
                        now_play_video_num = VIDEO_FIRE;
                        video_select = VIDEO_FIRE;
                    }
                    else if (fire_music_pcm_flag && MY_VOICE_SWITCH.fire)
                    {
                        rt_kprintf("消防EN2\n");
                        player_audio_render_share_play(MY_SET.sound, 1, false);
                    }
                    else if (play_elevator.music_fire && MY_VOICE_SWITCH.fire)
                    {
                        rt_kprintf("消防EN\n");
                        wait_fire_play_flag = false;
                        player_audio_render_share_play(MY_SET.sound, 1, two_play_music);
                    }
                    else if (play_elevator.video_reset_appease && MY_VOICE_SWITCH.appease &&
                        !appease_music_mp3_flag && !fire_music_mp3_flag &&
                            !reset_appease_music_mp3_flag && !reset_help_appease_music_mp3_flag &&
                            !reset_end_appease_music_mp3_flag && !up_obs_music_mp3_flag &&
                            !down_obs_music_mp3_flag)
                    {
                        rt_kprintf("复位安抚信号\n");
                        in_video_state_flag = true;
                        now_play_video_num = VIDEO_RESET_APPEASE;
                        video_select = VIDEO_RESET_APPEASE;
                    }
                    else if (reset_appease_music_pcm_flag && MY_VOICE_SWITCH.appease)
                    {
                        rt_kprintf("复位安抚EN2\n");
                        player_audio_render_share_play(MY_SET.sound, 8, false);
                    }
                    else if (play_elevator.video_reset_help_appease && MY_VOICE_SWITCH.appease &&
                        !appease_music_mp3_flag && !fire_music_mp3_flag &&
                            !reset_appease_music_mp3_flag && !reset_help_appease_music_mp3_flag &&
                            !reset_end_appease_music_mp3_flag && !up_obs_music_mp3_flag &&
                            !down_obs_music_mp3_flag)
                    {
                        rt_kprintf("复位救援平层安抚信号\n");
                        now_play_video_num = VIDEO_RESET_HELP_APPEASE;
                        in_video_state_flag = true;
                        video_select = VIDEO_RESET_HELP_APPEASE;
                    }
                    else if (reset_help_appease_music_pcm_flag && MY_VOICE_SWITCH.appease)
                    {
                        rt_kprintf("复位救援平层安抚EN2\n");
                        player_audio_render_share_play(MY_SET.sound, 9, false);
                    }
                    else if (play_elevator.video_reset_end_appease && MY_VOICE_SWITCH.appease &&
                        !appease_music_mp3_flag && !fire_music_mp3_flag &&
                            !reset_appease_music_mp3_flag && !reset_help_appease_music_mp3_flag &&
                            !reset_end_appease_music_mp3_flag && !up_obs_music_mp3_flag &&
                            !down_obs_music_mp3_flag)
                    {
                        rt_kprintf("复位结束安抚信号\n");
                        now_play_video_num = VIDEO_RESET_END_APPEASE;
                        in_video_state_flag = true;
                        video_select = VIDEO_RESET_END_APPEASE;
                    }
                    else if (reset_end_appease_music_pcm_flag && MY_VOICE_SWITCH.appease)
                    {
                        rt_kprintf("复位结束安抚EN2\n");
                        player_audio_render_share_play(MY_SET.sound, 10, false);
                    }

                    else if (play_elevator.video_overload && MY_VOICE_SWITCH.ols && overload_play_cnt != 1 &&
                        !appease_music_mp3_flag && !fire_music_mp3_flag && !wait_overload_flag && !wait_overload_cnt_flag &&
                            !reset_appease_music_mp3_flag && !reset_help_appease_music_mp3_flag && !wait_overload_mp3_flag &&
                            !reset_end_appease_music_mp3_flag && !up_obs_music_mp3_flag &&
                            !down_obs_music_mp3_flag)
                    {
                        rt_kprintf("超载信号\n");
                        if(!energy_conservation)
                            overload_play_cnt = 1;
                        now_play_video_num = VIDEO_OVERLOAD;
                        in_video_state_flag = true;
                        video_select = VIDEO_OVERLOAD;
                    }
                    else if (overload_music_pcm_flag && MY_VOICE_SWITCH.ols )
                    {
                        rt_kprintf("超载EN2\n");
                        player_audio_render_share_play(MY_SET.sound, 7, false);
                        if(delete_over_flag)
                        {
                            delete_over_flag = false;
                            overload_play_cnt = 0;
                            overload_wait_cnt = 0;
                            rt_kprintf("wait_overload_cnt_flag = %d\n", wait_overload_cnt_flag);
                            rt_kprintf("wait_overload_flag = %d\n", wait_overload_flag);
                            rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
                            rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);
                        }
                        overload_play_cnt = 0;
                    }else if(play_elevator.video_down_obs && !appease_music_mp3_flag &&
                        !fire_music_mp3_flag &&
                            !reset_appease_music_mp3_flag && !reset_help_appease_music_mp3_flag &&
                            !reset_end_appease_music_mp3_flag && !up_obs_music_mp3_flag &&
                            !down_obs_music_mp3_flag)
                    {
                        rt_kprintf("下行遇阻信号\n");
                        in_video_state_flag = true;
                        now_play_video_num = VIDEO_DOWN_OBS;
                        video_select = VIDEO_DOWN_OBS;
                    }else if(play_elevator.video_up_obs && !appease_music_mp3_flag &&
                            !fire_music_mp3_flag &&
                            !reset_appease_music_mp3_flag && !reset_help_appease_music_mp3_flag &&
                            !reset_end_appease_music_mp3_flag && !up_obs_music_mp3_flag &&
                            !down_obs_music_mp3_flag)
                    {
                        rt_kprintf("上行遇阻信号\n");
                        in_video_state_flag = true;
                        now_play_video_num = VIDEO_UP_OBS;
                        video_select = VIDEO_UP_OBS;
                    }else if(up_obs_music_pcm_flag)
                    {
                        rt_kprintf("上行遇阻EN2\n");
                        player_audio_render_share_play(MY_SET.sound, 193 + 69, false);
                    }else if(down_obs_music_pcm_flag)
                    {
                        rt_kprintf("下行遇阻EN2\n");
                        player_audio_render_share_play(MY_SET.sound, 194 + 69, false);
                    }
                    else if (play_elevator.music_reset_appease && MY_VOICE_SWITCH.appease)
                    {
                        rt_kprintf("复位安抚EN\n");
                        player_audio_render_share_play(MY_SET.sound, 8, two_play_music);
                    }
                    else if (play_elevator.music_reset_help_appease && MY_VOICE_SWITCH.appease)
                    {
                        rt_kprintf("复位救援平层安抚EN\n");
                        player_audio_render_share_play(MY_SET.sound, 9, two_play_music);
                    }
                    else if (play_elevator.music_reset_end_appease && MY_VOICE_SWITCH.appease)
                    {
                        rt_kprintf("复位结束安抚EN\n");
                        player_audio_render_share_play(MY_SET.sound, 10, two_play_music);
                    }
                    else if (play_elevator.music_overload && MY_VOICE_SWITCH.ols)
                    {
                        rt_kprintf("超载EN\n");
                        player_audio_render_share_play(MY_SET.sound, 7, two_play_music);
                        overload_play_cnt = 0;
                    }
                    else if (play_elevator.music_full_load)
                    {
                        rt_kprintf("满载\n");
                        player_audio_render_share_play(MY_SET.sound, 6, two_play_music);
                    }

                    else if (play_elevator.elevator_arr)
                    {
                        // rt_kprintf("到站my_cnt.last_play_state = %d\n", my_cnt.last_play_state);
                        // rt_kprintf("到站my_cnt.arr = %d\n", my_cnt.arr);
                        #if USE_PRI
                        #endif
                        // if(my_cnt.last_play_state != 2 && my_cnt.arr != 0)
                        if(my_cnt.arr != 0)
                        {
                            #if USE_PRI
                            rt_kprintf("my_cnt.arr = %d\n", my_cnt.arr);
                            rt_kprintf("my_cnt.up = %d\n", my_cnt.up);
                            rt_kprintf("my_cnt.down = %d\n", my_cnt.down);
                            rt_kprintf("my_cnt.open = %d\n", my_cnt.open);
                            rt_kprintf("my_cnt.close = %d\n", my_cnt.close);
                            #endif
                            my_cnt.arr --;
                            my_cnt.last_play_state = 2;
                            deal_arr();
                            #if USE_PRI
                            #endif
                            arr_ok_flag = true;
                            rt_kprintf("到站播放结束\n");
                        }
                    }
                    else if (play_elevator.music_close_door && MY_VOICE_SWITCH.door)
                    {
                        // rt_kprintf("here music_close_door\n");
                        if(my_cnt.last_play_state != 4 && my_cnt.close != 0)
                        {
                            my_cnt.last_play_state = 4;
                            my_cnt.close --;
                            open_or_close_door_cnt = 2;
                            // rt_kprintf("门关闭\n");
                            player_audio_render_share_play(MY_SET.sound, 5, two_play_music);
                        }
                    }else if (play_elevator.music_open_door && MY_VOICE_SWITCH.door && !play_elevator.elevator_arr)
                    {
                        // rt_kprintf("here elevator_open_door\n");
                        if(my_cnt.last_play_state != 3 && my_cnt.open != 0)
                        {
                            my_cnt.last_play_state = 3;
                            my_cnt.open --;
                            open_or_close_door_cnt = 1;
                            // rt_kprintf("门打开\n");
                            player_audio_render_share_play(MY_SET.sound, 4, two_play_music);
                        }
                    }else if (play_elevator.elevator_down && MY_VOICE_SWITCH.up)
                    {
                        // rt_kprintf("here elevator_down\n");
                        if(my_cnt.last_play_state != 1 && my_cnt.down != 0
                           && last_door_playing != LAST_DOOR_OPEN
                           && !down_played)
                        {
                            // rt_kprintf("下行\n");
                            my_cnt.last_play_state = 1;
                            my_cnt.down --;
                            wait_arr_flag = true;
                            down_played = true;
                            player_audio_render_share_play(MY_SET.sound, 3, two_play_music);
                        }
                    }
                    else if (play_elevator.elevator_up && MY_VOICE_SWITCH.up)
                    {
                        // rt_kprintf("last_door_playing = %d\n", last_door_playing);
                        // rt_kprintf("up_played = %d\n", up_played);
                        if(my_cnt.last_play_state != 1 && my_cnt.up != 0
                           && last_door_playing != LAST_DOOR_OPEN
                           && !up_played)
                        {
                            // rt_kprintf("上行\n");
                            wait_arr_flag = true;
                            my_cnt.up --;
                            my_cnt.last_play_state = 1;
                            up_played = true;
                            player_audio_render_share_play(MY_SET.sound, 2, two_play_music);
                        }
                    }

                    else if (last_state2_num == 6 && MY_SET.play_mode == PLAY_VIDEO)
                    {
                        if (buzz_cnt++ >= 15)
                        {
                            rt_kprintf("蜂鸣器\n");
                            buzz_cnt = 0;
                            player_audio_render_share_play(MY_SET.sound, 13, 0);
                        }
                    }
                }
                if (in_reset_flag && now_play_video_num == VIDEO_RESET_APPEASE)
                {
                    rt_kprintf("deal in_reset_flag\n");
                    in_reset_flag = false;
                }
                if (in_reset_end_flag && now_play_video_num == VIDEO_RESET_END_APPEASE)
                {
                    rt_kprintf("deal in_reset_end_flag\n");
                    in_reset_end_flag = false;
                }
                if (in_reset_help_flag && now_play_video_num == VIDEO_RESET_HELP_APPEASE)
                {
                    rt_kprintf("deal in_reset_help_flag\n");
                    in_reset_help_flag = false;
                }
                if (voice_language == VOICE_CN_EN || voice_language == VOICE_EN)
                    last_elevator_state2 = 0;
                static bool fire_temp = false;
                if(play_elevator.music_fire && overload_music_pcm_flag)
                {
                    if(!fire_temp)
                    {
                        wait_fire_play_flag = true;
                        fire_temp = true;
                    }else
                    {
                        fire_temp = false;
                    }
                }
                if (play_elevator.video_fire && now_play_video_num == VIDEO_FIRE)
                {
                    rt_kprintf("deal video_fire\n");
                    play_elevator.video_fire = false;
                }else if (play_elevator.music_fire && !wait_fire_play_flag)
                {
                    rt_kprintf("deal music_fire\n");
                    play_elevator.music_fire = false;
                }
                else if (play_elevator.video_reset_end_appease && now_play_video_num == VIDEO_RESET_END_APPEASE)
                {
                    rt_kprintf("deal video_reset_end_appease\n");
                    play_elevator.video_reset_end_appease = false;
                }
                else if (play_elevator.video_reset_help_appease && now_play_video_num == VIDEO_RESET_HELP_APPEASE)
                {
                    rt_kprintf("deal video_reset_help_appease\n");
                    play_elevator.video_reset_help_appease = false;
                }
                else if (play_elevator.video_reset_appease && now_play_video_num == VIDEO_RESET_APPEASE)
                {
                    rt_kprintf("deal video_reset_appease\n");
                    play_elevator.video_reset_appease = false;
                }
                else if (play_elevator.video_overload && now_play_video_num == VIDEO_OVERLOAD)
                {
                    if (have_overload_flag && !have_buzzer_flag)
                    {
                        rt_kprintf("还有超载信号\n");
                        if (voice_language == VOICE_CN)
                            overload_play_cnt = 0;
                    }
                    else
                    {
                        rt_kprintf("deal video_overload\n");
                        play_elevator.video_overload = false;
                    }
                }else if(play_elevator.video_down_obs && now_play_video_num == VIDEO_DOWN_OBS)
                {
                    rt_kprintf("deal video_down_obs\n");
                    play_elevator.video_down_obs = false;
                }else if(play_elevator.video_up_obs && now_play_video_num == VIDEO_UP_OBS)
                {
                    rt_kprintf("deal video_up_obs\n");
                    play_elevator.video_up_obs = false;
                }
                else if (play_elevator.music_reset_end_appease)
                    play_elevator.music_reset_end_appease = false;
                else if (play_elevator.music_reset_help_appease)
                    play_elevator.music_reset_help_appease = false;
                else if (play_elevator.music_reset_appease)
                    play_elevator.music_reset_appease = false;
                else if (play_elevator.music_overload)
                {
                    if (have_overload_flag)
                    {
                        rt_kprintf("还有超载音乐信号3\n");
                    }
                    else
                    {
                        rt_kprintf("deal music_overload\n");
                        play_elevator.music_overload = false;
                    }
                }
                else if (play_elevator.elevator_arr && arr_ok_flag)
                {
                    arr_ok_flag = false;
                    rt_kprintf("清除elevtor_arr\n");
                    in_arr_flag = false;
                    play_elevator.elevator_arr = false;
                }else if (play_elevator.music_close_door && open_or_close_door_cnt == 2)
                {
                    if((play_elevator.elevator_up || play_elevator.elevator_down) && play_elevator.elevator_arr)
                    {
                        rt_kprintf("关门信号和到站一起,清除上/下行\n");
                        play_elevator.elevator_up = false;
                        play_elevator.elevator_down = false;
                    }
                    if(play_elevator.music_open_door)
                    {
                        play_elevator.music_open_door = false;
                        rt_kprintf("关门时有开门信号等待播放，清除开门信号\n");
                    }
                    last_door_playing = LAST_DOOR_CLOSE;
                    play_elevator.music_close_door = false;
                    rt_kprintf("清除music_close_door\n");
                }
                else if (play_elevator.music_open_door && open_or_close_door_cnt == 1)
                {
                    if((play_elevator.elevator_up || play_elevator.elevator_down) && play_elevator.elevator_arr)
                    {
                        rt_kprintf("开门信号和到站一起,清除上/下行\n");
                        play_elevator.elevator_up = false;
                        play_elevator.elevator_down = false;
                    }
                    last_door_playing = LAST_DOOR_OPEN;
                    play_elevator.music_open_door = false;
                    rt_kprintf("清除music_open_door\n");
                }else if (play_elevator.music_appease)
                    play_elevator.music_appease = false;
                else if (play_elevator.music_full_load)
                    play_elevator.music_full_load = false;
                elevator_data.elevator_state = 0; // 电梯状态清零
            }
            if (play_elevator.elevator_up && up_played)
            {
                if(play_elevator.music_close_door)
                {
                    play_elevator.music_close_door = false;
                    rt_kprintf("清除上行时有关门信号等待播放，清除\n");
                }
                rt_kprintf("清除elevtor_up\n");
                up_played = false;
                play_elevator.elevator_up = false;
            }else if (play_elevator.elevator_down && down_played)
            {
                if(play_elevator.music_close_door)
                {
                    play_elevator.music_close_door = false;
                    rt_kprintf("清除下行时有关门信号等待播放，清除\n");
                }
                rt_kprintf("清除elevtor_down\n");
                down_played = false;
                play_elevator.elevator_down = false;
            }

        }
        rt_thread_delay(200);
    }
}

void deal_arr(void)
{
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            if (MY_VOICE_SWITCH.dzz)
            {
                rt_kprintf("播放到站\n");
                player_audio_render_share_play(MY_SET.sound, 12, false);
            }
        }
        else if (elevator_floor.state && MY_VOICE_SWITCH.floor)
        {
            rt_kprintf("播放楼层：%d 类型：%d\n", elevator_floor.floor, elevator_floor.state);
            switch (elevator_floor.state)
            {
            case FLOOR_NUM:
                player_audio_render_share_play(MY_SET.sound, 14 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_MINU:
                player_audio_render_share_play(MY_SET.sound, 25+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_A_Z:
                player_audio_render_share_play(MY_SET.sound, 34+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_1_9_A:
                player_audio_render_share_play(MY_SET.sound, 60+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_1_9_B:
                player_audio_render_share_play(MY_SET.sound, 69+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_1_6_C:
                player_audio_render_share_play(MY_SET.sound, 78+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_1_5_F:
                player_audio_render_share_play(MY_SET.sound, 84+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_A_0_9:
                player_audio_render_share_play(MY_SET.sound, 89+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_B_0_9:
                player_audio_render_share_play(MY_SET.sound, 99+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_C_1_4:
                player_audio_render_share_play(MY_SET.sound, 109+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_D_1_4:
                player_audio_render_share_play(MY_SET.sound, 113+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_G_1_9:
                player_audio_render_share_play(MY_SET.sound, 117+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_L_1_3:
                player_audio_render_share_play(MY_SET.sound, 126+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_M_0_9:
                player_audio_render_share_play(MY_SET.sound, 129+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_P_0_9:
                player_audio_render_share_play(MY_SET.sound, 139+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_R_1_3:
                player_audio_render_share_play(MY_SET.sound, 149+69 + elevator_floor.floor, two_play_music);
                break;
            case FLOOR_AF:
                player_audio_render_share_play(MY_SET.sound, 152+69, two_play_music);
                break;
            case FLOOR_AG:
                player_audio_render_share_play(MY_SET.sound, 153+69, two_play_music);
                break;
            case FLOOR_BE:
                player_audio_render_share_play(MY_SET.sound, 154+69, two_play_music);
                break;
            case FLOOR_CF:
                player_audio_render_share_play(MY_SET.sound, 155+69, two_play_music);
                break;
            case FLOOR_EG:
                player_audio_render_share_play(MY_SET.sound, 156+69, two_play_music);
                break;
            case FLOOR_GF:
                player_audio_render_share_play(MY_SET.sound, 157+69, two_play_music);
                break;
            case FLOOR_HP:
                player_audio_render_share_play(MY_SET.sound, 158+69, two_play_music);
                break;
            case FLOOR_KG:
                player_audio_render_share_play(MY_SET.sound, 159+69, two_play_music);
                break;
            case FLOOR_LB:
                player_audio_render_share_play(MY_SET.sound, 160+69, two_play_music);
                break;
            case FLOOR_LD:
                player_audio_render_share_play(MY_SET.sound, 161+69, two_play_music);
                break;
            case FLOOR_LF:
                player_audio_render_share_play(MY_SET.sound, 162+69, two_play_music);
                break;
            case FLOOR_LG:
                player_audio_render_share_play(MY_SET.sound, 163+69, two_play_music);
                break;
            case FLOOR_LL:
                player_audio_render_share_play(MY_SET.sound, 164+69, two_play_music);
                break;
            case FLOOR_LP:
                player_audio_render_share_play(MY_SET.sound, 165+69, two_play_music);
                break;
            case FLOOR_MR:
                player_audio_render_share_play(MY_SET.sound, 166+69, two_play_music);
                break;
            case FLOOR_MZ:
                player_audio_render_share_play(MY_SET.sound, 167+69, two_play_music);
                break;
            case FLOOR_PB:
                player_audio_render_share_play(MY_SET.sound, 168+69, two_play_music);
                break;
            case FLOOR_PC:
                player_audio_render_share_play(MY_SET.sound, 169+69, two_play_music);
                break;
            case FLOOR_PH:
                player_audio_render_share_play(MY_SET.sound, 170+69, two_play_music);
                break;
            case FLOOR_PM:
                player_audio_render_share_play(MY_SET.sound, 171+69, two_play_music);
                break;
            case FLOOR_RF:
                player_audio_render_share_play(MY_SET.sound, 172+69, two_play_music);
                break;
            case FLOOR_UB:
                player_audio_render_share_play(MY_SET.sound, 173+69, two_play_music);
                break;
            case FLOOR_UF:
                player_audio_render_share_play(MY_SET.sound, 174+69, two_play_music);
                break;
            case FLOOR_UG:
                player_audio_render_share_play(MY_SET.sound, 175+69, two_play_music);
                break;
            case FLOOR_UP:
                player_audio_render_share_play(MY_SET.sound, 176+69, two_play_music);
                break;
            case FLOOR_12A:
            player_audio_render_share_play(MY_SET.sound, 177+69, two_play_music);
                break;
            case FLOOR_12B:
                player_audio_render_share_play(MY_SET.sound, 178+69, two_play_music);
                break;
            case FLOOR_13A:
            player_audio_render_share_play(MY_SET.sound, 179+69, two_play_music);
                break;
            case FLOOR_13B:
            player_audio_render_share_play(MY_SET.sound, 180+69, two_play_music);
                break;
            case FLOOR_14A:
            player_audio_render_share_play(MY_SET.sound, 181+69, two_play_music);
                break;
            case FLOOR_14B:
            player_audio_render_share_play(MY_SET.sound, 182+69, two_play_music);
                break;
            case FLOOR_15A:
            player_audio_render_share_play(MY_SET.sound, 183+69, two_play_music);
                break;
            case FLOOR_15B:
            player_audio_render_share_play(MY_SET.sound, 184+69, two_play_music);
                break;
            case FLOOR_17A:
            player_audio_render_share_play(MY_SET.sound, 185+69, two_play_music);
                break;
            case FLOOR_17B:
            player_audio_render_share_play(MY_SET.sound, 186+69, two_play_music);
                break;
            case FLOOR_18A:
            player_audio_render_share_play(MY_SET.sound, 187+69, two_play_music);
                break;
            case FLOOR_18B:
            player_audio_render_share_play(MY_SET.sound, 188+69, two_play_music);
                break;
            case FLOOR_23A:
            player_audio_render_share_play(MY_SET.sound, 189+69, two_play_music);
                break;
            case FLOOR_23B:
            player_audio_render_share_play(MY_SET.sound, 190+69, two_play_music);
                break;
            case FLOOR_33A:
            player_audio_render_share_play(MY_SET.sound, 191+69, two_play_music);
                break;
            case FLOOR_33B:
            player_audio_render_share_play(MY_SET.sound, 192+69, two_play_music);
                break;
            default:
                break;
            }
        }
        elevator_floor.state == FLOOR_NONE;
    }
}
