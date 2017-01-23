# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
./ASF/xmega/drivers/cpu/ccp.s

OBJS += \
./ASF/xmega/drivers/cpu/ccp.o

S_DEPS += \
./ASF/xmega/drivers/cpu/ccp.d


# Each subdirectory must supply rules for building sources it contributes
ASF/xmega/drivers/cpu/%.o: ASF/xmega/drivers/cpu/%.s
	@echo 'AS      $<'
	@avr-gcc -x assembler-with-cpp -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
