# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./display/glcd-0.5.2/glcd.c \
./display/glcd-0.5.2/glcd_font.c \
./display/glcd-0.5.2/graphics.c \
./display/glcd-0.5.2/graphs.c \
./display/glcd-0.5.2/nhdlcd.c \
./display/glcd-0.5.2/text.c \
./display/glcd-0.5.2/text_tiny.c

OBJS += \
./display/glcd-0.5.2/glcd.o \
./display/glcd-0.5.2/glcd_font.o \
./display/glcd-0.5.2/graphics.o \
./display/glcd-0.5.2/graphs.o \
./display/glcd-0.5.2/nhdlcd.o \
./display/glcd-0.5.2/text.o \
./display/glcd-0.5.2/text_tiny.o

C_DEPS += \
./display/glcd-0.5.2/glcd.d \
./display/glcd-0.5.2/glcd_font.d \
./display/glcd-0.5.2/graphics.d \
./display/glcd-0.5.2/graphs.d \
./display/glcd-0.5.2/nhdlcd.d \
./display/glcd-0.5.2/text.d \
./display/glcd-0.5.2/text_tiny.d


# Each subdirectory must supply rules for building sources it contributes
display/glcd-0.5.2/%.o: display/glcd-0.5.2/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
