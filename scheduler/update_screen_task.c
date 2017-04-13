/*
 * update_screen_task.c
 *
 * Author: Nikita Kiryanov
 */
#include "scheduler/scheduler.h"
#include "gfx/gfx_components/gfx_information.h"
#include "gfx/action_menu/gfx_item_action.h"
#include "gfx/action_menu/gfx_action_menu.h"

enum display_state display_state;

static void update_screen(void *data)
{
	if (display_state == DISPLAY_MENU)
		present_menu->draw(present_menu);
	else
		frame_present->draw(frame_present);
}

static struct work update_screen_work = { .do_work = update_screen };

static double screen_get_recur_period(void)
{
    return 1;
}

struct scheduler_task screen_sec_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};
