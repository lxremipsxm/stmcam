/*Siddharth Vasudevan
OV7670/ov7670_stm32.c

This file provides architecture specific data so that the ov7670 can be interfaced
with my STM32F401RE. I've named the file ov7670_stm32.c (as opposed to just stm32f401re.c, or something similar)
only to avoid file name similarities between this file and the other dependencies I need to compile 
and flash code to the board. Additionally, I want it to be clear that this is not one of the main dependencies
for having code compile correctly for the board, but rather a requirement for the ov7670 to work.
This code will likely only work with stm32f401xe boards. For now, I will work in the main project directory, 
but I will move it into inc/ later.

Comments
--------
6/10/25: Created file

License and Credit
------------------
SPDX-FileCopyrightText: 2020 P Burgess for Adafruit Industries
SPDX-License-Identifier: MIT
*/

#include "inc/stm32f4xx.h"
#if defined(STM32F4)
#include "ov7670.h"
#include "src/lib/delays.h"
#include "pinmap.h"
#endif




/*
Additional methods I must define(ensure they are defined before the two methods above):
- OV7670_delay_ms(x)
- OV7670_pin_output(pin)
- OV7670_pin_write(pin, hi)
- OV7670_disable_interrupts()
- OV7670_enable_interrupts()*/



void OV7670_delay_ms(int x){
  //Assume delay_init() is already called. Ensure this is done in the main file.
  delay_ms(x);
}


void OV7670_pin_output(int pin){
  // sets a pin to output
  // Pins on OV7670 (Total 18)
  // -------------
  // Digital: {D0,...,D7}, I2C: {SCL, SDA}, Clock in: {XCLK}
  // Clock out: {PLK}, Power: {3.3V, DGND, PWDN}, Reset: {RET}
  // Sync: {VS, HS}
  //
  // Need a timer/PWM pin for XCLK, 8 digital pins, Default I2C, unsure about clock out for now
  // Power is obvious, but one GPIO should be dedicated to PWDN and RET each
  // In all, it seems I need 10 GPIOs, 8 input from OV7670 and 2 output to OV7670. Then only 8 pins are required for this function.

  // Looking at the OV7670_pins (unfortunately unnamed) type in ov7670.h, I need to worry about {enable, reset, xclk, pclk, vsync, hsync,
  // data[8], sda, and scl}. This represents all the pins minus the power pins. However, the only actual pins that
  // ever need output functionality are the reset and enable pins, so I can be pretty lax about how I do this.
  // I'm thinking a boolean-eque either-this-or-that kind of output setting, and do the same for pin_write.

  
  

}


void OV7670_pin_write(int pin, int hi){

  //Again, since the only pins that are declared output are enable and reset, this will be
  //quite straightforward.

}



//Since I'm avoiding the video functionality and I'm doing this in serial, I'm not fully sure 
//yet whether or not I will need these two, especially since I will most likely not use 
//the original author's OV7670_capture() method.
void OV7670_disable_interrupts(void);


void Ov7670_enable_interrupts(void);


OV7670_status OV7670_arch_begin(OV7670_host *host) {

    //Setup timer as required: This is not flexible, and will be defined for a single timer
    //as that is all I need to supply to XCLK

    //The code for the SAMD5 uses PCC, or parallel capture controller. I am not aware of whether 
    //STM32F01RE has a PCC equivalent, so I will do some more digging to find an equivalent or alternative
    //Update: No such alternative. Unfortunately, I'll have to drop the frame rate/quality and construct the 
    //image after serial transfers. 

    //return OV7670_STATUS_OK; OR OV7670_STATUS_ERR_PERIPHERAL;
}


void OV7670_capture(uint32_t *dest, uint16_t width, uint16_t height,
                    volatile uint32_t *vsync_reg, uint32_t vsync_bit,
                    volatile uint32_t *hsync_reg, uint32_t hsync_bit) {

    //This sends the picture over PCC for the SAMD5

    //I should be able to try using serial over the OV7670 output pins if possible.
    //I may have to redefine the params for this function in order for it to work
                    

}






/*-------------------Reference and og code below this line--------------------------------- */



// This is the SAMD51-specific parts of OV7670 camera interfacing (device-
// agnostic parts are in ov7670.c). It configures and accesses hardware-
// specific peripherals (timer PWM and the parallel capture controller).
// It is mostly, but not entirely, platform agnostic. If compiling for the
// Arduino platform, the pin MUXing function pinPeripheral() is used, and
// other platforms will require different code or function calls in those
// spots (commented throughout). Unlike some of the trivially-mapped
// functions declared at the start of ov7670.h (e.g. OV7670_delay_ms()),
// pinPeripheral() AND ESPECIALLY ITS ARGUMENTS are innately SAMD51-
// Arduino-centric and it doesn't make sense to try to build a whole
// platform-agnostic remap around that...instead, call or implement pin
// MUXing functions "manually" in those few spots.


#include "inc/stm32f4xx.h"
#if defined(STM32F4)
#include "ov7670.h"

#if defined(ARDUINO)
#include "wiring_private.h" // pinPeripheral() function
#endif

// Each supported architecture MUST provide this function with this name,
// arguments and return type. It receives a pointer to a structure with
// at least a list of pins, and usually additional device-specific data
// attached to the 'arch' element.
// SAMD51 host config sets up timer and parallel capture peripheral,
// as these are mostly low-level register twiddles. It does NOT set up
// DMA transfers, handled in higher-level calling code if needed.
OV7670_status OV7670_arch_begin(OV7670_host *host) {

  // LOOK UP TIMER OR TCC BASED ON ADDRESS IN HOST STRUCT ------------------

  static const struct {
    void *base;       ///< TC or TCC peripheral base address
    uint8_t GCLK_ID;  ///< Timer ID for GCLK->PCHCTRL
    const char *name; ///< Printable timer ID for debug use
  } timer[] = {
#if defined(TC0)
    {TC0, TC0_GCLK_ID, "TC0"},
#endif
#if defined(TC1)
    {TC1, TC1_GCLK_ID, "TC1"},
#endif
#if defined(TC2)
    {TC2, TC2_GCLK_ID, "TC2"},
#endif
#if defined(TC3)
    {TC3, TC3_GCLK_ID, "TC3"},
#endif
#if defined(TC4)
    {TC4, TC4_GCLK_ID, "TC4"},
#endif
#if defined(TC5)
    {TC5, TC5_GCLK_ID, "TC5"},
#endif
#if defined(TC6)
    {TC6, TC6_GCLK_ID, "TC6"},
#endif
#if defined(TC7)
    {TC7, TC7_GCLK_ID, "TC7"},
#endif
#if defined(TC8)
    {TC8, TC8_GCLK_ID, "TC8"},
#endif
#if defined(TC9)
    {TC9, TC9_GCLK_ID, "TC9"},
#endif
#if defined(TC10)
    {TC10, TC10_GCLK_ID, "TC10"},
#endif
#if defined(TC11)
    {TC11, TC11_GCLK_ID, "TC11"},
#endif
#if defined(TC12)
    {TC12, TC12_GCLK_ID, "TC12"},
#endif
    {NULL, 0, NULL}, // NULL separator between TC and TCC lists
#if defined(TCC0)
    {TCC0, TCC0_GCLK_ID, "TCC0"},
#endif
#if defined(TCC1)
    {TCC1, TCC1_GCLK_ID, "TCC1"},
#endif
#if defined(TCC2)
    {TCC2, TCC2_GCLK_ID, "TCC2"},
#endif
#if defined(TCC3)
    {TCC3, TCC3_GCLK_ID, "TCC3"},
#endif
#if defined(TCC4)
    {TCC4, TCC4_GCLK_ID, "TCC4"},
#endif
  };

  uint8_t timer_list_index;
  bool is_tcc = false; // Initial part of list is timer/counters, not TCCs
  // Scan timer[] list until a matching timer/TCC is found...
  for (timer_list_index = 0;
       (timer_list_index < sizeof timer / sizeof timer[0]) &&
       (timer[timer_list_index].base != host->arch->timer);
       timer_list_index++) {
    if (!timer[timer_list_index].base) { // NULL separator?
      is_tcc = true; // In the TCC (not TC) part of the list now
    }
  }
  if (timer_list_index >= sizeof timer / sizeof timer[0]) {
    return OV7670_STATUS_ERR_PERIPHERAL; // No matching TC/TCC found
  }

  // CONFIGURE TIMER FOR XCLK OUT ------------------------------------------

  uint8_t id = timer[timer_list_index].GCLK_ID;

  // Route timer's peripheral channel control to GCLK1 (48 MHz)

  GCLK->PCHCTRL[id].bit.CHEN = 0;    // Peripheral channel disable
  while (GCLK->PCHCTRL[id].bit.CHEN) // Wait for disable
    ;
  // Select generator 1, enable channel, use .reg so it's an atomic op
  GCLK->PCHCTRL[id].reg = GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN;
  while (!GCLK->PCHCTRL[id].bit.CHEN) // Wait for enable
    ;

  if (is_tcc) { // Is a TCC peripheral

    Tcc *tcc = (Tcc *)timer[timer_list_index].base;

    tcc->CTRLA.bit.ENABLE = 0; // Disable TCC before configuring
    while (tcc->SYNCBUSY.bit.ENABLE)
      ;
    tcc->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV1_Val; // 1:1 Prescale
    tcc->WAVE.bit.WAVEGEN = TCC_WAVE_WAVEGEN_NPWM_Val;       // Normal PWM
    while (tcc->SYNCBUSY.bit.WAVE)
      ;

    uint16_t period = 48000000 / OV7670_XCLK_HZ - 1;
    tcc->PER.bit.PER = period;
    while (tcc->SYNCBUSY.bit.PER)
      ;
    tcc->CC[1].bit.CC = (period + 1) / 2; // Aim for ~50% duty cycle
    while (tcc->SYNCBUSY.bit.CC1)
      ;
    tcc->CTRLA.bit.ENABLE = 1;
    while (tcc->SYNCBUSY.bit.ENABLE)
      ;

#if defined(ARDUINO)
    pinPeripheral(host->pins->xclk,
                  host->arch->xclk_pdec ? PIO_TCC_PDEC : PIO_TIMER_ALT);
#else
      // CircuitPython, etc. pin mux here
#endif

  } else { // Is a TC peripheral

    Tc *tc = (Tc *)timer[timer_list_index].base;

    // TO DO: ADD TC PERIPHERAL (NOT TCC) CODE HERE

#if defined(ARDUINO)
    pinPeripheral(host->pins->xclk, PIO_TIMER);
#else
    // CircuitPython, etc. pin mux here
#endif

  } // end TC/TCC

  // SET UP PCC PERIPHERAL -------------------------------------------------

  PCC->MR.bit.PCEN = 0; // Make sure PCC is disabled before setting MR reg

  PCC->IDR.reg = 0b1111;       // Disable all PCC interrupts
  MCLK->APBDMASK.bit.PCC_ = 1; // Enable PCC clock

  // Set up pin MUXes for the camera clock, sync and data pins.
  // I2C pins are already configured, XCLK was done above, and
  // the reset and enable pins are handled in the calling code.
  // Must do this before setting MR reg.
#if defined(ARDUINO)
  // PCC pins are set in stone on SAMD51
  pinPeripheral(PIN_PCC_CLK, PIO_PCC);
  pinPeripheral(PIN_PCC_DEN1, PIO_PCC); // VSYNC
  pinPeripheral(PIN_PCC_DEN2, PIO_PCC); // HSYNC
  pinPeripheral(PIN_PCC_D0, PIO_PCC);
  pinPeripheral(PIN_PCC_D1, PIO_PCC);
  pinPeripheral(PIN_PCC_D2, PIO_PCC);
  pinPeripheral(PIN_PCC_D3, PIO_PCC);
  pinPeripheral(PIN_PCC_D4, PIO_PCC);
  pinPeripheral(PIN_PCC_D5, PIO_PCC);
  pinPeripheral(PIN_PCC_D6, PIO_PCC);
  pinPeripheral(PIN_PCC_D7, PIO_PCC);
#else
  // CircuitPython, etc. pin mux here
#endif

  // Accumulate 4 bytes into RHR register (two 16-bit pixels)
  PCC->MR.reg = PCC_MR_CID(0x1) |   // Clear on falling DEN1 (VSYNC)
                PCC_MR_ISIZE(0x0) | // Input data bus is 8 bits
                PCC_MR_DSIZE(0x2);  // "4 data" at a time (accumulate in RHR)

  PCC->MR.bit.PCEN = 1; // Enable PCC

  return OV7670_STATUS_OK;
}

// Non-DMA capture function using previously-initialized PCC peripheral.
void OV7670_capture(uint32_t *dest, uint16_t width, uint16_t height,
                    volatile uint32_t *vsync_reg, uint32_t vsync_bit,
                    volatile uint32_t *hsync_reg, uint32_t hsync_bit) {

  while (*vsync_reg & vsync_bit)
    ; // Wait for VSYNC low (frame end)
  OV7670_disable_interrupts();
  while (!*vsync_reg & vsync_bit)
    ; // Wait for VSYNC high (frame start)

  width /= 2;                             // PCC receives 2 pixels at a time
  for (uint16_t y = 0; y < height; y++) { // For each row...
    while (*hsync_reg & hsync_bit)
      ; //  Wait for HSYNC low (row end)
    while (!*hsync_reg & hsync_bit)
      ;                               //  Wait for HSYNC high (row start)
    for (int x = 0; x < width; x++) { //   For each column pair...
      while (!PCC->ISR.bit.DRDY)
        ;                     //    Wait for PCC data ready
      *dest++ = PCC->RHR.reg; //    Store 2 pixels
    }
  }

  OV7670_enable_interrupts();
}

// DEVICE-SPECIFIC FUNCTIONS FOR NON-ARDUINO PLATFORMS ---------------------

// If a platform doesn't offer a device-agnostic function for certain
// operations (i.e. can't be trivially remapped in ov7670.h as is done for
// Arduino), those device-specific functions can be declared here, with a
// platform-specific #ifdef around them. These functions may include:
// OV7670_delay_ms(x)
// OV7670_pin_output(pin)
// OV7670_pin_write(pin, hi)
// OV7670_disable_interrupts()
// OV7670_enable_interrupts()
// Also see notes at top regarding pin MUXing in this file.

#endif // end __SAMD5__

// Notes from past self: early version of this code that I adopted used
// GCLK5 to provide the XCLK signal to the camera, and was written before
// the Grand Central SAMD51 Arduino definition was fully formed. GCLK5
// became vital for other purposes (looks like it feeds PLLs that time
// nearly everything else). Fortunately there's a timer peripheral (TCC1)
// also available on that same pin, so that's what's now used to provide
// PCC_XCLK. The choice of pin/timer for that will likely be different
// for other SAMD51 boards (e.g. Feather, ItsyBitsy), but as written here
// must be a TC or TCC, not a GCLK source. Also: early code required that
// the cache be disabled. That no longer seems to be necessary, PCC and
// related code runs fine even with cache.

