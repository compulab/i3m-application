#include "scheduler.h"

static struct work print_works_count_work = { .do_work = print_work_count };

static double print_works_get_recur_period(void)
{
	return 1;
}

struct scheduler_task print_works_count_sec_task = {
    .work = &print_works_count_work,
    .get_recur_period = print_works_get_recur_period,
};
