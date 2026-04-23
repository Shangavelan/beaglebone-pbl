#ifndef TIMER_H
#define TIMER_H

#include "types.h"

/* DMTimer2 Base Address */
#define DMTIMER2_BASE          0x48040000

/* Clock Module (PRCM) Registers for Timer2 */
#define CM_PER_TIMER2_CLKCTRL  0x44E00080
#define CLKSEL_TIMER2_CLK      0x44E00508

/* DMTimer Register Offsets */
#define DMTIMER_TIDR           (DMTIMER2_BASE + 0x00)  /* Identification Register */
#define DMTIMER_TIOCP_CFG      (DMTIMER2_BASE + 0x10)  /* Configuration Register */
#define DMTIMER_IRQSTATUS      (DMTIMER2_BASE + 0x28)  /* Interrupt Status Register */
#define DMTIMER_IRQENABLE_SET  (DMTIMER2_BASE + 0x2C)  /* Interrupt Enable Register */
#define DMTIMER_TCLR           (DMTIMER2_BASE + 0x24)  /* Control Register */
#define DMTIMER_TCRR           (DMTIMER2_BASE + 0x3C)  /* Counter Register */
#define DMTIMER_TLDR           (DMTIMER2_BASE + 0x40)  /* Load Register */
#define DMTIMER_TTGR           (DMTIMER2_BASE + 0x44)  /* Trigger Register */

/* Function Prototypes */
void TIMER2_init(void);
void TIMER2_reset(void);
uint32_t TIMER2_get_count(void);
void delay_ms(uint32_t ms);

#endif
