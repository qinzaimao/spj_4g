#ifndef __ELEVATOR_PLAY_H__
#define __ELEVATOR_PLAY_H__

#include "main.h"

#define VIDEO_FIRE_CN PCM_PATH(YM62_8.pcm)        // 消防中文播报
#define VIDEO_FIRE_EN PCM_PATH(YE62_8.pcm)        // 消防英文播报
#define VIDEO_UP_CN PCM_PATH(WMUP.pcm)            // 升降中文播报
#define VIDEO_UP_EN PCM_PATH(WEUP.pcm)            // 升降英文播报
#define VIDEO_DOWN_CN PCM_PATH(WMDN.pcm)          // 升降中文播报
#define VIDEO_DOWN_EN PCM_PATH(WEDN.pcm)          // 升降英文播报
#define VIDEO_OPEN_CN PCM_PATH(YM63_1.pcm)        // 开门中文播报
#define VIDEO_OPEN_EN PCM_PATH(YE63_1.pcm)        // 开门英文播报
#define VIDEO_CLOSE_CN PCM_PATH(YM63_2.pcm)       // 关门中文播报
#define VIDEO_CLOSE_EN PCM_PATH(YE63_2.pcm)       // 关门英文播报
#define VIDEO_PEAK_CN PCM_PATH(YM63_3.pcm)        // 高峰中文播报
#define VIDEO_PEAK_EN PCM_PATH(YE63_3.pcm)        // 高峰英文播报
#define VIDEO_OVERLOAD_CN PCM_PATH(YM62_5.pcm)    // 超载中文播报
#define VIDEO_OVERLOAD_EN PCM_PATH(YE62_5.pcm)    // 超载英文播报
#define VIDEO_APPEASE_1_CN PCM_PATH(YM62_1_3.pcm) // 安抚中文播报
#define VIDEO_APPEASE_1_EN PCM_PATH(YE62_1_3.pcm) // 安抚英文播报
#define VIDEO_APPEASE_2_CN PCM_PATH(YM62_2.pcm)   // 安抚中文播报
#define VIDEO_APPEASE_2_EN PCM_PATH(YE62_2.pcm)   // 安抚英文播报
#define VIDEO_APPEASE_3_CN PCM_PATH(YM62_4.pcm)   // 安抚中文播报
#define VIDEO_APPEASE_3_EN PCM_PATH(YE62_4.pcm)   // 安抚英文播报

#define VIDEO_APPEASE_CN   PCM_PATH(YM63_1_1.pcm) // 困人安抚中文播报
#define VIDEO_APPEASE_EN   PCM_PATH(YE63_1_1.pcm) // 困人安抚英文播报

#define VIDEO_BELL_CN PCM_PATH(BELL.pcm)   //
#define VIDEO_BELL_EN PCM_PATH(BELL.pcm)   //

#define VIDEO_WNBUZ_CN PCM_PATH(WNBUZ.pcm)   //
#define VIDEO_WNBUZ_EN PCM_PATH(WNBUZ.pcm)   //

#define VIDEO_UP_OBS_CN PCM_PATH(YM62_6.pcm)   //
#define VIDEO_UP_OBS_EN PCM_PATH(YE62_6.pcm)   //

#define VIDEO_DOWN_OBS_CN PCM_PATH(YM62_7.pcm)   //
#define VIDEO_DOWN_OBS_EN PCM_PATH(YE62_7.pcm)   //



#define VIDEO_00_CN PCM_PATH(MA00.pcm)   //
#define VIDEO_01_CN PCM_PATH(MA01.pcm)   //
#define VIDEO_02_CN PCM_PATH(MA02.pcm)   //
#define VIDEO_03_CN PCM_PATH(MA03.pcm)   //
#define VIDEO_04_CN PCM_PATH(MA04.pcm)   //
#define VIDEO_05_CN PCM_PATH(MA05.pcm)   //
#define VIDEO_06_CN PCM_PATH(MA06.pcm)   //
#define VIDEO_07_CN PCM_PATH(MA07.pcm)   //
#define VIDEO_08_CN PCM_PATH(MA08.pcm)   //
#define VIDEO_09_CN PCM_PATH(MA09.pcm)   //9
#define VIDEO_10_CN PCM_PATH(MA10.pcm)   //10
#define VIDEO_11_CN PCM_PATH(MA11.pcm)   //11
#define VIDEO_12_CN PCM_PATH(MA12.pcm)   //12
#define VIDEO_13_CN PCM_PATH(MA13.pcm)   //13
#define VIDEO_14_CN PCM_PATH(MA14.pcm)   //14
#define VIDEO_15_CN PCM_PATH(MA15.pcm)   //15
#define VIDEO_16_CN PCM_PATH(MA16.pcm)   //16
#define VIDEO_17_CN PCM_PATH(MA17.pcm)   //17
#define VIDEO_18_CN PCM_PATH(MA18.pcm)   //18
#define VIDEO_19_CN PCM_PATH(MA19.pcm)   //19
#define VIDEO_20_CN PCM_PATH(MA20.pcm)   //20
#define VIDEO_21_CN PCM_PATH(MA21.pcm)   //21
#define VIDEO_22_CN PCM_PATH(MA22.pcm)   //22
#define VIDEO_23_CN PCM_PATH(MA23.pcm)   //23
#define VIDEO_24_CN PCM_PATH(MA24.pcm)   //24
#define VIDEO_25_CN PCM_PATH(MA25.pcm)   //25
#define VIDEO_26_CN PCM_PATH(MA26.pcm)   //26
#define VIDEO_27_CN PCM_PATH(MA27.pcm)   //27
#define VIDEO_28_CN PCM_PATH(MA28.pcm)   //28
#define VIDEO_29_CN PCM_PATH(MA29.pcm)   //29
#define VIDEO_30_CN PCM_PATH(MA30.pcm)   //30
#define VIDEO_31_CN PCM_PATH(MA31.pcm)   //31
#define VIDEO_32_CN PCM_PATH(MA32.pcm)   //32
#define VIDEO_33_CN PCM_PATH(MA33.pcm)   //33
#define VIDEO_34_CN PCM_PATH(MA34.pcm)   //34
#define VIDEO_35_CN PCM_PATH(MA35.pcm)   //35
#define VIDEO_36_CN PCM_PATH(MA36.pcm)   //36
#define VIDEO_37_CN PCM_PATH(MA37.pcm)   //37
#define VIDEO_38_CN PCM_PATH(MA38.pcm)   //38
#define VIDEO_39_CN PCM_PATH(MA39.pcm)   //39
#define VIDEO_40_CN PCM_PATH(MA40.pcm)   //40
#define VIDEO_41_CN PCM_PATH(MA41.pcm)   //41
#define VIDEO_42_CN PCM_PATH(MA42.pcm)   //42
#define VIDEO_43_CN PCM_PATH(MA43.pcm)   //43
#define VIDEO_44_CN PCM_PATH(MA44.pcm)   //44
#define VIDEO_45_CN PCM_PATH(MA45.pcm)   //45
#define VIDEO_46_CN PCM_PATH(MA46.pcm)   //46
#define VIDEO_47_CN PCM_PATH(MA47.pcm)   //47
#define VIDEO_48_CN PCM_PATH(MA48.pcm)   //48
#define VIDEO_49_CN PCM_PATH(MA49.pcm)   //49
#define VIDEO_50_CN PCM_PATH(MA50.pcm)   //50
#define VIDEO_51_CN PCM_PATH(MA51.pcm)   //51
#define VIDEO_52_CN PCM_PATH(MA52.pcm)   //52
#define VIDEO_53_CN PCM_PATH(MA53.pcm)   //53
#define VIDEO_54_CN PCM_PATH(MA54.pcm)   //54
#define VIDEO_55_CN PCM_PATH(MA55.pcm)   //55
#define VIDEO_56_CN PCM_PATH(MA56.pcm)   //56
#define VIDEO_57_CN PCM_PATH(MA57.pcm)   //57
#define VIDEO_58_CN PCM_PATH(MA58.pcm)   //58
#define VIDEO_59_CN PCM_PATH(MA59.pcm)   //59
#define VIDEO_60_CN PCM_PATH(MA60.pcm)   //60
#define VIDEO_61_CN PCM_PATH(MA61.pcm)   //61
#define VIDEO_62_CN PCM_PATH(MA62.pcm)   //62
#define VIDEO_63_CN PCM_PATH(MA63.pcm)   //63
#define VIDEO_64_CN PCM_PATH(MA64.pcm)   //64
#define VIDEO_65_CN PCM_PATH(MA65.pcm)   //65
#define VIDEO_66_CN PCM_PATH(MA66.pcm)   //66
#define VIDEO_67_CN PCM_PATH(MA67.pcm)   //67
#define VIDEO_68_CN PCM_PATH(MA68.pcm)   //68
#define VIDEO_69_CN PCM_PATH(MA69.pcm)   //69
#define VIDEO_70_CN PCM_PATH(MA70.pcm)   //70
#define VIDEO_71_CN PCM_PATH(MA71.pcm)   //71
#define VIDEO_72_CN PCM_PATH(MA72.pcm)   //72
#define VIDEO_73_CN PCM_PATH(MA73.pcm)   //73
#define VIDEO_74_CN PCM_PATH(MA74.pcm)   //74
#define VIDEO_75_CN PCM_PATH(MA75.pcm)   //75
#define VIDEO_76_CN PCM_PATH(MA76.pcm)   //76
#define VIDEO_77_CN PCM_PATH(MA77.pcm)   //77
#define VIDEO_78_CN PCM_PATH(MA78.pcm)   //78
#define VIDEO_79_CN PCM_PATH(MA79.pcm)   //79
#define VIDEO_MINU_1_CN PCM_PATH(MB01.pcm)   //-1
#define VIDEO_MINU_2_CN PCM_PATH(MB02.pcm)   //-2
#define VIDEO_MINU_3_CN PCM_PATH(MB03.pcm)   //-3
#define VIDEO_MINU_4_CN PCM_PATH(MB04.pcm)   //-4
#define VIDEO_MINU_5_CN PCM_PATH(MB05.pcm)   //-5
#define VIDEO_MINU_6_CN PCM_PATH(MB06.pcm)   //-6
#define VIDEO_MINU_7_CN PCM_PATH(MB07.pcm)   //-7
#define VIDEO_MINU_8_CN PCM_PATH(MB08.pcm)   //-8
#define VIDEO_MINU_9_CN PCM_PATH(MB09.pcm)   //-9

#define VIDEO_A_CN PCM_PATH(MC0A.pcm)   //A
#define VIDEO_B_CN PCM_PATH(MC0B.pcm)   //A
#define VIDEO_C_CN PCM_PATH(MC0C.pcm)   //A
#define VIDEO_D_CN PCM_PATH(MC0D.pcm)   //A
#define VIDEO_E_CN PCM_PATH(MC0E.pcm)   //A
#define VIDEO_F_CN PCM_PATH(MC0F.pcm)   //A
#define VIDEO_G_CN PCM_PATH(MC0G.pcm)   //A
#define VIDEO_H_CN PCM_PATH(MC0H.pcm)   //A
#define VIDEO_I_CN PCM_PATH(MC0I.pcm)   //A
#define VIDEO_J_CN PCM_PATH(MC0J.pcm)   //A
#define VIDEO_K_CN PCM_PATH(MC0K.pcm)   //A
#define VIDEO_L_CN PCM_PATH(MC0L.pcm)   //A
#define VIDEO_M_CN PCM_PATH(MC0M.pcm)   //A
#define VIDEO_N_CN PCM_PATH(MC0N.pcm)   //A
#define VIDEO_O_CN PCM_PATH(MC0O.pcm)   //A
#define VIDEO_P_CN PCM_PATH(MC0P.pcm)   //A
#define VIDEO_Q_CN PCM_PATH(MC0Q.pcm)   //A
#define VIDEO_R_CN PCM_PATH(MC0R.pcm)   //A
#define VIDEO_S_CN PCM_PATH(MC0S.pcm)   //A
#define VIDEO_T_CN PCM_PATH(MC0T.pcm)   //A
#define VIDEO_U_CN PCM_PATH(MC0U.pcm)   //A
#define VIDEO_V_CN PCM_PATH(MC0V.pcm)   //A
#define VIDEO_W_CN PCM_PATH(MC0W.pcm)   //A
#define VIDEO_X_CN PCM_PATH(MC0X.pcm)   //A
#define VIDEO_Y_CN PCM_PATH(MC0Y.pcm)   //A
#define VIDEO_Z_CN PCM_PATH(MC0Z.pcm)   //A

#define VIDEO_1A_CN PCM_PATH(MC1A.pcm)   //A
#define VIDEO_2A_CN PCM_PATH(MC2A.pcm)   //A
#define VIDEO_3A_CN PCM_PATH(MC3A.pcm)   //A
#define VIDEO_4A_CN PCM_PATH(MC4A.pcm)   //A
#define VIDEO_5A_CN PCM_PATH(MC5A.pcm)   //A
#define VIDEO_6A_CN PCM_PATH(MC6A.pcm)   //A
#define VIDEO_7A_CN PCM_PATH(MC7A.pcm)   //A
#define VIDEO_8A_CN PCM_PATH(MC8A.pcm)   //A
#define VIDEO_9A_CN PCM_PATH(MC9A.pcm)   //A

#define VIDEO_1B_CN PCM_PATH(MC1B.pcm)   //A
#define VIDEO_2B_CN PCM_PATH(MC2B.pcm)   //A
#define VIDEO_3B_CN PCM_PATH(MC3B.pcm)   //A
#define VIDEO_4B_CN PCM_PATH(MC4B.pcm)   //A
#define VIDEO_5B_CN PCM_PATH(MC5B.pcm)   //A
#define VIDEO_6B_CN PCM_PATH(MC6B.pcm)   //A
#define VIDEO_7B_CN PCM_PATH(MC7B.pcm)   //A
#define VIDEO_8B_CN PCM_PATH(MC8B.pcm)   //A
#define VIDEO_9B_CN PCM_PATH(MC9B.pcm)   //A

#define VIDEO_1C_CN PCM_PATH(MC1C.pcm)   //A
#define VIDEO_2C_CN PCM_PATH(MC2C.pcm)   //A
#define VIDEO_3C_CN PCM_PATH(MC3C.pcm)   //A
#define VIDEO_4C_CN PCM_PATH(MC4C.pcm)   //A
#define VIDEO_5C_CN PCM_PATH(MC5C.pcm)   //A
#define VIDEO_6C_CN PCM_PATH(MC6C.pcm)   //A

#define VIDEO_1F_CN PCM_PATH(MC1F.pcm)   //A
#define VIDEO_2F_CN PCM_PATH(MC2F.pcm)   //A
#define VIDEO_3F_CN PCM_PATH(MC3F.pcm)   //A
#define VIDEO_4F_CN PCM_PATH(MC4F.pcm)   //A
#define VIDEO_5F_CN PCM_PATH(MC5F.pcm)   //A

#define VIDEO_A0_CN PCM_PATH(MDA0.pcm)   //A
#define VIDEO_A1_CN PCM_PATH(MDA1.pcm)   //A
#define VIDEO_A2_CN PCM_PATH(MDA2.pcm)   //A
#define VIDEO_A3_CN PCM_PATH(MDA3.pcm)   //A
#define VIDEO_A4_CN PCM_PATH(MDA4.pcm)   //A
#define VIDEO_A5_CN PCM_PATH(MDA5.pcm)   //A
#define VIDEO_A6_CN PCM_PATH(MDA6.pcm)   //A
#define VIDEO_A7_CN PCM_PATH(MDA7.pcm)   //A
#define VIDEO_A8_CN PCM_PATH(MDA8.pcm)   //A
#define VIDEO_A9_CN PCM_PATH(MDA9.pcm)   //A

#define VIDEO_B0_CN PCM_PATH(MDB0.pcm)   //A
#define VIDEO_B1_CN PCM_PATH(MDB1.pcm)   //A
#define VIDEO_B2_CN PCM_PATH(MDB2.pcm)   //A
#define VIDEO_B3_CN PCM_PATH(MDB3.pcm)   //A
#define VIDEO_B4_CN PCM_PATH(MDB4.pcm)   //A
#define VIDEO_B5_CN PCM_PATH(MDB5.pcm)   //A
#define VIDEO_B6_CN PCM_PATH(MDB6.pcm)   //A
#define VIDEO_B7_CN PCM_PATH(MDB7.pcm)   //A
#define VIDEO_B8_CN PCM_PATH(MDB8.pcm)   //A
#define VIDEO_B9_CN PCM_PATH(MDB9.pcm)   //A


#define VIDEO_C1_CN PCM_PATH(MDC1.pcm)   //A
#define VIDEO_C2_CN PCM_PATH(MDC2.pcm)   //A
#define VIDEO_C3_CN PCM_PATH(MDC3.pcm)   //A
#define VIDEO_C4_CN PCM_PATH(MDC4.pcm)   //A

#define VIDEO_D1_CN PCM_PATH(MDD1.pcm)   //A
#define VIDEO_D2_CN PCM_PATH(MDD2.pcm)   //A
#define VIDEO_D3_CN PCM_PATH(MDD3.pcm)   //A
#define VIDEO_D4_CN PCM_PATH(MDD4.pcm)   //A

#define VIDEO_G1_CN PCM_PATH(MDG1.pcm)   //A
#define VIDEO_G2_CN PCM_PATH(MDG2.pcm)   //A
#define VIDEO_G3_CN PCM_PATH(MDG3.pcm)   //A
#define VIDEO_G4_CN PCM_PATH(MDG4.pcm)   //A
#define VIDEO_G5_CN PCM_PATH(MDG5.pcm)   //A
#define VIDEO_G6_CN PCM_PATH(MDG6.pcm)   //A
#define VIDEO_G7_CN PCM_PATH(MDG7.pcm)   //A
#define VIDEO_G8_CN PCM_PATH(MDG8.pcm)   //A
#define VIDEO_G9_CN PCM_PATH(MDG9.pcm)   //A


#define VIDEO_L1_CN PCM_PATH(MDL1.pcm)   //A
#define VIDEO_L2_CN PCM_PATH(MDL2.pcm)   //A
#define VIDEO_L3_CN PCM_PATH(MDL3.pcm)   //A

#define VIDEO_M0_CN PCM_PATH(MDM0.pcm)   //A
#define VIDEO_M1_CN PCM_PATH(MDM1.pcm)   //A
#define VIDEO_M2_CN PCM_PATH(MDM2.pcm)   //A
#define VIDEO_M3_CN PCM_PATH(MDM3.pcm)   //A
#define VIDEO_M4_CN PCM_PATH(MDM4.pcm)   //A
#define VIDEO_M5_CN PCM_PATH(MDM5.pcm)   //A
#define VIDEO_M6_CN PCM_PATH(MDM6.pcm)   //A
#define VIDEO_M7_CN PCM_PATH(MDM7.pcm)   //A
#define VIDEO_M8_CN PCM_PATH(MDM8.pcm)   //A
#define VIDEO_M9_CN PCM_PATH(MDM9.pcm)   //A

#define VIDEO_P0_CN PCM_PATH(MDP0.pcm)   //A
#define VIDEO_P1_CN PCM_PATH(MDP1.pcm)   //A
#define VIDEO_P2_CN PCM_PATH(MDP2.pcm)   //A
#define VIDEO_P3_CN PCM_PATH(MDP3.pcm)   //A
#define VIDEO_P4_CN PCM_PATH(MDP4.pcm)   //A
#define VIDEO_P5_CN PCM_PATH(MDP5.pcm)   //A
#define VIDEO_P6_CN PCM_PATH(MDP6.pcm)   //A
#define VIDEO_P7_CN PCM_PATH(MDP7.pcm)   //A
#define VIDEO_P8_CN PCM_PATH(MDP8.pcm)   //A
#define VIDEO_P9_CN PCM_PATH(MDP9.pcm)   //A

#define VIDEO_R1_CN PCM_PATH(MDR1.pcm)   //A
#define VIDEO_R2_CN PCM_PATH(MDR2.pcm)   //A
#define VIDEO_R3_CN PCM_PATH(MDR3.pcm)   //A

#define VIDEO_AF_CN PCM_PATH(MEAF.pcm)   //A
#define VIDEO_AG_CN PCM_PATH(MEAG.pcm)   //A
#define VIDEO_BE_CN PCM_PATH(MEBE.pcm)   //A
#define VIDEO_CF_CN PCM_PATH(MECF.pcm)   //A
#define VIDEO_EG_CN PCM_PATH(MEEG.pcm)   //A
#define VIDEO_GF_CN PCM_PATH(MEGF.pcm)   //A
#define VIDEO_HP_CN PCM_PATH(MEHP.pcm)   //A
#define VIDEO_KG_CN PCM_PATH(MEKG.pcm)   //A
#define VIDEO_LB_CN PCM_PATH(MELB.pcm)   //A
#define VIDEO_LD_CN PCM_PATH(MELD.pcm)   //A
#define VIDEO_LF_CN PCM_PATH(MELF.pcm)   //A
#define VIDEO_LG_CN PCM_PATH(MELG.pcm)   //A
#define VIDEO_LL_CN PCM_PATH(MELL.pcm)   //A
#define VIDEO_LP_CN PCM_PATH(MELP.pcm)   //A
#define VIDEO_MR_CN PCM_PATH(MEMR.pcm)   //A
#define VIDEO_MZ_CN PCM_PATH(MEMZ.pcm)   //A
#define VIDEO_PB_CN PCM_PATH(MEPB.pcm)   //A
#define VIDEO_PC_CN PCM_PATH(MEPC.pcm)   //A
#define VIDEO_PH_CN PCM_PATH(MEPH.pcm)   //A
#define VIDEO_PM_CN PCM_PATH(MEPM.pcm)   //A
#define VIDEO_RF_CN PCM_PATH(MERF.pcm)   //A
#define VIDEO_UB_CN PCM_PATH(MEUB.pcm)   //A
#define VIDEO_UF_CN PCM_PATH(MEUF.pcm)   //A
#define VIDEO_UG_CN PCM_PATH(MEUG.pcm)   //A
#define VIDEO_UPP_CN PCM_PATH(MEUP.pcm)   //A

#define VIDEO_12A_CN PCM_PATH(MF12A.pcm)   //A
#define VIDEO_12B_CN PCM_PATH(MF12B.pcm)   //A
#define VIDEO_13A_CN PCM_PATH(MF13A.pcm)   //A
#define VIDEO_13B_CN PCM_PATH(MF13B.pcm)   //A
#define VIDEO_14A_CN PCM_PATH(MF14A.pcm)   //A
#define VIDEO_14B_CN PCM_PATH(MF14B.pcm)   //A
#define VIDEO_15A_CN PCM_PATH(MF15A.pcm)   //A
#define VIDEO_15B_CN PCM_PATH(MF15B.pcm)   //A
#define VIDEO_17A_CN PCM_PATH(MF17A.pcm)   //A
#define VIDEO_17B_CN PCM_PATH(MF17B.pcm)   //A
#define VIDEO_18A_CN PCM_PATH(MF18A.pcm)   //A
#define VIDEO_18B_CN PCM_PATH(MF18B.pcm)   //A
#define VIDEO_23A_CN PCM_PATH(MF23A.pcm)   //A
#define VIDEO_23B_CN PCM_PATH(MF23B.pcm)   //A
#define VIDEO_33A_CN PCM_PATH(MF33A.pcm)   //A
#define VIDEO_33B_CN PCM_PATH(MF33B.pcm)   //A
//英文
//英文
//英文
//英文
//英文
//英文
//英文
//英文
//英文
//英文
#define VIDEO_00_EN PCM_PATH(EA00.pcm)   //
#define VIDEO_01_EN PCM_PATH(EA01.pcm)   //
#define VIDEO_02_EN PCM_PATH(EA02.pcm)   //
#define VIDEO_03_EN PCM_PATH(EA03.pcm)   //
#define VIDEO_04_EN PCM_PATH(EA04.pcm)   //
#define VIDEO_05_EN PCM_PATH(EA05.pcm)   //
#define VIDEO_06_EN PCM_PATH(EA06.pcm)   //
#define VIDEO_07_EN PCM_PATH(EA07.pcm)   //
#define VIDEO_08_EN PCM_PATH(EA08.pcm)   //
#define VIDEO_09_EN PCM_PATH(EA09.pcm)   //
#define VIDEO_10_EN PCM_PATH(EA10.pcm)   //
#define VIDEO_11_EN PCM_PATH(EA11.pcm)   //
#define VIDEO_12_EN PCM_PATH(EA12.pcm)   //
#define VIDEO_13_EN PCM_PATH(EA13.pcm)   //
#define VIDEO_14_EN PCM_PATH(EA14.pcm)   //
#define VIDEO_15_EN PCM_PATH(EA15.pcm)   //
#define VIDEO_16_EN PCM_PATH(EA16.pcm)   //
#define VIDEO_17_EN PCM_PATH(EA17.pcm)   //
#define VIDEO_18_EN PCM_PATH(EA18.pcm)   //
#define VIDEO_19_EN PCM_PATH(EA19.pcm)   //
#define VIDEO_20_EN PCM_PATH(EA20.pcm)   //
#define VIDEO_21_EN PCM_PATH(EA21.pcm)   //
#define VIDEO_22_EN PCM_PATH(EA22.pcm)   //
#define VIDEO_23_EN PCM_PATH(EA23.pcm)   //
#define VIDEO_24_EN PCM_PATH(EA24.pcm)   //
#define VIDEO_25_EN PCM_PATH(EA25.pcm)   //
#define VIDEO_26_EN PCM_PATH(EA26.pcm)   //
#define VIDEO_27_EN PCM_PATH(EA27.pcm)   //
#define VIDEO_28_EN PCM_PATH(EA28.pcm)   //
#define VIDEO_29_EN PCM_PATH(EA29.pcm)   //
#define VIDEO_30_EN PCM_PATH(EA30.pcm)   //
#define VIDEO_31_EN PCM_PATH(EA31.pcm)   //
#define VIDEO_32_EN PCM_PATH(EA32.pcm)   //
#define VIDEO_33_EN PCM_PATH(EA33.pcm)   //
#define VIDEO_34_EN PCM_PATH(EA34.pcm)   //
#define VIDEO_35_EN PCM_PATH(EA35.pcm)   //
#define VIDEO_36_EN PCM_PATH(EA36.pcm)   //
#define VIDEO_37_EN PCM_PATH(EA37.pcm)   //
#define VIDEO_38_EN PCM_PATH(EA38.pcm)   //
#define VIDEO_39_EN PCM_PATH(EA39.pcm)   //
#define VIDEO_40_EN PCM_PATH(EA40.pcm)   //
#define VIDEO_41_EN PCM_PATH(EA41.pcm)   //
#define VIDEO_42_EN PCM_PATH(EA42.pcm)   //
#define VIDEO_43_EN PCM_PATH(EA43.pcm)   //
#define VIDEO_44_EN PCM_PATH(EA44.pcm)   //
#define VIDEO_45_EN PCM_PATH(EA45.pcm)   //
#define VIDEO_46_EN PCM_PATH(EA46.pcm)   //
#define VIDEO_47_EN PCM_PATH(EA47.pcm)   //
#define VIDEO_48_EN PCM_PATH(EA48.pcm)   //
#define VIDEO_49_EN PCM_PATH(EA49.pcm)   //
#define VIDEO_50_EN PCM_PATH(EA50.pcm)   //
#define VIDEO_51_EN PCM_PATH(EA51.pcm)   //
#define VIDEO_52_EN PCM_PATH(EA52.pcm)   //
#define VIDEO_53_EN PCM_PATH(EA53.pcm)   //
#define VIDEO_54_EN PCM_PATH(EA54.pcm)   //
#define VIDEO_55_EN PCM_PATH(EA55.pcm)   //
#define VIDEO_56_EN PCM_PATH(EA56.pcm)   //
#define VIDEO_57_EN PCM_PATH(EA57.pcm)   //
#define VIDEO_58_EN PCM_PATH(EA58.pcm)   //
#define VIDEO_59_EN PCM_PATH(EA59.pcm)   //
#define VIDEO_60_EN PCM_PATH(EA60.pcm)   //
#define VIDEO_61_EN PCM_PATH(EA61.pcm)   //
#define VIDEO_62_EN PCM_PATH(EA62.pcm)   //
#define VIDEO_63_EN PCM_PATH(EA63.pcm)   //
#define VIDEO_64_EN PCM_PATH(EA64.pcm)   //
#define VIDEO_65_EN PCM_PATH(EA65.pcm)   //
#define VIDEO_66_EN PCM_PATH(EA66.pcm)   //
#define VIDEO_67_EN PCM_PATH(EA67.pcm)   //
#define VIDEO_68_EN PCM_PATH(EA68.pcm)   //
#define VIDEO_69_EN PCM_PATH(EA69.pcm)   //
#define VIDEO_70_EN PCM_PATH(EA70.pcm)   //
#define VIDEO_71_EN PCM_PATH(EA71.pcm)   //
#define VIDEO_72_EN PCM_PATH(EA72.pcm)   //
#define VIDEO_73_EN PCM_PATH(EA73.pcm)   //
#define VIDEO_74_EN PCM_PATH(EA74.pcm)   //
#define VIDEO_75_EN PCM_PATH(EA75.pcm)   //
#define VIDEO_76_EN PCM_PATH(EA76.pcm)   //
#define VIDEO_77_EN PCM_PATH(EA77.pcm)   //
#define VIDEO_78_EN PCM_PATH(EA78.pcm)   //
#define VIDEO_79_EN PCM_PATH(EA79.pcm)   //

#define VIDEO_MINU_1_EN PCM_PATH(EB01.pcm)   //-1
#define VIDEO_MINU_2_EN PCM_PATH(EB02.pcm)   //-1
#define VIDEO_MINU_3_EN PCM_PATH(EB03.pcm)   //-1
#define VIDEO_MINU_4_EN PCM_PATH(EB04.pcm)   //-1
#define VIDEO_MINU_5_EN PCM_PATH(EB05.pcm)   //-1
#define VIDEO_MINU_6_EN PCM_PATH(EB06.pcm)   //-1
#define VIDEO_MINU_7_EN PCM_PATH(EB07.pcm)   //-1
#define VIDEO_MINU_8_EN PCM_PATH(EB08.pcm)   //-1
#define VIDEO_MINU_9_EN PCM_PATH(EB09.pcm)   //-1

#define VIDEO_A_EN PCM_PATH(EC0A.pcm)   //A
#define VIDEO_B_EN PCM_PATH(EC0B.pcm)   //A
#define VIDEO_C_EN PCM_PATH(EC0C.pcm)   //A
#define VIDEO_D_EN PCM_PATH(EC0D.pcm)   //A
#define VIDEO_E_EN PCM_PATH(EC0E.pcm)   //A
#define VIDEO_F_EN PCM_PATH(EC0F.pcm)   //A
#define VIDEO_G_EN PCM_PATH(EC0G.pcm)   //A
#define VIDEO_H_EN PCM_PATH(EC0H.pcm)   //A
#define VIDEO_I_EN PCM_PATH(EC0I.pcm)   //A
#define VIDEO_J_EN PCM_PATH(EC0J.pcm)   //A
#define VIDEO_K_EN PCM_PATH(EC0K.pcm)   //A
#define VIDEO_L_EN PCM_PATH(EC0L.pcm)   //A
#define VIDEO_M_EN PCM_PATH(EC0M.pcm)   //A
#define VIDEO_N_EN PCM_PATH(EC0N.pcm)   //A
#define VIDEO_O_EN PCM_PATH(EC0O.pcm)   //A
#define VIDEO_P_EN PCM_PATH(EC0P.pcm)   //A
#define VIDEO_Q_EN PCM_PATH(EC0Q.pcm)   //A
#define VIDEO_R_EN PCM_PATH(EC0R.pcm)   //A
#define VIDEO_S_EN PCM_PATH(EC0S.pcm)   //A
#define VIDEO_T_EN PCM_PATH(EC0T.pcm)   //A
#define VIDEO_U_EN PCM_PATH(EC0U.pcm)   //A
#define VIDEO_V_EN PCM_PATH(EC0V.pcm)   //A
#define VIDEO_W_EN PCM_PATH(EC0W.pcm)   //A
#define VIDEO_X_EN PCM_PATH(EC0X.pcm)   //A
#define VIDEO_Y_EN PCM_PATH(EC0Y.pcm)   //A
#define VIDEO_Z_EN PCM_PATH(EC0Z.pcm)   //A


#define VIDEO_1A_EN PCM_PATH(EC1A.pcm)   //A
#define VIDEO_2A_EN PCM_PATH(EC2A.pcm)   //A
#define VIDEO_3A_EN PCM_PATH(EC3A.pcm)   //A
#define VIDEO_4A_EN PCM_PATH(EC4A.pcm)   //A
#define VIDEO_5A_EN PCM_PATH(EC5A.pcm)   //A
#define VIDEO_6A_EN PCM_PATH(EC6A.pcm)   //A
#define VIDEO_7A_EN PCM_PATH(EC7A.pcm)   //A
#define VIDEO_8A_EN PCM_PATH(EC8A.pcm)   //A
#define VIDEO_9A_EN PCM_PATH(EC9A.pcm)   //A

#define VIDEO_1B_EN PCM_PATH(EC1B.pcm)   //A
#define VIDEO_2B_EN PCM_PATH(EC2B.pcm)   //A
#define VIDEO_3B_EN PCM_PATH(EC3B.pcm)   //A
#define VIDEO_4B_EN PCM_PATH(EC4B.pcm)   //A
#define VIDEO_5B_EN PCM_PATH(EC5B.pcm)   //A
#define VIDEO_6B_EN PCM_PATH(EC6B.pcm)   //A
#define VIDEO_7B_EN PCM_PATH(EC7B.pcm)   //A
#define VIDEO_8B_EN PCM_PATH(EC8B.pcm)   //A
#define VIDEO_9B_EN PCM_PATH(EC9B.pcm)   //A

#define VIDEO_1C_EN PCM_PATH(EC1C.pcm)   //A
#define VIDEO_2C_EN PCM_PATH(EC2C.pcm)   //A
#define VIDEO_3C_EN PCM_PATH(EC3C.pcm)   //A
#define VIDEO_4C_EN PCM_PATH(EC4C.pcm)   //A
#define VIDEO_5C_EN PCM_PATH(EC5C.pcm)   //A
#define VIDEO_6C_EN PCM_PATH(EC6C.pcm)   //A

#define VIDEO_1F_EN PCM_PATH(EC1F.pcm)   //A
#define VIDEO_2F_EN PCM_PATH(EC2F.pcm)   //A
#define VIDEO_3F_EN PCM_PATH(EC3F.pcm)   //A
#define VIDEO_4F_EN PCM_PATH(EC4F.pcm)   //A
#define VIDEO_5F_EN PCM_PATH(EC5F.pcm)   //A

#define VIDEO_A0_EN PCM_PATH(EDA0.pcm)   //A
#define VIDEO_A1_EN PCM_PATH(EDA1.pcm)   //A
#define VIDEO_A2_EN PCM_PATH(EDA2.pcm)   //A
#define VIDEO_A3_EN PCM_PATH(EDA3.pcm)   //A
#define VIDEO_A4_EN PCM_PATH(EDA4.pcm)   //A
#define VIDEO_A5_EN PCM_PATH(EDA5.pcm)   //A
#define VIDEO_A6_EN PCM_PATH(EDA6.pcm)   //A
#define VIDEO_A7_EN PCM_PATH(EDA7.pcm)   //A
#define VIDEO_A8_EN PCM_PATH(EDA8.pcm)   //A
#define VIDEO_A9_EN PCM_PATH(EDA9.pcm)   //A

#define VIDEO_B0_EN PCM_PATH(EDB0.pcm)   //A
#define VIDEO_B1_EN PCM_PATH(EDB1.pcm)   //A
#define VIDEO_B2_EN PCM_PATH(EDB2.pcm)   //A
#define VIDEO_B3_EN PCM_PATH(EDB3.pcm)   //A
#define VIDEO_B4_EN PCM_PATH(EDB4.pcm)   //A
#define VIDEO_B5_EN PCM_PATH(EDB5.pcm)   //A
#define VIDEO_B6_EN PCM_PATH(EDB6.pcm)   //A
#define VIDEO_B7_EN PCM_PATH(EDB7.pcm)   //A
#define VIDEO_B8_EN PCM_PATH(EDB8.pcm)   //A
#define VIDEO_B9_EN PCM_PATH(EDB9.pcm)   //A

#define VIDEO_C1_EN PCM_PATH(EDC1.pcm)   //A
#define VIDEO_C2_EN PCM_PATH(EDC2.pcm)   //A
#define VIDEO_C3_EN PCM_PATH(EDC3.pcm)   //A
#define VIDEO_C4_EN PCM_PATH(EDC4.pcm)   //A

#define VIDEO_D1_EN PCM_PATH(EDD1.pcm)   //A
#define VIDEO_D2_EN PCM_PATH(EDD2.pcm)   //A
#define VIDEO_D3_EN PCM_PATH(EDD3.pcm)   //A
#define VIDEO_D4_EN PCM_PATH(EDD4.pcm)   //A


#define VIDEO_G1_EN PCM_PATH(EDG1.pcm)   //A
#define VIDEO_G2_EN PCM_PATH(EDG2.pcm)   //A
#define VIDEO_G3_EN PCM_PATH(EDG3.pcm)   //A
#define VIDEO_G4_EN PCM_PATH(EDG4.pcm)   //A
#define VIDEO_G5_EN PCM_PATH(EDG5.pcm)   //A
#define VIDEO_G6_EN PCM_PATH(EDG6.pcm)   //A
#define VIDEO_G7_EN PCM_PATH(EDG7.pcm)   //A
#define VIDEO_G8_EN PCM_PATH(EDG8.pcm)   //A
#define VIDEO_G9_EN PCM_PATH(EDG9.pcm)   //A

#define VIDEO_L1_EN PCM_PATH(EDL1.pcm)   //A
#define VIDEO_L2_EN PCM_PATH(EDL2.pcm)   //A
#define VIDEO_L3_EN PCM_PATH(EDL3.pcm)   //A

#define VIDEO_M0_EN PCM_PATH(EDM0.pcm)   //A
#define VIDEO_M1_EN PCM_PATH(EDM1.pcm)   //A
#define VIDEO_M2_EN PCM_PATH(EDM2.pcm)   //A
#define VIDEO_M3_EN PCM_PATH(EDM3.pcm)   //A
#define VIDEO_M4_EN PCM_PATH(EDM4.pcm)   //A
#define VIDEO_M5_EN PCM_PATH(EDM5.pcm)   //A
#define VIDEO_M6_EN PCM_PATH(EDM6.pcm)   //A
#define VIDEO_M7_EN PCM_PATH(EDM7.pcm)   //A
#define VIDEO_M8_EN PCM_PATH(EDM8.pcm)   //A
#define VIDEO_M9_EN PCM_PATH(EDM9.pcm)   //A

#define VIDEO_P0_EN PCM_PATH(EDP0.pcm)   //A
#define VIDEO_P1_EN PCM_PATH(EDP1.pcm)   //A
#define VIDEO_P2_EN PCM_PATH(EDP2.pcm)   //A
#define VIDEO_P3_EN PCM_PATH(EDP3.pcm)   //A
#define VIDEO_P4_EN PCM_PATH(EDP4.pcm)   //A
#define VIDEO_P5_EN PCM_PATH(EDP5.pcm)   //A
#define VIDEO_P6_EN PCM_PATH(EDP6.pcm)   //A
#define VIDEO_P7_EN PCM_PATH(EDP7.pcm)   //A
#define VIDEO_P8_EN PCM_PATH(EDP8.pcm)   //A
#define VIDEO_P9_EN PCM_PATH(EDP9.pcm)   //A

#define VIDEO_R1_EN PCM_PATH(EDR1.pcm)   //A
#define VIDEO_R2_EN PCM_PATH(EDR2.pcm)   //A
#define VIDEO_R3_EN PCM_PATH(EDR3.pcm)   //A

#define VIDEO_AF_EN PCM_PATH(EEAF.pcm)   //A
#define VIDEO_AG_EN PCM_PATH(EEAG.pcm)   //A
#define VIDEO_BE_EN PCM_PATH(EEBE.pcm)   //A
#define VIDEO_CF_EN PCM_PATH(EECF.pcm)   //A
#define VIDEO_EG_EN PCM_PATH(EEEG.pcm)   //A
#define VIDEO_GF_EN PCM_PATH(EEGF.pcm)   //A
#define VIDEO_HP_EN PCM_PATH(EEHP.pcm)   //A
#define VIDEO_KG_EN PCM_PATH(EEKG.pcm)   //A
#define VIDEO_LB_EN PCM_PATH(EELB.pcm)   //A
#define VIDEO_LD_EN PCM_PATH(EELD.pcm)   //A
#define VIDEO_LF_EN PCM_PATH(EELF.pcm)   //A
#define VIDEO_LG_EN PCM_PATH(EELG.pcm)   //A
#define VIDEO_LL_EN PCM_PATH(EELL.pcm)   //A
#define VIDEO_LP_EN PCM_PATH(EELP.pcm)   //A
#define VIDEO_MR_EN PCM_PATH(EEMR.pcm)   //A
#define VIDEO_MZ_EN PCM_PATH(EEMZ.pcm)   //A
#define VIDEO_PB_EN PCM_PATH(EEPB.pcm)   //A
#define VIDEO_PC_EN PCM_PATH(EEPC.pcm)   //A
#define VIDEO_PH_EN PCM_PATH(EEPH.pcm)   //A
#define VIDEO_PM_EN PCM_PATH(EEPM.pcm)   //A
#define VIDEO_RF_EN PCM_PATH(EERF.pcm)   //A
#define VIDEO_UB_EN PCM_PATH(EEUB.pcm)   //A
#define VIDEO_UF_EN PCM_PATH(EEUF.pcm)   //A
#define VIDEO_UG_EN PCM_PATH(EEUG.pcm)   //A
#define VIDEO_UPP_EN PCM_PATH(EEUP.pcm)   //A

#define VIDEO_12A_EN PCM_PATH(EF12A.pcm)   //A
#define VIDEO_12B_EN PCM_PATH(EF12B.pcm)   //A
#define VIDEO_13A_EN PCM_PATH(EF13A.pcm)   //A
#define VIDEO_13B_EN PCM_PATH(EF13B.pcm)   //A
#define VIDEO_14A_EN PCM_PATH(EF14A.pcm)   //A
#define VIDEO_14B_EN PCM_PATH(EF14B.pcm)   //A
#define VIDEO_15A_EN PCM_PATH(EF15A.pcm)   //A
#define VIDEO_15B_EN PCM_PATH(EF15B.pcm)   //A
#define VIDEO_17A_EN PCM_PATH(EF17A.pcm)   //A
#define VIDEO_17B_EN PCM_PATH(EF17B.pcm)   //A
#define VIDEO_18A_EN PCM_PATH(EF18A.pcm)   //A
#define VIDEO_18B_EN PCM_PATH(EF18B.pcm)   //A
#define VIDEO_23A_EN PCM_PATH(EF23A.pcm)   //A
#define VIDEO_23B_EN PCM_PATH(EF23B.pcm)   //A
#define VIDEO_33A_EN PCM_PATH(EF33A.pcm)   //A
#define VIDEO_33B_EN PCM_PATH(EF33B.pcm)   //A

void elevator_play_thread_entry(void *parameter);



#endif /* __ELEVATOR_PLAY_H__ */
