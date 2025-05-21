/* STM32F401RE (512KB Flash, 96KB RAM) 

Keywords: 
_etext: end of .text section
_sdata: start of .data in RAM
_edata: End of .data in RAM
_sbss: start of .bss in RAM
_ebss: end of .bss in RAM
_estack: top of stack

*/

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K /* Flash: read + exec, stores code, const data, and .data load image */
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 96K  /* RAM: read + write + exec, stores stack, heap, and variables */

}

SECTIONS
{
  .text : 
  {
    KEEP(*(.isr_vector))      /* Interrupt vector table, startup handlers*/
    *(.text*)                 /* Includes all program code */
    *(.rodata*)               /* Includes read-only constants */
    _etext = .;               /* Defines where .data starts in FLASH */
  } > FLASH

  .data : AT (ADDR(.text) + SIZEOF(.text))
  {
    _sdata = .;               /* Start of initialized data in RAM */
    *(.data*)                 /* Holds initialized data(global/static variables) */
    _edata = .;               /* End address of initialized data in RAM */
  } > RAM

  .bss :
  {
    _sbss = .;                /* Start of uninitialized data in RAM */
    *(.bss*)                  /* Holds uninitialized global/static variables */
    *(COMMON)                 
    _ebss = .;                /* End of uninitialized data in RAM */
  } > RAM

  .stack (COPY) :
  {
    _estack = ORIGIN(RAM) + LENGTH(RAM);  /* Defines top of stack to top of RAM */
  } > RAM                     
}
