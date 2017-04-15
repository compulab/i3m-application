#include "asf.h"
#include "twi/twi_slave.h"
#include "scheduler/scheduler.h"
#include "screen_saver/screen_saver.h"
#include "screens/screen_saver/screen_saver.h"
#include "sram/sram_handle.h"
#include "wdt/wdt.h"
#include "twi/twi_master.h"
#include "twi/i2c_buffer.h"
#include "power/power.h"
#include "uart/uart.h"
#include "rtc/rtc.h"
#include "adc/adc.h"
#include "eeprom/eeprom_layout.h"
#include "eeprom/eeprom.h"
#include "ASF/common/services/usb/udc/udc.h"

/*
 * Power state GPIO change
 */
ISR(PORTF_INT0_vect)
{
	update_power_state();
}

/*
 * Buttons GPIO change
 */
ISR(PORTF_INT1_vect)
{
	handle_button_pressed();
}

/*
 * PORTF contains the UI buttons and the power states
 */
static void portf_init(void)
{
	uint8_t sreg = SREG;
	uint8_t power_pin_cfg =(uint8_t)  PORT_ISC_BOTHEDGES_gc | PORT_OPC_PULLUP_gc;
	uint8_t buttons_pin_cfg = PIN4_bm | PIN5_bm | PIN6_bm;
	cli();
	PORTF.PIN4CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN5CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN6CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN0CTRL = power_pin_cfg;
	PORTF.PIN1CTRL = power_pin_cfg;
	PORTF.PIN2CTRL = power_pin_cfg;
	/* Restore status register. */
	SREG = sreg;
	PORTF.INTCTRL =  PORT_INT0LVL_MED_gc | PORT_INT1LVL_MED_gc;
	PORTF.INT0MASK = PIN0_bm | PIN1_bm |PIN2_bm;
	PORTF.INT1MASK = buttons_pin_cfg;
}

/*
 * Init first menu shown
 */
static void init_menu(void)
{
	set_menu_by_id(&present_menu, 0);
	present_menu->draw(present_menu);
	enable_screen_saver_mode();
}

/*
 * Init current power state
 */
static void power_state_init(void)
{
	update_power_state();
}

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

static void init_ambient(void)
{
	i2c_buffer.layout.ambs = 0;
}

void tasks_init(void)
{
	rtc_scheduler_init();
	tc_scheduler_init();
	switch_rtc_interrupt_schedule(true);
	switch_tc_interrupt_schedule(true);
}

#ifndef APPLICATION_VER_MSB
#define APPLICATION_VER_MSB 0
#endif

#ifndef APPLICATION_VER_LSB
#define APPLICATION_VER_LSB 1
#endif

/*
 * Initialize and validate Screen Saver configuration
 * Initialize ambient sensor
 * validate application version
 * validate screen brightness
 */
static void init_information(void)
{
	update_screen_saver_from_eeprom();

	if (computer_data.packed.screen_saver_config == 0xff || computer_data.packed.screen_saver_update_time == 0xff)
		reset_screen_saver_config();
	else if (computer_data.details.screen_saver_update_time < SCREEN_SAVER_SECOND_MIN_VALUE)
		computer_data.details.screen_saver_update_time = DEFAULT_SCREEN_SAVER_TIME;
	else
		computer_data.packed.screen_saver_update_time = eeprom_get_screen_saver_time();

	init_ambient();
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

void usb_init(void)
{
	udc_start();
}

static void init(void)
{
	wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_2KCLK);
	wdt_enable();
	sysclk_init();
	board_init();
	uart_init();
	load_config_block();
	cli();
	gfx_mono_init();
	glcd_init();
	sram_handle_init();
	init_updateable_data();
	adc_init();
	pmic_init();
	portf_init();
	power_state_init();
	twi_slave_init();
	TWI_init();
	init_menu();
	sei();
	sleepmgr_init();
	tc_init();
	rtc_init();
	usb_init();
	tasks_init();
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

bool my_callback_cdc_enable(void)
{
	return stdio_usb_enable();
}
void my_callback_cdc_disable(void)
{
	stdio_usb_disable();
}

#include "ASF/common/services/usb/class/cdc/device/udi_cdc.h"

const int BUF_SIZE = 1;
char buf[1] = "C";
bool no_error = true;
void task(void)
{
	if (udi_cdc_is_rx_ready()) {
		buf[0] = udi_cdc_getc();
		no_error = udi_cdc_putc(buf[0]);
		if (!no_error) {
			buf[0] = '!';
		}
	}
}

int main(int argc, char *argv[])
{

	computer_data.details.error_count = 0;
	works_count = 0;

	if (USB.CTRLB & USB_ATTACH_bm) {
		USB.CTRLB &= ~USB_ATTACH_bm;
		ccp_write_io((uint8_t *)&RST.CTRL, RST.CTRL | RST_SWRST_bm);
	}

	init();
	wdt_reset();
	while (true) {
		task();
		wakeup = false;
		if (!work_handler()) {
			sleepmgr_enter_sleep();
			sleep_interuptable(1000); /* 1 ms */
		} else {
			wdt_reset();
		}
	}

	return 0;
}
