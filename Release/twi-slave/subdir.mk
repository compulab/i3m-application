################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../twi-slave/sp_driver.s 

C_SRCS += \
../twi-slave/eeprom_driver.c \
../twi-slave/twi-memory.c \
../twi-slave/twi-xmega.c 

OBJS += \
./twi-slave/eeprom_driver.o \
./twi-slave/sp_driver.o \
./twi-slave/twi-memory.o \
./twi-slave/twi-xmega.o 

S_DEPS += \
./twi-slave/sp_driver.d 

C_DEPS += \
./twi-slave/eeprom_driver.d \
./twi-slave/twi-memory.d \
./twi-slave/twi-xmega.d 


# Each subdirectory must supply rules for building sources it contributes
twi-slave/%.o: ../twi-slave/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega64a3u -DF_CPU=2000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

twi-slave/%.o: ../twi-slave/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atxmega64a3u -DF_CPU=2000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


