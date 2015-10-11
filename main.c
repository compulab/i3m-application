#include <string.h>
#include <math.h>
#include "u8glib/gfx_components.h"
#include "debug.h"
#include "u8glib/menu-handler.h"
#include "timer/tc.h"
#include "adc/adc.h"
#include "config/cnf_blk_components.h"
#include "u8glib/gfx_utils.h"
#include "twi/test_twi.h"
#include "twi/twi_slave.h"
#include "asf.h"
#include "u8glib/menu.h"
#include "u8glib/menu_utils.h"
#include "adc/adc.h"

#define MENU_SHOW_TIME		150
#define SLEEP_TIME			500
#define DEMO_SHOW


char power[10];

ISR(TWIE_TWIS_vect)
{
	twi_slave_interrupt_handler();
}

ISR(TWIC_TWIM_vect)
{
	interrupt_handler();
}


ISR(PORTF_INT0_vect)
{
	update_power_state();
}

ISR(PORTF_INT1_vect){
	handle_button_pressed();
}

void show_menu_u8g()
{
	show_menu(u8g_menus[0]);
}

void update_adc()
{
	set_power_data(power);
	u8g_draw_string(10,power);
}

ISR(TCC0_OVF_vect)
{
	tc_counter++;
	if (tc_counter % 400 == 0)
		update_adc();
	else if (tc_counter % 300 == 0)
		u8g_clear_screen();
//	if (tc_counter == MENU_SHOW_TIME && !present_menu->visible)
//		show_menu_u8g();
//	else if (tc_counter == SLEEP_TIME)
//		show_splash();
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
//	load_config_block_u8g();
//	show_menu_u8g();
//	load_config_block();
//	set_menu_by_id(&present_menu, 0);
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

#else
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
	computer_data.direct_string = NULL;
}

void init()
{
	board_init();
	sysclk_init();
	u8g_init();
//	gfx_mono_init();
	init_menu();
//	updated_info_init();
	adc_init();
	pmic_init();
	portf_init();
	power_state_init();
	sei();
	twi_slave_init();
	twi_master_init();
}



const u8g_pgm_uint8_t temp_title[] = "New Menu Title";
const u8g_pgm_uint8_t temp_item1[] = "Item 1";
const u8g_pgm_uint8_t __attribute__((section (".configData"))) temp_item2[] = "Item 2";

struct menu_item_node second_item = {
		.text = temp_item2,
		.prev = 0,
		.next = 0,
		.visible = true
};


struct menu_item_node first_item = {
		.text = temp_item1,
		.next = &second_item,
		.prev = 0,
		.visible = true
};

struct menu temp_menu = {
		.title = temp_title,
		.first_item = &first_item,
		.last_item = &second_item,
		.curr_selected_item = &first_item
};

int main(void)
{
	init();

	udc_start();
	udc_attach();

	update_adc();

	while(true)
	{}
}
