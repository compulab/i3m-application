# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./timer/tc.c \
./timer/timer.c \
./timer/adc_task.c \
./timer/ambient_task.c \
./timer/pending_req_task.c

OBJS += \
./timer/tc.o \
./timer/timer.o \
./timer/adc_task.o \
./timer/ambient_task.o \
./timer/pending_req_task.o

C_DEPS += \
./timer/tc.d \
./timer/timer.d \
./timer/adc_task.d \
./timer/ambient_task.d \
./timer/pending_req_task.d


# Each subdirectory must supply rules for building sources it contributes
timer/%.o: timer/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
