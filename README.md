# Camera with STM32 and OV7670

## Getting Started

I wanted to start out by creating a simple UART script to flash to my STM32. This would give me an introduction to the compilation tools I would need, help me create a simple user-defined UART library I could then use to debug later on, and also give me an idea of how my makefiles should look for future updates. I also will avoid installing the STM32Cube IDE, since I want to work more bare-metal, and try to handle memory and registers myself.


### Progression

#### arm-none-eabi toolchain - 5/20/25

Downloaded the .exe file off the ARM official downloads page. Took incredibly long despite being only 203MB. 

Installed with no issues. Also added to PATH and verified using arm-none-eabi-gcc --version.


#### Open On-Chip Debugger (OpenOCD)

Downloaded and installed, no hiccups. Added to PATH.

#### ST-Link - 5/25/25

Installed with no issues.

#### Dependencies

I grabbed a couple files from the [ST electronics github repository](https://github.com/STMicroelectronics/STM32CubeF4) in order to satisfy dependencies and simplify working with registers on the STM32, and then rearranged my project directory so that it’s more organized. The file organization will look about the same for different stages of the project, give or take some additional libraries, but it makes it easier to work with since I won’t have to modify the makefile much at all for most parts of the project.

So far, the files are organized like this:
Camera/
	Test_code/
		inc/
		src/
		startup/
		system/
		linker/
		makefile



startup/: contains startup_stm32f401xe.s. This preps the board for running the main code
system/: contains system_stm32f4xx.c. It defines the clock source and vector table location.
linker/ contains the linker file, linker.ld
src/ contains my source code, main.c, and library files, which I will put in a subdirectory called lib/
inc/: contains the following, which are dependencies required by system_stm32f4xx.c.

![inc_filestructure.png|500]


	



