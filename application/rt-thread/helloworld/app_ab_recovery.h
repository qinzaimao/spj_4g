/*
 * Copyright (c) 2023-2026, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __APP_AB_RECOVERY_H__
#define __APP_AB_RECOVERY_H__

#include <rtthread.h>
#include <aic_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 初始化崩溃捕获与AB区保护机制 */
void app_ab_recovery_init(void);

/* 应用程序健康启动确认（清除异常启动计数） */
void app_ab_boot_success(void);

/* 手动切换当前运行分区并重启 */
void app_ab_switch(void);

/* 查看当前AB区状态信息 */
void app_ab_show_status(void);

/* 人工触发崩溃测试（用于验证自动回退到B区） */
void app_ab_test_crash(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_AB_RECOVERY_H__ */
