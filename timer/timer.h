/*
 * timer.h
 *
 *  Created on: Nov 9, 2015
 *      Author: arkadi
 */

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_


#ifndef GFX_GFX_ACTION_MENU_H_
#include "../gfx/action_menu/gfx_action_menu.h"
#endif

#ifndef FP_UTILS_H_
#include "../Fp-utils.h"
#endif

#ifndef TWI_MASTER_H_
#include "../twi/twi_master.h"
#endif

#include "tc.h"

//#define SUPPORT_HOLD_BUTTON

#define MENU_SHOW_TIME		150
#define SLEEP_TIME			500
//#define MOVE_BUTTON_TIME	7
#define UPDATE_ADC_SEC		1
//#define MAX_CLICK_SEC		MOVE_BUTTON_TIME
#define UPDATE_REQ_SEC		3
#define UPDATE_SCREEN_SEC	2
#define UPDATE_AMBIENT_SEC	4


#define NUMBER_OF_TASKS		5

enum TYPE_OF_TASK {
	SCREEN_SAVER_TASK =	0,
	AMBIENT_TASK	  =	1,
	ADC_TASK		  = 2,
	PENDING_REQ_TASK 	= 3,
	UPDATE_SCREEN_TASK	= 4,
};

struct scheduler_task {
	int overlaps_count;
	uint16_t offset;
	struct work *work;
	void (* set_new_timer)(void);
};



void tc_button_pressed();

void tc_no_button_pressed();

void reset_ambient();

void enable_sleep_mode();

void disable_sleep_mode();

void tc_handle_overflow_interrupt();

void tc_handle_cmp_interrupt();

void tasks_init();

#endif /* TIMER_TIMER_H_ */
