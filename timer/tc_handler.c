/*
 * tc_handler.c
 *
 *  Created on: Oct 13, 2015
 *      Author: arkadi
 */

#include "tc-handler.h"

bool sleep_mode_enabled;
bool update_buttons;

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

void tc_handle()
{
	update_adc();
	if (update_buttons && (tc_counter % MOVE_BUTTON_TIME == 0))
		handle_button_pressed();
	if (tc_counter == SLEEP_TIME) {
		tc_counter = 0;
		if (sleep_mode_enabled)
			show_splash();
	}
}
