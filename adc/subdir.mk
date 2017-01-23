# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./adc/adc.c

OBJS += \
./adc/adc.o

C_DEPS += \
./adc/adc.d


# Each subdirectory must supply rules for building sources it contributes
adc/%.o: adc/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
