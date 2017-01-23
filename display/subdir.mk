# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./display/display_render.c

OBJS += \
./display/display_render.o

C_DEPS += \
./display/display_render.d


# Each subdirectory must supply rules for building sources it contributes
display/%.o: display/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

