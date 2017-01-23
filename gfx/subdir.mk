# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./gfx/gfx_utils.c \
./gfx/menu-handler.c

OBJS += \
./gfx/gfx_utils.o \
./gfx/menu-handler.o

C_DEPS += \
./gfx/gfx_utils.d \
./gfx/menu-handler.d


# Each subdirectory must supply rules for building sources it contributes
gfx/%.o: gfx/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
