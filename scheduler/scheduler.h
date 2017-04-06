/*
 * scheduler.h
 *
 * Author: Nikita Kiryanov
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

#include "Fp-utils.h"

struct scheduler_task {
	struct work *work;
	//get_recur_period: returns recur period in seconds
	double (*get_recur_period)(void);
};

extern bool reset_screen_saver_req;

void rtc_scheduler_init(void);
void tc_scheduler_init(void);
void switch_tc_interrupt_schedule(bool on);
void switch_rtc_interrupt_schedule(bool on);

void reset_screen_saver(void);
void update_screen_timer(void);
void tasks_init(void);

#endif
