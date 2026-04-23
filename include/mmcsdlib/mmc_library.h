#include "mmcsdlib/mmc_proto.h"
#include "mmcsd/mmcsd.h"

#define MMC_SOFTRESET (0x1 << 1)

#define MMC_CMD_GO_IDLE_STATE 0
#define MMC_RSP_NONE (0)

#define SD_CMD_SEND_IF_COND 8
#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_CRC (1 << 2)
#define MMC_RSP_OPCODE (1 << 4)
#define MMC_RSP_R7 (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

#define MMC_CMD_APP_CMD 55
#define MMC_RSP_R1 (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define SD_CMD_APP_SEND_OP_COND 41
#define MMC_RSP_R3 (MMC_RSP_PRESENT)
#define OCR_HCS 0x40000000
#define OCR_BUSY 0x80000000

#define MMC_RSP_136 (1 << 1)
#define MMC_RSP_R2 (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)

#define MMC_RSP_R6 (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

#define MMC_RSP_R1b	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE| \
			MMC_RSP_BUSY)
unsigned int HSMMCSDCardPresent(unsigned int regBase);
unsigned int SDControllerInit(unsigned int base);
unsigned int SDMMCgoIdle();
unsigned int mmc_init_to_transfer();