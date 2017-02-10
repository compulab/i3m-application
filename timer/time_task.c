#include "timer.h"

void time_task()
{
	uart_send_string("time_task\n\r");
	calendar_add_second_to_date(&computer_date_time);
}

static double time_get_recur_period(void)
{
	return 1;
}

static struct work time_work = { .do_work = time_task };

struct scheduler_task time_sec_task = {
    .work = &time_work,
    .get_recur_period = time_get_recur_period,
};
