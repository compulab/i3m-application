# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./twi/eeprom.c \
./twi/eeprom_driver.c \
./twi/sram_handle.c \
./twi/twi_master.c \
./twi/twi_slave.c

OBJS += \
./twi/eeprom.o \
./twi/eeprom_driver.o \
./twi/sram_handle.o \
./twi/twi_master.o \
./twi/twi_slave.o

C_DEPS += \
./twi/eeprom.d \
./twi/eeprom_driver.d \
./twi/sram_handle.d \
./twi/twi_master.d \
./twi/twi_slave.d


# Each subdirectory must supply rules for building sources it contributes
twi/%.o: twi/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
