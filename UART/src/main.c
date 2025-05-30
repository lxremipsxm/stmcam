/*Siddharth Vasudevan
USART/src/main.c

Test code for uart.h and delays.h

Comments
--------
5/30/25: Updating implementation of delays.h using TIM2 of STM32F401RE
*/

#include "stm32f4xx.h"
#include "delays.h"
#include "uart.h"


void main(void){
    uart_com_init();
    delay_init();

    while(1){

        char strs[3][10] = {"interior", "crocodile", "alligator"};
        for (int i=0; i<3; i++){
            send_com_string(strs[i]);
            send_com_string("\r\n");
            delay_ms(500);
        }
        
    }
}