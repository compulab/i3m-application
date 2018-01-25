/*
 *	scheduler.c
 *
 * 	This scheduler uses the Timer/Counter facilities to implement a task scheduling mechanism that
 * 	supports a fine granularity of schedule timing (ticks as a time unit).
 *
 *	Operation details:
 *	Unlike an RTC that can keep counting up for a long time before resetting, the TC performs a
 *	short count-up-and-reset loop. Since we wish to be able to define any time period for task
 *	recurrence, even one that is longer than the max count value, tc scheduler must keep track of
 *	how many times a count overlap has to happen before the remainder of the wait period for a given
 *	task becomes small enough to fit into the current count up (and of course, it has to know what
 *	this remainder of the wait period is in order to schedule the task for the current count up).
 *	This information is stored as task metadata.
 *
 * 	Author: Nikita Kiryanov
 */

#include "scheduler.h"
#include "lib/syntax.h"
#include "work-queue/work.h"
#include "asf.h"

extern struct scheduler_task adc_tick_task;
extern struct scheduler_task pending_req_tick_task;
extern struct scheduler_task screen_tick_task;

struct tc_scheduler_task {
	struct scheduler_task task;
	int overlaps_count;
	double offset;
};

volatile void *tc_module = NULL;

static bool cmp_enable = false;

#define TIMER_MAX_VALUE			0xffff
#define NUMBER_OF_TICK_TASKS	3
static struct tc_scheduler_task tick_tasks_to_do[NUMBER_OF_TICK_TASKS] = {{{ 0 }}};

static struct tc_scheduler_task new_tick_task(struct scheduler_task task)
{
	struct tc_scheduler_task res = {{ 0 }};
	res.task = task;
    res.overlaps_count = -1;
    res.offset = 0;
	return res;
}

static void task_set_timer(int task_index)
{
	struct tc_scheduler_task *task = &tick_tasks_to_do[task_index];
	task->offset = task->task.get_recur_period();
}

static void tc_overflow_callback(void)
{
	tc_clear_overflow(tc_module);
	array_foreach(struct tc_scheduler_task, tick_tasks_to_do, index) {
		tick_tasks_to_do[index].offset -= 0.1;
		if (tick_tasks_to_do[index].offset <= 0) {
			insert_work(tick_tasks_to_do[index].task.work);
			task_set_timer(index);
		}
	}
}

void scheduler_init(volatile void *tc)
{
	tc_module = tc;
	tc_set_overflow_interrupt_callback(tc_module, tc_overflow_callback);
	tc_set_overflow_interrupt_level(tc_module, TC_INT_LVL_HI);

    tick_tasks_to_do[0] = new_tick_task(pending_req_tick_task);
	tick_tasks_to_do[1] = new_tick_task(adc_tick_task);
	tick_tasks_to_do[2] = new_tick_task(screen_tick_task);

	array_foreach(struct tc_scheduler_task, tick_tasks_to_do, index)
		task_set_timer(index);
}