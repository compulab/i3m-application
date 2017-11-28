/*
 * scheduler.h
 *
 * Author: Nikita Kiryanov
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

#include <stdbool.h>

struct scheduler_task {
	struct work *work;
	//get_recur_period: returns recur period in seconds
	double (*get_recur_period)(void);
};

void scheduler_init(volatile void *tc);

#endif
