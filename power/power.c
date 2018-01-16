/*
 * power.c
 *
 * Created: 3/23/2017 5:10:21 PM
 *  Author: Nikita
 */

#include "power.h"
#include "twi/i2c_buffer.h"
#include "work-queue/work.h"
#include "gfx/gfx_assets.h"
#include "gfx/gfx_gui_control.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "eeprom/eeprom_layout.h"
#include <string.h>

#define SLEEP_MSG 			"SLEEP"
#define HIBERNATE_MSG		"HIBERNATE"
#define POWER_OFF_MSG		"OFF"
#define SLEEP_BRIGHTNESS 	100

static void enter_dim_mode(char *msg, uint8_t x, uint8_t y)
{
	ssd1306_set_contrast(SLEEP_BRIGHTNESS);
	gfx_display_msg(msg, x, y);
}

enum power_state current_power_state = POWER_ON;

static void clear_regs(uint8_t *beg_addr, uint8_t *end_addr)
{
	for (uint8_t *addr = beg_addr; addr < end_addr; addr++)
		*addr = 0x00;
}

static void reset_temperatures(void)
{
	uint8_t *p_computer_data = (uint8_t *)&computer_data;
	for (int i = 0; i < ((uint16_t)&computer_data.details.direct_string - (uint16_t)&computer_data); i++)
		p_computer_data[i] = 0x00;
	clear_regs((uint8_t *)&i2c_buffer.raw[CPU0T], (uint8_t *)&i2c_buffer.raw[RESERVED42]);
	clear_regs((uint8_t *)&i2c_buffer.raw[CPU0F_LSB], (uint8_t *)&i2c_buffer.raw[RESERVED83]);
}

static void handle_power_state_changed(void *data)
{
	switch(current_power_state) {
	case POWER_ON:
		ssd1306_set_contrast(eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS));
		gfx_switch_to_current_menu();
		break;
	case POWER_STD:
		enter_dim_mode(HIBERNATE_MSG, 30, 25);
		break;
	case POWER_STR:
		enter_dim_mode(SLEEP_MSG, 30, 20);
		break;
	case POWER_OFF:
		enter_dim_mode(POWER_OFF_MSG, 40, 20);
		reset_temperatures();
		break;
	}
}

struct work power_state_work = { .do_work = handle_power_state_changed, .data = NULL, .next = NULL };

static bool can_update_power_state = false;

static void update_power_state(void)
{
	if (!can_update_power_state)
		return;

	enum power_state last_power_state = current_power_state;

	uint8_t fp_s5 = gpio_pin_is_low(FP_S5) ? 0 : 1;
	uint8_t fp_s4 = gpio_pin_is_low(FP_S4) ? 0 : 1;
	uint8_t fp_s3 = gpio_pin_is_low(FP_S3) ? 0 : 1;
	current_power_state = fp_s5 | (fp_s4 << 1) | (fp_s3 << 2);

	if (current_power_state != last_power_state) {
		//Airtop 2 changes startes very slowly. Hide this by displaying splash screen when going from
		//off state to on state.
		if (last_power_state == POWER_OFF) {
			ssd1306_set_contrast(eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS));
			gfx_switch_to_current_menu();
			gfx_show_splash_screen(3000);
		}
		i2c_buffer.layout.power_state = current_power_state;
		insert_work(&power_state_work);
	}
}

void power_state_init(void)
{
	can_update_power_state = true;
	update_power_state();
}

ISR(PORTF_INT0_vect)
{
	update_power_state();
}