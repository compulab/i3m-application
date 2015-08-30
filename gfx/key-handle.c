/*
 * keypad.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */
#include "key-handle.h"

#include "../asf.h"

bool left_button_status = true,
	right_button_status = true,
	select_button_status = true,
	debug_button_status = true;

enum button_state get_button_status(bool *last_status, uint8_t button_pos){
		bool current_status = ioport_get_value(button_pos);
		enum button_state button_state;
/*		delay_ms(300);
		bool longStatus = ioport_get_value(button_pos); */

		if (current_status && !(*last_status))
/*			if (longStatus == currentStatus)
				return BUTTON_HOLD;
			else */
				button_state = BUTTON_CLICK;
		else
			button_state = BUTTON_NOT_PRESSED;

		*last_status = current_status;
		return button_state;
	}


enum button_state get_left_button_status(){
	return get_button_status(&left_button_status,FP_LEFT_BUTTON);
}

enum button_state get_right_button_status(){
	return get_button_status(&right_button_status,FP_RIGHT_BUTTON);
}

bool get_hold_button_status(){
	return get_button_status(&debug_button_status,FP_DEBUG_BACK_BUTTON) == BUTTON_CLICK;
}

enum button_state get_select_button_status(){
	if (get_hold_button_status()) return BUTTON_HOLD; /* for debug only */
	return get_button_status(&select_button_status,FP_OK_BUTTON);
}

void getPressedKey(enum key_Select *selected_Key){
	enum button_state left_button = get_left_button_status(),
			right_button = get_right_button_status(),
			select_button = get_select_button_status();
	*selected_Key = KEY_NONE;
	if (left_button == BUTTON_CLICK) *selected_Key = *selected_Key | KEY_LEFT;
	if (right_button == BUTTON_CLICK) *selected_Key = *selected_Key | KEY_RIGHT;
	if (select_button == BUTTON_CLICK) *selected_Key = *selected_Key | KEY_SELECT;
	if (select_button == BUTTON_HOLD) *selected_Key = *selected_Key | KEY_HOLD;
}
