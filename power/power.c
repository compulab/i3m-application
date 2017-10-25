/*
 * power.c
 *
 * Created: 3/23/2017 5:10:21 PM
 *  Author: Nikita
 */

#include "power.h"
#include "adc/adc.h"
#include "twi/i2c_buffer.h"
#include "work-queue/work.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "eeprom/eeprom_layout.h"
#include "sram/sram_handle.h"
#include <string.h>

#define SLEEP_MSG 			"SLEEP"
#define HIBERNATE_MSG		"HIBERNATE"
#define POWER_OFF_MSG		"OFF"
#define SLEEP_BRIGHTNESS 	100

static void enter_dim_mode(char *msg)
{
	ssd1306_set_contrast(SLEEP_BRIGHTNESS);
	gfx_display_msg(msg);
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
		ssd1306_set_contrast(eeprom_get_brightness_value());
		break;
	case POWER_STD:
		enter_dim_mode(HIBERNATE_MSG);
		break;
	case POWER_STR:
		enter_dim_mode(SLEEP_MSG);
		break;
	case POWER_OFF:
		enter_dim_mode(POWER_OFF_MSG);
		reset_temperatures();
		break;
	}
	update_computer_state();
}

struct work power_state_work = { .do_work = handle_power_state_changed, .data = NULL, .next = NULL };

void update_power_state(void)
{
	enum power_state  last_power_state = current_power_state;

	if (gpio_pin_is_low(FP_S5)) {
		current_power_state = POWER_OFF;
	} else if (gpio_pin_is_low(FP_S4)) {
		if (last_power_state != POWER_ON) /* Hibernate mode can be only after power on*/
			return;
		current_power_state = POWER_STD;
	} else if (gpio_pin_is_low(FP_S3)) {
		if (last_power_state != POWER_ON) /* Sleep mode can be only after power on*/
			return;
		current_power_state = POWER_STR;
	} else {
		current_power_state = POWER_ON;
	}

	if (current_power_state != last_power_state) {
		i2c_buffer.layout.power_state = current_power_state;
		insert_work(&power_state_work);
	}
}

