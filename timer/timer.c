/*
 * timer.c
 *
 *  Created on: Nov 9, 2015
 *      Author: arkadi
 */

#include "timer.h"
#include "../lib/syntax.h"

bool reset_screen_saver_req;

struct scheduler_sec_task {
	struct scheduler_task task;
	int secs_left;
};

struct scheduler_tick_task {
	struct scheduler_task task;
	int overlaps_count;
	uint16_t offset;
};

#define NUMBER_OF_TICK_TASKS		3
static struct scheduler_tick_task tick_tasks_to_do[NUMBER_OF_TICK_TASKS];

#define UPDATE_SCREEN_TASK		3

#define NUMBER_OF_SEC_TASKS		4
static struct scheduler_sec_task sec_tasks_to_do[NUMBER_OF_SEC_TASKS];

void time_task()
{
	uart_send_string("time_task\n\r");
	calendar_add_second_to_date(&computer_date_time);
}

void update_screen_saver()
{
	if (screen_saver_mode_enabled && computer_data.details.screen_saver_visible == 1) {
		switch(computer_data.details.screen_saver_type) {
		case SCREEN_SAVER_SPLASH:
			show_logo();
			break;

		case SCREEN_SAVER_DASHBOARD:
			if (current_power_state != POWER_OFF && dashboard != NULL)
				show_frame(dashboard);
			break;

		case SCREEN_SAVER_CLOCK:
			if (clock != NULL && calendar_is_date_valid(&computer_date_time)) {
				show_frame(clock);
				display_state = DISPLAY_CLOCK;
			}
		}
	}
}

static struct work update_screen_work = { .do_work = update_info, .data = NULL, .next = NULL, };
static struct work print_works_count_work = { .do_work = print_work_count, .data = NULL, .next = NULL, };
//static struct work buttons_clear_work = { .do_work = handle_button_pressed, .data = NULL, .next = NULL, };
static struct work screen_saver_work = { .do_work = update_screen_saver, .data = NULL, .next = NULL, };
static struct work time_work = { .do_work = time_task , .data = NULL, .next = NULL, };

static uint32_t get_ticks_in_sec()
{
	return sysclk_get_cpu_hz() / tc_get_div();
}

void set_sec_task_timer(uint8_t sec_to_update, int task_id)
{
	sec_tasks_to_do[task_id].secs_left = sec_to_update;
}

void print_debug_tick_set(uint32_t ticks_in_sec, double sec, uint32_t ticks, uint16_t overflow)
{
	uart_send_string("Debug Tick set: \n\r");
	uart_send_num(ticks_in_sec, 16);
	uart_send_string(" - ticks in sec\n\r");
	uart_send_num(sec, 16);
	uart_send_string(" - secs to timeout\n\r");
	uart_send_num(ticks, 16);
	uart_send_string(" - ticks till timeout\n\r");
	uart_send_num(overflow, 16);
	uart_send_string(" - ticks to next round\n\r");
}

void set_tick_task_timer(double sec_to_update, int task_id)
{
	struct scheduler_tick_task *task = &tick_tasks_to_do[task_id];
	uint32_t ticks = sec_to_update * get_ticks_in_sec();
	uint16_t ticks_to_overflow = TIMER_MAX_VALUE - TCC0.CNT;
	if (sec_to_update == 0.5)
		print_debug_tick_set(get_ticks_in_sec(), sec_to_update, ticks, ticks_to_overflow);
	if (ticks < ticks_to_overflow) {
		task->offset = ticks + TCC0.CNT;
		task->overlaps_count = 0;
	} else {
		ticks -= ticks_to_overflow;
		task->overlaps_count = ticks / TIMER_MAX_VALUE + 1;
		task->offset = ticks % TIMER_MAX_VALUE;
	}
}

/*
 * Set timer for new work of updating works count - Debug
 */
double print_works_get_recur_period(void)
{
	return 1;
}

/*
 * Set timer for new work of updating RTC time
 */
double time_get_recur_period(void)
{
	return 1;
}

/*
 * Set timer for new work of updating screen saver
 */
double screen_saver_get_recur_period(void)
{
	return computer_data.details.screen_saver_update_time;
}

/*
 * Set timer for new work of screen information
 */
#define UPDATE_SCREEN_TIME		1
double screen_get_recur_period(void)
{
	return UPDATE_SCREEN_TIME;
}

void update_screen_timer(void)
{
	set_sec_task_timer(UPDATE_SCREEN_TIME, UPDATE_SCREEN_TASK);
}

/*
 * Reset screen saver timer
 */
void reset_screen_saver()
{
	screen_saver_get_recur_period();
}

static struct scheduler_sec_task sec_tasks_to_do[NUMBER_OF_SEC_TASKS] = { 0 };

static struct scheduler_tick_task tick_tasks_to_do[NUMBER_OF_TICK_TASKS] = { 0 };

static bool task_due_before_scheduled(uint8_t task_id)
{
	return tick_tasks_to_do[task_id].overlaps_count == 0 &&
		(!is_tc_cmp_enable() || tick_tasks_to_do[task_id].offset < TCC0.CCA);
}

static void schedule_closest_task(void)
{
	bool scheduled_task_exists = false;
	array_foreach(struct scheduler_tick_task, tick_tasks_to_do, index) {
		if (task_due_before_scheduled(index)) {
			TCC0.CCA = tick_tasks_to_do[index].offset;
			scheduled_task_exists = true;
		}
	}

	if (scheduled_task_exists)
		tc_cmp_enable();
	else
		tc_cmp_disable();
}

struct scheduler_task print_works_count_sec_task = {
    .work = &print_works_count_work,
    .get_recur_period = print_works_get_recur_period,
};
struct scheduler_task screen_saver_sec_task = {
    .work = &screen_saver_work,
    .get_recur_period = screen_saver_get_recur_period,
};
struct scheduler_task screen_sec_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};
struct scheduler_task time_sec_task = {
    .work = &time_work,
    .get_recur_period = time_get_recur_period,
};

static struct scheduler_tick_task new_tick_task(struct scheduler_task task)
{
	struct scheduler_tick_task res = { 0 };
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

void tc_scheduler_init(void)
{
    tick_tasks_to_do[0] = new_tick_task(pending_req_tick_task);
	tick_tasks_to_do[1] = new_tick_task(ambient_tick_task);
	tick_tasks_to_do[2] = new_tick_task(adc_tick_task);

	array_foreach(struct scheduler_tick_task, tick_tasks_to_do, index)
		set_tick_task_timer(tick_tasks_to_do[index].task.get_recur_period(), index);

	schedule_closest_task();
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
		set_sec_task_timer(sec_tasks_to_do[index].task.get_recur_period(), index);
}

void tasks_init(void)
{
	rtc_scheduler_init();
	tc_scheduler_init();
	switch_rtc_interrupt_schedule(true);
	switch_tc_interrupt_schedule(true);
}

void update_tasks_timeout(void)
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
			set_sec_task_timer(sec_tasks_to_do[i].task.get_recur_period(), i);
		}
	}
}

void ticks_task_update_overlap(void)
{
	if (!tc_can_schedule)
		return;

	for (uint8_t i = 0; i < NUMBER_OF_TICK_TASKS; i++) {
		if (tick_tasks_to_do[i].overlaps_count <= 0)
			continue;
		else
			tick_tasks_to_do[i].overlaps_count--;
	}

	schedule_closest_task();
}


void ticks_task_update_work(void)
{
	if (!tc_can_schedule)
		return;

	/* Find expired task and add it to the work queue */
	for (uint8_t i = 0; i < NUMBER_OF_TICK_TASKS; i++) {
		if (tick_tasks_to_do[i].overlaps_count == 0 &&  tick_tasks_to_do[i].offset <= TCC0.CNT) {
			if (!insert_work(tick_tasks_to_do[i].task.work))
				insert_to_log('T'+i);
			set_tick_task_timer(tick_tasks_to_do[i].task.get_recur_period(), i);
		}
	}

	schedule_closest_task();
}

/*
 * Updating the time of tasks (By seconds) to start
 */
ISR(RTC_OVF_vect)
{
	update_tasks_timeout();
}

/*
 * Updating the time of tasks (By ticks) to start
 */
ISR(TCC0_CCA_vect)
{
	ticks_task_update_work();
}

/*
 * Updating the overlaps of tick tasks
 */
ISR(TCC0_OVF_vect)
{
	ticks_task_update_overlap();
}
