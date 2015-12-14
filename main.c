#include "asf.h"
#include "twi/twi_slave.h"
#include "timer/timer.h"
#include "wdt/wdt.h"
#include "uart/uart.h"
#include "rtc/rtc.h"

ISR(TWIE_TWIS_vect)
{
	twi_slave_interrupt_handler();
}

ISR(RTC_OVF_vect)
{
	rtc_handle_sec_update();
}

ISR(PORTF_INT0_vect)
{
	update_power_state();
}

ISR(PORTF_INT1_vect)
{
	handle_button_pressed();
}

ISR(TCC0_OVF_vect)
{
	tc_handle_overflow_interrupt();
}

ISR(TCC0_CCA_vect)
{
	tc_handle_cmp_interrupt();
}

ISR(TWIC_TWIM_vect)
{
	handle_twi_master();
}

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

void init_menu()
{
	set_menu_by_id(&present_menu, 0);
	show_current_menu(true);
	enable_screen_saver_mode();
}

void power_state_init()
{
	update_power_state();
}

void update_fp_info()
{
	computer_data.details.screen_saver_update_time = 6;
	computer_data.details.screen_saver_visible = 0;
	computer_data.details.screen_saver_type = 0;
	reset_ambient();
	eeprom_write_byte(SCREEN_SAVER_CONFIG_ADDRESS, computer_data.packed.screen_saver_config);
	eeprom_write_byte(SCREEN_SAVER_EEPROM_ADDRESS, computer_data.packed.screen_saver_update_time);
	if (eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS) == 0x00)
		eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, 0xff);
}

void updated_info_init()
{
	update_fp_info();
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
	load_config_block();
	cli();
	gfx_mono_init();
	sram_handle_init();
	updated_info_init();
	adc_init();
	pmic_init();
	portf_init();
	power_state_init();
	twi_slave_init();
	TWI_init();
	uart_init();
	init_menu();
	sei();
	tc_init();
	rtc_init();
	insert_to_log('P');
	tasks_init();
	insert_to_log('U');
}

bool sleep_interuptable(uint32_t timeout_us)
{
	uint32_t us = 0;

	while (us < timeout_us && !wakeup) {
		us++;
		delay_us(1);
	}

	return wakeup;
}

void debug_print_log()
{
	bool is_changed = false;
	if (log_twi.bottom != log_twi.top) {
		for (; log_twi.bottom < log_twi.top; log_twi.bottom++){
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

	init();
	uart_send_num(RST.STATUS , 16);
	if ((RST.STATUS & RST_WDRF_bm) != 0)
		uart_send_string("WDT reset\n\r");
	else
		uart_send_string("start main\n\r");

	wdt_reset();
	while (true) {
		debug_print_log();
		wakeup = false;
		if (!work_handler()) {
			sleep_interuptable(1000); /* 1 ms */
		} else {
			wdt_reset();
		}
	}

	return 0;
}
