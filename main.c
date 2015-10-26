#include <string.h>
#include <math.h>
#include "debug.h"
#include "gfx/menu-handler.h"
#include "timer/tc.h"
#include "adc/adc.h"
#include "config/cnf_blk_components.h"
#include "gfx/gfx_utils.h"
#include "twi/test_twi.h"
#include "twi/twi_slave.h"
#include "asf.h"
#include "gfx/menu-handler.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "adc/adc.h"
#include "timer/tc-handler.h"
#include "twi/sram_handle.h"
#include "twi/master/twi.h"
//#define DEMO_SHOW


ISR(TWIE_TWIS_vect)
{
	twi_slave_interrupt_handler();
}

//ISR(TWIC_TWIM_vect)
//{
//	interrupt_handler();
//}


ISR(PORTF_INT0_vect)
{
	update_power_state();
}

ISR(PORTF_INT1_vect){
	handle_button_pressed();
}

ISR(TCC0_OVF_vect)
{
	tc_counter++;
	tc_handle();
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
	gfx_action_menu_init(present_menu, true);
//	draw_string_in_buffer("hello", 10, 10, fonts[1]);
	tc_init();
}


void power_state_init()
{
	update_power_state();
}

void updated_info_init()
{
#ifdef DEMO_SHOW
	computer_data.valid_ambient_temp = true;
	computer_data.ambient_temp = 65;
	computer_data.valid_gpu_temp = true;
	computer_data.gpu_temp = 55;

	computer_data.valid_cpu_temp[0] = true;
	computer_data.cpu_temp[0] = 45;
	computer_data.valid_cpu_fq[0] = true;
	computer_data.cpu_fq[0] = 4470;

	computer_data.valid_cpu_temp[1] = true;
	computer_data.cpu_temp[1] = 65;
	computer_data.valid_cpu_fq[1] = true;
	computer_data.cpu_fq[1] = 4300;

	computer_data.valid_cpu_temp[2] = true;
	computer_data.cpu_temp[2] = 35;
	computer_data.valid_cpu_fq[2] = true;
	computer_data.cpu_fq[2] = 4100;

	computer_data.valid_cpu_temp[3] = true;
	computer_data.cpu_temp[3] = 75;
	computer_data.valid_cpu_fq[3] = true;
	computer_data.cpu_fq[3] = 4800;

	computer_data.valid_hdd_temp[0] = true;
	computer_data.hdd_temp[0] = 65;
	computer_data.valid_hdd_size[0] = true;
	computer_data.hdd_size[0] = 550;
	computer_data.hdd_tera_units[0] = false;

	computer_data.valid_hdd_temp[1] = true;
	computer_data.hdd_temp[1] = 65;
	computer_data.valid_hdd_size[1] = true;
	computer_data.hdd_size[1] = 550;
	computer_data.hdd_tera_units[1] = false;

	computer_data.valid_hdd_temp[2] = true;
	computer_data.hdd_temp[2] = 85;
	computer_data.valid_hdd_size[2] = true;
	computer_data.hdd_size[2] = 2;
	computer_data.hdd_tera_units[2] = true;

	computer_data.valid_hdd_temp[3] = true;
	computer_data.hdd_temp[3] = 45;
	computer_data.valid_hdd_size[3] = true;
	computer_data.hdd_size[3] = 150;
	computer_data.hdd_tera_units[3] = false;

	computer_data.valid_mem[0] = true;
	computer_data.mem_slot_sz[0] = 3;

	computer_data.valid_mem[1] = true;
	computer_data.mem_slot_sz[1] = 2;

	computer_data.valid_mem[2] = true;
	computer_data.mem_slot_sz[2] = 5;

	computer_data.valid_mem[3] = true;
	computer_data.mem_slot_sz[3] = 0;

	computer_data.direct_string =  malloc(sizeof(struct direct_string_item));
	computer_data.direct_string->content = "testing";
	computer_data.direct_string->type = "type";
	computer_data.direct_string->next = 0;

#else
	computer_data.direct_string = 0;
	computer_data.valid_ambient_temp = false;
	computer_data.valid_gpu_temp = false;
	for (int i=0; i < MAX_CPU; i++){
		computer_data.valid_cpu_temp[i] = false;
		computer_data.valid_cpu_fq[i] = false;
	}
	for (int i=0; i < MAX_HDD; i++){
		computer_data.valid_hdd_temp[i] = false;
		computer_data.valid_hdd_size[i] = false;
	}
#endif
	computer_data.req_cpu_fq = false;
	computer_data.req_cpu_temp = false;
	computer_data.req_gpu_temp = false;
	computer_data.wen = false;
}

void init()
{
	board_init();
	sysclk_init();
//	u8g_init();
	sram_handle_init();
	gfx_mono_init();
	init_menu();
	tc_handle_init();
	updated_info_init();
	adc_init();
	pmic_init();
	portf_init();
	power_state_init();
	sei();
	twi_slave_init();
	TWI_init();
}

//uint8_t d;
//
//void update_d(uint8_t data)
//{
//	d = data;
//}
//
//uint8_t addr = 0x00;
//int main(void)
//{
//	bool second_twi = false;
//	init();
//	d = 0xff;
//	while(true){
////		MSG("Start")
////		delay_s(2);
//		if (!second_twi) {
//			if (TWI_read_reg(AMBIENT_TWI_ADDRESS, AMBIENT_TEMPERATURE_ADDRESS, &d, 2)){
//				MSG("1 YEss")
//				second_twi = true;
//	//				addr ++;
//			} else {
//				MSG("1 No")
//	//			addr = 0x00;
//			}
//		} else {
//			if (TWI_read(AMBIENT_TWI_ADDRESS, &d, 2)){
//				MSG("2 YEss")
////				second_twi = true;
//	//				addr ++;
//			} else {
//				MSG("2 No")
//				delay_s(2);
//				if (TWI_read_reg(AMBIENT_TWI_ADDRESS, AMBIENT_TEMPERATURE_ADDRESS, &d, 2)){
//							MSG("2 YEss")
//				}
//	//			addr = 0x00;
//			}
//		}
//
////		send_read_package(update_d, real);
//		MSG_dec(d);
////		MSG("done")
//		delay_s(5);
//		ssd1306_clear();
//	}
//}

//
int main(void)
{
	init();


	while(true)
	{}

}
