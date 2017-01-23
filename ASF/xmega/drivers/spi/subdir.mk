# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./ASF/xmega/drivers/spi/spi.c

OBJS += \
./ASF/xmega/drivers/spi/spi.o

C_DEPS += \
./ASF/xmega/drivers/spi/spi.d


# Each subdirectory must supply rules for building sources it contributes
ASF/xmega/drivers/spi/%.o: ASF/xmega/drivers/spi/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
