#include <stdint.h>
#include "hw_types.h"
#include "gpio/gpio.h"
#include "nonstrware/UART.h"
#include "mmcapp/mmcsd_app.h"
#include "mmcsd/mmcsd.h"
#include "mmcsdlib/mmc_library.h"
#include "mmcsdlib/mmc_proto.h"
#include "soc/soc_AM335x.h"

#define GPIO1_BASE		0x4804C000

#define CM_PER_BASE		0x44e00000
#define CM_PER_GPIO1		0xAC

#define CM_PER_MMC0 0x3C

#define TIME 500000

struct omap_boot_parameters {
    unsigned int reserved;               // Offset 0
    unsigned int boot_device_descriptor;  // Offset 4
    unsigned char boot_device;           // Offset 8 (The "5" is here!)
    unsigned char reset_reason;          // Offset 9
    unsigned char padding[2];            // Alignment padding
};

static unsigned int saved_rom_ptr = 0;

/* Linker script symbols */
extern uint32_t __bss_start;
extern uint32_t __bss_end;

void system_init(void) {
    /* 1. Clear the .bss section (Zero-initialize global variables) */
    uint32_t *bss = &__bss_start;
    while (bss < &__bss_end) {
        *bss++ = 0;
    }
}

void jump_to_spl(void) {
    UARTPuts(UART0, "\n\rPreparing for Handoff...\n\r");

    // Disable SD interrupts 
    HWREG(SOC_MMCHS_0_REGS+MMCHS_IE) = 0;
    HWREG(SOC_MMCHS_0_REGS + MMCHS_ISE) = 0;

    UARTPuts(UART0, "Jumping to SPL at 0x402F0400. See you on the other side!\n\r");

    // 3. Wait for UART to finish printing last byte
    // 0x20 is the Transmit Empty bit in LSR
    //while (!(HWREG(SOC_UART_0_REGS + 0x14) & 0x20));

    // 4. THE JUMP
    //put the ptr to r0 again
    unsigned int spl_entry = 0x402F0400;

    asm volatile(
        "mov r0, %0\n"    // Move saved_rom_ptr into r0
        "bx %1\n"         // Jump to 0x402F0400
        : 
        : "r" (saved_rom_ptr), "r" (spl_entry)
        : "r0"            // Tell the compiler we are manualy touching r0
    );
    for(;;); 
}

// Dummy raise() function to satisfy libgcc's division-by-zero handler
int raise(int signum) {
    // If division by zero happens, we just hang the CPU infinitely.
    for(;;); 
    return 0;
}

// Define a dummy struct
struct MyDriverData {
    uint32_t reg1;
    uint32_t reg2;
    uint32_t reg3;
    uint32_t reg4;
};

#define LED_MASK (15 << 21)

void print_rom_report(unsigned int ptr) {
    struct omap_boot_parameters *params = (struct omap_boot_parameters *)ptr;

    DEBUG_LOG_STR(UART0, "\r\n--- SHIM BOOT AUDIT ---\r\n");
    
    DEBUG_LOG_STR(UART0, "R0 (Pointer): ");
    DEBUG_LOG_HEX(UART0, ptr);
    
    // Check if the pointer is in the valid internal SRAM range
    if (ptr < 0x402F0000 || ptr > 0x40310000) {
        DEBUG_LOG_STR(UART0, " [!] INVALID RANGE");
    }

    DEBUG_LOG_STR(UART0, "\r\nBoot Device: ");
    DEBUG_LOG_HEX(UART0, params->boot_device); // Expect 0x5 for MMC1
    
    DEBUG_LOG_STR(UART0, "\r\nBoot Mode:   ");
    DEBUG_LOG_HEX(UART0, params->reset_reason);   // 1 = RAW, 2 = FAT
    
    DEBUG_LOG_STR(UART0, "\r\nDescriptor:  ");
    DEBUG_LOG_HEX(UART0, params->boot_device_descriptor);
    
    DEBUG_LOG_STR(UART0, "\r\n-----------------------\r\n");
}
/*
void _main (void)
{

	// 1. Force a division (libgcc will handle this successfully)
	volatile uint32_t a = 1000;
	volatile uint32_t b = 3;
	volatile uint32_t result = a / b; 

// 2. Force GCC to secretly inject 'memset'
	struct MyDriverData data1 = {0}; 

// 3. Force GCC to secretly inject 'memcpy'
	struct MyDriverData data2 = data1;

    HWREG(CM_PER_BASE + CM_PER_GPIO1) = (1 << 18) | 0x2;
    HWREG(GPIO1_BASE + GPIO_OE) &= ~(15 << 21);

    for(;;)
    {
        // 3. Set LEDs High
        HWREG(GPIO1_BASE + GPIO_SETDATAOUT) = (15 << 21);
        
        // Simple delay loop
        for(volatile unsigned int i = 0; i < TIME; i++);

        // 4. Clear LEDs Low
        HWREG(GPIO1_BASE + GPIO_CLRDATAOUT) = (15 << 21);
        
        for(volatile unsigned int i = 0; i < TIME; i++);
    }
}*/

void verify_relocation(void) {
    unsigned int current_pc;
    char hex_buffer[11]; // To hold "0xXXXXXXXX\0"

    // 1. Grab the actual PC using inline assembly
    asm volatile ("mov %0, pc" : "=r" (current_pc));

    // 2. Print the status
    DEBUG_LOG_STR(UART0, "\r\n--- MLO RELOCATION REPORT ---\r\n");
    DEBUG_LOG_STR(UART0, "Current Execution Address (PC): ");
    
    // Use your existing hex printing function here
    // (Assuming you have one, or just print the raw check)
    if (current_pc >= 0x40308000) {
        DEBUG_LOG_STR(UART0, "0x40308XXX [SUCCESS: HIGH SRAM]\r\n");
    } else {
        DEBUG_LOG_STR(UART0, "0x402F0XXX [FAILED: STILL IN BOOT ZONE]\r\n");
    }
    DEBUG_LOG_STR(UART0, "------------------------------\r\n");
}

void _main (unsigned int rom_ptr)
{
    //save that r0 value and print it out in here
    saved_rom_ptr = rom_ptr;

	UART_initUART(UART0, 115200, STOP1, PARITY_NONE, FLOW_OFF);

    verify_relocation();

    print_rom_report(saved_rom_ptr);

    mmcsdInits();

    jump_to_spl();

    //hand off to spl by now the spl is loaded into sram    
}
