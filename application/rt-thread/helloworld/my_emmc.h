#ifndef _MY_EMMC_H_
#define _MY_EMMC_H_

#include <stdint.h>
#include <rtthread.h>

/* Forward declaration for RT-Thread MMC host */
struct rt_mmcsd_host;

/* 获取 MMC host 指针 */
struct rt_mmcsd_host *aic_sdmc_get_host(int id);

/* FFU 私有命令宏 */
#define MMC_CMD_GEN_PRIVATE     56
#define FFU_OP_QUERY_VER        0xB0
#define FFU_OP_UPDATE_FW        0xB2
#define FFU_BLOCK_SIZE          512
#define FFU_BUFF_32BYTE_CNT     8

/* 互斥锁初始化 */
void sdmc_bus_mutex_init(void);

/* 全局升级标记 */
extern uint8_t g_ffu_upgrading_flag;

/* FFU 核心接口 - 使用 rt_int32_t 替代 s32 */
rt_int32_t ffu_get_version_state(struct rt_mmcsd_host *host, uint32_t *out_v1, uint32_t *out_ffu_state);
rt_int32_t ffu_firmware_upgrade(struct rt_mmcsd_host *host, uint8_t *fw_bin, uint32_t fw_len);
rt_int32_t my_emmc_ffu_upgrade_test(int mmc_id, uint8_t *fw_bin, uint32_t fw_size);
void emmc_thread_entry(void *parameter);

#endif /* _MY_EMMC_H_ */
