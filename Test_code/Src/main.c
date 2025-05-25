/*Siddharth Vasudevan
Test_code/src/main.c
*/

#include "stm32f4xx.h"
#include <stdint.h> 

void delay(volatile uint32_t count) {
    while (count--) {
        __asm__("nop"); //no operation on current clock cycle
    }
}

int main(void) {
    //Enable GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    //Config PA5 as output
    GPIOA->MODER &= ~(3UL << (5 * 2));   
    GPIOA->MODER |=  (1UL << (5 * 2)); 

    while (1) {
        // Toggle PA5
        GPIOA->ODR ^= (1 << 5);
        delay(1000000);
    }
}