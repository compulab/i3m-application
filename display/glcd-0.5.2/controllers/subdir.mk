# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./display/glcd-0.5.2/controllers/NT75451.c \
./display/glcd-0.5.2/controllers/PCD8544.c \
./display/glcd-0.5.2/controllers/ST7565R.c

OBJS += \
./display/glcd-0.5.2/controllers/NT75451.o \
./display/glcd-0.5.2/controllers/PCD8544.o \
./display/glcd-0.5.2/controllers/ST7565R.o

C_DEPS += \
./display/glcd-0.5.2/controllers/NT75451.d \
./display/glcd-0.5.2/controllers/PCD8544.d \
./display/glcd-0.5.2/controllers/ST7565R.d


# Each subdirectory must supply rules for building sources it contributes
display/glcd-0.5.2/controllers/%.o: display/glcd-0.5.2/controllers/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
