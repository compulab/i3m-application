#include "asf.h"
#include "twi/twi_slave.h"
#include "timer/timer.h"
#include "wdt/wdt.h"
#include "uart/uart.h"
#include "rtc/rtc.h"

/*
 * Managing I2C requests as described in
 * http://droid/mirror/mediawiki/index.php/Airtop_FrontPanel_I2C_Registers_Layout
 */
ISR(TWIE_TWIS_vect)
{
	twi_slave_interrupt_handler();
}

/*
 * Communication with I2C as master. used for getting the ambient temp
 */
ISR(TWIC_TWIM_vect)
{
	handle_twi_master();
}


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
void portf_init()
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
void init_menu()
{
	set_menu_by_id(&present_menu, 0);
	show_current_menu(true);
	enable_screen_saver_mode();
}

/*
 * Init current power state
 */
void power_state_init()
{
	update_power_state();
}

/*
 * Update screen saver configuration as they saved in EEPROM
 */
void update_screen_saver_from_eeprom()
{
	computer_data.packed.screen_saver_config = eeprom_read_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS);
	computer_data.packed.screen_saver_update_time = eeprom_read_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS);
}

/*
 * Reset screen saver configuration to default values
 */
void reset_screen_saver_config()
{
	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, SCREEN_SAVER_CONFIGURATION_DEFAULT);
	eeprom_write_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS, SCREEN_SAVER_TIME_DEFAULT);
	update_screen_saver_from_eeprom();
}

/*
 * Initialize and validate Screen Saver configuration
 * Initialize ambient sensor
 * validate application version
 * validate screen brightness
 */
void init_information()
{
	update_screen_saver_from_eeprom();

	if (computer_data.packed.screen_saver_config == 0xff || computer_data.packed.screen_saver_update_time == 0xff)
		reset_screen_saver_config();
	else if (computer_data.details.screen_saver_update_time < SCREEN_SAVER_SECOND_MIN_VALUE)
		computer_data.details.screen_saver_update_time = DEFAULT_SCREEN_SAVER_TIME;
	else
		computer_data.packed.screen_saver_update_time = eeprom_read_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS);

	init_ambient();
	layout.l.iwren = 0;

	if (eeprom_read_byte(APPLICATION_VER_MSB_EEPROM_ADDRESS) != APPLICATION_VER_MSB)
		eeprom_write_byte(APPLICATION_VER_MSB_EEPROM_ADDRESS, APPLICATION_VER_MSB);
	if (eeprom_read_byte(APPLICATION_VER_LSB_EEPROM_ADDRESS) != APPLICATION_VER_LSB)
		eeprom_write_byte(APPLICATION_VER_LSB_EEPROM_ADDRESS, APPLICATION_VER_LSB);
	if (eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS) == 0x00)
		eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, BRIGHTNESS_DEFAULT);
}

/*
 * Initializing of computer data struct for new data
 */
void init_updateable_data()
{
	init_information();
	uint8_t *p_computer_data = (uint8_t *)&computer_data;
	for (uint8_t i = 0; i < (sizeof(computer_data) / 8); i++)
		p_computer_data[i] = 0;
}

void init()
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
	tc_init();
	rtc_init();
	tasks_init();
}

/*
 * Sleep function makes the system Idle for timeout_us microseconds or until wakeup flag will be set
 */
bool sleep_interuptable(uint32_t timeout_us)
{
	uint32_t us = 0;

	while (us < timeout_us && !wakeup) {
		us++;
		delay_us(1);
	}

	return wakeup;
}

/*
 * Debug method for printing debug log
 */
void debug_print_log()
{
	bool is_changed = false;
	if (log_twi.bottom != log_twi.top) {
		for (; log_twi.bottom < log_twi.top; log_twi.bottom++) {
			uart_send_char(log_twi.data[log_twi.bottom]);
			uart_send_string(", ");
			is_changed = true;
		}

		if (is_changed)
			uart_send_string(".\n\r");
	}

	if (0 != computer_data.details.error_count) {
		uart_send_num(computer_data.details.error_count, 10);
		uart_send_string(" - errors\n\r");
	}
}

int main(int argc, char *argv[])
{

	log_twi.bottom = log_twi.top = 0;
	computer_data.details.error_count = 0;
	works_count = 0;

	if (USB.CTRLB & USB_ATTACH_bm) {
		USB.CTRLB &= ~USB_ATTACH_bm;
		ccp_write_io((uint8_t *)&RST.CTRL, RST.CTRL | RST_SWRST_bm);
	}

	init();
	wdt_reset();
	while (true) {
//		debug_print_log();
		wakeup = false;
		if (!work_handler()) {
			sleep_interuptable(1000); /* 1 ms */
		} else {
			wdt_reset();
		}
	}

	return 0;
}
