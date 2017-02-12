# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./scheduler/rtc_scheduler.c \
./scheduler/tc_scheduler.c \
./scheduler/ambient_task.c \
./scheduler/screen_saver_task.c

OBJS += \
./scheduler/rtc_scheduler.o \
./scheduler/tc_scheduler.o \
./scheduler/ambient_task.o \
./scheduler/screen_saver_task.o

C_DEPS += \
./scheduler/rtc_scheduler.d \
./scheduler/tc_scheduler.d \
./scheduler/ambient_task.d \
./scheduler/screen_saver_task.d

# Each subdirectory must supply rules for building sources it contributes
scheduler/%.o: scheduler/%.c
	@echo 'CC      $<'
	@avr-gcc -Wall -Os -fms-extensions -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atxmega256a3u -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
