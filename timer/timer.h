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
#include "../uart/uart.h"
#include "../calendar/calendar.h"

//#define SUPPORT_HOLD_BUTTON

#define MENU_SHOW_TIME			150
#define SLEEP_TIME				500
//#define MOVE_BUTTON_TIME		7
#define UPDATE_ADC_SEC			1
#define UPDATE_SCREEN_TIME		1
//#define MAX_CLICK_SEC		MOVE_BUTTON_TIME
#define UPDATE_REQ_SEC			1
#define UPDATE_SCREEN_SAVER_SEC	5
#define UPDATE_SCREEN_SEC		1
#define UPDATE_AMBIENT_SEC		4


#define NUMBER_OF_TICK_TASKS		3

enum TYPE_OF_TICK_TASK {
	PENDING_REQ_TASK 	= 	0,
	AMBIENT_TASK	  	=	1,
	ADC_TASK		 	=	2,
};

struct scheduler_tick_task {
	int overlaps_count;
	uint16_t offset;
	struct work *work;
	double (* get_recur_period)(void);
};

#define NUMBER_OF_SEC_TASKS		4

enum TYPE_OF_SEC_TASK {
	SCREEN_SAVER_TASK 		=	0,
	TIME_TASK 				=	1,
	PRINT_WORKS_COUNT_TASK	=	2,
	UPDATE_SCREEN_TASK		= 	3,

};

struct scheduler_sec_task {
	int secs_left;
	struct work *work;
	double (* get_recur_period)(void);
};

extern bool reset_screen_saver_req;

void reset_screen_saver(void);

void update_screen_timer();

void tc_button_pressed(void);

void tc_no_button_pressed(void);

void init_ambient(void);

void update_tasks_timeout(void);

void ticks_task_update_overlap(void);

void ticks_task_update_work(void);

void tasks_init(void);

#endif /* TIMER_TIMER_H_ */
