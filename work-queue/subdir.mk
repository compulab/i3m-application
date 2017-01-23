# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./work-queue/work.c

OBJS += \
./work-queue/work.o

C_DEPS += \
./work-queue/work.d


# Each subdirectory must supply rules for building sources it contributes
work-queue/%.o: work-queue/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
