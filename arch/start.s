.globl _start
.equ CM_PER_GPIO1_CLKCTRL, 0x44e000AC
.equ GPIO1_OE, 0x4804C134
.equ GPIO1_SETDATAOUT, 0x4804C194
.equ GPIO1_CLEARDATAOUT, 0x4804C190
.equ SYS_INIT_SP_ADDR, 0x4030AC00

_start:
    mov r7, r0 
    mrs r0, cpsr
    bic r0, r0, #0x1F 
    orr r0, r0, #0x13 
    orr r0, r0, #0xC0 
    msr cpsr, r0

    mrc p15,0,r0,c1,c0,2    
    orr r0,r0,#0x00F00000   
    mcr p15,0,r0,c1,c0,2    
    isb                     
    mov r0,#0x40000000      
    vmsr fpexc,r0           

    /* --- 1. SET UP STACK EARLY --- */
    /* We must do this before blinking so we can save registers */
    ldr r0, =SYS_INIT_SP_ADDR
    bic r0, r0, #7            
    mov sp, r0                

    /* --- 2. SAVE ROM REGISTERS --- */
    /* Protect everything the Boot ROM left behind */
    push {r0-r12} 

    /* --- 3. ASSEMBLY BLINK --- */
    ldr r0, =CM_PER_GPIO1_CLKCTRL
    mov r1, #0x02
    str r1, [r0]

    mov r1, #100
wait_clk:
    subs r1, r1, #1
    bne wait_clk

    ldr r0, =GPIO1_OE
    ldr r1, [r0]
    bic r1, r1, #(1 << 21)
    str r1, [r0]

    mov r3, #5
asm_blink:
    ldr r0, =GPIO1_SETDATAOUT
    mov r1, #(1 << 21)
    str r1, [r0]
    
    ldr r2, =0x00A00000
1:  subs r2, r2, #1
    bne 1b

    ldr r0, =GPIO1_CLEARDATAOUT
    str r1, [r0]

    ldr r2, =0x00A00000
2:  subs r2, r2, #1
    bne 2b

    subs r3, r3, #1
    bne asm_blink

    adr r4, _start            @ Source (Current PC-relative)
    ldr r5, =_start            @ Destination (Link address 0x4030...)
    ldr r6, =__bss_start
    sub r6, r6, r5            @ Size

copy_loop:
    ldr r7, [r4], #4
    str r7, [r5], #4
    subs r6, r6, #4
    bne copy_loop

    @ THE TELEPORT JUMP
    ldr r4, =relocate_done    @ Get the HIGH SRAM address of the label
    bx  r4                    @ Jump to the relocated code

relocate_done:
    @ Now we are running at 0x4030...

    /* --- 4. RESTORE REGISTERS & JUMP --- */
    /* The C code now sees the exact same register state as if the blink never happened */
    pop {r0-r12}

    mov r0, r7 /*now it should take our actual r0 value and shove it into r0*/
    
    bl _main

.loop: b .loop

/* ... (Your PUT32/GET32 functions remain exactly the same here) ... */
.globl PUT32
PUT32:
   str r1,[r0]
   bx lr
   
.globl GET32
GET32:
   ldr r0,[r0]
   bx lr
   
.globl PUT16
PUT16:
   strh r1,[r0]
   bx lr
   
.globl GET16
GET16:
   ldrh r0,[r0]
   bx lr
   
.globl PUT8
PUT8:
   strb r1,[r0]
   bx lr
   
.globl GET8
GET8:
   ldrb r0,[r0]
   bx lr