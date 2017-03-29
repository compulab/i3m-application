/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"
#include "scheduler/scheduler.h"
#include "twi/i2c_buffer.h"
#include "lib/syntax.h"
#include "power/power.h"
#include "gfx/gfx_information.h"
#include "gfx/gfx_item_action.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "screens/static_data/static_data.h"

enum display_state display_state;

struct calendar_date computer_date_time = {
    .second = 40,
    .minute = 02,
    .hour = 11,
    .date = 26,
    .month = 11,
    .year = 2015
};

static bool is_data_need_update_packed(uint8_t data, struct gfx_information *info, bool is_visible)
{
	if (!is_visible)
		return false;

	bool data_valid = BIT_ON(data, info->info_data);
	return present_menu->visible ? !data_valid : data_valid;
}

static bool is_information_need_to_change(struct gfx_information *info, bool is_visible)
{
	switch (info->info_type) {
	case SHOW_COMPUTER_POWER:
		return !present_menu->visible;
	case SHOW_CPU_FREQUENCY:
		return is_data_need_update_packed(computer_data.packed.cpu_freq_set, info, is_visible);
	case SHOW_HDD_SIZE:
		return is_hdd_size_need_update(info, is_visible);
	case SHOW_MEMORY_SIZE:
		return is_mem_size_need_update(info, is_visible);
	case SHOW_HDD_TEMPERTURE:
		return is_hdd_temp_need_update(info, is_visible);
	case SHOW_CPU_TEMPERTURE:
		return is_data_need_update_packed(computer_data.packed.cpu_temp_set, info, is_visible);
	case SHOW_GPU_TEMPERTURE:
		return is_gpu_temp_need_update(info, is_visible);
	case SHOW_AMBIENT_TEMPERATURE:
		return is_ambient_need_update(info, is_visible);
	case SET_SCREEN_SAVER_ENABLE:
	case SET_SCREEN_SAVER_TYPE:
	case SET_SCREEN_SAVER_TIME:
	case SET_BRIGHTNESS:
		return false;
	case SHOW_POST_CODE:
	case SHOW_RTC_DAY:
	case SHOW_RTC_MONTH:
	case SHOW_RTC_YEAR:
	case SHOW_RTC_HOUR:
	case SHOW_RTC_SEC:
	case SHOW_RTC_MIN:
	case SHOW_USB_SERIAL_INPUT:
		return true;
	default:
		return false;
	}
}

static bool is_menu_need_update(struct gfx_action_menu *menu)
{
	bool need_to_update = false;
	for (uint8_t i = 0; i < menu->menu->num_elements; i++) {
		if (menu->actions[i].type == ACTION_TYPE_SHOW_FRAME) {
			if (menu->actions[i].frame == dashboard)
				continue;
			list_foreach(struct gfx_information_node *, menu->actions[i].frame->information_head, info_node) {
				need_to_update = is_information_need_to_change(&info_node->information, menu->actions[i].visible);
				if (need_to_update)
					return true;
			}
		}
	}
	return false;
}

static bool is_frame_need_update(struct gfx_frame *frame)
{
	list_foreach(struct gfx_information_node *, frame->information_head, info_node)
		if (is_information_need_to_change(&info_node->information, true))
			return true;

	return false;
}

void update_screen(void *data)
{
	switch (display_state) {
	case DISPLAY_MENU:
		if (is_menu_need_update(present_menu))
			gfx_action_menu_init(present_menu, true);
		break;
	case DISPLAY_CLOCK:
		frame_present->draw(frame_present, true);
		break;
	case DISPLAY_FRAME:
		if (is_frame_need_update(frame_present))
			frame_present->draw(frame_present, true);
		break;
	case DISPLAY_DASHBOARD:
		if (is_frame_need_update(dashboard))
			dashboard->draw(dashboard, true);
		break;
	default:
		break;
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
