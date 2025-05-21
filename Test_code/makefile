# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Project files
SRCS = main.c startup_stm32f401xe.c system_stm32f4xx.c
INCLUDES = -I. -ICMSIS

# Linker script
LDSCRIPT = linker_script.ld

# Compiler flags
CFLAGS = -mcpu=cortex-m4 -mthumb -std=gnu11 -O2 -Wall -ffunction-sections -fdata-sections \
         -g -DSTM32F401xE $(INCLUDES)

# Linker flags
LDFLAGS = -T$(LDSCRIPT) -mcpu=cortex-m4 -mthumb -nostartfiles -Wl,--gc-sections

# Output
TARGET = firmware

# Default target
all: $(TARGET).bin

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link the final ELF executable
$(TARGET).elf: $(SRCS:.c=.o) $(LDSCRIPT)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

# Convert ELF to binary for flashing
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Clean build files
clean:
	rm -f *.o *.elf *.bin

.PHONY: all clean
