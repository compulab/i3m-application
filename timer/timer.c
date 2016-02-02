/*
 * timer.c
 *
 *  Created on: Nov 9, 2015
 *      Author: arkadi
 */

#include "timer.h"


#define MAX_AMBIENT_UPDATE_FAIL	2

bool first_ambient_read;
bool reset_screen_saver_req;
uint8_t ambient_update_fail_count;

static struct scheduler_tick_task tick_tasks_to_do[NUMBER_OF_TICK_TASKS];
static struct scheduler_sec_task sec_tasks_to_do[NUMBER_OF_SEC_TASKS];

void reset_ambient()
{
	first_ambient_read = false;
	layout.l.ambs = 0;
}

void update_ambient_temp()
{
	uart_send_string("update_ambient_temp\n\r");
	if (current_power_state == POWER_ON) {
		bool valid_update;
		if (first_ambient_read) {
			valid_update = TWI_read_reg(AMBIENT_TWI_ADDRESS, AMBIENT_TEMPERATURE_ADDRESS, &layout.l.ambt, 2);
			first_ambient_read = !valid_update;
		} else {
			valid_update = TWI_read_reg(AMBIENT_TWI_ADDRESS, AMBIENT_TEMPERATURE_ADDRESS, &layout.l.ambt, 2);
		}

		if (valid_update) {
			ambient_update_fail_count = 0;
			layout.l.ambs = 1;
		} else if (ambient_update_fail_count == MAX_AMBIENT_UPDATE_FAIL) {
			layout.l.ambs = 0;
		} else  {
			ambient_update_fail_count++;
			update_ambient_temp();
		}
	}
}

void time_task()
{
	uart_send_string("time_task\n\r");
	calendar_add_second_to_date(&computer_date_time);
}

void tc_handle_init()
{
	first_ambient_read = false;
	ambient_update_fail_count = 0;
}

void update_screen_saver()
{
	if (screen_saver_mode_enabled && computer_data.details.screen_saver_visible == 1) {
		switch(computer_data.details.screen_saver_type) {
		case SCREEN_SAVER_SPLASH:
			show_splash();
			break;
		case SCREEN_SAVER_DASHBOARD:
			if (current_power_state != POWER_OFF && dashboard != NULL) {
				show_frame(dashboard);
				display_state = DISPLAY_DASHBOARD;
			}
			break;
		}
	}
}

static struct work requests_work = { .do_work = update_requests, .data = NULL, .next = NULL, };
static struct work ambient_work = { .do_work = update_ambient_temp, .data = NULL, .next = NULL, };
static struct work adc_work = { .do_work = update_adc, .data = NULL, .next = NULL, };
static struct work update_screen_work = { .do_work = update_info, .data = NULL, .next = NULL, };
static struct work print_works_count_work = { .do_work = print_work_count, .data = NULL, .next = NULL, };
//static struct work buttons_clear_work = { .do_work = handle_button_pressed, .data = NULL, .next = NULL, };
static struct work screen_saver_work = { .do_work = update_screen_saver, .data = NULL, .next = NULL, };
static struct work time_work = { .do_work = time_task , .data = NULL, .next = NULL, };



uint32_t get_ticks_in_sec()
{
	uint16_t tc_div;
	switch (TCC0.CTRLA & TC_CLKSEL_DIV1024_gc) {
	case TC_CLKSEL_DIV1_gc:
		tc_div = 1;
		break;
	case TC_CLKSEL_DIV2_gc:
		tc_div = 2;
		break;
	case TC_CLKSEL_DIV4_gc:
		tc_div = 4;
		break;
	case TC_CLKSEL_DIV8_gc:
		tc_div = 8;
		break;
	case TC_CLKSEL_DIV64_gc:
		tc_div = 64;
		break;
	case TC_CLKSEL_DIV256_gc:
		tc_div = 256;
		break;
	case TC_CLKSEL_DIV1024_gc:
		tc_div = 1024;
		break;
	default:
		tc_div = 1;
		break;
	}

	return sysclk_get_cpu_hz() / tc_div;
}

void set_sec_task_timer(uint8_t sec_to_update, enum TYPE_OF_SEC_TASK type)
{
	sec_tasks_to_do[type].secs_left = sec_to_update;
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

void set_tick_task_timer(double sec_to_update, enum TYPE_OF_TICK_TASK type)
{
	struct scheduler_tick_task *task = &tick_tasks_to_do[type];
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


void print_works_count_timer()
{
	set_sec_task_timer(1, PRINT_WORKS_COUNT_TASK);
}

void time_set_timer()
{
	set_sec_task_timer(1 , TIME_TASK);
}

void screen_saver_set_timer()
{
	set_sec_task_timer(computer_data.details.screen_saver_update_time , SCREEN_SAVER_TASK);
}

void ambient_set_timer()
{
	set_tick_task_timer(UPDATE_AMBIENT_SEC, AMBIENT_TASK);
}

void adc_set_timer()
{
	set_tick_task_timer(UPDATE_ADC_SEC, ADC_TASK);
}

void update_screen_timer()
{
	set_sec_task_timer(UPDATE_SCREEN_TIME, UPDATE_SCREEN_TASK);
}

void pending_req_set_timer()
{
	set_tick_task_timer(UPDATE_REQ_SEC, PENDING_REQ_TASK);
}

void reset_screen_saver()
{
	screen_saver_set_timer();
}

static struct scheduler_sec_task sec_tasks_to_do[NUMBER_OF_SEC_TASKS] = {
		{
				.secs_left = -1,
				.work = &screen_saver_work,
				.set_new_timer = screen_saver_set_timer,
		},
		{
				.secs_left = -1,
				.work = &time_work,
				.set_new_timer = time_set_timer,
		},
		{
				.secs_left = -1,
				.work = &print_works_count_work,
				.set_new_timer = print_works_count_timer,
		},
		{
				.secs_left = -1,
				.work = &update_screen_work,
				.set_new_timer = update_screen_timer,
		},
};



static struct scheduler_tick_task tick_tasks_to_do[NUMBER_OF_TICK_TASKS] = {
		{
				.overlaps_count = -1,
				.offset = 0,
				.work = &requests_work,
				.set_new_timer = pending_req_set_timer,
		},
		{
				.overlaps_count = -1,
				.offset = 0,
				.work = &ambient_work,
				.set_new_timer = ambient_set_timer,
		},
		{
				.overlaps_count = -1,
				.offset = 0,
				.work = &adc_work,
				.set_new_timer = adc_set_timer,
		},


};

bool set_task_cmp(uint8_t task_id)
{
	if (tick_tasks_to_do[task_id].overlaps_count == 0 && (!is_tc_cmp_enable() || tick_tasks_to_do[task_id].offset < TCC0.CCA)) {
		TCC0.CCA = tick_tasks_to_do[task_id].offset;
		return true;
	}

	return false;
}

void find_next_task(void)
{
	bool task_is_set = false;
	for (uint8_t i = 0; i < NUMBER_OF_TICK_TASKS; i++) {
		if (set_task_cmp(i))
			task_is_set = true;
	}

	if (task_is_set)
		tc_cmp_enable();
	else
		tc_cmp_disable();
}

void tasks_init(void)
{
	for (uint8_t i = 0; i < NUMBER_OF_TICK_TASKS; i++)
		tick_tasks_to_do[i].set_new_timer();
	for (uint8_t i = 0; i < NUMBER_OF_SEC_TASKS; i++)
		sec_tasks_to_do[i].set_new_timer();


	find_next_task();
}

void rtc_handle_sec_update(void)
{
	for (int i = 0; i < NUMBER_OF_SEC_TASKS; i ++) {
		if (sec_tasks_to_do[i].secs_left > 0)
			sec_tasks_to_do[i].secs_left--;
	}

	for (int i = 0; i < NUMBER_OF_SEC_TASKS; i ++) {
		if (sec_tasks_to_do[i].secs_left == 0) {
			if (!insert_work(sec_tasks_to_do[i].work))
				insert_to_log('S'+i);
			sec_tasks_to_do[i].set_new_timer();
		}
	}
}

void tc_handle_overflow_interrupt(void)
{
	for (uint8_t i = 0; i < NUMBER_OF_TICK_TASKS; i++) {
		if (tick_tasks_to_do[i].overlaps_count <= 0)
			continue;
		else
			tick_tasks_to_do[i].overlaps_count--;
	}

	find_next_task();
}


void tc_handle_cmp_interrupt(void)
{
	/* Find expired task and add it to the work queue */
	for (uint8_t i = 0; i < NUMBER_OF_TICK_TASKS; i++) {
		if (tick_tasks_to_do[i].overlaps_count == 0 &&  tick_tasks_to_do[i].offset <= TCC0.CNT) {
			if (!insert_work(tick_tasks_to_do[i].work))
				insert_to_log('T'+i);
			tick_tasks_to_do[i].set_new_timer();
		}
	}

	find_next_task();
}
