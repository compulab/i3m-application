# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./ASF/common/services/sleepmgr/xmega/sleepmgr.c

OBJS += \
./ASF/common/services/sleepmgr/xmega/sleepmgr.o

C_DEPS += \
./ASF/common/services/sleepmgr/xmega/sleepmgr.d


# Each subdirectory must supply rules for building sources it contributes
ASF/common/services/sleepmgr/xmega/%.o: ASF/common/services/sleepmgr/xmega/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
