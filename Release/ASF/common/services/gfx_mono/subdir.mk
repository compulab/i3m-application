################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ASF/common/services/gfx_mono/gfx_mono_framebuffer.c \
../ASF/common/services/gfx_mono/gfx_mono_generic.c \
../ASF/common/services/gfx_mono/gfx_mono_menu.c \
../ASF/common/services/gfx_mono/gfx_mono_text.c \
../ASF/common/services/gfx_mono/gfx_mono_ug_2832hsweg04.c \
../ASF/common/services/gfx_mono/sysfont.c 

OBJS += \
./ASF/common/services/gfx_mono/gfx_mono_framebuffer.o \
./ASF/common/services/gfx_mono/gfx_mono_generic.o \
./ASF/common/services/gfx_mono/gfx_mono_menu.o \
./ASF/common/services/gfx_mono/gfx_mono_text.o \
./ASF/common/services/gfx_mono/gfx_mono_ug_2832hsweg04.o \
./ASF/common/services/gfx_mono/sysfont.o 

C_DEPS += \
./ASF/common/services/gfx_mono/gfx_mono_framebuffer.d \
./ASF/common/services/gfx_mono/gfx_mono_generic.d \
./ASF/common/services/gfx_mono/gfx_mono_menu.d \
./ASF/common/services/gfx_mono/gfx_mono_text.d \
./ASF/common/services/gfx_mono/gfx_mono_ug_2832hsweg04.d \
./ASF/common/services/gfx_mono/sysfont.d 


# Each subdirectory must supply rules for building sources it contributes
ASF/common/services/gfx_mono/%.o: ../ASF/common/services/gfx_mono/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega64a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


