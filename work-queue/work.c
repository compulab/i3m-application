/*
 * work.c
 *
 *  Created on: Nov 8, 2015
 *      Author: arkadi
 */
#include "work.h"
#include "../debug.h"
#include "../uart/uart.h"
#include "../Fp-utils.h"

static struct work_queue work_to_do = {
		.first =  NULL,
		.last =  NULL,
};


int insert_work(struct work *work)
{
	cli();
	struct work *new_work = malloc(sizeof(struct work));
	if (new_work == NULL) {
		computer_data.details.error_count++;
		insert_to_log('E');
		sei();
		return -1;
	}
	sei();
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

bool work_handler(void)
{
	if (work_to_do.last == NULL) {
		return false;
	}
//	uart_send_string("first: ");
//	uart_send_num((uint32_t)work_to_do.first, 16);
//	uart_send_string(" last: ");
//	uart_send_num((uint32_t)work_to_do.last, 16);
	cli();
	struct work* work = work_to_do.first;
	if (work_to_do.first == work_to_do.last) {
		work_to_do.first = NULL;
		work_to_do.last = NULL;
	} else {
		work_to_do.first = work_to_do.first->next;
	}
	sei();
//	uart_send_string(" curr: ");
//	uart_send_num((uint32_t)work, 16);
//	if (work_to_do.first == NULL)
//		uart_send_string(" last work");
//	uart_send_string("\n\r");
	work->do_work(work->data);
	cli();
	free(work);
	sei();
	if (computer_data.details.error_count > 0) {
			computer_data.details.error_count--;
			insert_to_log('O');
		}
	return true;
}

