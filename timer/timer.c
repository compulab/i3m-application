/*
 * timer.c
 *
 *  Created on: Nov 9, 2015
 *      Author: arkadi
 */

#include "timer.h"


#define MAX_AMBIENT_UPDATE_FAIL	2

bool sleep_mode_enabled;
bool update_buttons;
bool first_ambient_read;
uint8_t ambient_update_fail_count;

static struct scheduler_task tasks_to_do[NUMBER_OF_TASKS];

void tc_button_pressed()
{
	tc_counter = 0;
	update_buttons = true;
}

void tc_no_button_pressed()
{
	update_buttons = false;
}

void enable_sleep_mode()
{
	sleep_mode_enabled = true;
}

void disable_sleep_mode()
{
	sleep_mode_enabled = false;
}

void reset_ambient()
{
	first_ambient_read = false;
	layout.l.ambs = 0;
}

enum information_type update_information_type;

void update_ambient_temp()
{
	if (current_power_state == POWER_ON) {
		bool valid_update;
		uint8_t last_temp = layout.l.ambt;
		bool last_valid = layout.l.ambs != 0;
		if (first_ambient_read) {
			valid_update = TWI_read_reg(AMBIENT_TWI_ADDRESS, AMBIENT_TEMPERATURE_ADDRESS, &layout.l.ambt, 2);
			if (valid_update) {
				first_ambient_read = false;
			}
		} else {
			valid_update  = TWI_read(AMBIENT_TWI_ADDRESS, &layout.l.ambt, 2);
		}
		if (valid_update) {
			ambient_update_fail_count = 0;
			layout.l.ambs = 1;
		} else if (ambient_update_fail_count == MAX_AMBIENT_UPDATE_FAIL) {
			layout.l.ambs = 0;
		} else  {
			ambient_update_fail_count++;
		}
		if (ambient_update_fail_count == MAX_AMBIENT_UPDATE_FAIL || ((layout.l.ambs == 1) && (!last_valid || last_temp != layout.l.ambt)))
			update_information_frame(SHOW_AMBIENT_TEMPERATURE, true);
	}
}

void tc_handle_init()
{
	first_ambient_read = false;
	ambient_update_fail_count = 0;
}


//static struct work curr_screen_work = { .do_work = update_curr_screen, .data = NULL, .next = NULL, };
static struct work requests_work = { .do_work = update_requests, .data = NULL, .next = NULL, };
static struct work ambient_work = { .do_work = update_ambient_temp, .data = NULL, .next = NULL, };
static struct work adc_work = { .do_work = update_adc, .data = NULL, .next = NULL, };
//static struct work buttons_clear_work = { .do_work = handle_button_pressed, .data = NULL, .next = NULL, };
static struct work screen_saver_work = { .do_work = show_splash, .data = NULL, .next = NULL, };


uint16_t get_ticks_in_sec()
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
	return F_CPU / tc_div;
}

void set_task_timer(int sec_to_update, enum TYPE_OF_TASK type)
{
	struct scheduler_task *task = &tasks_to_do[type];
	uint32_t ticks = sec_to_update * get_ticks_in_sec();

	uint16_t ticks_to_overflow = TIMER_MAX_VALUE - TCC0.CNT;
	if (ticks < ticks_to_overflow) {
		task->offset = ticks + TCC0.CNT;
		task->overlaps_count = 0;
	} else {
		ticks -= ticks_to_overflow;
		task->overlaps_count = ticks / TIMER_MAX_VALUE + 1;
		task->offset = ticks % TIMER_MAX_VALUE;
	}
}

void screen_saver_set_timer()
{

}

void ambient_set_timer()
{
//	set_task_timer(UPDATE_AMBIENT_SEC, AMBIENT_TASK);
}

void adc_set_timer()
{
	set_task_timer(UPDATE_ADC_SEC, ADC_TASK);
}

void pending_req_set_timer()
{
//	set_task_timer(UPDATE_REQ_SEC, PENDING_REQ_TASK);
}

void update_screen_set_timer()
{
//	set_task_timer(UPDATE_SCREEN_SEC, UPDATE_SCREEN_TASK);
}


static struct scheduler_task tasks_to_do[NUMBER_OF_TASKS] = {
		{
				.overlaps_count = -1,
				.offset = 0,
				.work = &screen_saver_work,
				.set_new_timer = screen_saver_set_timer,
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
		{
				.overlaps_count = -1,
				.offset = 0,
				.work = &requests_work,
				.set_new_timer = pending_req_set_timer,
		},
		{
				.overlaps_count = -1,
				.offset = 0,
				.work = 0, //&curr_screen_work,
				.set_new_timer = update_screen_set_timer,
		},
};

bool set_task_cmp(uint8_t task_id)
{
	if (tasks_to_do[task_id].overlaps_count == 0 && (!is_tc_cmp_enable() || tasks_to_do[task_id].offset < TCC0.CCA)) {
		TCC0.CCA = tasks_to_do[task_id].offset;
		return true;
	}

	return false;
}

void find_next_task(void)
{
	bool task_is_set = false;
	for (uint8_t i = 0; i < NUMBER_OF_TASKS; i++) {
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
	for (uint8_t i = 0; i < NUMBER_OF_TASKS; i++)
		tasks_to_do[i].set_new_timer();

	find_next_task();
}

void tc_handle_overflow_interrupt(void)
{
	for (uint8_t i = 0; i < NUMBER_OF_TASKS; i++) {
		if (tasks_to_do[i].overlaps_count <= 0)
			continue;
		else
			tasks_to_do[i].overlaps_count--;
	}

	find_next_task();
}


void tc_handle_cmp_interrupt(void)
{
	/* Find expired task and add it to the work queue */
	for (uint8_t i = 0; i < NUMBER_OF_TASKS; i++) {
		if (tasks_to_do[i].overlaps_count == 0 && tasks_to_do[i].offset <= TCC0.CNT) {
			insert_work(tasks_to_do[i].work);
			tasks_to_do[i].set_new_timer();
		}
	}

	find_next_task();
}
