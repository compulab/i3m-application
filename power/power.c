/*
 * power.c
 *
 * Created: 3/23/2017 5:10:21 PM
 *  Author: Nikita
 */

#include <string.h>
#include "power.h"
#include "effects.h"
#include "twi/i2c_buffer.h"
#include "work-queue/work.h"
#include "gfx/gfx_information.h"

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

static void handle_power_off(void)
{
	//	reset_ambient();
	reset_temperatures();
}

static void handle_power_on(void)
{
	update_adc(NULL);
	update_ambient_temp(NULL);
}

static void handle_power_state_changed(void *data)
{
	switch(current_power_state) {
	case POWER_ON:
		handle_power_on();
		enter_power_on_mode();
		break;
	case POWER_STD:
		enter_hibernate_mode();
		break;
	case POWER_STR:
		enter_sleep_mode();
		break;
	case POWER_OFF:
		enter_power_off_mode();
		handle_power_off();
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

void sprintf_power_state(struct gfx_information *info, char *output_str)
{
	switch (current_power_state){
	case POWER_ON:
		strcpy(output_str, "Airtop on");
		break;
	case POWER_STR:
		strcpy(output_str, "Sleep");
		break;
	case POWER_STD:
		strcpy(output_str, "Hibernate");
		break;
	case POWER_OFF:
		strcpy(output_str, "Airtop off");
		break;
	default:
		strcpy(output_str, "");
		break;
	}
}

int gfx_information_init_show_power_state(struct gfx_information *info)
{
	info->to_string = sprintf_power_state;
	return 0;
}

static void sprintf_power_data(struct gfx_information *info, char *output_str)
{
	long power = computer_data.details.adc * 0.10137 + 2.9;

	if (!computer_data.details.adc_set)
		sprintf(output_str, "-");
	else if (power >= 6 && power <= 300)
		sprintf(output_str, "%ld W", power);
	else
		sprintf(output_str, "LOW");
}

int gfx_information_init_show_power_data(struct gfx_information *info)
{
	info->to_string = sprintf_power_data;
	return 0;
}