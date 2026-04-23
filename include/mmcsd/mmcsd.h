/**
 *  \file   hs_mmcsd.h
 *
 *  \brief  HS MMC/SD APIs and macros.
 *
 *   This file contains the driver API prototypes and macro definitions.
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __HS_MMCSD_H__
#define __HS_MMCSD_H__

#include "hw_hs_mmcsd.h"
#include "hw_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*
** Macros that can be passed to configure Standby mode
*/
#define HS_MMCSD_STANDBY_FORCE      (MMCHS_SYSCONFIG_STANDBYMODE_FORCE  << MMCHS_SYSCONFIG_STANDBYMODE_SHIFT)
#define HS_MMCSD_STANDBY_NONE       (MMCHS_SYSCONFIG_STANDBYMODE_NOIDLE << MMCHS_SYSCONFIG_STANDBYMODE_SHIFT)
#define HS_MMCSD_STANDBY_SMART      (MMCHS_SYSCONFIG_STANDBYMODE_SMART  << MMCHS_SYSCONFIG_STANDBYMODE_SHIFT)
#define HS_MMCSD_STANDBY_SMARTWAKE  (MMCHS_SYSCONFIG_STANDBYMODE_SMARTWAKE << MMCHS_SYSCONFIG_STANDBYMODE_SHIFT)

/*
** Macros that can be passed to configure clock activity during wake up period
*/
#define HS_MMCSD_CLOCK_OFF          (MMCHS_SYSCONFIG_CLOCKACTIVITY_NONE << MMCHS_SYSCONFIG_CLOCKACTIVITY_SHIFT)
#define HS_MMCSD_FCLK_OFF           (MMCHS_SYSCONFIG_CLOCKACTIVITY_OCP << MMCHS_SYSCONFIG_CLOCKACTIVITY_SHIFT)
#define HS_MMCSD_ICLK_OFF           (MMCHS_SYSCONFIG_CLOCKACTIVITY_FUNC << MMCHS_SYSCONFIG_CLOCKACTIVITY_SHIFT)
#define HS_MMCSD_CLOCK_ON           (MMCHS_SYSCONFIG_CLOCKACTIVITY_BOTH << MMCHS_SYSCONFIG_CLOCKACTIVITY_SHIFT)

/*
** Macros that can be passed to configure idle modes
*/
#define HS_MMCSD_SMARTIDLE_FORCE     (MMCHS_SYSCONFIG_SIDLEMODE_FORCE << MMCHS_SYSCONFIG_SIDLEMODE_SHIFT)
#define HS_MMCSD_SMARTIDLE_NONE      (MMCHS_SYSCONFIG_SIDLEMODE_NOIDLE << MMCHS_SYSCONFIG_SIDLEMODE_SHIFT)
#define HS_MMCSD_SMARTIDLE_SMART     (MMCHS_SYSCONFIG_SIDLEMODE_SMART << MMCHS_SYSCONFIG_SIDLEMODE_SHIFT)
#define HS_MMCSD_SMARTIDLE_SMARTWAKE (MMCHS_SYSCONFIG_SIDLEMODE_SMARTWAKE << MMCHS_SYSCONFIG_SIDLEMODE_SHIFT)

/*
** Macros that can be passed to configure wakeup modes
*/
#define HS_MMCSD_WAKEUP_ENABLE       (MMCHS_SYSCONFIG_ENAWAKEUP_ENABLED << MMCHS_SYSCONFIG_ENAWAKEUP_SHIFT)
#define HS_MMCSD_WAKEUP_DISABLE      (MMCHS_SYSCONFIG_ENAWAKEUP_DISABLED << MMCHS_SYSCONFIG_ENAWAKEUP_SHIFT)

/*
** Macros that can be passed to configure auto idle modes
*/
#define HS_MMCSD_AUTOIDLE_ENABLE     (MMCHS_SYSCONFIG_AUTOIDLE_ON << MMCHS_SYSCONFIG_AUTOIDLE_SHIFT)
#define HS_MMCSD_AUTOIDLE_DISABLE    (MMCHS_SYSCONFIG_AUTOIDLE_OFF << MMCHS_SYSCONFIG_AUTOIDLE_SHIFT)

/*****************************************************************************/
/*
** Macros that can be used for SD controller DMA request configuration
*/
#define HS_MMCSD_DMAREQ_EDGETRIG    (MMCHS_CON_SDMA_LNE_EARLYDEASSERT << MMCHS_CON_DMA_LNE_SHIFT)
#define HS_MMCSD_DMAREQ_LVLTRIG     (MMCHS_CON_SDMA_LNE_LATEDEASSERT << MMCHS_CON_DMA_LNE_SHIFT)

/*
** Macros that can be used for SD controller dual rate configuration
*/
#define HS_MMCSD_DUALRATE_ENABLE    (MMCHS_CON_DDR_DDRMODE <<  MMCHS_CON_DDR_SHIFT)
#define HS_MMCSD_DUALRATE_DISABLE   (MMCHS_CON_DDR_NORMALMODE << MMCHS_CON_DDR_SHIFT)

/*
** Macros that can be used for selecting the bus/data width
*/
#define HS_MMCSD_BUS_WIDTH_8BIT    (0x8)
#define HS_MMCSD_BUS_WIDTH_4BIT    (0x4)
#define HS_MMCSD_BUS_WIDTH_1BIT    (0x1)

/*
** Macros that can be used for starting/stopping a init stream
*/
#define HS_MMCSD_INIT_START         (MMCHS_CON_INIT_INITSTREAM << MMCHS_CON_INIT_SHIFT)
#define HS_MMCSD_INIT_STOP          (MMCHS_CON_INIT_NOINIT << MMCHS_CON_INIT_SHIFT)

/*
** Macros that can be used for setting drain type
*/
#define HS_MMCSD_OPENDRAIN          (MMCHS_CON_OD_OPENDRAIN << MMCHS_CON_OD_SHIFT)

/*
** Macros that can be used for forming the MMC/SD command
*/
/* Here\n
** cmd     : SD/MMC command number enumeration
** type    : specifies suspend upon CMD52 or resume upon CMD52 or abort upon CMD52/12.
** restype : no response, or response with or without busy
** rw      : direction of data transfer
*/
#define HS_MMCSD_CMD(cmd, type, restype, rw)    (cmd << MMCHS_CMD_INDX_SHIFT | type | restype | rw )

#define HS_MMCSD_NO_RESPONSE            (MMCHS_CMD_RSP_TYPE_NORSP << MMCHS_CMD_RSP_TYPE_SHIFT)
#define HS_MMCSD_136BITS_RESPONSE       (MMCHS_CMD_RSP_TYPE_136BITS << MMCHS_CMD_RSP_TYPE_SHIFT)
#define HS_MMCSD_48BITS_RESPONSE        (MMCHS_CMD_RSP_TYPE_48BITS << MMCHS_CMD_RSP_TYPE_SHIFT)
#define HS_MMCSD_48BITS_BUSY_RESPONSE   (MMCHS_CMD_RSP_TYPE_48BITS_BUSY << MMCHS_CMD_RSP_TYPE_SHIFT)

#define HS_MMCSD_CMD_TYPE_NORMAL        (MMCHS_CMD_CMD_TYPE_NORMAL << MMCHS_CMD_CMD_TYPE_SHIFT)
#define HS_MMCSD_CMD_TYPE_SUSPEND       (MMCHS_CMD_CMD_TYPE_SUSPEND << MMCHS_CMD_CMD_TYPE_SHIFT)
#define HS_MMCSD_CMD_TYPE_FUNCSEL       (MMCHS_CMD_CMD_TYPE_FUNC_SELECT << MMCHS_CMD_CMD_TYPE_SHIFT)
#define HS_MMCSD_CMD_TYPE_ABORT         (MMCHS_CMD_CMD_TYPE_ABORT << MMCHS_CMD_CMD_TYPE_SHIFT)

#define HS_MMCSD_CMD_DIR_READ           (MMCHS_CMD_DDIR_READ <<  MMCHS_CMD_DDIR_SHIFT)
#define HS_MMCSD_CMD_DIR_WRITE          (MMCHS_CMD_DDIR_WRITE <<  MMCHS_CMD_DDIR_SHIFT)
#define HS_MMCSD_CMD_DIR_DONTCARE       (MMCHS_CMD_DDIR_WRITE <<  MMCHS_CMD_DDIR_SHIFT)
/*
** Macros that can be used for checking the present state of the host controller
*/
#define HS_MMCSD_CARD_WRITEPROT        (MMCHS_PSTATE_WP)
#define HS_MMCSD_CARD_INSERTED         (MMCHS_PSTATE_CINS)        
#define HS_MMCSD_CARD_STABLE           (MMCHS_PSTATE_CSS)        
#define HS_MMCSD_BUFFER_READABLE       (MMCHS_PSTATE_BRE)        
#define HS_MMCSD_BUFFER_WRITEABLE      (MMCHS_PSTATE_BWE)        
#define HS_MMCSD_READ_INPROGRESS       (MMCHS_PSTATE_RTA)        
#define HS_MMCSD_WRITE_INPROGRESS      (MMCHS_PSTATE_WTA)        

/*
** Macros that can be used for configuring the power and transfer parameters
*/
#define HS_MMCSD_BUS_VOLT_1P8          (MMCHS_HCTL_SDVS_1V8 << MMCHS_HCTL_SDVS_SHIFT)
#define HS_MMCSD_BUS_VOLT_3P0          (MMCHS_HCTL_SDVS_3V0 << MMCHS_HCTL_SDVS_SHIFT)
#define HS_MMCSD_BUS_VOLT_3P3          (MMCHS_HCTL_SDVS_3V3 << MMCHS_HCTL_SDVS_SHIFT)
#define HS_MMCSD_BUS_POWER_ON          (MMCHS_HCTL_SDBP_PWRON << MMCHS_HCTL_SDBP_SHIFT)
#define HS_MMCSD_BUS_POWER_OFF         (MMCHS_HCTL_SDBP_PWROFF << MMCHS_HCTL_SDBP_SHIFT)
#define HS_MMCSD_BUS_HIGHSPEED         (MMCHS_HCTL_HSPE_HIGHSPEED << MMCHS_HCTL_HSPE_SHIFT)
#define HS_MMCSD_BUS_STDSPEED          (MMCHS_HCTL_HSPE_NORMALSPEED << MMCHS_HCTL_HSPE_SHIFT)

/*
** Macros that can be used for setting the clock, timeout values
*/
#define HS_MMCSD_DATA_TIMEOUT(n)        ((((n) - 13) & 0xF) << MMCHS_SYSCTL_DTO_SHIFT)
#define HS_MMCSD_CLK_DIVIDER(n)         ((n & 0x3FF) << MMCHS_SYSCTL_CLKD_SHIFT)
#define HS_MMCSD_CARDCLOCK_ENABLE       (MMCHS_SYSCTL_CEN_ENABLE << MMCHS_SYSCTL_CEN_SHIFT)
#define HS_MMCSD_CARDCLOCK_DISABLE      (MMCHS_SYSCTL_CEN_DISABLE << MMCHS_SYSCTL_CEN_SHIFT)
#define HS_MMCSD_INTCLOCK_ON            (MMCHS_SYSCTL_ICE_OSCILLATE << MMCHS_SYSCTL_ICE_SHIFT)
#define HS_MMCSD_INTCLOCK_OFF           (MMCHS_SYSCTL_ICE_STOP << MMCHS_SYSCTL_ICE_SHIFT)

/*
** Macros that can be used for resetting controller lines
*/
#define HS_MMCSD_DATALINE_RESET         (MMCHS_SYSCTL_SRD)
#define HS_MMCSD_CMDLINE_RESET          (MMCHS_SYSCTL_SRC)
#define HS_MMCSD_ALL_RESET              (MMCHS_SYSCTL_SRA)

/*
** Macros that can be used for interrupt enable/disable and status get operations
*/
#define HS_MMCSD_INTR_BADACCESS         (MMCHS_IE_BADA_ENABLE)
#define HS_MMCSD_INTR_CARDERROR         (MMCHS_IE_CERR_ENABLE)
#define HS_MMCSD_INTR_ADMAERROR         (MMCHS_IE_ADMAE_ENABLE)
#define HS_MMCSD_INTR_ACMD12ERR         (MMCHS_IE_ACE_ENABLE)
#define HS_MMCSD_INTR_DATABITERR        (MMCHS_IE_DEB_ENABLE)
#define HS_MMCSD_INTR_DATACRCERR        (MMCHS_IE_DCRC_ENABLE)
#define HS_MMCSD_INTR_DATATIMEOUT       (MMCHS_IE_DTO_ENABLE)
#define HS_MMCSD_INTR_CMDINDXERR        (MMCHS_IE_CIE_ENABLE)
#define HS_MMCSD_INTR_CMDBITERR         (MMCHS_IE_CEB_ENABLE)
#define HS_MMCSD_INTR_CMDCRCERR         (MMCHS_IE_CCRC_ENABLE)
#define HS_MMCSD_INTR_CMDTIMEOUT        (MMCHS_IE_CTO_ENABLE)
#define HS_MMCSD_INTR_CARDINS           (MMCHS_IE_CINS_ENABLE)                    
#define HS_MMCSD_INTR_BUFRDRDY          (MMCHS_IE_BRR_ENABLE)                    
#define HS_MMCSD_INTR_BUFWRRDY          (MMCHS_IE_BWR_ENABLE)                    
#define HS_MMCSD_INTR_TRNFCOMP          (MMCHS_IE_TC_ENABLE)
#define HS_MMCSD_INTR_CMDCOMP           (MMCHS_IE_CC_ENABLE)

#define HS_MMCSD_STAT_BADACCESS         (MMCHS_STAT_BADA)
#define HS_MMCSD_STAT_CARDERROR         (MMCHS_STAT_CERR)
#define HS_MMCSD_STAT_ADMAERROR         (MMCHS_STAT_ADMAE)
#define HS_MMCSD_STAT_ACMD12ERR         (MMCHS_STAT_ACE)
#define HS_MMCSD_STAT_DATABITERR        (MMCHS_STAT_DEB)
#define HS_MMCSD_STAT_DATACRCERR        (MMCHS_STAT_DCRC)
#define HS_MMCSD_STAT_ERR				(MMCHS_STAT_ERRI)
#define HS_MMCSD_STAT_DATATIMEOUT       (MMCHS_STAT_DTO)
#define HS_MMCSD_STAT_CMDINDXERR        (MMCHS_STAT_CIE)
#define HS_MMCSD_STAT_CMDBITERR         (MMCHS_STAT_CEB)
#define HS_MMCSD_STAT_CMDCRCERR         (MMCHS_STAT_CCRC)
#define HS_MMCSD_STAT_CMDTIMEOUT        (MMCHS_STAT_CTO)
#define HS_MMCSD_STAT_CARDINS           (MMCHS_STAT_CINS)                    
#define HS_MMCSD_STAT_BUFRDRDY          (MMCHS_STAT_BRR)                    
#define HS_MMCSD_STAT_BUFWRRDY          (MMCHS_STAT_BWR)                    
#define HS_MMCSD_STAT_TRNFCOMP          (MMCHS_STAT_TC)
#define HS_MMCSD_STAT_CMDCOMP           (MMCHS_STAT_CC)

#define HS_MMCSD_SIGEN_BADACCESS        (MMCHS_ISE_BADA_SIGEN)
#define HS_MMCSD_SIGEN_CARDERROR        (MMCHS_ISE_CERR_SIGEN)
#define HS_MMCSD_SIGEN_ADMAERROR        (MMCHS_ISE_ADMAE_SIGEN)
#define HS_MMCSD_SIGEN_ACMD12ERR        (MMCHS_ISE_ACE_SIGEN)
#define HS_MMCSD_SIGEN_DATABITERR       (MMCHS_ISE_DEB_SIGEN)
#define HS_MMCSD_SIGEN_DATACRCERR       (MMCHS_ISE_DCRC_SIGEN)
#define HS_MMCSD_SIGEN_DATATIMEOUT      (MMCHS_ISE_DTO_SIGEN)
#define HS_MMCSD_SIGEN_CMDINDXERR       (MMCHS_ISE_CIE_SIGEN)
#define HS_MMCSD_SIGEN_CMDBITERR        (MMCHS_ISE_CEB_SIGEN)
#define HS_MMCSD_SIGEN_CMDCRCERR        (MMCHS_ISE_CCRC_SIGEN)
#define HS_MMCSD_SIGEN_CMDTIMEOUT       (MMCHS_ISE_CTO_SIGEN)
#define HS_MMCSD_SIGEN_CARDINS          (MMCHS_ISE_CINS_SIGEN)                    
#define HS_MMCSD_SIGEN_BUFRDRDY         (MMCHS_ISE_BRR_SIGEN)                    
#define HS_MMCSD_SIGEN_BUFWRRDY         (MMCHS_ISE_BWR_SIGEN)                    
#define HS_MMCSD_SIGEN_TRNFCOMP         (MMCHS_ISE_TC_SIGEN)
#define HS_MMCSD_SIGEN_CMDCOMP          (MMCHS_ISE_CC_SIGEN)

/*
** Macros that can be used for checking the capabilites of the controller
*/
#define HS_MMCSD_SUPPORT_VOLT_1P8       (MMCHS_CAPA_VS18)
#define HS_MMCSD_SUPPORT_VOLT_3P0       (MMCHS_CAPA_VS30)
#define HS_MMCSD_SUPPORT_VOLT_3P3       (MMCHS_CAPA_VS33)
#define HS_MMCSD_SUPPORT_DMA            (MMCHS_CAPA_DS)
#define HS_MMCSD_SUPPORT_HIGHSPEED      (MMCHS_CAPA_HSS)
#define HS_MMCSD_SUPPORT_BLOCKLEN       (MMCHS_CAPA_MBL)


#define  SDVS_MASK (0x7 << 9)
#define SDVS_3V3 (0x7 << 9)
#define SDBP_PWRON (0x1 << 8)

//uboot con set
#define RESERVED_MASK (0x3 << 9)
#define CTPL_MMC_SD (0x0 << 11)
#define WPP_ACTIVEHIGH (0x0 << 8)
#define CDP_ACTIVEHIGH (0x0 << 7)
#define MIT_CTO (0x0 << 6)
#define DW8_1_4BITMODE (0x0 << 5)
#define MODE_FUNC (0x0 << 4)
#define STR_BLOCK (0x0 << 3)
#define HR_NOHOSTRESP (0x0 << 2)
#define INIT_NOINIT (0x0 << 1)
#define NOOPENDRAIN (0x0 << 0)
#define INIT_INITSTREAM (0x1 << 1)

//uboot sysctl
#define HSMMCSDRegOut(baseAddr, regOffset, mask, val) \
    (HWREG(baseAddr + regOffset) = (HWREG(baseAddr + regOffset) & ~(mask)) | ((val) & (mask)))

#define ICE_MASK (0x1 << 0)
#define DTO_MASK (0xF << 16)
#define CEN_MASK (0x1 << 2)
#define ICE_STOP (0x0 << 0)
#define DTO_15THDTO (0xE << 16)
#define CLKD_MASK (0x3FF << 6)
#define CLKD_OFFSET (6)
#define ICE_OSCILLATE (0x1 << 0)

#define CEN_ENABLE (0x1 << 2)


//interupts
#define IE_CC				(0x01 << 0)
#define IE_TC				(0x01 << 1)
#define IE_BWR				(0x01 << 4)
#define IE_BRR				(0x01 << 5)
#define IE_CTO				(0x01 << 16)
#define IE_CCRC				(0x01 << 17)
#define IE_CEB				(0x01 << 18)
#define IE_CIE				(0x01 << 19)
#define IE_DTO				(0x01 << 20)
#define IE_DCRC				(0x01 << 21)
#define IE_DEB				(0x01 << 22)
#define IE_ADMAE			(0x01 << 25)
#define IE_CERR				(0x01 << 28)
#define IE_BADA				(0x01 << 29)
#define INT_EN_MASK (IE_BADA | IE_CERR | IE_DEB | IE_DCRC |\
		IE_DTO | IE_CIE | IE_CEB | IE_CCRC | IE_ADMAE | IE_CTO |\
		IE_BRR | IE_BWR | IE_TC | IE_CC)


//commands
#define INDEX(i)			(i << 24)
#define DP_DATA				(0x1 << 21)
#define DP_NO_DATA			(0x0 << 21)
#define RSP_TYPE_OFFSET			(16)
#define RSP_TYPE_MASK			(0x3 << 16)
#define RSP_TYPE_NORSP			(0x0 << 16)
#define CCCE_CHECK			(0x1 << 19)
#define CCCE_NOCHECK			(0x0 << 19)
#define CICE_NOCHECK			(0x0 << 20)
#define DDIR_WRITE			(0x0 << 4)
#define RSP_TYPE_NONE	(RSP_TYPE_NORSP   | CCCE_NOCHECK | CICE_NOCHECK)
#define MMC_CMD0 (INDEX(0) | RSP_TYPE_NONE | DP_NO_DATA | DDIR_WRITE)
#define CC_MASK				(0x1 << 0)
#define MMC_RSP_R1 (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136 (1 << 1)
#define RSP_TYPE_LGHT136 (0x1 << 16)
#define MMC_RSP_BUSY (1 << 3)
#define RSP_TYPE_LGHT48B (0x3 << 16)
#define RSP_TYPE_LGHT48 (0x2 << 16)
#define MMC_RSP_CRC (1 << 2)
#define MMC_RSP_OPCODE (1 << 4)
#define CICE_CHECK (0x1 << 20)
#define ERRI_MASK (0x1 << 15)


#define MMCHS_RSP10 0x210
#define MMCHS_RSP32 0x214
#define MMCHS_RSP54 0x218
#define MMCHS_RSP76 0x21C

struct mmc_cmd {
    unsigned int cmdidx;      // Command Index (0, 8, 55, 41, etc.)
    unsigned int resp_type;   // Response Type flags (None, R1, R7, etc.)
    unsigned int cmdarg;      // 32-bit Argument for the command
    unsigned int response[4]; // Array to store the response from RSPxx registers
};

struct mmc_data {
	union {
		char *dest;
		const char *src;
	};
	uint flags;
	uint blocks;
	uint blocksize;
};

/*
** Function prototypes
*/
extern void HSMMCSDSupportedVoltSet(unsigned int baseAddr, unsigned int volt);
extern int HSMMCSDLinesReset(unsigned int baseAddr);
extern int HSMMCSDSoftReset(unsigned int baseAddr);
extern unsigned int HSMMCSDIsCardInserted(unsigned int baseAddr);
extern void MMC0_Hardware_Setup(void);
extern int HSMMCSDBusconfPower(unsigned int baseAddr);
extern int HSMMCSDConSet(unsigned int baseAddr);
extern int HSMMCSDWaitClockStable(unsigned int baseAddr);
extern int hsmmc_init_stream(unsigned int baseAddr);

extern void delay_1sec(void);
extern void delay_1ms(void);
extern int send_cmd(unsigned int base,struct mmc_cmd *cmd, struct mmc_data *data);
extern int mmc_read_sector(unsigned int block_addr, unsigned int *buffer);
extern int mmc_write_sector(unsigned int block_addr, unsigned int *data);
#ifdef __cplusplus
}
#endif

#endif
