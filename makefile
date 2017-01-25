VERSION = 0
PATCHLEVEL = 84
FRONT_PANEL_APP_VERSION = $(VERSION).$(PATCHLEVEL)

RM := rm -rf

# Add inputs and outputs from these tool invocations to the build variables
LSS += fp-application.lss
FLASH_IMAGE += fp-application.hex
EEPROM_IMAGE += fp-application.eep
SIZEDUMMY += sizedummy
AUTO_GENERATED_FILE = auto_generated.h

# All Target
all: fp-application.elf secondary-outputs

# All of the sources participating in the build are defined here
-include work-queue/subdir.mk
-include wdt/subdir.mk
-include uart/subdir.mk
-include twi/subdir.mk
-include timer/subdir.mk
-include rtc/subdir.mk
-include gfx/action_menu/subdir.mk
-include gfx/subdir.mk
-include display/glcd-0.5.2/devices/subdir.mk
-include display/glcd-0.5.2/controllers/subdir.mk
-include display/glcd-0.5.2/subdir.mk
-include display/subdir.mk
-include calendar/subdir.mk
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

# Tool invocations
fp-application.elf: auto-generated-files $(OBJS) $(USER_OBJS)
	@echo 'LD      $@'
	@avr-gcc -Wl,-Map,fp-application.map -mmcu=atxmega256a3u -o "fp-application.elf" $(OBJS) $(USER_OBJS) $(LIBS)

fp-application.lss: fp-application.elf
	@echo 'OBJDUMP $@'
	@-avr-objdump -h -S fp-application.elf  >"fp-application.lss"

fp-application.hex: fp-application.elf
	@echo 'OBJCOPY $@'
	@-avr-objcopy -O ihex -j .text -j .data -R .configSec -R .configData fp-application.elf  "fp-application.hex"

fp-application.eep: fp-application.elf
	@echo 'OBJCOPY $@'
	@-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex fp-application.elf  "fp-application.eep"

sizedummy: fp-application.elf
	@echo ' '
	@-avr-size --format=avr --mcu=atxmega256a3u fp-application.elf

auto-generated-files: $(AUTO_GENERATED_FILE)

$(AUTO_GENERATED_FILE):
	@echo 'GEN     $@'
	@( printf '#define VERSION "%s%s"\n' "$(FRONT_PANEL_APP_VERSION)" \
	'$(shell ./setversion)' ) > $@
	@date +'#define BUILD_DATE "%d %b %C%y"' >> $@
	@date +'#define BUILD_TIME "%T"' >> $@

# Other Targets
clean:
	-@echo 'CLEAN   $(AUTO_GENERATED_FILE)'
	-@$(RM) $(AUTO_GENERATED_FILE)
	-@echo 'CLEAN   .'
	-@$(RM) $(OBJS) $(ASM_DEPS) $(S_DEPS) $(S_UPPER_DEPS) $(C_DEPS)
	-@echo 'CLEAN   $(FLASH_IMAGE) $(ELFS) $(LSS) $(EEPROM_IMAGE) fp-application.elf fp-application.map'
	-@$(RM) $(FLASH_IMAGE) $(ELFS) $(EEPROM_IMAGE) fp-application.elf fp-application.map

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
