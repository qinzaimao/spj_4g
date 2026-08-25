#include "music.h"

#define LAST_DOOR_CLOSE 1
#define LAST_DOOR_OPEN 2

static uint8_t last_door_playing = 0;  // 关门语音正在播放标志
static bool up_played = false;  // 关门语音正在播放标志
static bool down_played = false;  // 关门语音正在播放标志

static bool volume_stop = false;
static uint8_t arr_cnt = 0;
static uint8_t open_or_close_cnt = 0;
static uint8_t play_ok_cnt = 0;          // 在双语模式下，播放成功的次数（x < 2）
static uint8_t last_play_music = 100;
static uint8_t state_play_cnt[50] = {0}; // 在双语模式下，播放成功的次数（x < 2）
static uint8_t play_open_cnt = 0;
static uint8_t play_close_cnt = 0;
/* LVGL文件路径宏定义 */
static char music_filename[][256] = {
    MUSIC_PATH(ZMC1.mp3),
    MUSIC_PATH(ZMC2.mp3),
    MUSIC_PATH(ZMC3.mp3),
};
// 中文数字0-79
static char MA_music_filename[][256] = {
    MUSIC_PATH(MA00.mp3),MUSIC_PATH(MA01.mp3),MUSIC_PATH(MA02.mp3),MUSIC_PATH(MA03.mp3),MUSIC_PATH(MA04.mp3),
    MUSIC_PATH(MA05.mp3),MUSIC_PATH(MA06.mp3),MUSIC_PATH(MA07.mp3),MUSIC_PATH(MA08.mp3),MUSIC_PATH(MA09.mp3),
    MUSIC_PATH(MA10.mp3),MUSIC_PATH(MA11.mp3),MUSIC_PATH(MA12.mp3),MUSIC_PATH(MA13.mp3),MUSIC_PATH(MA14.mp3),
    MUSIC_PATH(MA15.mp3),MUSIC_PATH(MA16.mp3),MUSIC_PATH(MA17.mp3),MUSIC_PATH(MA18.mp3),MUSIC_PATH(MA19.mp3),
    MUSIC_PATH(MA20.mp3),MUSIC_PATH(MA21.mp3),MUSIC_PATH(MA22.mp3),MUSIC_PATH(MA23.mp3),MUSIC_PATH(MA24.mp3),
    MUSIC_PATH(MA25.mp3),MUSIC_PATH(MA26.mp3),MUSIC_PATH(MA27.mp3),MUSIC_PATH(MA28.mp3),MUSIC_PATH(MA29.mp3),
    MUSIC_PATH(MA30.mp3),MUSIC_PATH(MA31.mp3),MUSIC_PATH(MA32.mp3),MUSIC_PATH(MA33.mp3),MUSIC_PATH(MA34.mp3),
    MUSIC_PATH(MA35.mp3),MUSIC_PATH(MA36.mp3),MUSIC_PATH(MA37.mp3),MUSIC_PATH(MA38.mp3),MUSIC_PATH(MA39.mp3),
    MUSIC_PATH(MA40.mp3),MUSIC_PATH(MA41.mp3),MUSIC_PATH(MA42.mp3),MUSIC_PATH(MA43.mp3),MUSIC_PATH(MA44.mp3),
    MUSIC_PATH(MA45.mp3),MUSIC_PATH(MA46.mp3),MUSIC_PATH(MA47.mp3),MUSIC_PATH(MA48.mp3),MUSIC_PATH(MA49.mp3),
    MUSIC_PATH(MA50.mp3),MUSIC_PATH(MA51.mp3),MUSIC_PATH(MA52.mp3),MUSIC_PATH(MA53.mp3),MUSIC_PATH(MA54.mp3),
    MUSIC_PATH(MA55.mp3),MUSIC_PATH(MA56.mp3),MUSIC_PATH(MA57.mp3),MUSIC_PATH(MA58.mp3),MUSIC_PATH(MA59.mp3),
    MUSIC_PATH(MA60.mp3),MUSIC_PATH(MA61.mp3),MUSIC_PATH(MA62.mp3),MUSIC_PATH(MA63.mp3),MUSIC_PATH(MA64.mp3),
    MUSIC_PATH(MA65.mp3),MUSIC_PATH(MA66.mp3),MUSIC_PATH(MA67.mp3),MUSIC_PATH(MA68.mp3),MUSIC_PATH(MA69.mp3),
    MUSIC_PATH(MA70.mp3),MUSIC_PATH(MA71.mp3),MUSIC_PATH(MA72.mp3),MUSIC_PATH(MA73.mp3),MUSIC_PATH(MA74.mp3),
    MUSIC_PATH(MA75.mp3),MUSIC_PATH(MA76.mp3),MUSIC_PATH(MA77.mp3),MUSIC_PATH(MA78.mp3),MUSIC_PATH(MA79.mp3),
};
// 英文数字0-79
static char EA_music_filename[][256] = {
    MUSIC_PATH(EA00.mp3),
    MUSIC_PATH(EA01.mp3),
    MUSIC_PATH(EA02.mp3),
    MUSIC_PATH(EA03.mp3),
    MUSIC_PATH(EA04.mp3),
    MUSIC_PATH(EA05.mp3),
    MUSIC_PATH(EA06.mp3),
    MUSIC_PATH(EA07.mp3),
    MUSIC_PATH(EA08.mp3),
    MUSIC_PATH(EA09.mp3),
    MUSIC_PATH(EA10.mp3),
    MUSIC_PATH(EA11.mp3),
    MUSIC_PATH(EA12.mp3),
    MUSIC_PATH(EA13.mp3),
    MUSIC_PATH(EA14.mp3),
    MUSIC_PATH(EA15.mp3),
    MUSIC_PATH(EA16.mp3),
    MUSIC_PATH(EA17.mp3),
    MUSIC_PATH(EA18.mp3),
    MUSIC_PATH(EA19.mp3),
    MUSIC_PATH(EA20.mp3),
    MUSIC_PATH(EA21.mp3),
    MUSIC_PATH(EA22.mp3),
    MUSIC_PATH(EA23.mp3),
    MUSIC_PATH(EA24.mp3),
    MUSIC_PATH(EA25.mp3),
    MUSIC_PATH(EA26.mp3),
    MUSIC_PATH(EA27.mp3),
    MUSIC_PATH(EA28.mp3),
    MUSIC_PATH(EA29.mp3),
    MUSIC_PATH(EA30.mp3),
    MUSIC_PATH(EA31.mp3),
    MUSIC_PATH(EA32.mp3),
    MUSIC_PATH(EA33.mp3),
    MUSIC_PATH(EA34.mp3),
    MUSIC_PATH(EA35.mp3),
    MUSIC_PATH(EA36.mp3),
    MUSIC_PATH(EA37.mp3),
    MUSIC_PATH(EA38.mp3),
    MUSIC_PATH(EA39.mp3),
    MUSIC_PATH(EA40.mp3),
    MUSIC_PATH(EA41.mp3),
    MUSIC_PATH(EA42.mp3),
    MUSIC_PATH(EA43.mp3),
    MUSIC_PATH(EA44.mp3),
    MUSIC_PATH(EA45.mp3),
    MUSIC_PATH(EA46.mp3),
    MUSIC_PATH(EA47.mp3),
    MUSIC_PATH(EA48.mp3),
    MUSIC_PATH(EA49.mp3),
    MUSIC_PATH(EA50.mp3),
    MUSIC_PATH(EA51.mp3),
    MUSIC_PATH(EA52.mp3),
    MUSIC_PATH(EA53.mp3),
    MUSIC_PATH(EA54.mp3),
    MUSIC_PATH(EA55.mp3),
    MUSIC_PATH(EA56.mp3),
    MUSIC_PATH(EA57.mp3),
    MUSIC_PATH(EA58.mp3),
    MUSIC_PATH(EA59.mp3),
    MUSIC_PATH(EA60.mp3),
    MUSIC_PATH(EA61.mp3),
    MUSIC_PATH(EA62.mp3),
    MUSIC_PATH(EA63.mp3),
    MUSIC_PATH(EA64.mp3),
    MUSIC_PATH(EA65.mp3),
    MUSIC_PATH(EA66.mp3),
    MUSIC_PATH(EA67.mp3),
    MUSIC_PATH(EA68.mp3),
    MUSIC_PATH(EA69.mp3),
    MUSIC_PATH(EA70.mp3),
    MUSIC_PATH(EA71.mp3),
    MUSIC_PATH(EA72.mp3),
    MUSIC_PATH(EA73.mp3),
    MUSIC_PATH(EA74.mp3),
    MUSIC_PATH(EA75.mp3),
    MUSIC_PATH(EA76.mp3),
    MUSIC_PATH(EA77.mp3),
    MUSIC_PATH(EA78.mp3),
    MUSIC_PATH(EA79.mp3),
};

static char MINU_CN_music_filename[][256] = {
    MUSIC_PATH(MB01.mp3), //-1
    MUSIC_PATH(MB02.mp3), //-2
    MUSIC_PATH(MB03.mp3), //-3
    MUSIC_PATH(MB04.mp3), //-4
    MUSIC_PATH(MB05.mp3), //-5
    MUSIC_PATH(MB06.mp3), //-6
    MUSIC_PATH(MB07.mp3), //-7
    MUSIC_PATH(MB08.mp3), //-8
    MUSIC_PATH(MB09.mp3), //-9
};

static char MINU_EN_music_filename[][256] = {
    MUSIC_PATH(EB01.mp3), //-1
    MUSIC_PATH(EB02.mp3), //-2
    MUSIC_PATH(EB03.mp3), //-3
    MUSIC_PATH(EB04.mp3), //-4
    MUSIC_PATH(EB05.mp3), //-5
    MUSIC_PATH(EB06.mp3), //-6
    MUSIC_PATH(EB07.mp3), //-7
    MUSIC_PATH(EB08.mp3), //-8
    MUSIC_PATH(EB09.mp3), //-9
};

static char A_Z_CN_music_filename[][256] = {
    MUSIC_PATH(MC0A.mp3), // A
    MUSIC_PATH(MC0B.mp3), // B
    MUSIC_PATH(MC0C.mp3), // C
    MUSIC_PATH(MC0D.mp3), // D
    MUSIC_PATH(MC0E.mp3), // E
    MUSIC_PATH(MC0F.mp3), // F
    MUSIC_PATH(MC0G.mp3), // G
    MUSIC_PATH(MC0H.mp3), // H
    MUSIC_PATH(MC0I.mp3), // I
    MUSIC_PATH(MC0J.mp3), // J
    MUSIC_PATH(MC0K.mp3), // K
    MUSIC_PATH(MC0L.mp3), // L
    MUSIC_PATH(MC0M.mp3), // M
    MUSIC_PATH(MC0N.mp3), // N
    MUSIC_PATH(MC0O.mp3), // O
    MUSIC_PATH(MC0P.mp3), // P
    MUSIC_PATH(MC0Q.mp3), // Q
    MUSIC_PATH(MC0R.mp3), // R
    MUSIC_PATH(MC0S.mp3), // S
    MUSIC_PATH(MC0T.mp3), // T
    MUSIC_PATH(MC0U.mp3), // U
    MUSIC_PATH(MC0V.mp3), // V
    MUSIC_PATH(MC0W.mp3), // W
    MUSIC_PATH(MC0X.mp3), // X
    MUSIC_PATH(MC0Y.mp3), // Y
    MUSIC_PATH(MC0Z.mp3), // Z
};

static char A_Z_EN_music_filename[][256] = {
    MUSIC_PATH(EC0A.mp3), // A
    MUSIC_PATH(EC0B.mp3), // B
    MUSIC_PATH(EC0C.mp3), // C
    MUSIC_PATH(EC0D.mp3), // D
    MUSIC_PATH(EC0E.mp3), // E
    MUSIC_PATH(EC0F.mp3), // F
    MUSIC_PATH(EC0G.mp3), // G
    MUSIC_PATH(EC0H.mp3), // H
    MUSIC_PATH(EC0I.mp3), // I
    MUSIC_PATH(EC0J.mp3), // J
    MUSIC_PATH(EC0K.mp3), // K
    MUSIC_PATH(EC0L.mp3), // L
    MUSIC_PATH(EC0M.mp3), // M
    MUSIC_PATH(EC0N.mp3), // N
    MUSIC_PATH(EC0O.mp3), // O
    MUSIC_PATH(EC0P.mp3), // P
    MUSIC_PATH(EC0Q.mp3), // Q
    MUSIC_PATH(EC0R.mp3), // R
    MUSIC_PATH(EC0S.mp3), // S
    MUSIC_PATH(EC0T.mp3), // T
    MUSIC_PATH(EC0U.mp3), // U
    MUSIC_PATH(EC0V.mp3), // V
    MUSIC_PATH(EC0W.mp3), // W
    MUSIC_PATH(EC0X.mp3), // X
    MUSIC_PATH(EC0Y.mp3), // Y
    MUSIC_PATH(EC0Z.mp3), // Z
};

static char A_1_9_CN_music_filename[][256] = {
    MUSIC_PATH(MC1A.mp3), // A
    MUSIC_PATH(MC2A.mp3), // A
    MUSIC_PATH(MC3A.mp3), // A
    MUSIC_PATH(MC4A.mp3), // A
    MUSIC_PATH(MC5A.mp3), // A
    MUSIC_PATH(MC6A.mp3), // A
    MUSIC_PATH(MC7A.mp3), // A
    MUSIC_PATH(MC8A.mp3), // A
    MUSIC_PATH(MC9A.mp3), // A
};

static char A_1_9_EN_music_filename[][256] = {
    MUSIC_PATH(EC1A.mp3), // A
    MUSIC_PATH(EC2A.mp3), // A
    MUSIC_PATH(EC3A.mp3), // A
    MUSIC_PATH(EC4A.mp3), // A
    MUSIC_PATH(EC5A.mp3), // A
    MUSIC_PATH(EC6A.mp3), // A
    MUSIC_PATH(EC7A.mp3), // A
    MUSIC_PATH(EC8A.mp3), // A
    MUSIC_PATH(EC9A.mp3), // A
};

static char B_1_9_CN_music_filename[][256] = {
    MUSIC_PATH(MC1B.mp3), // A
    MUSIC_PATH(MC2B.mp3), // A
    MUSIC_PATH(MC3B.mp3), // A
    MUSIC_PATH(MC4B.mp3), // A
    MUSIC_PATH(MC5B.mp3), // A
    MUSIC_PATH(MC6B.mp3), // A
    MUSIC_PATH(MC7B.mp3), // A
    MUSIC_PATH(MC8B.mp3), // A
    MUSIC_PATH(MC9B.mp3), // A
};
static char B_1_9_EN_music_filename[][256] = {
    MUSIC_PATH(EC1B.mp3), // A
    MUSIC_PATH(EC2B.mp3), // A
    MUSIC_PATH(EC3B.mp3), // A
    MUSIC_PATH(EC4B.mp3), // A
    MUSIC_PATH(EC5B.mp3), // A
    MUSIC_PATH(EC6B.mp3), // A
    MUSIC_PATH(EC7B.mp3), // A
    MUSIC_PATH(EC8B.mp3), // A
    MUSIC_PATH(EC9B.mp3), // A
};
static char C_1_6_CN_music_filename[][256] = {
    MUSIC_PATH(MC1C.mp3), // A
    MUSIC_PATH(MC2C.mp3), // A
    MUSIC_PATH(MC3C.mp3), // A
    MUSIC_PATH(MC4C.mp3), // A
    MUSIC_PATH(MC5C.mp3), // A
    MUSIC_PATH(MC6C.mp3), // A
};
static char C_1_6_EN_music_filename[][256] = {
    MUSIC_PATH(EC1C.mp3), // A
    MUSIC_PATH(EC2C.mp3), // A
    MUSIC_PATH(EC3C.mp3), // A
    MUSIC_PATH(EC4C.mp3), // A
    MUSIC_PATH(EC5C.mp3), // A
    MUSIC_PATH(EC6C.mp3), // A
};
static char F_1_5_CN_music_filename[][256] = {
    MUSIC_PATH(MC1F.mp3), // A
    MUSIC_PATH(MC2F.mp3), // A
    MUSIC_PATH(MC3F.mp3), // A
    MUSIC_PATH(MC4F.mp3), // A
    MUSIC_PATH(MC5F.mp3), // A
    MUSIC_PATH(MC6F.mp3), // A
};
static char F_1_5_EN_music_filename[][256] = {
    MUSIC_PATH(EC1F.mp3), // A
    MUSIC_PATH(EC2F.mp3), // A
    MUSIC_PATH(EC3F.mp3), // A
    MUSIC_PATH(EC4F.mp3), // A
    MUSIC_PATH(EC5F.mp3), // A
    MUSIC_PATH(EC6F.mp3), // A
};
static char A0_9_CN_music_filename[][256] = {
    MUSIC_PATH(MDA0.mp3), // A
    MUSIC_PATH(MDA1.mp3), // A
    MUSIC_PATH(MDA2.mp3), // A
    MUSIC_PATH(MDA3.mp3), // A
    MUSIC_PATH(MDA4.mp3), // A
    MUSIC_PATH(MDA5.mp3), // A
    MUSIC_PATH(MDA6.mp3), // A
    MUSIC_PATH(MDA7.mp3), // A
    MUSIC_PATH(MDA8.mp3), // A
    MUSIC_PATH(MDA9.mp3), // A
};
static char A0_9_EN_music_filename[][256] = {
    MUSIC_PATH(EDA0.mp3), // A
    MUSIC_PATH(EDA1.mp3), // A
    MUSIC_PATH(EDA2.mp3), // A
    MUSIC_PATH(EDA3.mp3), // A
    MUSIC_PATH(EDA4.mp3), // A
    MUSIC_PATH(EDA5.mp3), // A
    MUSIC_PATH(EDA6.mp3), // A
    MUSIC_PATH(EDA7.mp3), // A
    MUSIC_PATH(EDA8.mp3), // A
    MUSIC_PATH(EDA9.mp3), // A
};
static char B0_9_CN_music_filename[][256] = {
    MUSIC_PATH(MDB0.mp3), // A
    MUSIC_PATH(MDB1.mp3), // A
    MUSIC_PATH(MDB2.mp3), // A
    MUSIC_PATH(MDB3.mp3), // A
    MUSIC_PATH(MDB4.mp3), // A
    MUSIC_PATH(MDB5.mp3), // A
    MUSIC_PATH(MDB6.mp3), // A
    MUSIC_PATH(MDB7.mp3), // A
    MUSIC_PATH(MDB8.mp3), // A
    MUSIC_PATH(MDB9.mp3), // A
};
static char B0_9_EN_music_filename[][256] = {
    MUSIC_PATH(EDB0.mp3), // A
    MUSIC_PATH(EDB1.mp3), // A
    MUSIC_PATH(EDB2.mp3), // A
    MUSIC_PATH(EDB3.mp3), // A
    MUSIC_PATH(EDB4.mp3), // A
    MUSIC_PATH(EDB5.mp3), // A
    MUSIC_PATH(EDB6.mp3), // A
    MUSIC_PATH(EDB7.mp3), // A
    MUSIC_PATH(EDB8.mp3), // A
    MUSIC_PATH(EDB9.mp3), // A
};
static char C1_4_CN_music_filename[][256] = {
    MUSIC_PATH(MDC1.mp3), // A
    MUSIC_PATH(MDC2.mp3), // A
    MUSIC_PATH(MDC3.mp3), // A
    MUSIC_PATH(MDC4.mp3), // A
};
static char C1_4_EN_music_filename[][256] = {
    MUSIC_PATH(EDC1.mp3), // A
    MUSIC_PATH(EDC2.mp3), // A
    MUSIC_PATH(EDC3.mp3), // A
    MUSIC_PATH(EDC4.mp3), // A
};
static char D1_4_CN_music_filename[][256] = {
    MUSIC_PATH(MDD1.mp3), // A
    MUSIC_PATH(MDD2.mp3), // A
    MUSIC_PATH(MDD3.mp3), // A
    MUSIC_PATH(MDD4.mp3), // A
};
static char D1_4_EN_music_filename[][256] = {
    MUSIC_PATH(EDD1.mp3), // A
    MUSIC_PATH(EDD2.mp3), // A
    MUSIC_PATH(EDD3.mp3), // A
    MUSIC_PATH(EDD4.mp3), // A
};
static char G1_9_CN_music_filename[][256] = {
    MUSIC_PATH(MDG1.mp3), // A
    MUSIC_PATH(MDG2.mp3), // A
    MUSIC_PATH(MDG3.mp3), // A
    MUSIC_PATH(MDG4.mp3), // A
    MUSIC_PATH(MDG5.mp3), // A
    MUSIC_PATH(MDG6.mp3), // A
    MUSIC_PATH(MDG7.mp3), // A
    MUSIC_PATH(MDG8.mp3), // A
    MUSIC_PATH(MDG9.mp3), // A
};
static char G1_9_EN_music_filename[][256] = {
    MUSIC_PATH(EDG1.mp3), // A
    MUSIC_PATH(EDG2.mp3), // A
    MUSIC_PATH(EDG3.mp3), // A
    MUSIC_PATH(EDG4.mp3), // A
    MUSIC_PATH(EDG5.mp3), // A
    MUSIC_PATH(EDG6.mp3), // A
    MUSIC_PATH(EDG7.mp3), // A
    MUSIC_PATH(EDG8.mp3), // A
    MUSIC_PATH(EDG9.mp3), // A
};
static char L1_3_CN_music_filename[][256] = {
    MUSIC_PATH(MDL1.mp3), // A
    MUSIC_PATH(MDL2.mp3), // A
    MUSIC_PATH(MDL3.mp3), // A
};
static char L1_3_EN_music_filename[][256] = {
    MUSIC_PATH(EDL1.mp3), // A
    MUSIC_PATH(EDL2.mp3), // A
    MUSIC_PATH(EDL3.mp3), // A
};
static char M0_9_CN_music_filename[][256] = {
    MUSIC_PATH(MDM0.mp3), // A
    MUSIC_PATH(MDM1.mp3), // A
    MUSIC_PATH(MDM2.mp3), // A
    MUSIC_PATH(MDM3.mp3), // A
    MUSIC_PATH(MDM4.mp3), // A
    MUSIC_PATH(MDM5.mp3), // A
    MUSIC_PATH(MDM6.mp3), // A
    MUSIC_PATH(MDM7.mp3), // A
    MUSIC_PATH(MDM8.mp3), // A
    MUSIC_PATH(MDM9.mp3), // A
};
static char M0_9_EN_music_filename[][256] = {
    MUSIC_PATH(EDM0.mp3), // A
    MUSIC_PATH(EDM1.mp3), // A
    MUSIC_PATH(EDM2.mp3), // A
    MUSIC_PATH(EDM3.mp3), // A
    MUSIC_PATH(EDM4.mp3), // A
    MUSIC_PATH(EDM5.mp3), // A
    MUSIC_PATH(EDM6.mp3), // A
    MUSIC_PATH(EDM7.mp3), // A
    MUSIC_PATH(EDM8.mp3), // A
    MUSIC_PATH(EDM9.mp3), // A
};
static char P0_9_CN_music_filename[][256] = {
    MUSIC_PATH(MDP0.mp3), // A
    MUSIC_PATH(MDP1.mp3), // A
    MUSIC_PATH(MDP2.mp3), // A
    MUSIC_PATH(MDP3.mp3), // A
    MUSIC_PATH(MDP4.mp3), // A
    MUSIC_PATH(MDP5.mp3), // A
    MUSIC_PATH(MDP6.mp3), // A
    MUSIC_PATH(MDP7.mp3), // A
    MUSIC_PATH(MDP8.mp3), // A
    MUSIC_PATH(MDP9.mp3), // A
};
static char P0_9_EN_music_filename[][256] = {
    MUSIC_PATH(EDP0.mp3), // A
    MUSIC_PATH(EDP1.mp3), // A
    MUSIC_PATH(EDP2.mp3), // A
    MUSIC_PATH(EDP3.mp3), // A
    MUSIC_PATH(EDP4.mp3), // A
    MUSIC_PATH(EDP5.mp3), // A
    MUSIC_PATH(EDP6.mp3), // A
    MUSIC_PATH(EDP7.mp3), // A
    MUSIC_PATH(EDP8.mp3), // A
    MUSIC_PATH(EDP9.mp3), // A
};
static char R1_3_CN_music_filename[][256] = {
    MUSIC_PATH(MDR1.mp3), // A
    MUSIC_PATH(MDR2.mp3), // A
    MUSIC_PATH(MDR3.mp3), // A
};
static char R1_3_EN_music_filename[][256] = {
    MUSIC_PATH(EDR1.mp3), // A
    MUSIC_PATH(EDR2.mp3), // A
    MUSIC_PATH(EDR3.mp3), // A
};

/* 音量设置函数 */
int music_set_volume(int vol)
{
    if (vol < 0)
    {
        vol = 0;
    }
    if (vol > 100)
    {
        vol = 100;
    }
    music_audio_player->volume = vol;
    if (music_audio_player->render)
    {
        aic_audio_render_control(music_audio_player->render, AUDIO_RENDER_CMD_SET_VOL, &music_audio_player->volume);
    }
    return 0;
}
//播放对应有视频的音乐
void play_music(uint16_t music_num)
{
    if (MY_VOICE_SWITCH.voice)
    {
        switch (music_num)
        {
        case PLAY_MP3_FIRE:
            mini_audio_player_play(music_audio_player, MP3_FIRE_EN);
            break;
        case PLAY_MP3_RESET_APPEASE:
            mini_audio_player_play(music_audio_player, MP3_APPEASE_2_EN);
            break;
        case PLAY_MP3_RESET_HELP_APPEASE:
            mini_audio_player_play(music_audio_player, MP3_APPEASE_1_EN);
            break;
        case PLAY_MP3_RESET_END_APPEASE:
            mini_audio_player_play(music_audio_player, MP3_APPEASE_3_EN);
            break;
        case PLAY_MP3_OVERLOAD:
            mini_audio_player_play(music_audio_player, MP3_OVERLOAD_EN);
            break;
        case PLAY_MP3_APPEASE:
            if (voice_language != VOICE_CN_EN)
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MP3_APPEASE_1_CN : MP3_APPEASE_1_EN);
            else
            {
                if (play_ok_cnt == 1)
                    mini_audio_player_play(music_audio_player, MP3_DOWN_CN);
                else if (play_ok_cnt == 2)
                    mini_audio_player_play(music_audio_player, MP3_DOWN_EN);
            }
            mini_audio_player_play(music_audio_player, MP3_APPEASE_1_EN);
            break;
        case PLAY_MP3_UP_OBS:
            mini_audio_player_play(music_audio_player, MP3_UP_OBS_EN);
            break;
        case PLAY_MP3_DOWN_OBS:
            mini_audio_player_play(music_audio_player, MP3_DOWN_OBS_EN);
            break;
        case PLAY_MP3_BELL:
            mini_audio_player_play(music_audio_player, MP3_BELL);
            break;
        default:
            break;
        }
    }
}
    static uint8_t set_state = 0;
    static uint8_t set_state_2 = 0;
//定时设置音量
void user_set_volume(uint8_t vol)
{
    static uint8_t time_cnt = 0;

    if (time_cnt++ > 4 && !volume_stop)
    {
        time_cnt = 0;
        if (music_set_volume(vol) == 0)
        {
            if(!energy_conservation)
            {
                if(set_state != 2 || set_vol_flag)
                {
                    set_vol_flag = false;
                    set_state = 2;
                    sound_state = PIN_HIGH;
                    Sound_Init(PIN_HIGH);
                    // rt_kprintf("[music]:PIN HIGH 1\n") ;
                }
            }
            else
            {
                if(set_state != 1)
                {
                    set_state = 1;
                    sound_state = PIN_LOW;
                    Sound_Init(PIN_LOW);
                    // rt_kprintf("[music]:PIN LOW 5\n") ;
                }
            }
            if (vol == 0)
            {
                if(sound_state == PIN_HIGH)
                {
                    sound_state = PIN_LOW;
                    Sound_Init(PIN_LOW);
                    // rt_kprintf("[music]:PIN LOW 4\n") ;

                }
            }
            // rt_kprintf("music Initial volume set to: %d\n", MY_SET.sound);
        }
    }
}
//重新创建音乐播放器
void user_create_music(bool state)
{
    if (state)
    {
        restart_music_flag = false;
        sound_state = PIN_LOW;
        Sound_Init(PIN_LOW);
        // rt_kprintf("[music]:PIN LOW 3\n") ;
        set_state = 0;
        set_state_2 = 0;
        music_audio_player = mini_audio_player_create();
        if (music_audio_player == NULL)
        {
            rt_kprintf("Error: Failed to create audio player\n");
        }
        volume_stop = false;
        if(MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2]));
        else
            rt_thread_mdelay(1000);
        rt_kprintf("创建音乐播放器成功\n");
    }
    if (music_audio_player == NULL)
        rt_kprintf("music_audio_player is NULL\n");
}
//判断是否需要销毁音乐播放器（比如有串口信号到来，需要播放音频文件）
void user_wait_renew(void)
{
    static bool delete_music_flag = false;
    bool music_state_flag = (appease_music_mp3_flag || full_load_mp3_flag ||
                fire_music_mp3_flag || reset_appease_music_mp3_flag ||
                open_door_music_mp3_flag || close_door_music_mp3_flag ||
                up_obs_music_mp3_flag || down_obs_music_mp3_flag || arr_mp3_flag ||
                up_music_mp3_flag || down_music_mp3_flag || (overload_music_mp3_flag && !in_play_overload_music) ||
                reset_help_appease_music_mp3_flag || reset_end_appease_music_mp3_flag ||
                (last_state2_num == 6 && MY_SET.play_mode == PLAY_IMAGE && !overload_start_flag));

    if(music_state_flag && !delete_music_flag && voice_start_work)
    {
        if(MY_SET.play_mode == PLAY_IMAGE && !music_state[0] && !music_state[1] && !music_state[2]);
        else if(MY_SET.play_mode == PLAY_IMAGE && (music_state[0] || music_state[1] || music_state[2]))
            music_renew_flag = true;
        delete_music_flag = true;
    }else if(!music_state_flag && delete_music_flag)
    {
        delete_music_flag = false;
    }
}
//销毁音乐播放器
void user_start_renew(bool state)
{
    if (state)
    {
        last_play_music = 100;
        music_renew_flag = false;
        set_volume_flag = false;
        if (music_audio_player != NULL)
        {
            mini_audio_player_stop(music_audio_player);
            int ret = mini_audio_player_destroy(music_audio_player);
            if (ret == 0)
            {
                rt_kprintf("音乐播放器销毁成功\n");

                volume_stop = true;
                music_des_flag = true;
                restart_music_flag = true;
                // music_need_renew = true;
                music_audio_player = NULL;
            }
            else
            {
                rt_kprintf("音乐播放器销毁失败\n");
            }
        }
        else
        {
            music_des_flag = true;
            rt_kprintf("音乐播放器 is NULL\n");
        }
    }
}
//播放对应没有视频的音乐
void user_play_state_1(bool state)
{
    if (state)
    {
        // rt_kprintf("开始播放电梯对应状态语音 in 1\n");
        play_ok_cnt++;
        // rt_kprintf("play_ok_cnt = %d\n", play_ok_cnt);
        if (appease_music_mp3_flag && MY_VOICE_SWITCH.appease)
        {
            rt_kprintf("appease_music_mp3_flag\n");
            if (voice_language == VOICE_CN_EN)
            {
                if (play_ok_cnt == 1)
                    mini_audio_player_play(music_audio_player, MP3_APPEASE_CN);
                else if (play_ok_cnt == 2)
                    mini_audio_player_play(music_audio_player, MP3_APPEASE_EN);
            }
            else if (play_ok_cnt == 1)
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MP3_APPEASE_CN : MP3_APPEASE_EN);
        }
        else if (full_load_mp3_flag)
        {
            rt_kprintf("full_load_mp3_flag\n");
            if (voice_language == VOICE_CN_EN)
            {
                if (play_ok_cnt == 1)
                    mini_audio_player_play(music_audio_player, MP3_PEAK_CN);
                else if (play_ok_cnt == 2)
                    mini_audio_player_play(music_audio_player, MP3_PEAK_EN);
            }
            else if (play_ok_cnt == 1)
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MP3_PEAK_CN : MP3_PEAK_EN);
        } else if (open_door_music_mp3_flag && MY_VOICE_SWITCH.door && play_open_cnt != 1 && play_close_cnt != 1)
        {
            rt_kprintf("open_door_music_mp3_flag\n");
            open_or_close_cnt = 1;
                if (voice_language == VOICE_CN_EN)
                {
                    if (play_ok_cnt == 1)
                        mini_audio_player_play(music_audio_player, MP3_OPEN_CN);
                    else if (play_ok_cnt == 2)
                    {
                        mini_audio_player_play(music_audio_player, MP3_OPEN_EN);
                        play_open_cnt = 1;
                    }
                }
                else if (play_ok_cnt == 1)
                {
                    mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MP3_OPEN_CN : MP3_OPEN_EN);
                    play_open_cnt = 1;
                }
        }else if (close_door_music_mp3_flag && MY_VOICE_SWITCH.door && play_close_cnt != 1 && play_open_cnt != 2)
        {
                rt_kprintf("close_door_music_mp3_flag\n");
                open_or_close_cnt = 2;
                    if (voice_language == VOICE_CN_EN)
                    {
                        if (play_ok_cnt == 1)
                            mini_audio_player_play(music_audio_player, MP3_CLOSE_CN);
                        else if (play_ok_cnt == 2)
                        {
                            mini_audio_player_play(music_audio_player, MP3_CLOSE_EN);
                            play_close_cnt = 1;
                        }
                    }
                    else if (play_ok_cnt == 1)
                    {
                        mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MP3_CLOSE_CN : MP3_CLOSE_EN);
                        play_close_cnt = 1;
                    }
        }
       else if (up_music_mp3_flag && MY_VOICE_SWITCH.up
            && play_close_cnt != 1
            && last_door_playing != LAST_DOOR_OPEN
            && !up_played)
        {
            rt_kprintf("up_music_mp3_flag\n");

            if (voice_language == VOICE_CN_EN)
            {
                if (play_ok_cnt == 1)
                    mini_audio_player_play(music_audio_player, MP3_UP_CN);
                else if (play_ok_cnt == 2)
                {
                    mini_audio_player_play(music_audio_player, MP3_UP_EN);
                    up_played = true;
                }
            }
            else if (play_ok_cnt == 1)
            {
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MP3_UP_CN : MP3_UP_EN);
                up_played = true;
            }
        }
        else if (down_music_mp3_flag && MY_VOICE_SWITCH.up
                 && last_door_playing != LAST_DOOR_OPEN
                 && !down_played)
        {
            rt_kprintf("down_music_mp3_flag\n");
            if (voice_language == VOICE_CN_EN)
            {
                if (play_ok_cnt == 1)
                    mini_audio_player_play(music_audio_player, MP3_DOWN_CN);
                else if (play_ok_cnt == 2)
                {
                    mini_audio_player_play(music_audio_player, MP3_DOWN_EN);
                    down_played = true;
                }
            }
            else if (play_ok_cnt == 1)
            {
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MP3_DOWN_CN : MP3_DOWN_EN);
                down_played = true;
            }
        }
        if ((play_ok_cnt == 3 && voice_language == VOICE_CN_EN) || (play_ok_cnt == 2 && voice_language != VOICE_CN_EN))
        {
            play_ok_cnt = 0;
            if (appease_music_mp3_flag)
                appease_music_mp3_flag = false;
            else if (full_load_mp3_flag)
                full_load_mp3_flag = false;

            else if (open_door_music_mp3_flag)
            {
                play_open_cnt = 0;
                if(up_music_mp3_flag && arr_mp3_flag)
                {
                    rt_kprintf("清除开门信号时有上行和到站一起,清除上行\n");
                    up_music_mp3_flag = false;
                }
                if(down_music_mp3_flag && arr_mp3_flag)
                {
                    rt_kprintf("清除开门信号时有下行和到站一起,清除下行\n");
                    down_music_mp3_flag = false;
                }
                rt_kprintf("清除open_door_music_mp3_flag\n");
                last_door_playing = LAST_DOOR_OPEN;
                open_door_music_mp3_flag = false;
            }else if (close_door_music_mp3_flag && play_close_cnt)
            {
                play_close_cnt = 0;
                if(open_door_music_mp3_flag)
                {
                    rt_kprintf("清除关门信号时有开门,清除\n");
                    open_door_music_mp3_flag = false;
                }
                if(up_music_mp3_flag && arr_mp3_flag)
                {
                    rt_kprintf("清除关门信号时有上行和到站一起,清除上行\n");
                    up_music_mp3_flag = false;
                }
                if(down_music_mp3_flag && arr_mp3_flag)
                {
                    rt_kprintf("清除关门信号时有下行和到站一起,清除下行\n");
                    down_music_mp3_flag = false;
                }
                rt_kprintf("清除close_door_music_mp3_flag\n");
                last_door_playing = LAST_DOOR_CLOSE;
                close_door_music_mp3_flag = false;
            }
            if (up_music_mp3_flag && up_played)
            {
                rt_kprintf("清除up_music_mp3_flag\n");
                up_music_mp3_flag = false;
                up_played = false;
            }
            else if (down_music_mp3_flag && down_played)
            {
                rt_kprintf("清除down_music_mp3_flag\n");
                down_music_mp3_flag = false;
                down_played = false;
            }
        }
    }
}
//播放对应有视频的音乐,里面会调用play_music函数
void user_play_state_2(bool state)
{
    if (state)
    {
        rt_kprintf("开始播放电梯对应状态语音 in 2\n");

        if (fire_music_mp3_flag && MY_VOICE_SWITCH.fire)
        {
            state_play_cnt[0]++;
            if (state_play_cnt[0] == 1)
                play_music(PLAY_MP3_FIRE);
        }
        else if (reset_appease_music_mp3_flag && MY_VOICE_SWITCH.appease)
        {
            state_play_cnt[1]++;
            if (state_play_cnt[1] == 1)
                play_music(PLAY_MP3_RESET_HELP_APPEASE);
        }
        else if (reset_help_appease_music_mp3_flag && MY_VOICE_SWITCH.appease)
        {
            state_play_cnt[2]++;
            if (state_play_cnt[2] == 1)
                play_music(PLAY_MP3_RESET_APPEASE);
        }
        else if (reset_end_appease_music_mp3_flag && MY_VOICE_SWITCH.appease)
        {
            state_play_cnt[3]++;
            if (state_play_cnt[3] == 1)
                play_music(PLAY_MP3_RESET_END_APPEASE);
        }
        else if ((overload_music_mp3_flag && !in_play_overload_music) && MY_VOICE_SWITCH.ols)
        {
            rt_kprintf("进入overload_music_mp3_flag\n");
            rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
            rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);
            if (voice_language == VOICE_CN_EN)
            {
                overload_wait_cnt++;

                if (overload_play_cnt == 1 && overload_wait_cnt == 1)
                {
                    rt_kprintf("PLAY_MP3_OVERLOAD\n");
                    rt_kprintf("PLAY_MP3_OVERLOAD\n");
                    rt_kprintf("PLAY_MP3_OVERLOAD\n");
                    play_music(PLAY_MP3_OVERLOAD);
                }
            }
            else if (voice_language == VOICE_EN && !in_play_overload_music && MY_SET.play_mode == PLAY_IMAGE)
            {
                if((music_state[0] || music_state[1] || music_state[2]))
                {
                    in_play_overload_music = true;
                    rt_kprintf("in_play_overload_music = true;\n");
                }
                play_music(PLAY_MP3_OVERLOAD);
            }
            if(overload_wait_cnt > 10)
            {
                overload_wait_cnt = 0;
                overload_music_mp3_flag = false;
                rt_kprintf("清除超载音乐\n");
                rt_kprintf("wait_overload_cnt_flag = false;\n");
                rt_kprintf("wait_overload_mp3_flag = %d\n", wait_overload_mp3_flag);
                rt_kprintf("wait_overload_cnt_flag = %d\n", wait_overload_cnt_flag);
                rt_kprintf("wait_overload_flag = %d\n", wait_overload_flag);
                rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
                rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);
            }
        }
        else if (up_obs_music_mp3_flag)
        {
            state_play_cnt[4]++;
            if (state_play_cnt[4] == 1)
                play_music(PLAY_MP3_UP_OBS);
        }
        else if (down_obs_music_mp3_flag)
        {
            state_play_cnt[5]++;
            if (state_play_cnt[5] == 1)
                play_music(PLAY_MP3_DOWN_OBS);
        }
    }
    rt_kprintf("退出电梯对应状态语音:mp3\n");
    if(delete_over_flag)
    {
        delete_over_flag = false;
        overload_play_cnt = 0;
        // overload_wait_cnt = 0;
        // rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
        // rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);
    }
}
//播放到站和楼层
void user_play_state_arr(void)
{
    arr_cnt++;
    if (arr_cnt == 1)
    {
        if (MY_VOICE_SWITCH.dzz)
        {
            rt_kprintf("播放到站\n");
            mini_audio_player_play(music_audio_player, MP3_BELL);
        }
    }
    else if (elevator_floor.state && MY_VOICE_SWITCH.floor)
    {
        rt_kprintf("播放楼层：%d 类型：%d\n", elevator_floor.floor, elevator_floor.state);
        if (voice_language == VOICE_CN_EN)
        {
            if (arr_cnt == 2)
            {
                switch (elevator_floor.state)
                {
                case FLOOR_NUM:
                    mini_audio_player_play(music_audio_player, MA_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_MINU:
                    mini_audio_player_play(music_audio_player, MINU_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_A_Z:
                    mini_audio_player_play(music_audio_player, A_Z_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_1_9_A:
                    mini_audio_player_play(music_audio_player, A_1_9_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_1_9_B:
                    mini_audio_player_play(music_audio_player, B_1_9_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_1_6_C:
                    mini_audio_player_play(music_audio_player, C_1_6_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_1_5_F:
                    mini_audio_player_play(music_audio_player, F_1_5_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_A_0_9:
                    mini_audio_player_play(music_audio_player, A0_9_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_B_0_9:
                    mini_audio_player_play(music_audio_player, B0_9_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_C_1_4:
                    mini_audio_player_play(music_audio_player, C1_4_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_D_1_4:
                    mini_audio_player_play(music_audio_player, D1_4_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_G_1_9:
                    mini_audio_player_play(music_audio_player, G1_9_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_L_1_3:
                    mini_audio_player_play(music_audio_player, L1_3_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_M_0_9:
                    mini_audio_player_play(music_audio_player, M0_9_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_P_0_9:
                    mini_audio_player_play(music_audio_player, P0_9_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_R_1_3:
                    mini_audio_player_play(music_audio_player, R1_3_CN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_AF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEAF.mp3));
                    break;
                case FLOOR_AG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEAG.mp3));
                    break;
                case FLOOR_BE:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEBE.mp3));
                    break;
                case FLOOR_CF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MECF.mp3));
                    break;
                case FLOOR_EG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEEG.mp3));
                    break;
                case FLOOR_GF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEGF.mp3));
                    break;
                case FLOOR_HP:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEHP.mp3));
                    break;
                case FLOOR_KG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEKG.mp3));
                    break;
                case FLOOR_LB:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MELB.mp3));
                    break;
                case FLOOR_LD:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MELD.mp3));
                    break;
                case FLOOR_LF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MELF.mp3));
                    break;
                case FLOOR_LG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MELG.mp3));
                    break;
                case FLOOR_LL:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MELL.mp3));
                    break;
                case FLOOR_LP:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MELP.mp3));
                    break;
                case FLOOR_MR:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEMR.mp3));
                    break;
                case FLOOR_MZ:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEMZ.mp3));
                    break;
                case FLOOR_PB:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEPB.mp3));
                    break;
                case FLOOR_PC:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEPC.mp3));
                    break;
                case FLOOR_PH:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEPH.mp3));
                    break;
                case FLOOR_PM:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEPM.mp3));
                    break;
                case FLOOR_RF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MERF.mp3));
                    break;
                case FLOOR_UB:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEUB.mp3));
                    break;
                case FLOOR_UF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEUF.mp3));
                    break;
                case FLOOR_UG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEUG.mp3));
                    break;
                case FLOOR_UP:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MEUP.mp3));
                    break;
                case FLOOR_12A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF12A.mp3));
                    break;
                case FLOOR_12B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF12B.mp3));
                    break;
                case FLOOR_13A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF13A.mp3));
                    break;
                case FLOOR_13B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF13B.mp3));
                    break;
                case FLOOR_14A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF14A.mp3));
                    break;
                case FLOOR_14B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF14B.mp3));
                    break;
                case FLOOR_15A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF15A.mp3));
                    break;
                case FLOOR_15B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF15B.mp3));
                    break;
                case FLOOR_17A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF17A.mp3));
                    break;
                case FLOOR_17B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF17B.mp3));
                    break;
                case FLOOR_18A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF18A.mp3));
                    break;
                case FLOOR_18B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF18B.mp3));
                    break;
                case FLOOR_23A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF23A.mp3));
                    break;
                case FLOOR_23B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF23B.mp3));
                    break;
                case FLOOR_33A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF33A.mp3));
                    break;
                case FLOOR_33B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(MF33B.mp3));
                    break;
                default:
                    break;
                }
            }
            else if (arr_cnt == 3)
            {
                switch (elevator_floor.state)
                {
                case FLOOR_NUM:
                    mini_audio_player_play(music_audio_player, EA_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_MINU:
                    mini_audio_player_play(music_audio_player, MINU_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_A_Z:
                    mini_audio_player_play(music_audio_player, A_Z_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_1_9_A:
                    mini_audio_player_play(music_audio_player, A_1_9_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_1_9_B:
                    mini_audio_player_play(music_audio_player, B_1_9_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_1_6_C:
                    mini_audio_player_play(music_audio_player, C_1_6_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_1_5_F:
                    mini_audio_player_play(music_audio_player, F_1_5_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_A_0_9:
                    mini_audio_player_play(music_audio_player, A0_9_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_B_0_9:
                    mini_audio_player_play(music_audio_player, B0_9_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_C_1_4:
                    mini_audio_player_play(music_audio_player, C1_4_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_D_1_4:
                    mini_audio_player_play(music_audio_player, D1_4_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_G_1_9:
                    mini_audio_player_play(music_audio_player, G1_9_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_L_1_3:
                    mini_audio_player_play(music_audio_player, L1_3_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_M_0_9:
                    mini_audio_player_play(music_audio_player, M0_9_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_P_0_9:
                    mini_audio_player_play(music_audio_player, P0_9_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_R_1_3:
                    mini_audio_player_play(music_audio_player, R1_3_EN_music_filename[elevator_floor.floor]);
                    break;
                case FLOOR_AF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEAF.mp3));
                    break;
                case FLOOR_AG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEAG.mp3));
                    break;
                case FLOOR_BE:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEBE.mp3));
                    break;
                case FLOOR_CF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EECF.mp3));
                    break;
                case FLOOR_EG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEEG.mp3));
                    break;
                case FLOOR_GF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEGF.mp3));
                    break;
                case FLOOR_HP:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEHP.mp3));
                    break;
                case FLOOR_KG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEKG.mp3));
                    break;
                case FLOOR_LB:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EELB.mp3));
                    break;
                case FLOOR_LD:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EELD.mp3));
                    break;
                case FLOOR_LF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EELF.mp3));
                    break;
                case FLOOR_LG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EELG.mp3));
                    break;
                case FLOOR_LL:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EELL.mp3));
                    break;
                case FLOOR_LP:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EELP.mp3));
                    break;
                case FLOOR_MR:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEMR.mp3));
                    break;
                case FLOOR_MZ:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEMZ.mp3));
                    break;
                case FLOOR_PB:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEPB.mp3));
                    break;
                case FLOOR_PC:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEPC.mp3));
                    break;
                case FLOOR_PH:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEPH.mp3));
                    break;
                case FLOOR_PM:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEPM.mp3));
                    break;
                case FLOOR_RF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EERF.mp3));
                    break;
                case FLOOR_UB:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEUB.mp3));
                    break;
                case FLOOR_UF:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEUF.mp3));
                    break;
                case FLOOR_UG:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEUG.mp3));
                    break;
                case FLOOR_UP:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EEUP.mp3));
                    break;
                case FLOOR_12A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF12A.mp3));
                    break;
                case FLOOR_12B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF12B.mp3));
                    break;
                case FLOOR_13A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF13A.mp3));
                    break;
                case FLOOR_13B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF13B.mp3));
                    break;
                case FLOOR_14A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF14A.mp3));
                    break;
                case FLOOR_14B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF14B.mp3));
                    break;
                case FLOOR_15A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF15A.mp3));
                    break;
                case FLOOR_15B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF15B.mp3));
                    break;
                case FLOOR_17A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF17A.mp3));
                    break;
                case FLOOR_17B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF17B.mp3));
                    break;
                case FLOOR_18A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF18A.mp3));
                    break;
                case FLOOR_18B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF18B.mp3));
                    break;
                case FLOOR_23A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF23A.mp3));
                    break;
                case FLOOR_23B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF23B.mp3));
                    break;
                case FLOOR_33A:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF33A.mp3));
                    break;
                case FLOOR_33B:
                    mini_audio_player_play(music_audio_player, MUSIC_PATH(EF33B.mp3));
                    break;
                default:
                    break;
                }
            }
        }
        else if (arr_cnt == 2)
        {
            switch (elevator_floor.state)
            {
            case FLOOR_NUM:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MA_music_filename[elevator_floor.floor] : EA_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_MINU:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MINU_CN_music_filename[elevator_floor.floor] : MINU_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_A_Z:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? A_Z_CN_music_filename[elevator_floor.floor] : A_Z_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_1_9_A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? A_1_9_CN_music_filename[elevator_floor.floor] : A_1_9_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_1_9_B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? B_1_9_CN_music_filename[elevator_floor.floor] : B_1_9_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_1_6_C:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? C_1_6_CN_music_filename[elevator_floor.floor] : C_1_6_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_1_5_F:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? F_1_5_CN_music_filename[elevator_floor.floor] : F_1_5_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_A_0_9:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? B_1_9_CN_music_filename[elevator_floor.floor] : A0_9_EN_music_filename[elevator_floor.floor]);
            case FLOOR_B_0_9:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? B_1_9_CN_music_filename[elevator_floor.floor] : B0_9_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_C_1_4:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? C1_4_CN_music_filename[elevator_floor.floor] : C1_4_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_D_1_4:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? D1_4_CN_music_filename[elevator_floor.floor] : D1_4_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_G_1_9:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? G1_9_CN_music_filename[elevator_floor.floor] : G1_9_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_L_1_3:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? L1_3_CN_music_filename[elevator_floor.floor] : L1_3_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_M_0_9:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? M0_9_CN_music_filename[elevator_floor.floor] : M0_9_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_P_0_9:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? P0_9_CN_music_filename[elevator_floor.floor] : P0_9_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_R_1_3:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? R1_3_CN_music_filename[elevator_floor.floor] : R1_3_EN_music_filename[elevator_floor.floor]);
                break;
            case FLOOR_AF:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEAF.mp3) : MUSIC_PATH(EEAF.mp3));
                break;
            case FLOOR_AG:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEAG.mp3) : MUSIC_PATH(EEAG.mp3));
                break;
            case FLOOR_BE:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEBE.mp3) : MUSIC_PATH(EEBE.mp3));
                break;
            case FLOOR_CF:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MECF.mp3) : MUSIC_PATH(EECF.mp3));
                break;
            case FLOOR_EG:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEEG.mp3) : MUSIC_PATH(EEEG.mp3));
                break;
            case FLOOR_GF:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEGF.mp3) : MUSIC_PATH(EEGF.mp3));
                break;
            case FLOOR_HP:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEHP.mp3) : MUSIC_PATH(EEHP.mp3));
                break;
            case FLOOR_KG:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEKG.mp3) : MUSIC_PATH(EEKG.mp3));
                break;
            case FLOOR_LB:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MELB.mp3) : MUSIC_PATH(EELB.mp3));
                break;
            case FLOOR_LD:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MELD.mp3) : MUSIC_PATH(EELD.mp3));
                break;
            case FLOOR_LF:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MELF.mp3) : MUSIC_PATH(EELF.mp3));
                break;
            case FLOOR_LG:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MELG.mp3) : MUSIC_PATH(EELG.mp3));
                break;
            case FLOOR_LL:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MELL.mp3) : MUSIC_PATH(EELL.mp3));
                break;
            case FLOOR_LP:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MELP.mp3) : MUSIC_PATH(EELP.mp3));
                break;
            case FLOOR_MR:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEMR.mp3) : MUSIC_PATH(EEMR.mp3));
                break;
            case FLOOR_MZ:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEMZ.mp3) : MUSIC_PATH(EEMZ.mp3));
                break;
            case FLOOR_PB:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEPB.mp3) : MUSIC_PATH(EEPB.mp3));
                break;
            case FLOOR_PC:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEPC.mp3) : MUSIC_PATH(EEPC.mp3));
                break;
            case FLOOR_PH:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEPH.mp3) : MUSIC_PATH(EEPH.mp3));
                break;
            case FLOOR_PM:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEPM.mp3) : MUSIC_PATH(EEPM.mp3));
                break;
            case FLOOR_RF:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MERF.mp3) : MUSIC_PATH(EERF.mp3));
                break;
            case FLOOR_UB:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEUB.mp3) : MUSIC_PATH(EEUB.mp3));
                break;
            case FLOOR_UF:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEUF.mp3) : MUSIC_PATH(EEUF.mp3));
                break;
            case FLOOR_UG:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEUG.mp3) : MUSIC_PATH(EEUG.mp3));
                break;
            case FLOOR_UP:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MEUP.mp3) : MUSIC_PATH(EEUP.mp3));
                break;
            case FLOOR_12A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF12A.mp3) : MUSIC_PATH(EF12A.mp3));
                break;
            case FLOOR_12B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF12B.mp3) : MUSIC_PATH(EF12B.mp3));
                break;
            case FLOOR_13A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF13A.mp3) : MUSIC_PATH(EF13A.mp3));
                break;
            case FLOOR_13B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF13B.mp3) : MUSIC_PATH(EF13B.mp3));
                break;
            case FLOOR_14A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF14A.mp3) : MUSIC_PATH(EF14A.mp3));
                break;
            case FLOOR_14B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF14B.mp3) : MUSIC_PATH(EF14B.mp3));
                break;
            case FLOOR_15A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF15A.mp3) : MUSIC_PATH(EF15A.mp3));
                break;
            case FLOOR_15B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF15B.mp3) : MUSIC_PATH(EF15B.mp3));
                break;
            case FLOOR_17A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF17A.mp3) : MUSIC_PATH(EF17A.mp3));
                break;
            case FLOOR_17B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF17B.mp3) : MUSIC_PATH(EF17B.mp3));
                break;
            case FLOOR_18A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF18A.mp3) : MUSIC_PATH(EF18A.mp3));
                break;
            case FLOOR_18B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF18B.mp3) : MUSIC_PATH(EF18B.mp3));
                break;
            case FLOOR_23A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF23A.mp3) : MUSIC_PATH(EF23A.mp3));
                break;
            case FLOOR_23B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF23B.mp3) : MUSIC_PATH(EF23B.mp3));
                break;
            case FLOOR_33A:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF33A.mp3) : MUSIC_PATH(EF33A.mp3));
                break;
            case FLOOR_33B:
                mini_audio_player_play(music_audio_player, voice_language == VOICE_CN ? MUSIC_PATH(MF33B.mp3) : MUSIC_PATH(EF33B.mp3));
                break;
            default:
                break;
            }
        }
    }

    if ((arr_cnt == 4 && voice_language == VOICE_CN_EN) || (arr_cnt == 3 && voice_language != VOICE_CN_EN) ||
        (arr_cnt == 2 && !MY_VOICE_SWITCH.floor))
    {
        arr_cnt = 0;
        arr_mp3_flag = false;
        elevator_floor.state == FLOOR_NONE;
        rt_kprintf("到站播放结束\n");
        in_arr_flag = false;
    }
}

void delete_video_music(void)
{
    if(set_volume_flag)
    {
        set_volume_flag = false;
        if (music_audio_player != NULL)
        {
            mini_audio_player_stop(music_audio_player);
            int ret = mini_audio_player_destroy(music_audio_player);
            if (ret == 0)
            {
                rt_kprintf("音乐播放器销毁成功\n");
                music_audio_player = NULL;
            }
            else
            {
                rt_kprintf("音乐播放器销毁失败\n");
            }
        }
        else
        {
            music_des_flag = true;
            rt_kprintf("音乐播放器 is NULL\n");
        }
    }
}
//播放蜂鸣器音乐
void user_play_state_buzz(void)
{
    static uint8_t buzz_cnt = 0;
    if (buzz_cnt++ >= 30)
    {
        rt_kprintf("蜂鸣器 music\n");
        buzz_cnt = 0;
        mini_audio_player_play(music_audio_player, MUSIC_PATH(WNBUZ.mp3));
    }
}
//播放背景音乐
void user_play_state_bg(void)
{
    uint8_t play_music_num = music_state[0] + music_state[1] + music_state[2];
    int target_idx = -1; // 目标播放索引

    if (play_music_num == 1)
    {
        // 只有一个激活时，直接找到对应索引
        target_idx = music_state[0] ? 0 : (music_state[1] ? 1 : 2);
    }
    // 合并play_music_num为2和3的情况（逻辑完全相同）
    else if (play_music_num >= 2)
    {
        /* 第一个音乐的特殊播放条件：
            1. 音乐0处于激活状态
            2. 上次播放的不是音乐0
            3. 满足以下条件之一：
            a. 只有2个音乐激活时
            b. 3个音乐都激活，且上次播放的是音乐2或初始状态(100)
        */
        if (music_state[0] && last_play_music != 0 &&
            (play_music_num == 2 || (play_music_num == 3 && (last_play_music == 2 || last_play_music == 100))))
            target_idx = 0;
        else if (music_state[1] && last_play_music != 1)
            target_idx = 1;
        else if (music_state[2] && last_play_music != 2)
            target_idx = 2;
    }
    // 找到有效目标索引时播放音乐并更新上次播放记录
    if (target_idx != -1)
    {
        last_play_music = target_idx;
        mini_audio_player_play(music_audio_player, music_filename[target_idx]);
        music_set_volume(MY_SET.sound);
    }
}
//清理播放后的标志位
void user_clear_play_flag(void)
{
    if (reset_end_appease_music_mp3_flag && state_play_cnt[3] == 2)
    {
        state_play_cnt[3] = 0;
        reset_end_appease_music_mp3_flag = false;
    }
    else if (reset_help_appease_music_mp3_flag && state_play_cnt[2] == 2)
    {
        state_play_cnt[2] = 0;
        reset_help_appease_music_mp3_flag = false;
    }
    else if (fire_music_mp3_flag && state_play_cnt[0] == 2)
    {
        state_play_cnt[0] = 0;
        fire_music_mp3_flag = false;
    }
    else if (reset_appease_music_mp3_flag && state_play_cnt[1] == 2)
    {
        state_play_cnt[1] = 0;
        reset_appease_music_mp3_flag = false;
    }
    else if (down_obs_music_mp3_flag && state_play_cnt[5] == 2)
    {
        state_play_cnt[5] = 0;
        down_obs_music_mp3_flag = false;
    }
    else if (up_obs_music_mp3_flag && state_play_cnt[4] == 2)
    {
        state_play_cnt[4] = 0;
        up_obs_music_mp3_flag = false;
    }
    // else if (overload_music_mp3_flag)
    // {
        if (have_overload_flag && !have_buzzer_flag)
        {
            rt_kprintf("还有超载音乐信号2\n");
            wait_overload_cnt_flag = false;
            // rt_kprintf("wait_overload_cnt_flag = false;\n");
            // rt_kprintf("wait_overload_mp3_flag = %d\n", wait_overload_mp3_flag);
            // rt_kprintf("wait_overload_cnt_flag = %d\n", wait_overload_cnt_flag);
            // rt_kprintf("wait_overload_flag = %d\n", wait_overload_flag);
            // rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
            // rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);

            if (overload_wait_cnt > 1 || wait_overload_mp3_flag)
            {
                static uint8_t temp_cnt = 0;
                if(wait_overload_mp3_flag && wait_overload_flag)
                {
                    temp_cnt = 1;
                }else if(wait_overload_mp3_flag) {
                    wait_overload_mp3_flag = false;
                    rt_kprintf("wait_overload_mp3_flag = false;\n");
                }
                if(temp_cnt == 1)
                {
                    if(++ temp_cnt > 2)
                    {
                        temp_cnt = 0;
                        wait_overload_mp3_flag = false;
                        rt_kprintf("wait_overload_mp3_flag = false;\n");
                    }
                }else rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);
                if(wait_overload_flag)
                {
                    wait_overload_flag = false;
                    rt_kprintf("wait_overload_flag = false;\n");
                }
                rt_kprintf("重新赋值超载变量\n");
                rt_kprintf("重新赋值超载变量\n");
                rt_kprintf("重新赋值超载变量\n");
                if(voice_language == VOICE_CN_EN)
                {
                    overload_start_flag = true;
                    overload_music_mp3_flag = false;
                    rt_kprintf("overload_music_mp3_flag = false;\n");
                }
                overload_wait_cnt = 0;
                overload_play_cnt = 0;
                // wait_overload_cnt_flag = false;
            }
        }
        else
        {
            if(have_buzzer_flag)
            {
                if (overload_wait_cnt >= 1) overload_wait_cnt = 0;
            }

            // overload_music_mp3_flag = false;
            // rt_kprintf("overload_music_mp3_flag = false;\n");
            if(wait_overload_mp3_flag && !overload_music_mp3_flag)
            {
                wait_overload_mp3_flag = false;
                rt_kprintf("wait_overload_mp3_flag = false;\n");
            }
            overload_play_cnt = 0;
            // overload_wait_cnt = 0;
            play_elevator.video_overload = false;
            // rt_kprintf("超载信号恢复正常了\n");
            // rt_kprintf("wait_overload_cnt_flag = false;\n");
            // rt_kprintf("wait_overload_mp3_flag = %d\n", wait_overload_mp3_flag);
            // rt_kprintf("wait_overload_cnt_flag = %d\n", wait_overload_cnt_flag);
            // rt_kprintf("wait_overload_flag = %d\n", wait_overload_flag);
            // rt_kprintf("overload_play_cnt = %d\n", overload_play_cnt);
            // rt_kprintf("overload_wait_cnt = %d\n", overload_wait_cnt);
        }
    // }
}

/* 音频播放线程函数 */
void music_thread_entry(void *parameter)
{
    while(!init_set_img_ok)
    {
        rt_thread_mdelay(1000);
    }
    static uint8_t save_play_music = 0; // 双语音模式下的变量，记录要播放的状态

    /* 创建音频播放器 */
    sound_state = PIN_LOW;
    Sound_Init(PIN_LOW);
    // rt_kprintf("[music]:PIN LOW 2\n") ;
    set_state = 0;
    set_state_2 = 0;
    music_audio_player = mini_audio_player_create();
    if (music_audio_player == NULL)
    {
        rt_kprintf("Error: Failed to create audio player\n");
    }

    while (1)
    {
        /*
            MY_SET.play_mode == PLAY_IMAGE 判断是否为图片播放模式
            video_select 判断是否在播放视频
            init_set_img_ok （开机动画播放完毕）
        */
        if (MY_SET.play_mode == PLAY_IMAGE && !video_select && !set_volume_flag)
        {
            // 设置音量
            user_set_volume(MY_SET.sound);
            // 重新创建音频播放器，根据 restart_music_flag 判断是否需要重新创建
            user_create_music(restart_music_flag);
            //判断是否有串口信号到来，播放音频文件
            user_wait_renew();
            //播放器初始化完成并且处于停止状态
            if ((mini_audio_player_get_state(music_audio_player) == MINI_AUDIO_PLAYER_STATE_STOPED ||
                 mini_audio_player_get_state(music_audio_player) == MINI_AUDIO_PLAYER_STATE_INIT) &&
                !energy_conservation && !music_renew_flag && !video_in_updating)
            {

                // 处理电梯状态相关的语音播放逻辑
                if ((appease_music_mp3_flag ||
                    ((full_load_mp3_flag || (open_door_music_mp3_flag && !arr_mp3_flag) ||
                    up_music_mp3_flag || down_music_mp3_flag ||
                    (close_door_music_mp3_flag && !arr_mp3_flag)) &&
                    !fire_music_mp3_flag &&
                    !reset_appease_music_mp3_flag && !reset_help_appease_music_mp3_flag &&
                    !reset_end_appease_music_mp3_flag && !up_obs_music_mp3_flag &&
                    !down_obs_music_mp3_flag)))
                {
                    //播放没有视频的电梯状态语音
                    bool state1 = my_page == PAGE_HOME || my_page == PAGE_HOME_HOR;
                    user_play_state_1(state1);
                }
                else if (((overload_music_mp3_flag && !in_play_overload_music) || reset_appease_music_mp3_flag ||
                          fire_music_mp3_flag || reset_end_appease_music_mp3_flag ||
                          up_obs_music_mp3_flag || down_obs_music_mp3_flag ||
                          reset_help_appease_music_mp3_flag))
                {
                    //播放有视频的电梯状态语音
                    bool state2 = (my_page == PAGE_HOME || my_page == PAGE_HOME_HOR);
                    user_play_state_2(state2);
                }
                else if (arr_mp3_flag)
                {
                    //播放到站和楼层语音
                    user_play_state_arr();
                }
                else if (last_state2_num == 6)
                {

                    //播放蜂鸣器音乐
                    user_play_state_buzz();
                }
                else if ((music_state[0] || music_state[1] || music_state[2]) && last_state2_num != 6 && voice_start_work)
                {
                    //播放背景音乐
                    user_play_state_bg();

                }
                else if(!(music_state[0] || music_state[1] || music_state[2]))
                {
                    sound_state = PIN_LOW;
                    Sound_Init(PIN_LOW);

                    // rt_kprintf("[music]:PIN LOW 1\n") ;
                }

                // 处理电梯状态相关的语音播放逻辑结束
                user_clear_play_flag();
            }
        }
        //开始销毁音乐播放器
        user_start_renew(music_renew_flag);
        static bool frist_tcp_flag = false;
        if(is_tcp_connected && !frist_tcp_flag && MY_SET.play_mode == PLAY_IMAGE)
        {
            rt_kprintf("tcp连接成功,开始同步\n");
            frist_tcp_flag = true;
            if (is_tcp_connected && MY_SET_DHCP.host_state == true && !video_in_updating)
            {
                send_light(false, MY_SET.backlight);
            }
            if(my_page == PAGE_HOME || my_page == PAGE_HOME_HOR)
            {
                rt_thread_mdelay(100);
                tcp_raw_time_flag = true;
            }
        }


        /* 延时，避免CPU占用过高 */
        rt_thread_mdelay(20);
    }
}
