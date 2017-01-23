# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./ASF/xmega/boards/stk600/rc064x/init.c

OBJS += \
./ASF/xmega/boards/stk600/rc064x/init.o

C_DEPS += \
./ASF/xmega/boards/stk600/rc064x/init.d


# Each subdirectory must supply rules for building sources it contributes
ASF/xmega/boards/stk600/rc064x/%.o: ASF/xmega/boards/stk600/rc064x/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
