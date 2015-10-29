/*
 * tc_handler.c
 *
 *  Created on: Oct 13, 2015
 *      Author: arkadi
 */

#include "tc-handler.h"

#define MAX_AMBIENT_UPDATE_FAIL	2

bool sleep_mode_enabled;
bool update_buttons;
bool first_ambient_read;
uint8_t ambient_update_fail_count;

void tc_button_pressed()
{
	tc_counter = 0;
	update_buttons = true;
}

void tc_no_button_pressed()
{
	update_buttons = false;
}

void enable_sleep_mode()
{
	sleep_mode_enabled = true;
}

void disable_sleep_mode()
{
	sleep_mode_enabled = false;
}

void update_ambient_temp()
{
	bool valid_update;
	uint8_t last_temp = layout.l.ambt;
	bool last_valid = layout.l.ambs != 0;
	if (first_ambient_read) {
		valid_update = TWI_read_reg(AMBIENT_TWI_ADDRESS, AMBIENT_TEMPERATURE_ADDRESS, &layout.l.ambt, 2);
		if (valid_update) {
			first_ambient_read = false;
		}
	} else {
		valid_update  = TWI_read(AMBIENT_TWI_ADDRESS, &layout.l.ambt, 2);
	}
	if (valid_update) {
		ambient_update_fail_count = 0;
		layout.l.ambs = 1;
	} else if (ambient_update_fail_count == MAX_AMBIENT_UPDATE_FAIL) {
		layout.l.ambs = 0;
	} else  {
		ambient_update_fail_count++;
	}
	if (ambient_update_fail_count == MAX_AMBIENT_UPDATE_FAIL || ((layout.l.ambs == 1) && (!last_valid || last_temp != layout.l.ambt)))
		update_information_frame(SHOW_AMBIENT_TEMPERATURE, true);
}

void tc_handle_init()
{
	first_ambient_read = false;
	ambient_update_fail_count = 0;
}

void tc_handle()
{

	if (tc_counter % 10 == 0)
		update_ambient_temp();
	if (tc_counter % 35 == 0)
		update_adc();
	if (update_buttons && (tc_counter % MOVE_BUTTON_TIME == 0))
		handle_button_pressed();
	if (tc_counter == SLEEP_TIME) {
		tc_counter = 0;
		if (sleep_mode_enabled)
			show_splash();
	}
}
