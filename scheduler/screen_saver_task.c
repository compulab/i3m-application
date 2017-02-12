#include "scheduler.h"

bool reset_screen_saver_req;

static double screen_saver_get_recur_period(void)
{
    return computer_data.details.screen_saver_update_time;
}

void update_screen_saver()
{
	if (!screen_saver_mode_enabled || computer_data.details.screen_saver_visible != 1)
        return;

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

static struct work screen_saver_work = { .do_work = update_screen_saver };

struct scheduler_task screen_saver_sec_task = {
    .work = &screen_saver_work,
    .get_recur_period = screen_saver_get_recur_period,
};
