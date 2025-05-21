#include <stdint.h>

/* Symbols defined in the linker script */
extern uint32_t _etext;   // End of .text, start of .data load address in Flash
extern uint32_t _sdata;   // Start of .data in RAM
extern uint32_t _edata;   // End of .data in RAM
extern uint32_t _sbss;    // Start of .bss
extern uint32_t _ebss;    // End of .bss
extern uint32_t _estack;  // Top of stack (set in vector table)

/* Forward declaration of main */
int main(void);

/* Weak aliases for default handlers */
void Default_Handler(void);
void Reset_Handler(void);

void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

/* Interrupt vector table */
__attribute__((section(".isr_vector")))
const void *vector_table[] = {
    (void *)&_estack,         // Initial stack pointer
    Reset_Handler,            // Reset handler
    NMI_Handler,              // NMI
    HardFault_Handler,        // Hard Fault
    MemManage_Handler,        // Memory Management
    BusFault_Handler,         // Bus Fault
    UsageFault_Handler,       // Usage Fault
    0, 0, 0, 0,               // Reserved
    SVC_Handler,              // SVCall
    0,                        // Reserved for Debug
    0,                        // Reserved
    PendSV_Handler,           // PendSV
    SysTick_Handler,          // SysTick
    // ... additional IRQ handlers (fill out as needed)
};

/* Reset handler */
void Reset_Handler(void) {
    uint32_t *src, *dst;

    // Copy .data from FLASH to RAM
    src = &_etext;
    dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // Zero-initialize .bss section
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    // Call main (should never return)
    main();

    // If main returns, trap forever
    while (1);
}

/* Default interrupt handler */
void Default_Handler(void) {
    while (1); // Hang forever if an unhandled interrupt occurs
}
