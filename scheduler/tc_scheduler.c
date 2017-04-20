/*
 *	tc_scheduler.c
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
#include "timer/tc.h"
#include "work-queue/work.h"
#include "asf.h"

extern struct scheduler_task adc_tick_task;
extern struct scheduler_task ambient_tick_task;
extern struct scheduler_task pending_req_tick_task;

struct tc_scheduler_task {
	struct scheduler_task task;
	int overlaps_count;
	uint16_t offset;
};

#define NUMBER_OF_TICK_TASKS		3
static struct tc_scheduler_task tick_tasks_to_do[NUMBER_OF_TICK_TASKS] = {{{ 0 }}};

static struct tc_scheduler_task new_tick_task(struct scheduler_task task)
{
	struct tc_scheduler_task res = {{ 0 }};
	res.task = task;
    res.overlaps_count = -1;
    res.offset = 0;
	return res;
}

static bool tc_can_schedule = false;
void switch_tc_interrupt_schedule(bool on)
{
	tc_can_schedule = on;
}

static uint32_t get_ticks_in_sec(void)
{
	return sysclk_get_cpu_hz() / tc_get_div();
}

static bool task_due_before_scheduled(uint8_t task_id)
{
	return tick_tasks_to_do[task_id].overlaps_count == 0 &&
			(!is_tc_cmp_enable() || tick_tasks_to_do[task_id].offset < TCC0.CCA);
}

static void schedule_closest_task(void)
{
	bool scheduled_task_exists = false;
	array_foreach(struct tc_scheduler_task, tick_tasks_to_do, index) {
		if (task_due_before_scheduled(index)) {
			tc_set_cmp_value(tick_tasks_to_do[index].offset);
			scheduled_task_exists = true;
		}
	}

	if (scheduled_task_exists)
		tc_cmp_enable();
	else
		tc_cmp_disable();
}

static void task_set_timer(int task_index)
{
	struct tc_scheduler_task *task = &tick_tasks_to_do[task_index];
	uint32_t ticks = task->task.get_recur_period() * get_ticks_in_sec();
	uint16_t ticks_to_overflow = TIMER_MAX_VALUE - TCC0.CNT;
	if (ticks < ticks_to_overflow) {
		task->overlaps_count = 0;
		task->offset = ticks + TCC0.CNT;
	} else {
		ticks -= ticks_to_overflow;
		task->overlaps_count = ticks / TIMER_MAX_VALUE + 1;
		task->offset = ticks % TIMER_MAX_VALUE;
	}
}

void tc_scheduler_init(void)
{
    tick_tasks_to_do[0] = new_tick_task(pending_req_tick_task);
	tick_tasks_to_do[1] = new_tick_task(ambient_tick_task);
	tick_tasks_to_do[2] = new_tick_task(adc_tick_task);

	array_foreach(struct tc_scheduler_task, tick_tasks_to_do, index)
		task_set_timer(index);

	schedule_closest_task();
}

ISR(TCC0_CCA_vect)
{
	if (!tc_can_schedule)
		return;

	/* Find expired task and add it to the work queue */
	array_foreach(struct tc_scheduler_task, tick_tasks_to_do, index) {
		if (tick_tasks_to_do[index].overlaps_count == 0 &&  tick_tasks_to_do[index].offset <= TCC0.CNT) {
			insert_work(tick_tasks_to_do[index].task.work);
			task_set_timer(index);
		}
	}

	schedule_closest_task();
}

ISR(TCC0_OVF_vect)
{
	if (!tc_can_schedule)
		return;

	array_foreach(struct tc_scheduler_task, tick_tasks_to_do, index) {
		if (tick_tasks_to_do[index].overlaps_count <= 0)
			continue;
		else
			tick_tasks_to_do[index].overlaps_count--;
	}

	schedule_closest_task();
}
