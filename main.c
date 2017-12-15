#include "twi/twi_slave.h"
#include "scheduler/scheduler.h"
#include "ASF/xmega/drivers/wdt/wdt.h"
#include "twi/i2c_buffer.h"
#include "power/power.h"
#include "work-queue/work.h"
#include "config/config_block.h"
#include "config/menus/fp_menus_config.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "ASF/common/services/usb/udc/udc.h"
#include "ASF/common/utils/stdio/stdio_usb/stdio_usb.h"
#include "lib/sleep.h"
#include "asf.h"

void apply_app_settings(void);

static void tc_init(void)
{
	void *tc = &TCC0;
	tc_set_wgm(tc, TC_WG_NORMAL);
	tc_write_period(tc, 0xFFFF);
	tc_disable_cc_channels(tc, TC_CCAEN);
	tc_write_clock_source(tc, TC_CLKSEL_DIV1024_gc);
}

bool my_callback_cdc_enable(void)
{
	return stdio_usb_enable();
}

void my_callback_cdc_disable(void)
{
	stdio_usb_disable();
}

static void usb_init(void)
{
	udc_start();
}

static void adc_init(void)
{
	ADC_t *adc = &ADCA;
	uint8_t adc_ch = ADC_CH0;
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;

	adc_read_configuration(adc, &adc_conf);
	adcch_read_configuration(adc, adc_ch, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_12, ADC_REFSEL_INTVCC2_gc);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SINGLE, 1, 0);
	adc_set_clock_rate(&adc_conf, 100000UL);

	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_INTERNAL_GND, 0);
	adc_write_configuration(adc, &adc_conf);
	adcch_write_configuration(adc, adc_ch, &adcch_conf);
	adc_enable(adc);
}

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
	i2c_buffer_init();
	apply_app_settings();
	adc_init();
	pmic_init();
	power_state_init();
	twi_slave_init();
	sleepmgr_init();
	usb_init();
	tc_init();
	scheduler_init(&TCC0);
	tc_enable(&TCC0);
	sei();
}

int main(int argc, char *argv[])
{
	//prevent config section from being optimized out
	volatile int __attribute__((unused)) dont_optimize_this_away = pgm_read_byte_far(&menus_config);

#ifndef CONFIG_SECTION_ONLY
	init();
	while (true) {
		wdt_reset();
		wakeup = false;
		if (!work_handler())
			sleep_interuptable(1000);
	}
#endif

	return 0;
}
