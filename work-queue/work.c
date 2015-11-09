/*
 * work.c
 *
 *  Created on: Nov 8, 2015
 *      Author: arkadi
 */
#include "work.h"
#include "../debug.h"

static struct work_queue work_to_do = {
		.first =  NULL,
		.last =  NULL,
};

void insert_work(struct work *work)
{
	struct work *new_work = malloc(sizeof(struct work));
	new_work->data = work->data;
	new_work->do_work = work->do_work;
	new_work->next = NULL;
	cli();
	if (work_to_do.last != NULL)
		work_to_do.last->next = new_work;
	else
		work_to_do.first = new_work;
	work_to_do.last = new_work;
	sei();
}

bool work_handler(void)
{
	if (work_to_do.last == NULL) {
		return false;
	}
	struct work* last_work = work_to_do.first;
	cli();
	if (work_to_do.first == work_to_do.last) {
		work_to_do.first = NULL;
		work_to_do.last = NULL;
	} else {
		work_to_do.first = work_to_do.first->next;
	}
	sei();
	last_work->do_work(last_work->data);
	free(last_work);
	return true;
}

