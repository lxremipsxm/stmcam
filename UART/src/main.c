/*Siddharth Vasudevan
UART/src/main.c
*/

#include "stm32f4xx.h"
#include <stdint.h> 

void delay(volatile uint32_t count) {
    while (count--) {
        __asm__("nop"); //no operation on current clock cycle
    }
}


void setup_uart(void) {    
//Set up peripheral clock on GPIO A
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

//Enable USART1 clock
RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

//Enable alternate function (USART) on GPIO
GPIOA->MODER |= ((0x2) << (9*2)); //Selects Alternate function mode (10 * 2^18)
GPIOA->AFR[1] |= (0x7 << (9-8)*4); //Selects Alternate function 7, USART1

//Program M bit
USART1->CR1 |= USART_CR1_M;

//Set baud rate with BRR register
USART1->BRR = 0x683;

//Enable USART with UE bit
USART1->CR1 |= USART_CR1_UE;

//Set TE in CR1 to send idle frame as first transmission
USART1->CR1 |= USART_CR1_TE;

// In send_data(), write data to DR register (clears TXE register)
// wait till TXE = 1 
}

int main(void){
    setup_uart();
}