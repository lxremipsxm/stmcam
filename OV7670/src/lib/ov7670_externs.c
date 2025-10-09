/*Siddharth Vasudevan
OV7670/src/lib/ov7670_externs.c

Contains external functions required by ov7670.c

Comments
--------
6/10/25: Created file. No contents as of now.
10/9/25: Added some functionality
*/

#include <stdint.h>
#include "src/lib/uart.h"

void OV7670_print(char *str){
    send_com_string(str);
}


int OV7670_read_register(void *platform, uint8_t reg){
    

}


void OV7670_write_register(void *platform, uint8_t reg, uint8_t value){


}