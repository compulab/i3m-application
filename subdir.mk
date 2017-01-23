# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./Fp-utils.c \
./effects.c \
./layout.c \
./main.c

OBJS += \
./Fp-utils.o \
./effects.o \
./layout.o \
./main.o

C_DEPS += \
./Fp-utils.d \
./effects.d \
./layout.d \
./main.d


# Each subdirectory must supply rules for building sources it contributes
%.o: %.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
