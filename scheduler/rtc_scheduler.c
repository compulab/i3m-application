#include "scheduler.h"
#include "lib/syntax.h"
#include "work-queue/work.h"
#include "asf.h"

extern struct scheduler_task time_sec_task;

struct scheduler_sec_task {
	struct scheduler_task;
	int secs_left;
};

#define NUMBER_OF_SEC_TASKS		1

static struct scheduler_sec_task sec_tasks_to_do[NUMBER_OF_SEC_TASKS] = {{{ 0 }}};

static void sec_task_set_timer(int task_index)
{
	sec_tasks_to_do[task_index].secs_left = sec_tasks_to_do[task_index].get_recur_period();
}

static struct scheduler_sec_task new_sec_task(struct scheduler_task task)
{
	struct scheduler_sec_task res = {{ 0 }};
	res.work = task.work;
	res.get_recur_period = task.get_recur_period;
	res.secs_left = -1;
	return res;
}

static bool rtc_can_schedule = false;
void switch_rtc_interrupt_schedule(bool on)
{
	rtc_can_schedule = on;
}

void rtc_scheduler_init(void)
{
    sec_tasks_to_do[0] = new_sec_task(time_sec_task);

	array_foreach(struct scheduler_sec_task, sec_tasks_to_do, index)
		sec_task_set_timer(index);
}

ISR(RTC_OVF_vect)
{
	if (!rtc_can_schedule)
		return;

	for (int i = 0; i < NUMBER_OF_SEC_TASKS; i ++) {
		if (sec_tasks_to_do[i].secs_left > 0)
			sec_tasks_to_do[i].secs_left--;
	}

	for (int i = 0; i < NUMBER_OF_SEC_TASKS; i ++) {
		if (sec_tasks_to_do[i].secs_left == 0) {
			insert_work(sec_tasks_to_do[i].work);
			sec_task_set_timer(i);
		}
	}
}
