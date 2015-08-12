################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../ASF/xmega/drivers/nvm/nvm_asm.s 

C_SRCS += \
../ASF/xmega/drivers/nvm/nvm.c 

OBJS += \
./ASF/xmega/drivers/nvm/nvm.o \
./ASF/xmega/drivers/nvm/nvm_asm.o 

S_DEPS += \
./ASF/xmega/drivers/nvm/nvm_asm.d 

C_DEPS += \
./ASF/xmega/drivers/nvm/nvm.d 


# Each subdirectory must supply rules for building sources it contributes
ASF/xmega/drivers/nvm/%.o: ../ASF/xmega/drivers/nvm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega64a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ASF/xmega/drivers/nvm/%.o: ../ASF/xmega/drivers/nvm/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atxmega64a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


