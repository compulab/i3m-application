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

struct scheduler_task {
	struct work *work;
	double (*get_recur_period)(void);
};

extern bool reset_screen_saver_req;

struct scheduler_task adc_tick_task;
struct scheduler_task ambient_tick_task;
struct scheduler_task pending_req_tick_task;

void reset_screen_saver(void);
void update_screen_timer();
void tasks_init(void);

#endif /* TIMER_TIMER_H_ */
