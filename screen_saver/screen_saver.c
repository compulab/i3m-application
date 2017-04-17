/*
 * screen_saver_task.c
 *
 * Created: 3/18/2017 12:10:19 AM
 *  Author: Nikita
 */

#include "screen_saver.h"
#include "scheduler/scheduler.h"
#include "power/power.h"
#include "lib/syntax.h"
#include "gfx/action_menu/menu-handler.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "calendar/calendar.h"

bool reset_screen_saver_req;

bool screen_saver_mode_enabled;

void enable_screen_saver_mode(void)
{
	screen_saver_mode_enabled = true;
}

void disable_screen_saver_mode(void)
{
	screen_saver_mode_enabled = false;
}

static double screen_saver_get_recur_period(void)
{
	return computer_data.details.screen_saver_update_time;
}

static void update_screen_saver(void *data)
{
	if (!screen_saver_mode_enabled || !computer_data.details.screen_saver_visible)
		return;

	switch(computer_data.details.screen_saver_type) {
	case SCREEN_SAVER_SPLASH:
		gfx_show_screen_saver(DISPLAY_LOGO);
		break;

	case SCREEN_SAVER_DASHBOARD:
		if (current_power_state != POWER_OFF)
			gfx_show_screen_saver(DISPLAY_DASHBOARD);
		break;

	case SCREEN_SAVER_CLOCK:
		if (calendar_is_date_valid(&computer_date_time))
			gfx_show_screen_saver(DISPLAY_CLOCK);
		break;
	}
}

static struct work screen_saver_work = { .do_work = update_screen_saver };

struct scheduler_task screen_saver_sec_task = {
	.work = &screen_saver_work,
	.get_recur_period = screen_saver_get_recur_period,
};