#include "mmcsd/mmcsd.h"
#include "mmcsdlib/mmc_library.h"
#include "hw_cm_per.h"
#include "hw_cm_wkup.h"
#include "nonstrware/UART.h"
#include "hw_am335_ctrl.h"
#include "soc/soc_AM335x.h"


void basicCLKMMC(){
    //1 cmcllkl3
    HWREG(CM_PER + CM_PER_L3_CLKSTCTRL) = CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;
    //2 l4lsstclk
    HWREG(CM_PER + CM_PER_L4LS_CLKSTCTRL) = CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;
    //3 wkclkstctrl
    HWREG(CM_WKUP + CM_WKUP_CLKSTCTRL) = CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;


    HWREG(CM_PER + CM_PER_L3_CLKCTRL) = 0x2;
    while ((HWREG(CM_PER + CM_PER_L3_CLKCTRL) & (0x3 << 16)) != 0);

    // L4LS Interconnect
    HWREG(CM_PER + CM_PER_L4LS_CLKCTRL) = 0x2;
    while ((HWREG(CM_PER + CM_PER_L4LS_CLKCTRL) & (0x3 << 16)) != 0);

    // Control Module (Required for Muxing)
    HWREG(CM_WKUP + CM_WKUP_CONTROL_CLKCTRL) = 0x2;
    while ((HWREG(CM_WKUP + CM_WKUP_CONTROL_CLKCTRL) & (0x3 << 16)) != 0);
//CM_WKUP_L4WKUP_CLKCTRL

    HWREG(CM_WKUP + CM_WKUP_L4WKUP_CLKCTRL) = 0x2;
    while ((HWREG(CM_WKUP + CM_WKUP_L4WKUP_CLKCTRL) & (0x3 << 16)) != 0);
    
    // MMC0 Logic Block
    HWREG(CM_PER + CM_PER_MMC0_CLKCTRL) = 0x2;
    while ((HWREG(CM_PER + CM_PER_MMC0_CLKCTRL) & (0x3 << 16)) != 0);
}



unsigned int HSMMCSDCardPresent(unsigned int regBase)
{
    return HSMMCSDIsCardInserted(regBase);
}


void HSMMCSDPinMuxSetup(void)
{
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT3) =
                   (0 << CONTROL_CONF_MMC0_DAT3_CONF_MMC0_DAT3_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_DAT3_CONF_MMC0_DAT3_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_DAT3_CONF_MMC0_DAT3_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_DAT3_CONF_MMC0_DAT3_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT2) =
                   (0 << CONTROL_CONF_MMC0_DAT2_CONF_MMC0_DAT2_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_DAT2_CONF_MMC0_DAT2_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_DAT2_CONF_MMC0_DAT2_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_DAT2_CONF_MMC0_DAT2_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT1) =
                   (0 << CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT0) =
                   (0 << CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_CLK) =
                   (0 << CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_CMD) =
                   (0 << CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_RXACTIVE_SHIFT);

     HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS1) =
                   (5 << CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_RXACTIVE_SHIFT);
}

void HSMMCSDModuleClkConfig(void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_MMC0_CLKCTRL) |= 
                             CM_PER_MMC0_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_MMC0_CLKCTRL) & 
      CM_PER_MMC0_CLKCTRL_MODULEMODE) != CM_PER_MMC0_CLKCTRL_MODULEMODE_ENABLE);
}


void setupMMCLines(){
    HSMMCSDPinMuxSetup();
    HSMMCSDModuleClkConfig();
}


unsigned int SDControllerInit(unsigned int base)
{
    int status = 0;

    /*Refer to the MMC Host and Bus configuration steps in TRM */
    /* controller Reset */
    status = HSMMCSDSoftReset(base);

    if (status != 0)
    {
        DEBUG_LOG_STR(UART0,"HS MMC/SD Reset failed\n\r");
        return -1;
    }

    HSMMCSDLinesReset(base);

    HSMMCSDSupportedVoltSet(base,HS_MMCSD_SUPPORT_VOLT_3P3);

    HSMMCSDBusconfPower(base);

    HSMMCSDConSet(base);

    unsigned int dsor = 240;
    // 1. Stop the clock and set maximum timeout
    HSMMCSDRegOut(base, MMCHS_SYSCTL, 
                  (ICE_MASK | DTO_MASK | CEN_MASK), 
                  (ICE_STOP | DTO_15THDTO));

    // 2. Load the divider (dsor) and start the Internal Clock (ICE)
    HSMMCSDRegOut(base, MMCHS_SYSCTL, 
                  (ICE_MASK | CLKD_MASK), 
                  ((dsor << CLKD_OFFSET) | ICE_OSCILLATE));
    
    HSMMCSDWaitClockStable(base);

    DEBUG_LOG_STR(UART0,"HS MMC/SD clock source stable(not connected yet)\n\r");

    //gen the clock
    HWREG(base+MMCHS_SYSCTL) |= CEN_ENABLE;

    //SDBP poweron
    HWREG(base+MMCHS_HCTL) |= SDBP_PWRON;

    //interrupt
    HWREG(base+MMCHS_IE) = INT_EN_MASK;
    HWREG(base + MMCHS_ISE) = INT_EN_MASK;


    //int stream
    hsmmc_init_stream(base);

    return 0;

}

unsigned int SDMMCgoIdle(){
    

    struct mmc_cmd cmd0;

    DEBUG_LOG_STR(UART0,"We are trying to put sd to idle\n\r");

    delay_1sec();

    cmd0.cmdarg = MMC_CMD_GO_IDLE_STATE;
    cmd0.cmdidx = 0;
    cmd0.resp_type = MMC_RSP_NONE;


    int err = send_cmd(SOC_MMCHS_0_REGS,&cmd0,NULL);

    if(err != 0){
        DEBUG_LOG_STR(UART0,"oh man,we ran into an error\n\r");
        return -1;
    }

    delay_1sec();

    DEBUG_LOG_STR(UART0,"The sd is in idle mode rn\n\r");

    return 0;

}

unsigned int mmc_send_if_cond(){
    struct mmc_cmd cmd;

    DEBUG_LOG_STR(UART0,"get sd version and set volts?\n\r");

    cmd.cmdidx = SD_CMD_SEND_IF_COND;
    cmd.cmdarg = (1 << 8) | 0xAA;
    cmd.resp_type = MMC_RSP_R7;


    int err = send_cmd(SOC_MMCHS_0_REGS,&cmd,NULL);

    if(err != 0){
        DEBUG_LOG_STR(UART0,"oh man,we ran into an error\n\r");
        return -1;
    }

    DEBUG_LOG_STR(UART0,"The sd has done its first resp send\n\r");
    if ((cmd.response[0] & 0xFF) == 0xAA) {
        DEBUG_LOG_STR(UART0, "SUCCESS: The SD card echoed 0xAA! Protocol is ALIVE!\n\r");
    } else {
        DEBUG_LOG_STR(UART0, "WARNING: Card responded, but echo failed. (Bad voltage?)\n\r");
        return -1;
    }

    return 0;
}

unsigned int mmc_sd_send_op_cond() {
    struct mmc_cmd cmd;
    int timeout = 1000; // U-Boot standard timeout
    int err;

    DEBUG_LOG_STR(UART0, "Negotiating Voltage and Power-up (ACMD41)...\n\r");

    while (1) {
        // --- STEP 1: CMD55 (APP_CMD) ---
        // This tells the card the next command is application-specific
        cmd.cmdidx = MMC_CMD_APP_CMD;
        cmd.resp_type = MMC_RSP_R1;
        cmd.cmdarg = 0; // RCA is 0 in Idle State

        err = send_cmd(SOC_MMCHS_0_REGS,&cmd, NULL);
        if (err) {
            DEBUG_LOG_STR(UART0, "ERROR: CMD55 (APP_CMD) failed\n\r");
            return err;
        }

        // --- STEP 2: ACMD41 (SD_SEND_OP_COND) ---
        cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
        cmd.resp_type = MMC_RSP_R3; // R3 has NO CRC!

        // Note: For now, we assume SD_VERSION_2 since CMD8 passed

        cmd.cmdarg = 0x00FF8000;
        cmd.cmdarg |= OCR_HCS;

        err = send_cmd(SOC_MMCHS_0_REGS,&cmd, NULL);
        if (err) {
            DEBUG_LOG_STR(UART0, "ERROR: ACMD41 failed\n\r");
            return err;
        }

        // --- STEP 3: CHECK BUSY BIT ---
        // If Bit 31 is 1, the card is finished powering up
        if (cmd.response[0] & OCR_BUSY) {
            break;
        }

        if (timeout-- <= 0) {
            DEBUG_LOG_STR(UART0, "ERROR: ACMD41 Timed out waiting for OCR_BUSY\n\r");
            return -1;
        }

        // Use your tuned delay. 1ms is enough between polls.
        delay_1ms(); 
    }
    if (cmd.response[0] & OCR_HCS) {
        UARTPuts(UART0, "SUCCESS: Card is READY (High Capacity)\n\r");
    } else {
        UARTPuts(UART0, "SUCCESS: Card is READY (Standard Capacity)\n\r");
    }

    return 0;
}
//members to track the card
unsigned int card_rca = 0;
unsigned int card_high_capacity = 0;
unsigned int card_read_bl_len = 512;

int mmc_startup() {
    struct mmc_cmd cmd;
    int err;
    cmd.cmdidx = 2; 
    cmd.resp_type = MMC_RSP_R2; // 136-bit response
    cmd.cmdarg = 0;
    err = send_cmd(SOC_MMCHS_0_REGS,&cmd, NULL);
    if (err) {
        DEBUG_LOG_STR(UART0, "ERROR: CID Fetch (CMD2) failed\n\r");
        return err;
    }
    DEBUG_LOG_STR(UART0, "SUCCESS: Card Identified (CID received)\n\r");
    cmd.cmdidx = 3;
    cmd.cmdarg = 0;
    cmd.resp_type = MMC_RSP_R6;

    err = send_cmd(SOC_MMCHS_0_REGS,&cmd, NULL);
    if (err) {
        DEBUG_LOG_STR(UART0, "ERROR: RCA Fetch (CMD3) failed\n\r");
        return err;
    }
    card_rca = (cmd.response[0] >> 16) & 0xffff;
    DEBUG_LOG_STR(UART0, "SUCCESS: RCA Received: ");
    DEBUG_LOG_STR(UART0, "\n\r");
    cmd.cmdidx = 9;
    cmd.resp_type = MMC_RSP_R2; // 136-bit response
    cmd.cmdarg = card_rca << 16;

    err = send_cmd(SOC_MMCHS_0_REGS,&cmd, NULL);
    if (err) {
        DEBUG_LOG_STR(UART0, "ERROR: CSD Fetch (CMD9) failed\n\r");
        return err;
    }
    card_read_bl_len = 512; 

    cmd.cmdidx = 7;
    cmd.resp_type = MMC_RSP_R1b; // R1 with Busy signaling
    cmd.cmdarg = card_rca << 16;

    err = send_cmd(SOC_MMCHS_0_REGS,&cmd, NULL);
    if (err) {
        DEBUG_LOG_STR(UART0, "ERROR: Card Selection (CMD7) failed\n\r");
        return err;
    }
    UARTPuts(UART0, "SUCCESS: Card is in TRANSFER state. Ready for data!\n\r");
    return 0;
}


unsigned int mmc_init_to_transfer(){
    unsigned int err;


    err = SDMMCgoIdle();

    if(err != 0){
        return -1;
    }
    //once out of idle

    err = mmc_send_if_cond();
    if(err != 0){
        return -1;
    }

    err = mmc_sd_send_op_cond();
    if(err != 0){
        return -1;
    }

    err = mmc_startup();
    if(err != 0){
        return -1;
    }


    return 0;
}