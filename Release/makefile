################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include twi/subdir.mk
-include timer/subdir.mk
-include power-state/subdir.mk
-include gfx/action_menu/subdir.mk
-include gfx/subdir.mk
-include adc/subdir.mk
-include ASF/xmega/drivers/spi/subdir.mk
-include ASF/xmega/drivers/nvm/subdir.mk
-include ASF/xmega/drivers/cpu/subdir.mk
-include ASF/xmega/drivers/adc/xmega_aau/subdir.mk
-include ASF/xmega/drivers/adc/subdir.mk
-include ASF/xmega/boards/stk600/rc064x/subdir.mk
-include ASF/common/services/spi/xmega_spi/subdir.mk
-include ASF/common/services/sleepmgr/xmega/subdir.mk
-include ASF/common/services/ioport/xmega/subdir.mk
-include ASF/common/services/gfx_mono/subdir.mk
-include ASF/common/services/clock/xmega/subdir.mk
-include ASF/common/components/display/ssd1306/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Fp-msh.lss \

FLASH_IMAGE += \
Fp-msh.hex \

EEPROM_IMAGE += \
Fp-msh.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Fp-msh.elf secondary-outputs

# Tool invocations
Fp-msh.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Fp-msh.map -Wl,--section-start=.configSec=0x803000 -mmcu=atxmega64a3u -o "Fp-msh.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Fp-msh.lss: Fp-msh.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Fp-msh.elf  >"Fp-msh.lss"
	@echo 'Finished building: $@'
	@echo ' '

Fp-msh.hex: Fp-msh.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Fp-msh.elf  "Fp-msh.hex"
	@echo 'Finished building: $@'
	@echo ' '

Fp-msh.eep: Fp-msh.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Fp-msh.elf  "Fp-msh.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Fp-msh.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atxmega64a3u Fp-msh.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Fp-msh.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
