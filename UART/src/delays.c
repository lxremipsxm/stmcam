/*Siddharth Vasudevan
UART/src/delays.c

Comments:
5/29/25: Test successful, but delays not accurate at all. Will improve with STM32 TIM based delays
5/30/25: Recreating delay_ms with TIM2
*/

#include "stm32f4xx.h"
#include <stdint.h>
#include "delays.h"

void delay_init(){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void delay_us(volatile uint32_t usec){
    //Do nothing
}

void delay_ms(volatile uint32_t msec) {
    volatile long count = 16000*msec;
    while (count--){
        __asm__("nop"); //no operation on current cycle
    }
}

void delay_s(volatile uint32_t sec) {
    while(sec--){
        delay_ms(1000);
    }
}
