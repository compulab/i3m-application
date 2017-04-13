/*
 * update_screen_task.c
 *
 * Author: Nikita Kiryanov
 */
#include "scheduler/scheduler.h"
#include "lib/syntax.h"
#include "gfx/gfx_components/gfx_information.h"
#include "gfx/action_menu/gfx_item_action.h"
#include "gfx/action_menu/gfx_action_menu.h"

enum display_state display_state;

static bool is_information_need_to_change(struct gfx_information *info)
{
	switch (info->info_type) {
	case SET_SCREEN_SAVER_ENABLE:
	case SET_SCREEN_SAVER_TYPE:
	case SET_SCREEN_SAVER_TIME:
	case SET_BRIGHTNESS:
	case SHOW_SERIAL_NUMBER:
	case SHOW_APP_VERSION:
	case SHOW_PART_NUMBER:
	case SHOW_MAC_ADDRESS:
		return false;
	default:
		return true;
	}
}

static bool is_menu_need_update(struct gfx_action_menu *menu)
{
	for (uint8_t i = 0; i < menu->menu->num_elements; i++) {
		if (menu->actions[i].type != ACTION_TYPE_SHOW_FRAME)
			continue;

		if (menu->actions[i].frame == dashboard)
			continue;

		list_foreach(struct gfx_information_node *, menu->actions[i].frame->information_head, info_node) {
			if (is_information_need_to_change(&info_node->information))
				return true;
		}
	}

	return false;
}

static void update_screen(void *data)
{
	if (display_state == DISPLAY_MENU) {
		if (is_menu_need_update(present_menu))
			gfx_action_menu_display(present_menu);
	} else {
		frame_present->draw(frame_present, false);
	}
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
