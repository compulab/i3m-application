################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ASF/common/services/ioport/xmega/ioport_compat.c 

OBJS += \
./ASF/common/services/ioport/xmega/ioport_compat.o 

C_DEPS += \
./ASF/common/services/ioport/xmega/ioport_compat.d 


# Each subdirectory must supply rules for building sources it contributes
ASF/common/services/ioport/xmega/%.o: ../ASF/common/services/ioport/xmega/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega64a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


