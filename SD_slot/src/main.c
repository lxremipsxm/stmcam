/*Siddharth Vasudevan
SD_slot/src/main.c

This code contains test code for the SD card slot

Comments
--------
7/2/25: Created file
*/

#include "stm32f4xx.h"
#include "lib/delays.h"
#include "lib/uart.h"


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