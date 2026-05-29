#include "aic_osal.h"
#include "frame_allocator.h"
#include "mpp_fb.h"
#include "mpp_ge.h"
#include "mpp_log.h"
#include "mpp_mem.h"
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "aic_audio_render_manager.h"
#include "player_audio_test.h"
#include "../../../../../../application/rt-thread/helloworld/elevator_play.h"
#ifdef AIC_MPP_PLAYER_AUDIO_RENDER_SHARE_TEST

#define USER_AUDIO_RENDER_FILE_CNT 1
#define USER_AUDIO_RENDER_PRIORITY AUDIO_RENDER_SCENE_HIGHEST_PRIORITY
#define USER_AUDIO_RENDER_SAMPLERATE 44100
#define USER_AUDIO_RENDER_CHANNELS 2
#define AUDIO_PLAY_DELAY_US 100000  // 100ms 音频间隔
#define AUDIO_RESET_DELAY_US 100000   // 50ms 重置延迟
#define MAX_RETRY_COUNT 3            // 最大重试次数
#define CACHE_CLEAN_TIMES 3         // 缓存清理次数


// 定义全局音频渲染器实例
struct user_audio_render g_user_audio_render = {0};

static char g_user_file_path[][256] = {
    "NULL",
    VIDEO_FIRE_CN,      // 消防中文播报
    VIDEO_FIRE_EN,      // 消防英文播报
    VIDEO_UP_CN,        // 升降中文播报
    VIDEO_UP_EN,        // 升降英文播报
    VIDEO_DOWN_CN,      // 升降中文播报
    VIDEO_DOWN_EN,      // 升降英文播报
    VIDEO_OPEN_CN,      // 开门中文播报
    VIDEO_OPEN_EN,      // 开门英文播报
    VIDEO_CLOSE_CN,     // 关门中文播报
    VIDEO_CLOSE_EN,     // 关门英文播报
    VIDEO_PEAK_CN,      // 高峰中文播报
    VIDEO_PEAK_EN,      // 高峰英文播报
    VIDEO_OVERLOAD_CN,  // 超载中文播报
    VIDEO_OVERLOAD_EN,  // 超载英文播报
    VIDEO_APPEASE_1_CN, // 安抚中文播报
    VIDEO_APPEASE_1_EN, // 安抚英文播报
    VIDEO_APPEASE_2_CN, // 安抚中文播报
    VIDEO_APPEASE_2_EN, // 安抚英文播报
    VIDEO_APPEASE_3_CN, // 安抚中文播报
    VIDEO_APPEASE_3_EN, // 安抚英文播报
    VIDEO_APPEASE_CN,   //困人安抚中
    VIDEO_APPEASE_EN,   //困人安抚英

    VIDEO_BELL_CN,      // 叮 12
    VIDEO_BELL_EN,      //
    VIDEO_WNBUZ_CN,     //蜂鸣器 13
    VIDEO_WNBUZ_EN,
    VIDEO_00_CN,      //14
    VIDEO_00_EN,
    VIDEO_01_CN,      //15
    VIDEO_01_EN,
    VIDEO_02_CN,
    VIDEO_02_EN,
    VIDEO_03_CN,
    VIDEO_03_EN,
    VIDEO_04_CN,
    VIDEO_04_EN,
    VIDEO_05_CN,
    VIDEO_05_EN,
    VIDEO_06_CN,
    VIDEO_06_EN,
    VIDEO_07_CN,
    VIDEO_07_EN,
    VIDEO_08_CN,
    VIDEO_08_EN,
    VIDEO_09_CN,
    VIDEO_09_EN,
    VIDEO_10_CN,
    VIDEO_10_EN,//24
    VIDEO_11_CN,
    VIDEO_11_EN,
    VIDEO_12_CN,
    VIDEO_12_EN,
    VIDEO_13_CN,
    VIDEO_13_EN,
    VIDEO_14_CN,
    VIDEO_14_EN,
    VIDEO_15_CN,
    VIDEO_15_EN,
    VIDEO_16_CN,
    VIDEO_16_EN,
    VIDEO_17_CN,
    VIDEO_17_EN,
    VIDEO_18_CN,
    VIDEO_18_EN,
    VIDEO_19_CN,
    VIDEO_19_EN,
    VIDEO_20_CN,
    VIDEO_20_EN,
    VIDEO_21_CN,
    VIDEO_21_EN,
    VIDEO_22_CN,
    VIDEO_22_EN,
    VIDEO_23_CN,
    VIDEO_23_EN,
    VIDEO_24_CN,
    VIDEO_24_EN,
    VIDEO_25_CN,
    VIDEO_25_EN,
    VIDEO_26_CN,
    VIDEO_26_EN,
    VIDEO_27_CN,
    VIDEO_27_EN,
    VIDEO_28_CN,
    VIDEO_28_EN,
    VIDEO_29_CN,
    VIDEO_29_EN,
    VIDEO_30_CN,
    VIDEO_30_EN,
    VIDEO_31_CN,
    VIDEO_31_EN,
    VIDEO_32_CN,
    VIDEO_32_EN,
    VIDEO_33_CN,
    VIDEO_33_EN,
    VIDEO_34_CN,
    VIDEO_34_EN,
    VIDEO_35_CN,
    VIDEO_35_EN,
    VIDEO_36_CN,
    VIDEO_36_EN,
    VIDEO_37_CN,
    VIDEO_37_EN,
    VIDEO_38_CN,
    VIDEO_38_EN,
    VIDEO_39_CN,
    VIDEO_39_EN,
    VIDEO_40_CN,
    VIDEO_40_EN,
    VIDEO_41_CN,
    VIDEO_41_EN,
    VIDEO_42_CN,
    VIDEO_42_EN,
    VIDEO_43_CN,
    VIDEO_43_EN,
    VIDEO_44_CN,
    VIDEO_44_EN,
    VIDEO_45_CN,
    VIDEO_45_EN,
    VIDEO_46_CN,
    VIDEO_46_EN,
    VIDEO_47_CN,
    VIDEO_47_EN,
    VIDEO_48_CN,
    VIDEO_48_EN,
    VIDEO_49_CN,
    VIDEO_49_EN,
    VIDEO_50_CN,
    VIDEO_50_EN,
    VIDEO_51_CN,
    VIDEO_51_EN,
    VIDEO_52_CN,
    VIDEO_52_EN,
    VIDEO_53_CN,
    VIDEO_53_EN,
    VIDEO_54_CN,
    VIDEO_54_EN,
    VIDEO_55_CN,
    VIDEO_55_EN,
    VIDEO_56_CN,
    VIDEO_56_EN,
    VIDEO_57_CN,
    VIDEO_57_EN,
    VIDEO_58_CN,
    VIDEO_58_EN,
    VIDEO_59_CN,
    VIDEO_59_EN,
    VIDEO_60_CN,
    VIDEO_60_EN,
    VIDEO_61_CN,
    VIDEO_61_EN,
    VIDEO_62_CN,
    VIDEO_62_EN,
    VIDEO_63_CN,
    VIDEO_63_EN,
    VIDEO_64_CN,
    VIDEO_64_EN,
    VIDEO_65_CN,
    VIDEO_65_EN,
    VIDEO_66_CN,
    VIDEO_66_EN,
    VIDEO_67_CN,
    VIDEO_67_EN,
    VIDEO_68_CN,
    VIDEO_68_EN,
    VIDEO_69_CN,
    VIDEO_69_EN,
    VIDEO_70_CN,
    VIDEO_70_EN,
    VIDEO_71_CN,
    VIDEO_71_EN,
    VIDEO_72_CN,
    VIDEO_72_EN,
    VIDEO_73_CN,
    VIDEO_73_EN,
    VIDEO_74_CN,
    VIDEO_74_EN,
    VIDEO_75_CN,
    VIDEO_75_EN,
    VIDEO_76_CN,
    VIDEO_76_EN,
    VIDEO_77_CN,
    VIDEO_77_EN,
    VIDEO_78_CN,
    VIDEO_78_EN,
    VIDEO_79_CN,
    VIDEO_79_EN,//93
    VIDEO_MINU_1_CN,//25+69
    VIDEO_MINU_1_EN,
    VIDEO_MINU_2_CN,
    VIDEO_MINU_2_EN,
    VIDEO_MINU_3_CN,
    VIDEO_MINU_3_EN,
    VIDEO_MINU_4_CN,
    VIDEO_MINU_4_EN,
    VIDEO_MINU_5_CN,
    VIDEO_MINU_5_EN,
    VIDEO_MINU_6_CN,
    VIDEO_MINU_6_EN,
    VIDEO_MINU_7_CN,
    VIDEO_MINU_7_EN,
    VIDEO_MINU_8_CN,
    VIDEO_MINU_8_EN,
    VIDEO_MINU_9_CN,
    VIDEO_MINU_9_EN,//33
    VIDEO_A_CN,//34
    VIDEO_A_EN,
    VIDEO_B_CN,
    VIDEO_B_EN,
    VIDEO_C_CN,
    VIDEO_C_EN,
    VIDEO_D_CN,
    VIDEO_D_EN,
    VIDEO_E_CN,
    VIDEO_E_EN,
    VIDEO_F_CN,//39
    VIDEO_F_EN,
    VIDEO_G_CN,//40
    VIDEO_G_EN,
    VIDEO_H_CN,
    VIDEO_H_EN,
    VIDEO_I_CN,
    VIDEO_I_EN,
    VIDEO_J_CN,
    VIDEO_J_EN,
    VIDEO_K_CN,
    VIDEO_K_EN,
    VIDEO_L_CN,
    VIDEO_L_EN,
    VIDEO_M_CN,
    VIDEO_M_EN,
    VIDEO_N_CN,
    VIDEO_N_EN,
    VIDEO_O_CN,
    VIDEO_O_EN,
    VIDEO_P_CN,
    VIDEO_P_EN,
    VIDEO_Q_CN,//50
    VIDEO_Q_EN,
    VIDEO_R_CN,
    VIDEO_R_EN,
    VIDEO_S_CN,
    VIDEO_S_EN,
    VIDEO_T_CN,
    VIDEO_T_EN,
    VIDEO_U_CN,
    VIDEO_U_EN,
    VIDEO_V_CN,
    VIDEO_V_EN,
    VIDEO_W_CN,
    VIDEO_W_EN,
    VIDEO_X_CN,
    VIDEO_X_EN,
    VIDEO_Y_CN,
    VIDEO_Y_EN,
    VIDEO_Z_CN,
    VIDEO_Z_EN,//59
    VIDEO_1A_CN,//60
    VIDEO_1A_EN,
    VIDEO_2A_CN,
    VIDEO_2A_EN,
    VIDEO_3A_CN,
    VIDEO_3A_EN,
    VIDEO_4A_CN,
    VIDEO_4A_EN,
    VIDEO_5A_CN,
    VIDEO_5A_EN,
    VIDEO_6A_CN,
    VIDEO_6A_EN,
    VIDEO_7A_CN,
    VIDEO_7A_EN,
    VIDEO_8A_CN,
    VIDEO_8A_EN,
    VIDEO_9A_CN,
    VIDEO_9A_EN,
    VIDEO_1B_CN,//69
    VIDEO_1B_EN,
    VIDEO_2B_CN,
    VIDEO_2B_EN,
    VIDEO_3B_CN,
    VIDEO_3B_EN,
    VIDEO_4B_CN,
    VIDEO_4B_EN,
    VIDEO_5B_CN,
    VIDEO_5B_EN,
    VIDEO_6B_CN,
    VIDEO_6B_EN,
    VIDEO_7B_CN,
    VIDEO_7B_EN,
    VIDEO_8B_CN,
    VIDEO_8B_EN,
    VIDEO_9B_CN,
    VIDEO_9B_EN,//77
    VIDEO_1C_CN,//78
    VIDEO_1C_EN,
    VIDEO_2C_CN,//
    VIDEO_2C_EN,
    VIDEO_3C_CN,//
    VIDEO_3C_EN,
    VIDEO_4C_CN,//
    VIDEO_4C_EN,
    VIDEO_5C_CN,//
    VIDEO_5C_EN,
    VIDEO_6C_CN,//83
    VIDEO_6C_EN,
    VIDEO_1F_CN,//84
    VIDEO_1F_EN,
    VIDEO_2F_CN,//
    VIDEO_2F_EN,
    VIDEO_3F_CN,//
    VIDEO_3F_EN,
    VIDEO_4F_CN,//
    VIDEO_4F_EN,
    VIDEO_5F_CN,//
    VIDEO_5F_EN,
    VIDEO_A0_CN,//89
    VIDEO_A0_EN,
    VIDEO_A1_CN,//90
    VIDEO_A1_EN,
    VIDEO_A2_CN,
    VIDEO_A2_EN,
    VIDEO_A3_CN,
    VIDEO_A3_EN,
    VIDEO_A4_CN,
    VIDEO_A4_EN,
    VIDEO_A5_CN,
    VIDEO_A5_EN,
    VIDEO_A6_CN,
    VIDEO_A6_EN,
    VIDEO_A7_CN,
    VIDEO_A7_EN,
    VIDEO_A8_CN,
    VIDEO_A8_EN,
    VIDEO_A9_CN,
    VIDEO_A9_EN,//98
    VIDEO_B0_CN,//99
    VIDEO_B0_EN,
    VIDEO_B1_CN,//100
    VIDEO_B1_EN,
    VIDEO_B2_CN,
    VIDEO_B2_EN,
    VIDEO_B3_CN,
    VIDEO_B3_EN,
    VIDEO_B4_CN,
    VIDEO_B4_EN,
    VIDEO_B5_CN,
    VIDEO_B5_EN,
    VIDEO_B6_CN,
    VIDEO_B6_EN,
    VIDEO_B7_CN,
    VIDEO_B7_EN,
    VIDEO_B8_CN,
    VIDEO_B8_EN,
    VIDEO_B9_CN,
    VIDEO_B9_EN,//108
    VIDEO_C1_CN,//109
    VIDEO_C1_EN,
    VIDEO_C2_CN,
    VIDEO_C2_EN,
    VIDEO_C3_CN,
    VIDEO_C3_EN,
    VIDEO_C4_CN,
    VIDEO_C4_EN,//112
    VIDEO_D1_CN,//113
    VIDEO_D1_EN,
    VIDEO_D2_CN,
    VIDEO_D2_EN,
    VIDEO_D3_CN,
    VIDEO_D3_EN,
    VIDEO_D4_CN,
    VIDEO_D4_EN,//116
    VIDEO_G1_CN,//117
    VIDEO_G1_EN,
    VIDEO_G2_CN,
    VIDEO_G2_EN,
    VIDEO_G3_CN,
    VIDEO_G3_EN,
    VIDEO_G4_CN,
    VIDEO_G4_EN,
    VIDEO_G5_CN,
    VIDEO_G5_EN,
    VIDEO_G6_CN,
    VIDEO_G6_EN,
    VIDEO_G7_CN,
    VIDEO_G7_EN,
    VIDEO_G8_CN,
    VIDEO_G8_EN,
    VIDEO_G9_CN,
    VIDEO_G9_EN,//125
    VIDEO_L1_CN,//126
    VIDEO_L1_EN,
    VIDEO_L2_CN,
    VIDEO_L2_EN,
    VIDEO_L3_CN,
    VIDEO_L3_EN,//128
    VIDEO_M0_CN,//129
    VIDEO_M0_EN,
    VIDEO_M1_CN,
    VIDEO_M1_EN,
    VIDEO_M2_CN,
    VIDEO_M2_EN,
    VIDEO_M3_CN,
    VIDEO_M3_EN,
    VIDEO_M4_CN,
    VIDEO_M4_EN,
    VIDEO_M5_CN,
    VIDEO_M5_EN,
    VIDEO_M6_CN,
    VIDEO_M6_EN,
    VIDEO_M7_CN,
    VIDEO_M7_EN,
    VIDEO_M8_CN,
    VIDEO_M8_EN,
    VIDEO_M9_CN,
    VIDEO_M9_EN,//138
    VIDEO_P0_CN,//139
    VIDEO_P0_EN,
    VIDEO_P1_CN,
    VIDEO_P1_EN,
    VIDEO_P2_CN,
    VIDEO_P2_EN,
    VIDEO_P3_CN,
    VIDEO_P3_EN,
    VIDEO_P4_CN,
    VIDEO_P4_EN,
    VIDEO_P5_CN,
    VIDEO_P5_EN,
    VIDEO_P6_CN,
    VIDEO_P6_EN,
    VIDEO_P7_CN,
    VIDEO_P7_EN,
    VIDEO_P8_CN,
    VIDEO_P8_EN,
    VIDEO_P9_CN,
    VIDEO_P9_EN,//148
    VIDEO_R1_CN,//149
    VIDEO_R1_EN,
    VIDEO_R2_CN,
    VIDEO_R2_EN,
    VIDEO_R3_CN,
    VIDEO_R3_EN,//151
    VIDEO_AF_CN,//152
    VIDEO_AF_EN,
    VIDEO_AG_CN,
    VIDEO_AG_EN,
    VIDEO_BE_CN,
    VIDEO_BE_EN,
    VIDEO_CF_CN,
    VIDEO_CF_EN,
    VIDEO_EG_CN,
    VIDEO_EG_EN,
    VIDEO_GF_CN,
    VIDEO_GF_EN,
    VIDEO_HP_CN,
    VIDEO_HP_EN,
    VIDEO_KG_CN,
    VIDEO_KG_EN,
    VIDEO_LB_CN,
    VIDEO_LB_EN,
    VIDEO_LD_CN,
    VIDEO_LD_EN,
    VIDEO_LF_CN,
    VIDEO_LF_EN,
    VIDEO_LG_CN,
    VIDEO_LG_EN,
    VIDEO_LL_CN,
    VIDEO_LL_EN,
    VIDEO_LP_CN,
    VIDEO_LP_EN,
    VIDEO_MR_CN,
    VIDEO_MR_EN,
    VIDEO_MZ_CN,
    VIDEO_MZ_EN,
    VIDEO_PB_CN,
    VIDEO_PB_EN,
    VIDEO_PC_CN,
    VIDEO_PC_EN,
    VIDEO_PH_CN,
    VIDEO_PH_EN,
    VIDEO_PM_CN,
    VIDEO_PM_EN,
    VIDEO_RF_CN,
    VIDEO_RF_EN,
    VIDEO_UB_CN,
    VIDEO_UB_EN,
    VIDEO_UF_CN,
    VIDEO_UF_EN,
    VIDEO_UG_CN,
    VIDEO_UG_EN,
    VIDEO_UPP_CN,
    VIDEO_UPP_EN,//176
    VIDEO_12A_CN,//177
    VIDEO_12A_EN,
    VIDEO_12B_CN,
    VIDEO_12B_EN,
    VIDEO_13A_CN,
    VIDEO_13A_EN,
    VIDEO_13B_CN,
    VIDEO_13B_EN,
    VIDEO_14A_CN,
    VIDEO_14A_EN,
    VIDEO_14B_CN,
    VIDEO_14B_EN,
    VIDEO_15A_CN,
    VIDEO_15A_EN,
    VIDEO_15B_CN,
    VIDEO_15B_EN,
    VIDEO_17A_CN,
    VIDEO_17A_EN,
    VIDEO_17B_CN,
    VIDEO_17B_EN,
    VIDEO_18A_CN,
    VIDEO_18A_EN,
    VIDEO_18B_CN,
    VIDEO_18B_EN,
    VIDEO_23A_CN,
    VIDEO_23A_EN,
    VIDEO_23B_CN,
    VIDEO_23B_EN,
    VIDEO_33A_CN,
    VIDEO_33A_EN,
    VIDEO_33B_CN,
    VIDEO_33B_EN,//192

    VIDEO_UP_OBS_CN,
    VIDEO_UP_OBS_EN,
    VIDEO_DOWN_OBS_CN,
    VIDEO_DOWN_OBS_EN,
};
static s32 init_audio_renderer();
static int64_t audio_play_wait_time(int file_size);
static int audio_file_prepare(int num, char **file_path, int *file_size);

#include <dfs.h>
#include <dfs_fs.h>

// 强制清理DFS文件系统所有缓存
// void dfs_force_clean_all_cache(void)
// {
//     struct dfs_filesystem *fs;
//     struct dfs_mountpoint *mp;

//     /* 遍历所有挂载点 */
//     for (mp = dfs_mountpoint_list; mp != NULL; mp = mp->next)
//     {
//         fs = mp->fs;
//         if (fs && fs->ops && fs->ops->sync)
//         {
//             /* 同步文件系统 */
//             fs->ops->sync(fs);
//         }
//     }

//     /* 强制清理所有目录项和inode缓存 */
//     dfs_cache_clean();

//     rt_kprintf("[dfs] 所有文件系统缓存已清理\n");
// }

// 辅助函数：重置音频渲染器状态
static int reset_audio_renderer(struct user_audio_render *p_user)
{
    if (!p_user ||!p_user->render)
    {
        rt_kprintf("[audio] 错误：渲染器未初始化，无法重置\n");
        return -1;
    }
    // 加锁保护渲染器操作
    // rt_mutex_take(audio_mutex, RT_WAITING_FOREVER);
    int retry = 0;
    int ret = 0;

    // 加强版缓存清理 - 多次清理+重试
    for (int i = 0; i < CACHE_CLEAN_TIMES; i++)
    {
        retry = 0;
        do {
            ret = aic_audio_render_control(p_user->render, AUDIO_RENDER_CMD_CLEAR_CACHE, NULL);
            if (ret == 0) break;

            rt_kprintf("[audio] 警告：清除缓存失败(%d/%d)，重试 %d/%d\n",
                      i+1, CACHE_CLEAN_TIMES, retry + 1, MAX_RETRY_COUNT);
            usleep(AUDIO_RESET_DELAY_US);
        } while (++retry < MAX_RETRY_COUNT);

        if (ret != 0)
        {
            rt_kprintf("[audio] 错误：多次清除缓存失败\n");
            // rt_mutex_release(audio_mutex); // 失败前释放锁
            return -1;
        }
        usleep(5000); // 每次清理后延迟
    }

    // 延迟确保缓存清理完成
    usleep(AUDIO_RESET_DELAY_US / 10);

    // 重新应用基础参数，强制刷新状态
    retry = 0;
    do {
        ret = aic_audio_render_control(p_user->render, AUDIO_RENDER_CMD_SET_ATTR, &p_user->ao_attr);
        if (ret == 0) break;

        rt_kprintf("[audio] 警告：重置音频属性失败，重试 %d/%d\n", retry + 1, MAX_RETRY_COUNT);
        usleep(AUDIO_RESET_DELAY_US / 10);
    } while (++retry < MAX_RETRY_COUNT);

    if (ret!= 0)
    {
        rt_kprintf("[audio] 错误：多次重置音频属性失败\n");
        // rt_mutex_release(audio_mutex); // 失败前释放锁
        return -1;
    }
    // usleep(10000);
    // rt_kprintf("[audio] 渲染器状态重置完成\n");
    // rt_mutex_release(audio_mutex); // 失败前释放锁
    return 0;
}

// 辅助函数：验证音频参数一致性
static int verify_audio_params(struct user_audio_render *p_user)
{
    struct aic_audio_render_attr current_attr;

    if (aic_audio_render_control(p_user->render, AUDIO_RENDER_CMD_GET_ATTR, &current_attr)!= 0)
    {
        rt_kprintf("[audio] 错误：获取当前属性失败\n");
        return -1;
    }

    // 验证关键参数是否匹配
    // if (current_attr.sample_rate!= p_user->ao_attr.sample_rate ||
    //     current_attr.channels!= p_user->ao_attr.channels)
    // {
    //     rt_kprintf("[audio] 警告：音频参数不匹配 (期望: %dHz/%dch, 实际: %dHz/%dch)\n",
    //               p_user->ao_attr.sample_rate, p_user->ao_attr.channels,
    //               current_attr.sample_rate, current_attr.channels);
    //     return -1;
    // }

    return 0;
}

// 辅助函数：播放单个音频文件（带重试机制）
static int play_single_audio(struct user_audio_render *p_user_audio_render, uint16_t audio_index)
{
    // rt_kprintf("[audio] 开始播放单个音频，索引: %d\n", audio_index);

    char *file_path = NULL;
    int file_size = 0;
    int64_t play_time = 0;
    int ret = 0;
    int retry = 0;

    // 准备音频文件
    if (audio_file_prepare(audio_index, &file_path, &file_size)!= 0)
    {
        rt_kprintf("[audio] 错误：音频文件准备失败，索引: %d\n", audio_index);
        return -1;
    }

    // 打开文件读取数据
    // rt_kprintf("[audio] 开始读取音频数据：%s\n", file_path);
    int fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        rt_kprintf("[audio] 错误：无法打开文件 %s\n", file_path);
        return -1;
    }

    char *audio_buf = mpp_alloc(file_size);
    if (!audio_buf)
    {
        rt_kprintf("[audio] 错误：无法分配 %d 字节的音频缓冲区\n", file_size);
        close(fd);
        return -1;
    }
    memset(audio_buf, 0x80, file_size);  // 关键：避免缓冲区垃圾数据触发响声
    int read_size = read(fd, audio_buf, file_size);
    close(fd);
    if (read_size!= file_size)
    {
        rt_kprintf("[audio] 警告：实际读取字节数 %d 与文件大小 %d 不匹配\n", read_size, file_size);
    }


    if (reset_audio_renderer(p_user_audio_render) != 0)
    {
        rt_kprintf("[audio] 错误：渲染器状态异常，跳过本次播放\n");
        mpp_free(audio_buf);
        return -1;
    }

    // 验证音频参数
    if (verify_audio_params(p_user_audio_render)!= 0)
    {
        rt_kprintf("[audio] 警告：音频参数不一致，继续播放\n");
    }
    Sound_Init(PIN_HIGH);
    #if USE_PRI
    rt_kprintf("[audio]:PIN HIGH 1\n") ;
    #endif
    // 播放音频（带重试）
    do {
        ret = aic_audio_render_rend(p_user_audio_render->render, audio_buf, file_size);
        if (ret == 0) break;

        rt_kprintf("[audio] 警告：播放音频失败，重试 %d/%d\n", retry + 1, MAX_RETRY_COUNT);
        usleep(AUDIO_RESET_DELAY_US);
        reset_audio_renderer(p_user_audio_render);
    } while (++retry < MAX_RETRY_COUNT);

    if (ret!= 0)
    {
        rt_kprintf("[audio] 错误：多次尝试播放音频失败\n");
        mpp_free(audio_buf);
        return -1;
    }

    // 计算并等待播放完成
    play_time = audio_play_wait_time(file_size);
    // rt_kprintf("[audio] 等待播放完成（%ld 微秒）\n", play_time);
    if (play_time > 0)
    {
        usleep(10000);  // 使用计算出的准确播放时间
    }
    else
    {
        // 作为后备，使用基于文件大小的估算时间
        int estimated_time = (file_size * 1000) / (USER_AUDIO_RENDER_SAMPLERATE * 2);
        rt_kprintf("[audio] 使用估算播放时间：%d 毫秒\n", estimated_time);
        usleep(100000);
    }

    // 释放资源
    mpp_free(audio_buf);
    return 0;
}

static int audio_file_prepare(int num, char **file_path, int *file_size)
{
    // 严格的边界检查
    if (num < 0 || num >= sizeof(g_user_file_path)/sizeof(g_user_file_path[0]))
    {
        rt_kprintf("[audio] 错误：音频索引 %d 越界（最大索引: %d）\n",
                  num, (int)(sizeof(g_user_file_path)/sizeof(g_user_file_path[0]) - 1));
        *file_path = NULL;
        *file_size = 0;
        return -1;
    }

    // 返回系统路径
    *file_path = g_user_file_path[num];
    #if USE_PRI
    rt_kprintf("[audio] 准备播放的系统路径: %s\n", *file_path);
    #endif

    // 获取文件大小（带重试）
    int fd = -1;
    int retry = 0;
    do {
        fd = open(*file_path, O_RDONLY);
        if (fd >= 0) break;

        rt_kprintf("[audio] 警告：无法打开文件 %s，重试 %d/%d\n", *file_path, retry + 1, MAX_RETRY_COUNT);
        usleep(AUDIO_RESET_DELAY_US);
    } while (++retry < MAX_RETRY_COUNT);

    if (fd < 0)
    {
        rt_kprintf("[audio] 错误：多次尝试打开文件 %s 失败\n", *file_path);
        *file_size = 0;
        return -1;
    }

    *file_size = lseek(fd, 0, SEEK_END);
    close(fd);

    if (*file_size <= 0)
    {
        rt_kprintf("[audio] 错误：文件大小无效（%d 字节）\n", *file_size);
        return -1;
    }

    // rt_kprintf("[audio] 文件大小获取成功: %d 字节\n", *file_size);
    return 0;
}

static int64_t audio_play_wait_time(int file_size)
{
    // rt_kprintf("[audio] 进入 audio_play_wait_time，文件大小: %d 字节\n", file_size);

    if (file_size <= 0)
    {
        rt_kprintf("[audio] 警告：文件大小无效，返回等待时间 0\n");
        return 0;
    }
    // 加锁访问全局变量
    // rt_mutex_take(audio_mutex, RT_WAITING_FOREVER);
    bool render_valid = (g_user_audio_render.render != NULL);
    struct aic_audio_render_attr ao_attr_copy = g_user_audio_render.ao_attr;
    // rt_mutex_release(audio_mutex);

    // 检查渲染器状态
    if (!render_valid)
    {
        rt_kprintf("[audio] 错误：渲染器未初始化，无法计算播放时间\n");
        return 0;
    }

    int bytes_per_second = ao_attr_copy.sample_rate * 16 / 8 * ao_attr_copy.channels;

    // rt_kprintf("[audio] 音频参数：采样率 %d Hz，声道数 %d，每秒字节数: %d\n",
    //            p_ao_attr->sample_rate, p_ao_attr->channels, bytes_per_second);

    if (bytes_per_second <= 0)
    {
        rt_kprintf("[audio] 错误：音频参数无效，无法计算播放时间\n");
        return 0;
    }

    int64_t play_time = (int64_t)file_size * 1000000 / bytes_per_second;
    // 增加10%的安全余量，避免提前结束
    play_time = (play_time * 11) / 10;
    // rt_kprintf("[audio] 计算播放时间（含安全余量）：%ld 微秒（约 %ld 毫秒）\n", play_time, play_time / 1000);
    return play_time;
}

// 初始化音频渲染器（带完整错误处理）
static s32 init_audio_renderer()
{
    // 函数开头加锁
    rt_mutex_take(audio_mutex, RT_WAITING_FOREVER);
    // 如果已有实例，先销毁
    if (g_user_audio_render.render)
    {
        player_audio_render_destroy();
    }

    s32 ret = 0;
    s32 value = 0;
    struct audio_render_create_params create_params;

    // 初始化音频参数
    g_user_audio_render.ao_attr.channels = USER_AUDIO_RENDER_CHANNELS;
    g_user_audio_render.ao_attr.sample_rate = USER_AUDIO_RENDER_SAMPLERATE;
    // rt_kprintf("[audio] 初始化音频参数：声道数 %d，采样率 %d Hz\n",
    //            USER_AUDIO_RENDER_CHANNELS, USER_AUDIO_RENDER_SAMPLERATE);

    // 创建音频渲染器（带重试）
    int retry = 0;
    create_params.dev_id = 0;
    create_params.scene_type = AUDIO_RENDER_SCENE_WARNING_TONE;

    do {
        ret = aic_audio_render_create(&g_user_audio_render.render, &create_params);
        if (ret == 0) break;

        rt_kprintf("[audio] 警告：创建音频渲染器失败，重试 %d/%d\n", retry + 1, MAX_RETRY_COUNT);
        usleep(AUDIO_RESET_DELAY_US);
    } while (++retry < MAX_RETRY_COUNT);

    if (ret!= 0 ||!g_user_audio_render.render)
    {
        rt_kprintf("[audio] 错误：多次尝试创建音频渲染器失败\n");
        rt_mutex_release(audio_mutex);
        return -1;
    }
    // rt_kprintf("[audio] 音频渲染器创建成功，地址: %p\n", g_user_audio_render.render);

    // 初始化渲染器（带重试）
    retry = 0;
    do {
        ret = aic_audio_render_init(g_user_audio_render.render);
        if (ret == 0) break;

        rt_kprintf("[audio] 警告：初始化音频渲染器失败，重试 %d/%d\n", retry + 1, MAX_RETRY_COUNT);
        usleep(AUDIO_RESET_DELAY_US);
    } while (++retry < MAX_RETRY_COUNT);

    if (ret!= 0)
    {
        rt_kprintf("[audio] 错误：多次尝试初始化音频渲染器失败\n");
        goto cleanup;
    }

    // 保存原始属性
    if (aic_audio_render_control(g_user_audio_render.render,
                                 AUDIO_RENDER_CMD_GET_ATTR,
                                 &g_user_audio_render.original_ao_attr)!= 0)
    {
        rt_kprintf("[audio] 警告：获取原始属性失败，可能影响后续重置\n");
    }

    // 设置优先级
    value = USER_AUDIO_RENDER_PRIORITY;
    // rt_kprintf("[audio] 设置渲染器优先级: %d\n", value);
    if (aic_audio_render_control(g_user_audio_render.render,
                                 AUDIO_RENDER_CMD_SET_SCENE_PRIORITY,
                                 &value)!= 0)
    {
        rt_kprintf("[audio] 警告：设置优先级失败（可能不影响基本播放）\n");
        // 这里不返回错误，因为优先级设置失败可能不影响基本功能
    }

    // 清除缓存
    // rt_kprintf("[audio] 清除音频渲染器缓存...\n");
    if (aic_audio_render_control(g_user_audio_render.render,
                                 AUDIO_RENDER_CMD_CLEAR_CACHE,
                                 NULL)!= 0)
    {
        rt_kprintf("[audio] 错误：清除缓存失败\n");
        ret = -1;
        goto cleanup;
    }

    // 设置音频参数
    // rt_kprintf("[audio] 设置新的音频属性...\n");
    if (aic_audio_render_control(g_user_audio_render.render,
                                 AUDIO_RENDER_CMD_SET_ATTR,
                                 &g_user_audio_render.ao_attr)!= 0)
    {
        rt_kprintf("[audio] 错误：设置音频属性失败\n");
        ret = -1;
        goto cleanup;
    }

    g_user_audio_render.init_flag = 1;  // 标记为已初始化
    // rt_kprintf("[audio] 音频渲染器初始化成功\n");
    rt_mutex_release(audio_mutex); // 成功前释放锁
    return ret;

cleanup:
    aic_audio_render_destroy(g_user_audio_render.render);
    g_user_audio_render.render = NULL;
    g_user_audio_render.init_flag = 0;
    rt_mutex_release(audio_mutex); // 成功前释放锁
    return ret;
}

s32 player_audio_render_share_play(uint8_t volume, uint16_t select_play, bool en_cn)
{
    Sound_Init(PIN_LOW);
    #if USE_PRI
    rt_kprintf("[audio]:PIN LOW 3\n") ;
    #endif
    select_play = (select_play * 2) - 1;

    rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
    wait_elevtor_flag = true;
    rt_mutex_release(elevtor_mutex);

    // 函数开头加锁
    rt_mutex_take(audio_mutex, RT_WAITING_FOREVER);

    s32 ret = 0;
    s32 value = volume;

    if(fire_music_pcm_flag ||
    overload_music_pcm_flag || reset_appease_music_pcm_flag ||
    reset_end_appease_music_pcm_flag || reset_help_appease_music_pcm_flag ||
    up_obs_music_pcm_flag || down_obs_music_pcm_flag)
    {
        if(MY_SET.play_mode == PLAY_VIDEO)
            usleep(AUDIO_PLAY_DELAY_US);
        else
            usleep(AUDIO_PLAY_DELAY_US);
    }
    // 检查并初始化渲染器
    if (!g_user_audio_render.init_flag)
    {
        // 注意：init_audio_renderer内部已经加锁，这里先释放锁再调用，避免死锁
        rt_mutex_release(audio_mutex);
        if (init_audio_renderer()!= 0)
        {
            rt_kprintf("[audio] 错误：渲染器初始化失败，无法播放音频\n");
            return -1;
        }
        // 重新加锁
        rt_mutex_take(audio_mutex, RT_WAITING_FOREVER);
    }
    // 确认渲染器状态
    if (!g_user_audio_render.render)
    {
        rt_kprintf("[audio] 错误：渲染器实例不存在\n");
        rt_mutex_release(audio_mutex); // 失败前释放锁
        return -1;
    }
    // 更新音量（每次播放前确保音量正确）
    // rt_kprintf("[audio] 设置音量为: %d\n", value);
    if (aic_audio_render_control(g_user_audio_render.render,
                                 AUDIO_RENDER_CMD_SET_VOL,
                                 &value)!= 0)
    {
        rt_kprintf("[audio] 警告：设置音量失败（使用上次设置值）\n");
        // 这里不返回错误，音量设置失败不应该阻止播放
    }

    if (en_cn)
    {
        // 播放第一个音频（中文）
        if (play_single_audio(&g_user_audio_render, select_play)!= 0)
        {
            rt_kprintf("[audio] 错误：第一个音频播放失败\n");
            ret = -1;
        }
        else
        {
            if(wait_arr_flag)
            {
                wait_arr_flag = false;
                if (MY_VOICE_SWITCH.dzz && play_elevator.elevator_arr)
                {
                    wait_arr_cnt = 1;
                }
            }
            // 音频间隔
            Sound_Init(PIN_LOW);
            #if USE_PRI
            rt_kprintf("[audio]:PIN LOW 2\n") ;
            #endif
            usleep(10000);
            // 播放第二个音频（英文）
            if (play_single_audio(&g_user_audio_render, select_play + 1)!= 0)
            {
                rt_kprintf("[audio] 错误：第二个音频播放失败\n");
                ret = -1;
            }
        }
    }
    else
    {
        // 播放单个音频文件
        if(fire_music_pcm_flag ||
           overload_music_pcm_flag || reset_appease_music_pcm_flag ||
           reset_end_appease_music_pcm_flag || reset_help_appease_music_pcm_flag ||
           up_obs_music_pcm_flag || down_obs_music_pcm_flag)
        {

            play_single_audio(&g_user_audio_render, select_play + 1);
            if(fire_music_pcm_flag) fire_music_pcm_flag = false;
            else if(reset_appease_music_pcm_flag)reset_appease_music_pcm_flag = false;
            else if(reset_help_appease_music_pcm_flag)reset_help_appease_music_pcm_flag = false;
            else if(reset_end_appease_music_pcm_flag)reset_end_appease_music_pcm_flag = false;
            else if(up_obs_music_pcm_flag) up_obs_music_pcm_flag = false;
            else if(down_obs_music_pcm_flag) down_obs_music_pcm_flag = false;
            else if(overload_music_pcm_flag)
            {
                if(have_overload_flag && !have_buzzer_flag)
                {
                    if(voice_language == VOICE_CN_EN)
                    {
                        overload_start_flag = true;
                        overload_music_pcm_flag = false;
                    }
                    rt_kprintf("还有超载音乐信号1;\n");
                }else {
                    rt_kprintf("overload_music_pcm_flag = false;\n");
                    overload_music_pcm_flag = false;
                }
            }

        }
        else if (play_single_audio(&g_user_audio_render,
                             (voice_language == VOICE_CN)? select_play : select_play + 1)!= 0)
        {
            rt_kprintf("[audio] 错误：音频播放失败\n");
            ret = -1;
        }
    }
    // rt_kprintf("[audio] ====== 音频播放流程结束 ======\n");

    // 播放完成后销毁渲染器，确保主界面视频可以正常发声
    if(wait_arr_cnt == 1)
    {
        rt_kprintf("不销毁音频渲染器\n");
        wait_arr_cnt = 0;
    }
    else{
        Sound_Init(PIN_LOW);
        #if USE_PRI
        rt_kprintf("[audio]:PIN LOW 1\n") ;
        #endif
        if (rt_mutex_take(volume_mutex, RT_TICK_PER_SECOND / 10) == RT_EOK)
        {
            set_video_vol_flag = true;
            set_vol_cnt = 0; // 重置计数，确保立即执行
            rt_mutex_release(volume_mutex);
        }
        // 先释放锁，再调用销毁（销毁内部自己加锁）
        rt_mutex_release(audio_mutex);
        player_audio_render_destroy();
    }
    rt_mutex_release(audio_mutex); // 函数结尾释放锁
    // ========== 新增：强制清理DFS缓存 ==========
    // dfs_force_clean_all_cache();
    // ==========================================

    // rt_kprintf("[audio] ====== player_audio_render_share_play 函数结束，返回值: %d ======\n", ret);
    return ret;
}

// 显式销毁接口，播放完成后调用，确保音频设备释放
void player_audio_render_destroy()
{
    // 函数开头加锁
    rt_mutex_take(audio_mutex, RT_WAITING_FOREVER);
    if (g_user_audio_render.render)
    {
         // 2. 多次清理缓存
        for (int i = 0; i < CACHE_CLEAN_TIMES; i++)
        {
            aic_audio_render_control(g_user_audio_render.render, AUDIO_RENDER_CMD_CLEAR_CACHE, NULL);
            usleep(5000);
        }

        // 3. 恢复原始属性
        aic_audio_render_control(g_user_audio_render.render,
                                AUDIO_RENDER_CMD_SET_ATTR,
                                &g_user_audio_render.original_ao_attr);

        // 4. 销毁实例
        aic_audio_render_destroy(g_user_audio_render.render);

        // 5. 强制置空所有变量
        g_user_audio_render.render = NULL;
        g_user_audio_render.init_flag = 0;
        memset(&g_user_audio_render.ao_attr, 0, sizeof(struct aic_audio_render_attr));
        memset(&g_user_audio_render.original_ao_attr, 0, sizeof(struct aic_audio_render_attr));
        //  rt_kprintf("[audio] 音频渲染器已销毁，释放音频设备\n");
        rt_mutex_take(elevtor_mutex, RT_WAITING_FOREVER);
        wait_elevtor_flag = false;
        rt_mutex_release(elevtor_mutex);
    }else {
        rt_kprintf("[audio]无法获取音频渲染器\n");
    }
    rt_mutex_release(audio_mutex); // 函数结尾释放锁
}

#endif
