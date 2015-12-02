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
	load_config_block();
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
	layout.l.sig[0] = 'C';
	layout.l.sig[1] = 'L';
	layout.l.sig[2] = 'F';
	layout.l.sig[3] = 'P';
	layout.l.layout_ver = 0x01;
	layout.l.major_rev = 0x00;
	layout.l.minor_rev = 0x01;

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
	cli();
	wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_2KCLK);
	wdt_enable();
	board_init();
	sysclk_init();
	gfx_mono_init();
	init_menu();
	sram_handle_init();
	updated_info_init();
	adc_init();
	pmic_init();
	portf_init();
	power_state_init();
	twi_slave_init();
	TWI_init();
	uart_init();
	sei();
	tc_init();
	rtc_init();
	insert_to_log('P');
	tasks_init();
	insert_to_log('U');
}

int main(int argc, char *argv[])
{
	log_twi.bottom = log_twi.top = 0;

	init();

	wdt_reset();
	uart_send_string("start main\n\r");

	uint32_t error_count = 0;
	computer_data.details.error_count = 0;
	bool is_changed;
	while(true) {
		is_changed = false;
		if (log_twi.bottom != log_twi.top) {
			for (; log_twi.bottom < log_twi.top; log_twi.bottom++){
				uart_send_char(log_twi.data[log_twi.bottom]);
				uart_send_string(", ");
				is_changed = true;
			}
			if (is_changed)
				uart_send_string(".\n\r");
		}


		if (!work_handler()) {
			delay_ms(10);
		} else {
			wdt_reset();
		}



		if (0 != computer_data.details.error_count) {
			uart_send_num(computer_data.details.error_count - error_count, 10);
			uart_send_string(" - new errors\n\r");
			error_count = computer_data.details.error_count;
		}
	}

	return 0;
}
