#include "mmcsdlib/mmc_proto.h"
#include "soc/soc_AM335x.h"
#include "mmcsdlib/mmc_library.h"
#include "nonstrware/UART.h"


void mmcsd_Presence_Poll(void) {
    static unsigned int last_presence = 0xFF; // Start with an "unknown" state
    
    // Check the hardware
    // HSMMCSDCardPresent usually reads the PSTATE register bit 16
    unsigned int current_presence = HSMMCSDIsCardInserted(SOC_MMCHS_0_REGS);

    if (current_presence != last_presence) {
        if (current_presence == 1) {
            DEBUG_LOG_STR(UART0, "SD Card Inserted!\n\r");
        } else {
            DEBUG_LOG_STR(UART0, "SD Card Removed!\n\r");
        }
        last_presence = current_presence;
    }
}

void verify_sd_string_test() {
    unsigned int write_buf[128];
    unsigned int read_buf[128];
    unsigned int safe_sector = 32768; 
    char *message = "Hello Atomic Embedded! SD Driver is ALIVE.";
    int i;

    DEBUG_LOG_STR(UART0, "\n\r--- STRING PERSISTENCE TEST ---\n\r");

    // 1. Clear buffers
    for (i = 0; i < 128; i++) {
        write_buf[i] = 0;
        read_buf[i] = 0;
    }

    // 2. Copy the string into our write buffer
    // Casting to (char*) allows us to treat the 512-byte block as a string
    char *dest = (char *)write_buf;
    for (i = 0; message[i] != '\0' && i < 511; i++) {
        dest[i] = message[i];
    }

    // 3. Write to SD
    if (mmc_write_sector(safe_sector, write_buf) == 0) {
        DEBUG_LOG_STR(UART0, "Message stored in Silicon...\n\r");
        
        // 4. Read back from SD
        if (mmc_read_sector(safe_sector, read_buf) == 0) {
            DEBUG_LOG_STR(UART0, "Message retrieved: ");
            
            // 5. Print the result directly as a string
            DEBUG_LOG_STR(UART0, (char *)read_buf); 
            DEBUG_LOG_STR(UART0, "\n\r");
        }
    }
    
    DEBUG_LOG_STR(UART0, "-------------------------------\n\r");
}

void load_payload_to_sram(unsigned int *destination) {
    unsigned int start_sector = 1024; // THE GPT-SAFE OFFSET
    unsigned int total_sectors = 160;  // 60KB (Safe margin for SPL)
    int sectors_read = 0;
    int kb_loaded = 0;

    DEBUG_LOG_STR(UART0, "\n\r--- PAYLOAD LOADER START ---\n\r");
    DEBUG_LOG_STR(UART0, "Loading 60KB to SRAM Address: 0x");
    DEBUG_LOG_HEX(UART0, (unsigned int)destination);
    DEBUG_LOG_STR(UART0, "\n\r");

    DEBUG_LOG_STR(UART0, "Waiting for card readiness...\n\r");
    while(!(HWREG(SOC_MMCHS_0_REGS + MMCHS_PSTATE) & (1 << 20))); // Wait for DAT0 to go high (Not Busy)

    for (sectors_read = 0; sectors_read < total_sectors; sectors_read++) {
        // Read one sector into the current destination pointer
        if (mmc_read_sector(start_sector + sectors_read, destination) != 0) {
            DEBUG_LOG_STR(UART0, "\n\rFATAL: SD Read Error at Sector ");
            // UARTPutInt(UART0, start_sector + sectors_read);
            return;
        }

        // Increment destination by 128 words (512 bytes)
        destination += 128;

        // Progress Print: Every 2 sectors = 1KB
        if ((sectors_read + 1) % 2 == 0) {
            kb_loaded++;
            DEBUG_LOG_STR(UART0, "#"); // Visual progress bar
            if (kb_loaded % 10 == 0) {
                DEBUG_LOG_STR(UART0, " [");
                // UARTPutInt(UART0, kb_loaded);
                DEBUG_LOG_STR(UART0, "KB Loaded]\n\r");
            }
        }
    }

    DEBUG_LOG_STR(UART0, "\n\r--- LOAD COMPLETE: 60KB in SRAM ---\n\r");
}

void verify_spl_in_sram(void) {
    // This must match the destination used in load_payload_to_sram
    unsigned int *spl_mem = (unsigned int *)0x402F0400; 
    
    DEBUG_LOG_STR(UART0, "\n\r--- SRAM PAYLOAD INTEGRITY CHECK ---\n\r");
    DEBUG_LOG_STR(UART0, "Inspecting SRAM at 0x402F0400...\n\r");

    // Print 8 words (32 bytes)
    for(int i = 0; i < 8; i++) {
        // Print the address for clarity every 4 words
        if (i % 4 == 0) {
            DEBUG_LOG_STR(UART0, "\n\r0x");
            DEBUG_LOG_HEX(UART0, (unsigned int)&spl_mem[i]);
            DEBUG_LOG_STR(UART0, ": ");
        }

        DEBUG_LOG_HEX(UART0, spl_mem[i]);
        DEBUG_LOG_STR(UART0, " ");
    }
    
    DEBUG_LOG_STR(UART0, "\n\r\n\rChecking for ARM Branch (0xEAxxxxxx)... ");
    
    // Simple logic check: Most SPLs start with an ARM Branch instruction
    if ((spl_mem[0] & 0xFF000000) == 0xEA000000) {
        DEBUG_LOG_STR(UART0, "VALID\n\r");
    } else {
        DEBUG_LOG_STR(UART0, "INVALID/EMPTY\n\r");
    }
    
    DEBUG_LOG_STR(UART0, "------------------------------------\n\r");
}

void mmcsdInits(){
    basicCLKMMC();
    
    setupMMCLines();
    //lets see if sd card there physically
    //mmcsd_Presence_Poll();
    //init the controller and send cmd0
    SDControllerInit(SOC_MMCHS_0_REGS);
    
    //at this point our sd card is alive and mux,clk settings are correct
    mmc_init_to_transfer();

    //verify_sd_string_test();

    unsigned int *payload_address = (unsigned int *)0x402F0400;

    load_payload_to_sram(payload_address);

    verify_spl_in_sram();

    //SDMMCgoIdle();//lets idle our sd card

    /*for(;;) {
        mmcsd_Presence_Poll();
        
        // Short delay so you don't flood the CPU
        for(volatile int i = 0; i < 100000; i++);
    }*/
    
    


}

