# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./wdt/wdt.c

OBJS += \
./wdt/wdt.o

C_DEPS += \
./wdt/wdt.d


# Each subdirectory must supply rules for building sources it contributes
wdt/%.o: wdt/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
