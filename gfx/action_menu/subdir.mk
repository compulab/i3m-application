# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./gfx/action_menu/gfx_action_menu.c \
./gfx/action_menu/graphic_menu.c

OBJS += \
./gfx/action_menu/gfx_action_menu.o \
./gfx/action_menu/graphic_menu.o

C_DEPS += \
./gfx/action_menu/gfx_action_menu.d \
./gfx/action_menu/graphic_menu.d


# Each subdirectory must supply rules for building sources it contributes
gfx/action_menu/%.o: gfx/action_menu/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
