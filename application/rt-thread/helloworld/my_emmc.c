/*
 * Copyright (c) 2022-2025, Artinchip Technology Co., Ltd
 * FFU Firmware Upgrade Module Host + AX317/AA2703/AA2705 Target Only
 * SPDX-License-Identifier: Apache-2.0
 * Fix: SDMC1不存在，切回SDMC0；保留400K低速+CMD56空数据段修复
 */
#include "my_emmc.h"
#include <stdio.h>
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/mmcsd_core.h>
#include <drivers/mmcsd_card.h>

/* FFU Appotech Private Macro (AX317/AA2703/AA2705 Universal) */
#define MMC_CMD_GEN_PRIVATE     56
#define FFU_OP_QUERY_VER        0xB0    // CMD56: Query FW Version & FFU State
#define FFU_OP_ENTER_TRANSFER   0xB1    // CMD56: Enter transfer mode state=3
#define FFU_OP_UPDATE_FW        0xB2    // CMD56: Flash solidify command
#define FFU_BLOCK_SIZE          512
#define FFU_BUFF_32BYTE_CNT     0x08    // 32Byte = 8 * uint32 checksum buffer
#define FFU_MUTEX_WAIT_TICK     (RT_TICK_PER_SECOND * 2)
#define FFU_SHORT_DELAY_MS      200
#define FFU_LONG_DELAY_MS       500
#define FFU_PROGRESS_WAIT_MS    10000   // Doc mandatory 10s wait after 0xB2
#define FFU_CARD_RESET_DELAY    800
#define FFU_SLOW_CLOCK_HZ       400000  // 400K slow clock for AX317 slave

/* Log switch: set 0 to disable mass production print */
#define FFU_LOG_ENABLE          1

/* RT-Thread MMCSD Response Flag Definition */
#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_CRC     (1 << 2)
#define RESP_R1         (MMC_RSP_PRESENT | MMC_RSP_CRC)
#define RESP_NONE       0

/* Log Wrapper */
#if FFU_LOG_ENABLE
#define pr_info(fmt, ...)  printf("[FFU_INFO] "fmt"\r\n", ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf("[FFU_ERR] "fmt"\r\n", ##__VA_ARGS__)
#define pr_warn(fmt, ...)  printf("[FFU_WARN] "fmt"\r\n", ##__VA_ARGS__)
#else
#define pr_info(fmt, ...)
#define pr_err(fmt, ...)
#define pr_warn(fmt, ...)
#endif
static rt_int32_t ffu_read_single_blk(struct rt_mmcsd_host *host, uint32_t blk_no, uint8_t *buf);
/* Global SD Bus Mutex & Upgrade Running Flag */
static rt_mutex_t g_sdmc_bus_mutex = RT_NULL;
uint8_t g_ffu_upgrading_flag = 0;

/** SD Bus Mutex Init */
void sdmc_bus_mutex_init(void)
{
    if (g_sdmc_bus_mutex != RT_NULL)
    {
        pr_info("sdmc mutex already created\n");
        return;
    }
    g_sdmc_bus_mutex = rt_mutex_create("sdmc_mutex", RT_IPC_FLAG_PRIO);
    if (g_sdmc_bus_mutex == RT_NULL)
    {
        pr_err("create sdmc mutex failed\n");
    }
}

/**
 * FFU Standard 32Byte XOR Checksum (Doc 5 Spec)
 */
static uint32_t CMDCheckSum(uint32_t *Buff)
{
    if (Buff == RT_NULL)
    {
        pr_err("CMDCheckSum input buffer NULL!\n");
        return 0xFFFFFFFF;
    }
    uint32_t sum = 0;
    for (int i = 0; i < FFU_BUFF_32BYTE_CNT; i++)
    {
        sum ^= Buff[i];
    }
    return sum;
}

/**
 * Full SD Card Init CMD0+CMD8+ACMD41, force 400K slow clock
 */
static rt_int32_t ffu_card_reset(struct rt_mmcsd_host *host)
{
    if (host == RT_NULL)
    {
        pr_err("ffu_card_reset host handle NULL\n");
        return -1;
    }

    mmcsd_set_clock(host, FFU_SLOW_CLOCK_HZ);
    rt_thread_mdelay(FFU_SHORT_DELAY_MS);

    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_req req;
    memset(&cmd, 0, sizeof(cmd));
    memset(&req, 0, sizeof(req));

    // CMD0
    cmd.cmd_code = 0;
    cmd.arg = 0;
    cmd.flags = RESP_NONE;
    cmd.retries = 3;
    req.cmd = &cmd;
    req.data = RT_NULL;
    mmcsd_send_request(host, &req);
    rt_thread_mdelay(FFU_LONG_DELAY_MS);

    // CMD8
    memset(&cmd, 0, sizeof(cmd));
    cmd.cmd_code = 8;
    cmd.arg = 0x000001AA;
    cmd.flags = RESP_R1;
    cmd.retries = 2;
    req.cmd = &cmd;
    mmcsd_send_request(host, &req);
    rt_thread_mdelay(FFU_LONG_DELAY_MS);

    // CMD55
    memset(&cmd, 0, sizeof(cmd));
    cmd.cmd_code = 55;
    cmd.arg = 0;
    cmd.flags = RESP_R1;
    req.cmd = &cmd;
    mmcsd_send_request(host, &req);
    rt_thread_mdelay(FFU_SHORT_DELAY_MS);

    // ACMD41
    memset(&cmd, 0, sizeof(cmd));
    cmd.cmd_code = 41;
    cmd.arg = 0x40000000;
    cmd.flags = RESP_R1;
    req.cmd = &cmd;
    mmcsd_send_request(host, &req);
    rt_thread_mdelay(FFU_CARD_RESET_DELAY);

    // ==========新增标准CMD17调试==========
    uint8_t test_buf[FFU_BLOCK_SIZE] = {0};
    rt_int32_t test_ret = ffu_read_single_blk(host, 0, test_buf);
    pr_info("[DEBUG] STD CMD17 test ret = %d", test_ret);
    // =====================================

    if (host->card == RT_NULL)
    {
        pr_err("ffu_card_reset: No SD card recognized after init\n");
        return -3;
    }
    return 0;
}
/**
 * Send CMD56 Private Command
 * Fix: add 512byte dummy data block, AX317不允许data=NULL
 */
static rt_int32_t ffu_send_cmd56(struct rt_mmcsd_host *host, uint8_t op_code, uint32_t *buf32)
{
    if (host == RT_NULL || buf32 == RT_NULL)
    {
        pr_err("ffu_send_cmd56 invalid param\n");
        return -1;
    }
    if (host->card == RT_NULL)
    {
        pr_err("ffu_send_cmd56: No SD card on host\n");
        return -3;
    }

    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_data data;
    uint8_t dummy_buf[FFU_BLOCK_SIZE] = {0}; // 强制附加空512数据块
    uint32_t chk = CMDCheckSum(buf32);
    uint32_t chk_24 = chk & 0x00FFFFFF;

    memset(&cmd, 0, sizeof(cmd));
    memset(&req, 0, sizeof(req));
    memset(&data, 0, sizeof(data));

    cmd.cmd_code = MMC_CMD_GEN_PRIVATE;
    cmd.arg = ((uint32_t)op_code << 24) | chk_24;
    cmd.flags = RESP_R1;
    cmd.retries = 5; // 最大重试5次

    // 必须携带数据段
    data.blksize = FFU_BLOCK_SIZE;
    data.blks = 1;
    data.buf = dummy_buf;
    data.flags = DATA_DIR_READ;
    req.cmd = &cmd;
    req.data = &data;

    pr_info("CMD56 op=0x%02X arg=0x%08X chk=0x%06X", op_code, cmd.arg, chk_24);
    rt_thread_mdelay(FFU_SHORT_DELAY_MS);
    mmcsd_send_request(host, &req);
    rt_thread_mdelay(FFU_LONG_DELAY_MS);

    if (cmd.err != 0)
    {
        pr_err("CMD56 send fail err=%d resp0=0x%08X\n", cmd.err, cmd.resp[0]);
        return cmd.err;
    }
    return 0;
}

/** CMD17 Read Single Block */
static rt_int32_t ffu_read_single_blk(struct rt_mmcsd_host *host, uint32_t blk_no, uint8_t *buf)
{
    if (host == RT_NULL || buf == RT_NULL)
    {
        pr_err("ffu_read_single_blk invalid param\n");
        return -1;
    }
    if (host->card == RT_NULL)
    {
        pr_err("ffu_read_single_blk: No SD card\n");
        return -3;
    }

    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_data data;
    memset(&cmd, 0, sizeof(cmd));
    memset(&req, 0, sizeof(req));
    memset(&data, 0, sizeof(data));

    cmd.cmd_code = 17;
    cmd.arg = blk_no;
    cmd.flags = RESP_R1;
    cmd.retries = 3;
    data.blksize = FFU_BLOCK_SIZE;
    data.blks = 1;
    data.buf = buf;
    data.flags = DATA_DIR_READ;
    req.cmd = &cmd;
    req.data = &data;

    mmcsd_send_request(host, &req);
    rt_thread_mdelay(FFU_SHORT_DELAY_MS);
    if (cmd.err != 0 || data.err != 0)
    {
        pr_err("CMD17 blk%d err cmd=%d data=%d\n", blk_no, cmd.err, data.err);
        return -1;
    }
    return 0;
}

/** Query FW Version & FFU State (Doc 4.2 Step1) */
rt_int32_t ffu_get_version_state(struct rt_mmcsd_host *host, uint32_t *out_v1, uint32_t *out_ffu_state)
{
    if (host == RT_NULL || out_v1 == RT_NULL || out_ffu_state == RT_NULL)
    {
        pr_err("ffu_get_version_state param null\n");
        return -1;
    }
    uint8_t stat_buf[FFU_BLOCK_SIZE] = {0};
    uint32_t buf32[FFU_BUFF_32BYTE_CNT] = {
        0x12345678, 0x87654321, 0x11223344, 0x44332211,
        0xAABBCCDD, 0xDDCCBBAA, 0x55667788, 0x88776655
    };
    rt_int32_t ret;

    ret = ffu_card_reset(host);
    if (ret != 0)
        return ret;

    ret = ffu_send_cmd56(host, FFU_OP_QUERY_VER, buf32);
    if (ret != 0)
    {
        pr_err("CMD56 0xB0 query version fail\n");
        return ret;
    }

    ret = ffu_read_single_blk(host, 0, stat_buf);
    if (ret != 0)
    {
        pr_err("Read status block 0 failed\n");
        return -1;
    }

    *out_v1 = *(uint32_t *)&stat_buf[0];
    *out_ffu_state = *(uint32_t *)&stat_buf[4];
    pr_info("Read FW_VER=0x%08X FFU_STATE=%d\n", *out_v1, *out_ffu_state);
    return 0;
}

/** CMD24 Write Single Block */
static rt_int32_t ffu_write_single_blk(struct rt_mmcsd_host *host, uint32_t blk_no, uint8_t *buf)
{
    if (host == RT_NULL || buf == RT_NULL)
    {
        pr_err("ffu_write_single_blk invalid param\n");
        return -1;
    }
    if (host->card == RT_NULL)
    {
        pr_err("ffu_write_single_blk: No SD card\n");
        return -3;
    }

    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_data data;
    memset(&cmd, 0, sizeof(cmd));
    memset(&req, 0, sizeof(req));
    memset(&data, 0, sizeof(data));

    cmd.cmd_code = 24;
    cmd.arg = blk_no;
    cmd.flags = RESP_R1;
    cmd.retries = 3;
    data.blksize = FFU_BLOCK_SIZE;
    data.blks = 1;
    data.buf = buf;
    data.flags = DATA_DIR_WRITE;
    req.cmd = &cmd;
    req.data = &data;

    mmcsd_send_request(host, &req);
    rt_thread_mdelay(FFU_SHORT_DELAY_MS);
    if (cmd.err != 0 || data.err != 0)
    {
        pr_err("CMD24 blk%d err cmd=%d data=%d\n", blk_no, cmd.err, data.err);
        return -1;
    }
    return 0;
}

/** Write + Readback Verify Block */
static rt_int32_t ffu_write_verify_pkt(struct rt_mmcsd_host *host, uint32_t blk_idx, uint8_t *pkt_buf)
{
    uint8_t read_back[FFU_BLOCK_SIZE] = {0};
    rt_int32_t ret;
    int retry_cnt = 0;
    const int MAX_RETRY = 3;

    while (retry_cnt < MAX_RETRY)
    {
        ret = ffu_write_single_blk(host, blk_idx, pkt_buf);
        if (ret != 0)
        {
            retry_cnt++;
            pr_warn("Blk %d write fail retry %d/%d\n", blk_idx, retry_cnt, MAX_RETRY);
            rt_thread_mdelay(FFU_LONG_DELAY_MS);
            continue;
        }
        ret = ffu_read_single_blk(host, blk_idx, read_back);
        if (ret != 0)
        {
            retry_cnt++;
            pr_warn("Blk %d readback fail retry %d/%d\n", blk_idx, retry_cnt, MAX_RETRY);
            rt_thread_mdelay(FFU_LONG_DELAY_MS);
            continue;
        }
        if (memcmp(pkt_buf, read_back, FFU_BLOCK_SIZE) == 0)
        {
            return 0;
        }
        retry_cnt++;
        pr_warn("Blk %d data mismatch retry %d/%d\n", blk_idx, retry_cnt, MAX_RETRY);
        rt_thread_mdelay(FFU_LONG_DELAY_MS);
    }
    pr_err("Blk %d max retry reach write verify failed\n", blk_idx);
    return -1;
}

/** Full FFU Upgrade Main Flow (Strict Doc4.2) */
rt_int32_t ffu_firmware_upgrade(struct rt_mmcsd_host *host, uint8_t *fw_bin, uint32_t fw_len)
{
    uint32_t v1, v2, ffu_state;
    uint32_t total_blk = 0;
    uint32_t blk_idx = 0;
    uint32_t buf32_transfer[FFU_BUFF_32BYTE_CNT] = {0};
    uint8_t *align_fw_buf = RT_NULL;
    rt_int32_t ret = 0;
    uint8_t pkt_buf[FFU_BLOCK_SIZE];

    ret = ffu_get_version_state(host, &v1, &ffu_state);
    if (ret != 0)
        goto exit_release;
    if (ffu_state != 0)
    {
        pr_err("FFU busy state=%d, power cycle AX317 to reset\n", ffu_state);
        ret = -10;
        goto exit_release;
    }

    // Align firmware to 512 block
    if ((fw_len % FFU_BLOCK_SIZE) != 0)
    {
        uint32_t align_total_len = ((fw_len / FFU_BLOCK_SIZE) + 1) * FFU_BLOCK_SIZE;
        align_fw_buf = rt_malloc(align_total_len);
        if (align_fw_buf == RT_NULL)
        {
            pr_err("malloc align fw buffer fail\n");
            ret = -13;
            goto exit_release;
        }
        memset(align_fw_buf, 0xFF, align_total_len);
        memcpy(align_fw_buf, fw_bin, fw_len);
        total_blk = align_total_len / FFU_BLOCK_SIZE;
        pr_info("FW auto align orig_len=%d align_len=%d total_blk=%d\n", fw_len, align_total_len, total_blk);
    }
    else
    {
        align_fw_buf = fw_bin;
        total_blk = fw_len / FFU_BLOCK_SIZE;
        pr_info("FW aligned len=%d total_blk=%d\n", fw_len, total_blk);
    }

    // CMD56 0xB1 Enter Transfer Mode, fill total block count
    buf32_transfer[0] = total_blk;
    buf32_transfer[1] = 0x00000001;
    buf32_transfer[2] = 0x00000002;
    buf32_transfer[3] = 0x00000004;
    buf32_transfer[4] = 0x00000008;
    buf32_transfer[5] = 0x00000010;
    buf32_transfer[6] = 0x00000020;
    buf32_transfer[7] = 0x00000040;
    ret = ffu_send_cmd56(host, FFU_OP_ENTER_TRANSFER, buf32_transfer);
    if (ret != 0)
    {
        pr_err("Send enter transfer cmd 0xB1 fail\n");
        goto exit_release;
    }
    rt_thread_mdelay(FFU_LONG_DELAY_MS);

    ret = ffu_get_version_state(host, &v1, &ffu_state);
    if (ffu_state != 3)
    {
        pr_err("Enter transfer mode fail state=%d expect=3\n", ffu_state);
        ret = -11;
        goto exit_release;
    }
    pr_info("AX317 transfer mode ready, start write firmware blocks\n");

    // Write all blocks with verify
    while (blk_idx < total_blk)
    {
        memcpy(pkt_buf, align_fw_buf + blk_idx * FFU_BLOCK_SIZE, FFU_BLOCK_SIZE);
        ret = ffu_write_verify_pkt(host, blk_idx, pkt_buf);
        if (ret < 0)
        {
            pr_err("Write blk %d fatal error\n", blk_idx);
            goto exit_release;
        }
        blk_idx++;
        if ((blk_idx % 10) == 0)
            pr_info("Write progress: %d/%d blocks\n", blk_idx, total_blk);
    }
    pr_info("All firmware blocks write complete\n");

    // CMD56 0xB2 Solidify firmware
    uint32_t buf32_update[FFU_BUFF_32BYTE_CNT] = {
        0x98765432, 0x23456789, 0x99887766, 0x66778899,
        0x1A2B3C4D, 0x4D3C2B1A, 0x778899AA, 0xAA998877
    };
    ret = ffu_send_cmd56(host, FFU_OP_UPDATE_FW, buf32_update);
    if (ret != 0)
    {
        pr_err("Send update firmware cmd 0xB2 fail\n");
        goto exit_release;
    }

    pr_info("Wait 10s for AX317 flash programming...\n");
    rt_thread_mdelay(FFU_PROGRESS_WAIT_MS);

    // Soft reset SD
    ffu_card_reset(host);
    rt_thread_mdelay(FFU_LONG_DELAY_MS);

    // Read new version v2
    ret = ffu_get_version_state(host, &v2, &ffu_state);
    if (ret != 0)
    {
        pr_err("Query new version after upgrade failed\n");
        goto exit_release;
    }

    if (v2 > v1)
    {
        pr_info("======== FFU UPGRADE SUCCESS ========\nOld Ver=0x%08X New Ver=0x%08X\n", v1, v2);
        ret = 0;
    }
    else
    {
        pr_err("======== FFU UPGRADE FAILED ========\nNew version not newer old=0x%08X new=0x%08X\n", v1, v2);
        ret = -20;
    }

exit_release:
    if (align_fw_buf != RT_NULL && align_fw_buf != fw_bin)
    {
        rt_free(align_fw_buf);
        align_fw_buf = RT_NULL;
    }
    return ret;
}

/** External Application API */
rt_int32_t my_emmc_ffu_upgrade_test(int mmc_id, uint8_t *fw_bin, uint32_t fw_size)
{
    struct rt_mmcsd_host *host = RT_NULL;
    rt_int32_t ret;

    if (g_ffu_upgrading_flag == 1)
    {
        pr_err("FFU upgrading now, reject new request\n");
        return -30;
    }

    host = aic_sdmc_get_host(mmc_id);
    if (!host)
    {
        pr_err("SDMC%d host handle unavailable\n", mmc_id);
        return -1;
    }
    if (fw_bin == RT_NULL || fw_size == 0)
    {
        pr_err("Firmware bin buffer empty\n");
        return -3;
    }

    ret = rt_mutex_take(g_sdmc_bus_mutex, FFU_MUTEX_WAIT_TICK);
    if (ret != RT_EOK)
    {
        pr_err("Obtain sdmc bus mutex timeout\n");
        return -4;
    }
    g_ffu_upgrading_flag = 1;

    ret = ffu_firmware_upgrade(host, fw_bin, fw_size);

    g_ffu_upgrading_flag = 0;
    rt_mutex_release(g_sdmc_bus_mutex);
    pr_info("SDMC bus mutex released\n");
    return ret;
}

/** Background Upgrade Thread Entry (切回SDMC0) */
void emmc_thread_entry(void *parameter)
{
    struct rt_mmcsd_host *host = RT_NULL;
    uint32_t fw_ver_old, ffu_state;
    rt_int32_t ret;
#ifdef FFU_USE_EXTERNAL_FW
    extern uint8_t target_fw_bin[];
    extern uint32_t target_fw_len;
#endif

    rt_thread_mdelay(1000);
    pr_info("EMMC FFU background thread start\n");

    ret = rt_mutex_take(g_sdmc_bus_mutex, FFU_MUTEX_WAIT_TICK);
    if (ret != RT_EOK)
    {
        pr_err("Obtain sdmc mutex timeout, thread exit\n");
        goto thread_exit;
    }
    g_ffu_upgrading_flag = 1;

    // 修复：恢复SDMC0，系统仅存在SDMC0控制器
    host = aic_sdmc_get_host(0);
    if (!host)
    {
        pr_err("SDMC0 host unavailable\n");
        goto release_mutex;
    }

    ret = ffu_get_version_state(host, &fw_ver_old, &ffu_state);
    if (ret != 0)
    {
        pr_err("Query FFU initial state err:%d\n", ret);
        goto release_mutex;
    }
    pr_info("AX317 Old FW Ver:0x%08X FFU State:%d\n", fw_ver_old, ffu_state);
    if (ffu_state != 0)
    {
        pr_err("FFU state busy, power cycle AX317 chip\n");
        goto release_mutex;
    }

#ifdef FFU_USE_EXTERNAL_FW
    ret = ffu_firmware_upgrade(host, target_fw_bin, target_fw_len);
    if (ret == 0)
        pr_info("==== Background FFU Upgrade SUCCESS ====\n");
    else
        pr_err("==== Background FFU Upgrade FAILED err:%d ====\n");
#else
    pr_info("FFU demo skip upgrade, define FFU_USE_EXTERNAL_FW macro to enable firmware bin\n");
#endif

release_mutex:
    g_ffu_upgrading_flag = 0;
    rt_mutex_release(g_sdmc_bus_mutex);
    pr_info("SDMC bus mutex released\n");
thread_exit:
    pr_info("EMMC FFU background thread exit, do not self delete thread\n");
}

/* 调试测试接口，控制台调用 test_cmd56(0) */
void test_cmd56(int sdmc_id)
{
    struct rt_mmcsd_host *host = aic_sdmc_get_host(sdmc_id);
    if (!host)
    {
        pr_err("test_cmd56: sdmc%d host null\n", sdmc_id);
        return;
    }
    uint32_t test_buf[8] = {0x12345678,0x87654321,0x11223344,0x44332211,0xAABBCCDD,0xDDCCBBAA,0x55667788,0x88776655};
    ffu_card_reset(host);
    ffu_send_cmd56(host, FFU_OP_QUERY_VER, test_buf);
}
