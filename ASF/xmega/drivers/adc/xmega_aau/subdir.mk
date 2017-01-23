# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./ASF/xmega/drivers/adc/xmega_aau/adc_aau.c

OBJS += \
./ASF/xmega/drivers/adc/xmega_aau/adc_aau.o

C_DEPS += \
./ASF/xmega/drivers/adc/xmega_aau/adc_aau.d


# Each subdirectory must supply rules for building sources it contributes
ASF/xmega/drivers/adc/xmega_aau/%.o: ASF/xmega/drivers/adc/xmega_aau/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
