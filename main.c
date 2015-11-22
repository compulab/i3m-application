#include "asf.h"
#include "twi/twi_slave.h"
#include "timer/timer.h"
#include "wdt/wdt.h"
#include "uart/uart.h"
#include "rtc/rtc.h"

ISR(TWIE_TWIS_vect)
{
	wdt_reset();
	twi_slave_interrupt_handler();
}

ISR(RTC_OVF_vect)
{
	wdt_reset();
	rtc_handle_sec_update();
}

ISR(PORTF_INT0_vect)
{
	wdt_reset();
	update_power_state();
}

ISR(PORTF_INT1_vect)
{
	wdt_reset();
	handle_button_pressed();
}

ISR(TCC0_OVF_vect)
{
	wdt_reset();
	tc_handle_overflow_interrupt();
}

ISR(TCC0_CCA_vect)
{
	wdt_reset();
	tc_handle_cmp_interrupt();
}

ISR(TWIC_TWIM_vect)
{
	wdt_reset();
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
	tc_init();
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



	computer_data.details.screen_saver_update_time = 2;
	computer_data.details.screen_saver_visible = 1;
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
	wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_8KCLK);
	wdt_enable();
	board_init();
	sysclk_init();
	sram_handle_init();
	updated_info_init();
	gfx_mono_init();
	init_menu();
	adc_init();
	pmic_init();
	portf_init();
	power_state_init();
	sei();
	twi_slave_init();
	TWI_init();
	tasks_init();
	uart_init();
	rtc_init();
	wdt_reset();
}

int main(int argc, char *argv[])
{
	init();

	while(true) {
		wdt_reset();
			if (!work_handler())
				delay_ms(10);
	}

	return 0;
}
