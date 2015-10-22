/*
 * tc-handler.h
 *
 *  Created on: Oct 13, 2015
 *      Author: arkadi
 */

#ifndef TIMER_TC_HANDLER_H_
#define TIMER_TC_HANDLER_H_
#include "tc.h"
#include "../gfx/action_menu/gfx_action_menu.h"
#include "../Fp-utils.h"


//#define SUPPORT_HOLD_BUTTON

#define MENU_SHOW_TIME		150
#define SLEEP_TIME			500
#define MOVE_BUTTON_TIME	7
#define UPDATE_ADC_TIME		15
#define MAX_CLICK_TIME		MOVE_BUTTON_TIME

void tc_handle_init();

void tc_handle();

void tc_button_pressed();

void tc_no_button_pressed();

void enable_sleep_mode();

void disable_sleep_mode();

#endif /* TIMER_TC_HANDLER_H_ */
