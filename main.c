#include "twi/twi_slave.h"
#include "scheduler/scheduler.h"
#include "sram/sram_handle.h"
#include "ASF/xmega/drivers/wdt/wdt.h"
#include "timer/tc.h"
#include "twi/i2c_buffer.h"
#include "power/power.h"
#include "adc/adc.h"
#include "work-queue/work.h"
#include "config/config_block.h"
#include "config/menus/fp_menus_config.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "lib/sleep.h"
#include "asf.h"
#include "usb/usb.h"
#include "ASF/common/services/calendar/calendar.h"

void apply_app_settings(void);

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
	apply_app_settings();
	adc_init();
	pmic_init();
	power_state_init();
	twi_slave_init();
	sleepmgr_init();
	tc_init();
	usb_init();
	scheduler_init();
	sei();
}

int main(int argc, char *argv[])
{
	//prevent config section from being optimized out
	volatile struct cnf_blk *config_block = &menus_config;

#ifndef CONFIG_SECTION_ONLY
	init();
	while (true) {
		wdt_reset();
		wakeup = false;
		if (!work_handler())
			sleep_interuptable(1000);
	}
#else
	memcpy_P(&config_block, &menus_config, sizeof(struct cnf_blk));
#endif

	return 0;
}
