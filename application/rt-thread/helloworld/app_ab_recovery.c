/*
 * Copyright (c) 2023-2026, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_ab_recovery.h"
#include <rtconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aic_reboot_reason.h>
#include "wdt.h"

#ifdef AIC_ENV_INTERFACE
#include <env.h>
#endif

#ifdef RT_USING_POSIX_FS
#include <dfs_posix.h>
#endif

extern void (*trap_c_callback)(void);

/* CPU Exception (HardFault, Illegal Instruction, Memory Fault) 回调 */
static void app_cpu_exception_callback(void)
{
    rt_kprintf("\n======================================================\n");
    rt_kprintf(" [FATAL] CPU Exception occurred in current application!\n");
    rt_kprintf(" [AB-RECOVERY] Setting REBOOT_REASON_PANIC to fallback to backup partition...\n");
    rt_kprintf("======================================================\n\n");

    /* 1. 设置芯片常开域硬件寄存器，标记系统因Panic崩溃重启 */
    aic_set_reboot_reason(REBOOT_REASON_PANIC);

    /* 2. 简短延时确保串口打印完成 */
    for (volatile int i = 0; i < 3000000; i++) {
        __asm__ volatile ("nop");
    }

    /* 3. 立即触发硬件看门狗复位 */
    wdt_immediate_reset();
}

#ifdef RT_DEBUG
/* RT-Thread 断言失败回调 */
static void app_assert_callback(const char *ex, const char *func, rt_size_t line)
{
    rt_kprintf("\n======================================================\n");
    rt_kprintf(" [FATAL] RT_ASSERT FAILED: %s in %s:%d\n", ex, func, (int)line);
    rt_kprintf(" [AB-RECOVERY] Setting REBOOT_REASON_PANIC to fallback to backup partition...\n");
    rt_kprintf("======================================================\n\n");

    aic_set_reboot_reason(REBOOT_REASON_PANIC);

    for (volatile int i = 0; i < 3000000; i++) {
        __asm__ volatile ("nop");
    }

    wdt_immediate_reset();
}
#endif

/* 初始化崩溃捕获机制 */
void app_ab_recovery_init(void)
{
    /* 注册 CPU Exception 钩子 */
    trap_c_callback = app_cpu_exception_callback;

#ifdef RT_DEBUG
    /* 注册 RT-Thread 断言钩子 */
    rt_assert_set_hook(app_assert_callback);
#endif

    rt_kprintf("[AB-RECOVERY] Crash detection hooks registered successfully.\n");
}

/* 应用程序健康启动确认（清除异常启动计数及未决升级状态） */
void app_ab_boot_success(void)
{
#ifdef RT_USING_POSIX_FS
    /* 检查 /rodata 分区是否成功挂载且可正常访问 */
    DIR *dir = opendir("/rodata");
    if (!dir) {
        rt_kprintf("\n[AB-RECOVERY] CRITICAL: /rodata directory cannot be accessed! (Temperature drop / flash corrupt?)\n");
        rt_kprintf("[AB-RECOVERY] Triggering emergency auto-fallback to backup partition...\n");
        aic_set_reboot_reason(REBOOT_REASON_PANIC);
        for (volatile int i = 0; i < 3000000; i++) {
            __asm__ volatile ("nop");
        }
        wdt_immediate_reset();
        return;
    }
    closedir(dir);
#endif

#ifdef AIC_ENV_INTERFACE
    if (fw_env_open() == 0) {
        char *str = fw_getenv("bootcount");
        int count = str ? strtol(str, NULL, 10) : 0;
        char *upg = fw_getenv("upgrade_available");
        char *now = fw_getenv("osAB_now");
        char *ro_now = fw_getenv("rodataAB_now");

        rt_kprintf("[AB-RECOVERY] Currently running on Partition: OS=[%s], Rodata=[%s]\n",
                   now ? now : "Unknown", ro_now ? ro_now : "Unknown");

        /* 若存在异常启动计数或待确认升级标志，清零确认 */
        if (count > 0 || (upg && strncmp(upg, "1", 1) == 0)) {
            rt_kprintf("[AB-RECOVERY] Clearing bootcount (%d -> 0) & upgrade_available...\n", count);
            fw_env_write("bootcount", "0");
            fw_env_write("upgrade_available", "0");
            fw_env_flush();
        }
        fw_env_close();
    }
#else
    rt_kprintf("[AB-RECOVERY] System booted successfully.\n");
#endif
}

/* Finsh 命令：查看当前 AB 区状态 */
void app_ab_show_status(void)
{
#ifdef AIC_ENV_INTERFACE
    if (fw_env_open() == 0) {
        char *now = fw_getenv("osAB_now");
        char *next = fw_getenv("osAB_next");
        char *ro_now = fw_getenv("rodataAB_now");
        char *ro_next = fw_getenv("rodataAB_next");
        char *count = fw_getenv("bootcount");
        char *limit = fw_getenv("bootlimit");
        char *upg = fw_getenv("upgrade_available");

        rt_kprintf("\n========== [A/B System Status] ==========\n");
        rt_kprintf(" Current running OS partition (osAB_now)     : %s\n", now ? now : "N/A");
        rt_kprintf(" Next startup OS partition    (osAB_next)    : %s\n", next ? next : "N/A");
        rt_kprintf(" Current running Rodata       (rodataAB_now) : %s\n", ro_now ? ro_now : "N/A");
        rt_kprintf(" Next mount Rodata            (rodataAB_next): %s\n", ro_next ? ro_next : "N/A");
        rt_kprintf(" Boot failure count           (bootcount)    : %s\n", count ? count : "0");
        rt_kprintf(" Boot failure limit           (bootlimit)    : %s\n", limit ? limit : "N/A");
        rt_kprintf(" Upgrade available flag                      : %s\n", upg ? upg : "0");
        rt_kprintf("=========================================\n\n");

        fw_env_close();
    } else {
        rt_kprintf("[AB-RECOVERY] Failed to open env!\n");
    }
#else
    rt_kprintf("[AB-RECOVERY] ENV interface not compiled in application.\n");
#endif
}
MSH_CMD_EXPORT_ALIAS(app_ab_show_status, ab_status, Show current A/B partition and boot status);

/* Finsh 命令：手动切换下次启动分区 (A <-> B) 并重启 */
void app_ab_switch(void)
{
#ifdef AIC_ENV_INTERFACE
    if (fw_env_open() == 0) {
        char *next_os = fw_getenv("osAB_next");
        char *next_ro = fw_getenv("rodataAB_next");
        char new_os = (next_os && (next_os[0] == 'A' || next_os[0] == 'a')) ? 'B' : 'A';
        char new_ro = (next_ro && (next_ro[0] == 'A' || next_ro[0] == 'a')) ? 'B' : 'A';

        rt_kprintf("[AB-RECOVERY] Switching osAB_next (%s -> %c), rodataAB_next (%s -> %c)...\n",
                   next_os ? next_os : "A", new_os, next_ro ? next_ro : "A", new_ro);
        fw_env_write("osAB_next", new_os == 'A' ? "A" : "B");
        fw_env_write("rodataAB_next", new_ro == 'A' ? "A" : "B");
        fw_env_write("bootcount", "0");
        fw_env_flush();
        fw_env_close();

        rt_kprintf("[AB-RECOVERY] Resetting system now...\n");
        rt_thread_mdelay(200);
        wdt_immediate_reset();
    } else {
        rt_kprintf("[AB-RECOVERY] Failed to open env for switching!\n");
    }
#else
    rt_kprintf("[AB-RECOVERY] ENV interface not compiled in application.\n");
#endif
}
MSH_CMD_EXPORT_ALIAS(app_ab_switch, ab_switch, Switch startup partition between A and B and reboot);

/* Finsh 命令：故意触发崩溃测试 */
void app_ab_test_crash(void)
{
    rt_kprintf("\n[AB-TEST] Deliberately triggering memory crash (illegal pointer write)...\n");
    rt_thread_mdelay(100);

    /* 触发非法地址写入（CPU Exception: NO.7 Store/AMO Access Fault） */
    volatile uint32_t *bad_ptr = (volatile uint32_t *)0x00000000;
    *bad_ptr = 0xDEADBEEF;
}
MSH_CMD_EXPORT_ALIAS(app_ab_test_crash, ab_test_crash, Deliberately trigger crash to test auto-fallback to B);
