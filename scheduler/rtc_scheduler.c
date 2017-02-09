#include "scheduler.h"
#include "../lib/syntax.h"

extern struct scheduler_task print_works_count_sec_task;
extern struct scheduler_task screen_saver_sec_task;
extern struct scheduler_task screen_sec_task;
extern struct scheduler_task time_sec_task;

struct scheduler_sec_task {
	struct scheduler_task task;
	int secs_left;
};

#define NUMBER_OF_SEC_TASKS		4

static struct scheduler_sec_task sec_tasks_to_do[NUMBER_OF_SEC_TASKS] = { 0 };

static void sec_task_set_timer(int task_index)
{
	sec_tasks_to_do[task_index].secs_left = sec_tasks_to_do[task_index].task.get_recur_period();
}

//Note: murder this thing with fire ASAP
#define UPDATE_SCREEN_TASK		3
#define UPDATE_SCREEN_TIME		1
void update_screen_timer(void)
{
	sec_tasks_to_do[UPDATE_SCREEN_TASK].secs_left = UPDATE_SCREEN_TIME;
}

static struct scheduler_sec_task new_sec_task(struct scheduler_task task)
{
	struct scheduler_sec_task res = { 0 };
	res.task = task;
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
    sec_tasks_to_do[0] = new_sec_task(screen_saver_sec_task);
    sec_tasks_to_do[1] = new_sec_task(time_sec_task);
    sec_tasks_to_do[2] = new_sec_task(print_works_count_sec_task);
    sec_tasks_to_do[3] = new_sec_task(screen_sec_task);

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
			if (!insert_work(sec_tasks_to_do[i].task.work))
				insert_to_log('S'+i);
			sec_task_set_timer(i);
		}
	}
}
