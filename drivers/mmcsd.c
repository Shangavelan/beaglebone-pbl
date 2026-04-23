

#include "soc/soc_AM335x.h"
#include "hw_types.h"
#include "mmcsd/mmcsd.h"
#include "mmcsd/hw_hs_mmcsd.h"
#include "nonstrware/UART.h"

int HSMMCSDSoftReset(unsigned int baseAddr)
{
    volatile unsigned int timeout = 0xFFFF;

    HWREG(baseAddr + MMCHS_SYSCONFIG) |= MMCHS_SYSCONFIG_SOFTRESET;
    
    do {
        if ((HWREG(baseAddr + MMCHS_SYSSTATUS) & MMCHS_SYSSTATUS_RESETDONE) ==
                                               MMCHS_SYSSTATUS_RESETDONE)
        {
            break;
        }
    } while(timeout--);

    if ((HWREG(baseAddr + MMCHS_SYSCONFIG) & MMCHS_SYSCONFIG_SOFTRESET) == 0) {
        DEBUG_LOG_STR(UART0, "CERTAINTY: SoftReset bit auto-cleared to 0. Reset occurred!\n\r");
    } else {
        DEBUG_LOG_STR(UART0, "WARNING: SoftReset bit is still 1. Hardware hung?\n\r");
        return -1;
    }
    
    if (0 == timeout)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


int HSMMCSDLinesReset(unsigned int baseAddr)
{
    volatile unsigned int timeout = 0xFFFF;

    HWREG(baseAddr + MMCHS_SYSCTL) |= HS_MMCSD_ALL_RESET;

    do {
        if ((HWREG(baseAddr + MMCHS_SYSCTL) & HS_MMCSD_ALL_RESET) == HS_MMCSD_ALL_RESET)
        {
            break;
        }
    } while(timeout--);

    if (0 == timeout)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


void HSMMCSDSupportedVoltSet(unsigned int baseAddr, unsigned int volt)
{
    HWREG(baseAddr + MMCHS_CAPA) &= ~(MMCHS_CAPA_VS18 | MMCHS_CAPA_VS30 |
                                      MMCHS_CAPA_VS33);
    HWREG(baseAddr + MMCHS_CAPA) |= volt;
}

unsigned int HSMMCSDIsCardInserted(unsigned int baseAddr)
{
    return (HWREG(baseAddr + MMCHS_PSTATE) & MMCHS_PSTATE_CINS) >>
                MMCHS_PSTATE_CINS_SHIFT;
}



int HSMMCSDBusconfPower(unsigned int baseAddr){

    unsigned int hctl;
    HWREG(baseAddr + MMCHS_AC12) &= ~(1 << 19);//#define AC12_V1V8_SIGEN BIT(19)
    hctl = HWREG(baseAddr + MMCHS_HCTL) & ~SDVS_MASK;

    //now to set 3v3 only i guess
    hctl |= SDVS_3V3;

    HWREG(baseAddr + MMCHS_HCTL) = hctl;
    return 0;

}

int HSMMCSDConSet(unsigned int baseAddr){

    //DW8=(0x00000020u)..
    unsigned int reserve;
    reserve = HWREG(baseAddr + MMCHS_CON) & RESERVED_MASK;

    HWREG(baseAddr + MMCHS_CON) = reserve | CTPL_MMC_SD | WPP_ACTIVEHIGH | CDP_ACTIVEHIGH |
		MIT_CTO | DW8_1_4BITMODE | MODE_FUNC | STR_BLOCK |
		HR_NOHOSTRESP | INIT_NOINIT | NOOPENDRAIN;
    
    return 0;

}

int HSMMCSDWaitClockStable(unsigned int baseAddr)
{
    // A large enough value to wait ~100ms depending on CPU speed
    // On a 1GHz AM335x, this is plenty of time.
    volatile unsigned int timeout = 0xFFFF; 

    // Poll the ICS bit (Bit 1 of SYSCTL)
    // 0x2 is the mask for Bit 1
    while (!(HWREG(baseAddr + MMCHS_SYSCTL) & (0x2))) 
    {
        if (timeout-- == 0) 
        {
            // If we reach 0, the hardware is stuck
            DEBUG_LOG_STR(UART0, "ERROR: MMC Clock failed to stabilize (ICS Timeout)!\n\r");
            return -1; 
        }
    }

    DEBUG_LOG_STR(UART0, "SUCCESS: MMC Internal Clock is STABLE.\n\r");
    return 0;
}

int hsmmc_init_stream(unsigned int baseAddr){
    HWREG(baseAddr + MMCHS_CON) |= INIT_INITSTREAM;


    HWREG(baseAddr + MMCHS_CMD) = MMC_CMD0;
    volatile unsigned int timeout = 0x1FFFF;
    while (!(HWREG(baseAddr + MMCHS_STAT) & (1 << 0))) 
    {
        if (timeout-- == 0) 
        {
            DEBUG_LOG_STR(UART0, "ERROR: hsmmc_init_stream timed out waiting for CC!\n\r");
            return -1; // Equivalent to ETIMEDOUT
        }
    }
    HWREG(baseAddr + MMCHS_STAT) = CC_MASK;
    HWREG(baseAddr + MMCHS_CMD) = MMC_CMD0;

    timeout = 0x1FFFF;
    while (!(HWREG(baseAddr + MMCHS_STAT) & (1 << 0))) 
    {
        if (timeout-- == 0) 
        {
            DEBUG_LOG_STR(UART0, "ERROR: hsmmc_init_stream timed out waiting for CC!\n\r");
            return -1; // Equivalent to ETIMEDOUT
        }
    }
    HWREG(baseAddr + MMCHS_STAT) = CC_MASK;

    HWREG(baseAddr + MMCHS_CON) &= ~INIT_INITSTREAM;

    DEBUG_LOG_STR(UART0, "SUCCESS: CMD0 Init Stream passed. 80 clocks sent!\n\r");
    return 0;
}
/*
int send_cmd(struct mmc_cmd *cmd,struct mmc_data *data){
    
    unsigned int base = SOC_MMCHS_0_REGS; // Or your passed variable
    unsigned int flags = 0;

    // 1. THE INHIBIT WAIT
    // Always wait for the Command Inhibit (Bit 0) to clear
    while (HWREG(base + MMCHS_PSTATE) & 0x1);

    // 2. THE STATUS CLEAR
    // Wipe the slate clean
    HWREG(base + MMCHS_STAT) = 0xFFFFFFFF;

    // 3. THE FLAG LOGIC (Add more cases here as you go!)
    // For now: Just handle "No Response" (CMD0)
    if (cmd->resp_type == MMC_RSP_NONE) {
        flags = 0; 
    } 
    //FUTURE TASK: Add CMD8 (48-bit) logic here:
      // else if (cmd->resp_type == MMC_RSP_R7) {
        //   flags = (0x2 << 16) | (1 << 20) | (1 << 19); 
      // }
   // 

    // 4. FIRE THE COMMAND
    HWREG(base + MMCHS_ARG) = cmd->cmdarg;
    HWREG(base + MMCHS_CMD) = (cmd->cmdidx << 24) | flags;

    // 5. POLL FOR COMPLETION (Command Complete = Bit 0)
    volatile unsigned int timeout = 0xFFFF;
    while (!(HWREG(base + MMCHS_STAT) & 0x1)) {
        if (timeout-- == 0) {
            DEBUG_LOG_STR(UART0, "ERROR: Command Timeout!\n\r");
            return -1;
        }
    }

    
    //if (cmd->resp_type != MMC_RSP_NONE) {
     //   cmd->response[0] = HWREG(base + MMCHS_RSP10);
    //}

    // 7. CLEANUP
    HWREG(base + MMCHS_STAT) = 0x1; // Clear CC bit
    return 0;
}*/

int send_cmd(unsigned int base,struct mmc_cmd *cmd, struct mmc_data *data) {
    unsigned int flags = 0;
    unsigned int mmc_stat;

    // 1. THE INHIBIT WAIT (CMDI = Command Inhibit)
    // Wait for the hardware state machine to be idle
    while (HWREG(base + MMCHS_PSTATE) & 0x1);

    // 2. THE STATUS CLEAR
    // Clear all bits by writing 1s (Standard OMAP behavior)
    HWREG(base + MMCHS_STAT) = 0xFFFFFFFF;

    // 3. FLAG LOGIC (Derived from U-Boot mmc_send_cmd)
    if (!(cmd->resp_type & MMC_RSP_PRESENT)) {
        flags = RSP_TYPE_NONE;
    } else if (cmd->resp_type & MMC_RSP_136) {
        flags = RSP_TYPE_LGHT136;
    } else if (cmd->resp_type & MMC_RSP_BUSY) {
        flags = RSP_TYPE_LGHT48B;
    } else {
        flags = RSP_TYPE_LGHT48;
    }

    if (cmd->cmdidx == 17 || cmd->cmdidx == 24) {
        flags |= (1 << 21); // DP: Data Present (Bit 21)
        
        // DDIR: Data Direction (Bit 4)
        // 1 = Read (Card -> Host), 0 = Write (Host -> Card)
        if (cmd->cmdidx == 17) {
            flags |= (1 << 4); 
        }
    }

    // Enable Hardware Checks (CRC and Index)
    if (cmd->resp_type & MMC_RSP_CRC)
        flags |= CCCE_CHECK;
    if (cmd->resp_type & MMC_RSP_OPCODE)
        flags |= CICE_CHECK;

    // 4. FIRE THE COMMAND
    HWREG(base + MMCHS_ARG) = cmd->cmdarg;
    
    // Some eMMC/SD cards need a tiny breath before the CMD write
    // udelay(20); 
    
    HWREG(base + MMCHS_CMD) = (cmd->cmdidx << 24) | flags;

    // 5. POLL FOR COMPLETION
    volatile unsigned int timeout = 0xFFFF;
    while (1) {
        mmc_stat = HWREG(base + MMCHS_STAT);
        
        // Success: Command Complete bit is set
        if (mmc_stat & CC_MASK) {
            break; 
        }
        
        // Error: Check for Command Timeout (CTO) or other errors
        if (mmc_stat & ERRI_MASK) {
            DEBUG_LOG_STR(UART0, "ERROR: MMC Command Error! STAT: ");
            // You can print hex here to debug CTO (bit 16) or CCRC (bit 17)
            return -1;
        }

        if (timeout-- == 0) {
            DEBUG_LOG_STR(UART0, "ERROR: Command Timeout (Software)!\n\r");
            return -1;
        }
    }

    // 6. READ RESPONSE
    // If a response is expected, the HW puts it in the RSP registers
    if (cmd->resp_type & MMC_RSP_PRESENT) {
        if (cmd->resp_type & MMC_RSP_136) {
            // R2 Response (136-bit)
            /*
            cmd->response[3] = HWREG(base + MMCHS_RSP10);
            cmd->response[2] = HWREG(base + MMCHS_RSP32);
            cmd->response[1] = HWREG(base + MMCHS_RSP54);
            cmd->response[0] = HWREG(base + MMCHS_RSP76);*/
            cmd->response[0] = HWREG(base + MMCHS_RSP76); // Bits 127:96
            cmd->response[1] = HWREG(base + MMCHS_RSP54); // Bits 95:64
            cmd->response[2] = HWREG(base + MMCHS_RSP32); // Bits 63:32
            cmd->response[3] = HWREG(base + MMCHS_RSP10); // Bits 31:0
        } else {
            // R1, R3, R7 Responses (48-bit) are always in RSP10/RSP12
            cmd->response[0] = HWREG(base + MMCHS_RSP10);
        }
    }

    // 7. CLEANUP
    // Clear CC bit so we're ready for the next dance
    HWREG(base + MMCHS_STAT) = CC_MASK;
    
    return 0;
}

void delay_1sec(void) {
    // 1GHz = 10^9 cycles. 
    // A simple loop takes ~2-3 cycles per iteration.
    // We'll use a volatile counter to prevent the compiler from 
    // optimizing the "useless" loop away.
    volatile unsigned int count = 1000000; 
    
    while (count > 0) {
        count--;
    }
}

#define STAT_BRR            (1 << 5)  // Buffer Read Ready
#define STAT_BWR            (1 << 4)  // Buffer Write Ready

/**
 * Basic PIO Block Read (CMD17)
 * block_addr: The 512-byte sector index
 * buffer: Pointer to a 512-byte array (unsigned int* for 4-byte access)
 */
int mmc_read_sector(unsigned int block_addr, unsigned int *buffer) {
    struct mmc_cmd cmd;
    unsigned int base = SOC_MMCHS_0_REGS;

    HWREG(base + MMCHS_BLK) = 0x00010200; 
    cmd.cmdidx = 17;
    cmd.cmdarg = block_addr;
    cmd.resp_type = MMC_RSP_R1;
    if (send_cmd(base,&cmd, NULL) != 0) return -1;
    while (!(HWREG(base + MMCHS_STAT) & STAT_BRR));
    for (int i = 0; i < 128; i++) {
        buffer[i] = HWREG(base + MMCHS_DATA);
    }

    // 6. Wait for Transfer Complete (TC) bit 1
    // Without this, the next CMD17 will timeout because the bus is still busy.
    while (!(HWREG(base + MMCHS_STAT) & (1 << 1))); 
    HWREG(base + MMCHS_STAT) = STAT_BRR | (1 << 1);
    
    return 0;
}
/**
 * Basic PIO Block Write (CMD24)
 * block_addr: The 512-byte sector index
 * data: Pointer to 512 bytes of data to write
 */
int mmc_write_sector(unsigned int block_addr, unsigned int *data) {
    struct mmc_cmd cmd;
    unsigned int base = SOC_MMCHS_0_REGS;
    while (!(HWREG(base + MMCHS_PSTATE) & (1 << 20)));

    // 1. Set Block Geometry
    HWREG(base + MMCHS_BLK) = 0x00010200;

    // 2. Prepare CMD24
    cmd.cmdidx = 24;
    cmd.cmdarg = block_addr;
    cmd.resp_type = MMC_RSP_R1;

    // 3. Send Command
    if (send_cmd(base,&cmd, NULL) != 0) return -1;

    // 4. Poll for Buffer Write Ready (BWR)
    while (!(HWREG(base + MMCHS_STAT) & STAT_BWR));

    // 5. Fill the FIFO
    for (int i = 0; i < 128; i++) {
        HWREG(base + MMCHS_DATA) = data[i];
    }

    // 6. Clear BWR
    HWREG(base + MMCHS_STAT) = STAT_BWR;

    // 7. Wait for Transfer Complete (TC) to ensure flash is updated
    while (!(HWREG(base + MMCHS_STAT) & (1 << 1))); 
    HWREG(base + MMCHS_STAT) = (1 << 1);

    return 0;
}

void delay_1ms(){
    volatile unsigned int count = 100000; 
    
    while (count > 0) {
        count--;
    }
}
