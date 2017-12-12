/*
 * work.c
 *
 *  Created on: Nov 8, 2015
 *      Author: arkadi
 */

#include "work.h"
#include "layout.h"
#include "asf.h"
#include "lib/memory.h"

bool wakeup;

static struct work_queue work_to_do = {
	.first =  NULL,
	.last =  NULL,
};

/*
 * Adding new work to the queue
 */
int insert_work(struct work *work)
{
	struct work *new_work = malloc_locked(sizeof(struct work));
	if (new_work == NULL)
		return -1;

	new_work->data = work->data;
	new_work->do_work = work->do_work;
	new_work->next = NULL;
	cli();
	if (work_to_do.last != NULL)
		work_to_do.last->next = new_work;
	else
		work_to_do.first = new_work;

	work_to_do.last = new_work;
	wakeup = true;
	sei();
	return 0;
}

/*
 * If the work queue is not empty, removing work and handling it
 */
bool work_handler(void)
{
	if (work_to_do.last == NULL) {
		return false;
	}

	cli();
	struct work* work = work_to_do.first;
	if (work_to_do.first == work_to_do.last) {
		work_to_do.first = NULL;
		work_to_do.last = NULL;
	} else {
		work_to_do.first = work_to_do.first->next;
	}
	sei();
	work->do_work(work->data);
	cli();
	free(work);
	sei();

	return true;
}

