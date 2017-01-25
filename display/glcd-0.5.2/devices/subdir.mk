# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./display/glcd-0.5.2/devices/AVR8.c \
./display/glcd-0.5.2/devices/LPC111x.c \
./display/glcd-0.5.2/devices/LPC11Uxx.c \
./display/glcd-0.5.2/devices/PIC24H.c \
./display/glcd-0.5.2/devices/STM32F0xx.c \
./display/glcd-0.5.2/devices/STM32F10x.c \
./display/glcd-0.5.2/devices/XPLAINED_XMEGA_A3BU.c

OBJS += \
./display/glcd-0.5.2/devices/AVR8.o \
./display/glcd-0.5.2/devices/LPC111x.o \
./display/glcd-0.5.2/devices/LPC11Uxx.o \
./display/glcd-0.5.2/devices/PIC24H.o \
./display/glcd-0.5.2/devices/STM32F0xx.o \
./display/glcd-0.5.2/devices/STM32F10x.o \
./display/glcd-0.5.2/devices/XPLAINED_XMEGA_A3BU.o

C_DEPS += \
./display/glcd-0.5.2/devices/AVR8.d \
./display/glcd-0.5.2/devices/LPC111x.d \
./display/glcd-0.5.2/devices/LPC11Uxx.d \
./display/glcd-0.5.2/devices/PIC24H.d \
./display/glcd-0.5.2/devices/STM32F0xx.d \
./display/glcd-0.5.2/devices/STM32F10x.d \
./display/glcd-0.5.2/devices/XPLAINED_XMEGA_A3BU.d


# Each subdirectory must supply rules for building sources it contributes
display/glcd-0.5.2/devices/%.o: display/glcd-0.5.2/devices/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
