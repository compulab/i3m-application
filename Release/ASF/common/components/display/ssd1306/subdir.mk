################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ASF/common/components/display/ssd1306/font.c \
../ASF/common/components/display/ssd1306/ssd1306.c 

OBJS += \
./ASF/common/components/display/ssd1306/font.o \
./ASF/common/components/display/ssd1306/ssd1306.o 

C_DEPS += \
./ASF/common/components/display/ssd1306/font.d \
./ASF/common/components/display/ssd1306/ssd1306.d 


# Each subdirectory must supply rules for building sources it contributes
ASF/common/components/display/ssd1306/%.o: ../ASF/common/components/display/ssd1306/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega64a3u -DF_CPU=2000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


