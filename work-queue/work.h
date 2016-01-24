/*
 * work.h
 *
 *  Created on: Nov 8, 2015
 *      Author: arkadi
 */

#ifndef WORK_QUEUE_WORK_H_
#define WORK_QUEUE_WORK_H_

#include "../asf.h"
#include "../debug.h"
#include "../uart/uart.h"
//#include "../Fp-utils.h"

struct work {
	void (*do_work)(void *);
	void *data;
	struct work *next;
};

struct work_queue {
	struct work *first;
	struct work *last;
};

extern uint16_t works_count;
extern bool wakeup;

void * malloc_locked(size_t size);

int insert_work(struct work *work);

bool work_handler(void);

#endif /* WORK_QUEUE_WORK_H_ */
