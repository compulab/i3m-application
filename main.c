#include "twi/twi_slave.h"
#include "scheduler/scheduler.h"
#include "screens/screen_saver/screen_saver.h"
#include "sram/sram_handle.h"
#include "ASF/xmega/drivers/wdt/wdt.h"
#include "timer/tc.h"
#include "twi/i2c_buffer.h"
#include "power/power.h"
#include "rtc/rtc.h"
#include "adc/adc.h"
#include "eeprom/eeprom_layout.h"
#include "eeprom/eeprom.h"
#include "work-queue/work.h"
#include "config/config_block.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "asf.h"
#include "usb/usb.h"
#include "ASF/common/services/calendar/calendar.h"

/*
 * Update screen saver configuration as they saved in EEPROM
 */
static void update_screen_saver_from_eeprom(void)
{
	computer_data.packed.screen_saver_config = eeprom_get_screen_saver_config();
	computer_data.packed.screen_saver_update_time = eeprom_get_screen_saver_time();
}

/*
 * Reset screen saver configuration to default values
 */
static void reset_screen_saver_config(void)
{
	eeprom_set_screen_saver_config(SCREEN_SAVER_CONFIGURATION_DEFAULT);
	eeprom_set_screen_saver_time(SCREEN_SAVER_TIME_DEFAULT);
	update_screen_saver_from_eeprom();
}

#ifndef APPLICATION_VER_MSB
#define APPLICATION_VER_MSB 0
#endif

#ifndef APPLICATION_VER_LSB
#define APPLICATION_VER_LSB 1
#endif

static void init_information(void)
{
	update_screen_saver_from_eeprom();

	if (computer_data.packed.screen_saver_config == 0xff || computer_data.packed.screen_saver_update_time == 0xff)
		reset_screen_saver_config();
	else if (computer_data.details.screen_saver_update_time < SCREEN_SAVER_SECOND_MIN_VALUE)
		computer_data.details.screen_saver_update_time = DEFAULT_SCREEN_SAVER_TIME;
	else
		computer_data.packed.screen_saver_update_time = eeprom_get_screen_saver_time();

	i2c_buffer.layout.iwren = 0;

	if (eeprom_read_byte(APPLICATION_VER_MSB_EEPROM_ADDRESS) != APPLICATION_VER_MSB)
		eeprom_write_byte(APPLICATION_VER_MSB_EEPROM_ADDRESS, APPLICATION_VER_MSB);
	if (eeprom_read_byte(APPLICATION_VER_LSB_EEPROM_ADDRESS) != APPLICATION_VER_LSB)
		eeprom_write_byte(APPLICATION_VER_LSB_EEPROM_ADDRESS, APPLICATION_VER_LSB);
	if (eeprom_get_brightness_value() == 0x00)
		eeprom_set_brigntness_value(BRIGHTNESS_DEFAULT);
}

/*
 * Initializing of computer data struct for new data
 */
static void init_updateable_data(void)
{
	init_information();
	uint8_t *p_computer_data = (uint8_t *)&computer_data;
	for (uint8_t i = 0; i < (sizeof(computer_data) / 8); i++)
		p_computer_data[i] = 0;
}

extern struct gfx_frame *splash;
static void init(void)
{
	wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_2KCLK);
	wdt_enable();
	sysclk_init();
	board_init();
	load_config_block();
	cli();
	gfx_mono_init();
	glcd_init();
	sram_handle_init();
	init_updateable_data();
	adc_init();
	pmic_init();
	power_state_init();
	twi_slave_init();
	sleepmgr_init();
	tc_init();
	usb_init();
	scheduler_init();
	sei();
	//show_logo(splash);
}

/*
 * Sleep function makes the system Idle for timeout_us microseconds or until wakeup flag will be set
 */
static bool sleep_interuptable(uint32_t timeout_us)
{
	for (uint32_t us = 0; us < timeout_us && !wakeup; us++)
		delay_us(1);

	return wakeup;
}

int main(int argc, char *argv[])
{
	works_count = 0;

	init();
	while (true) {
		wdt_reset();
		wakeup = false;
		if (!work_handler())
			sleep_interuptable(1000); /* 1 ms */
	}

	return 0;
}
