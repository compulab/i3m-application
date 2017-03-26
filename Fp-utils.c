/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"
#include "uart/uart.h"
#include "scheduler/scheduler.h"
#include "screen_saver/screen_saver.h"
#include "twi/i2c_buffer.h"
#include "lib/syntax.h"
#include "screens/brightness/brightness.h"
#include "screens/static_data/static_data.h"

#include "power/power.h"
enum display_state display_state;
char power_value[10];

struct calendar_date computer_date_time = {
    .second = 40,
    .minute = 02,
    .hour = 11,
    .date = 26,
    .month = 11,
    .year = 2015
};

static void sprintf_post_code(char *str)
{
	sprintf(str, "%04X", i2c_buffer.layout.bios_post_code);
}

extern char buf[1];
static void set_usb_serial_string(char *str)
{
	strncpy(str, buf, 1);
}

void update_data_by_type(enum information_type type, char *output_str, uint8_t info)
{
	present_menu->is_active_frame = false;
	switch (type){
	case SHOW_MEMORY_SIZE:
		sprintf_memory_size(output_str, info);
		break;
	case SHOW_HDD_SIZE:
		sprintf_hdd_size(output_str, info);
		break;
	case SHOW_AMBIENT_TEMPERATURE:
		update_ambient_temp(NULL);
		sprintf_ambient_temp(output_str);
		break;
	case SHOW_GPU_TEMPERTURE:
		sprintf_gpu_temp(output_str);
		break;
	case SHOW_CPU_FREQUENCY:
		sprintf_cpu_freq(output_str, info);
		break;
	case SHOW_HDD_TEMPERTURE:
		sprintf_hdd_temp(output_str, info);
		break;
	case SHOW_POST_CODE:
		sprintf_post_code(output_str);
		break;
	case SHOW_COMPUTER_POWER:
		sprintf_power_data(output_str);
		break;
	case SHOW_SERIAL_NUMBER:
		sprintf_serial_number(output_str);
		break;
	case SHOW_APP_VERSION:
		sprintf_app_version(output_str, info);
		break;
	case SHOW_PART_NUMBER:
		sprintf_part_number(output_str);
		break;
	case SHOW_MAC_ADDRESS:
		sprintf_mac_address(output_str, info);
		break;
	case SHOW_POWER_STATE:
		sprintf_power_state(output_str);
		break;
	case SET_BRIGHTNESS:
		sprintf_brightness(output_str);
		break;
	case SHOW_CPU_TEMPERTURE:
		sprintf_cpu_temp(output_str, info);
		break;
	case SET_SCREEN_SAVER_ENABLE:
		sprintf_screen_saver_enable(output_str);
		break;
	case SET_SCREEN_SAVER_TIME:
		sprintf_screen_saver_time(output_str);
		break;
	case SET_SCREEN_SAVER_TYPE:
		sprintf_screen_saver_type(output_str);
		break;
	case SHOW_RTC_HOUR:
		sprintf_rtc_hour(output_str);
		break;
	case SHOW_RTC_MIN:
		sprintf_rtc_min(output_str);
		break;
	case SHOW_RTC_SEC:
		sprintf_rtc_sec(output_str);
		break;
	case SHOW_USB_SERIAL_INPUT:
		set_usb_serial_string(output_str);
	default:
		break;
	}

	if (type == SET_BRIGHTNESS)
		present_menu->is_active_frame = true;
}

static bool is_data_need_update_packed(uint8_t data, struct gfx_information *info, bool is_visible)
{
	if (!is_visible)
		return false;

	bool data_valid = BIT_ON(data, info->info_data);
	return present_menu->visible ? !data_valid : data_valid;
}

static bool is_information_need_to_change(struct gfx_information *info, bool is_visible)
{
	switch (info->info_type){
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
	struct gfx_information_node *info_node;
	for (uint8_t i = 0; i < menu->menu->num_elements; i++) {
		if (menu->actions[i].type == ACTION_TYPE_SHOW_FRAME) {
			if (menu->actions[i].frame == dashboard)
				continue;
			info_node = menu->actions[i].frame->information_head;
			while (info_node != 0){
				need_to_update = is_information_need_to_change(&info_node->information, menu->actions[i].visible);
				if (need_to_update)
					return true;

				info_node = info_node->next;
			}
		}
	}
	return false;
}

static bool is_info_need_update(struct gfx_information *info)
{
	return is_information_need_to_change(info, true);
}

static bool is_frame_need_update(struct gfx_frame *frame)
{
	struct gfx_information_node *info_node= frame->information_head;
	while (info_node != NULL) {
		if (is_info_need_update(&info_node->information))
			return true;
		info_node = info_node->next;
	}
	return false;
}

void update_computer_state(void)
{
	if (current_power_state == POWER_OFF)
		computer_state = COMPUTER_OFF;
	else if (current_power_state == POWER_ON && computer_state == COMPUTER_OFF)
		computer_state = COMPUTER_ON;
	else if (computer_data.packed.post_code == POST_CODE_BIOS_START)
		computer_state = COMPUTER_IN_BIOS;
	else if ((computer_state == COMPUTER_IN_BIOS) && (computer_data.packed.post_code == POST_CODE_BIOS_DONE))
		computer_state = COMPUTER_IN_OS;
}

void update_info(void *data)
{
	switch (display_state) {
	case DISPLAY_LOGO:
	case DISPLAY_ACTION_FRAME:
	case DISPLAY_DIM:
		return;

	case DISPLAY_MENU:
		if (is_menu_need_update(present_menu))
			gfx_action_menu_init(present_menu, true);
		break;

	case DISPLAY_CLOCK:
		gfx_frame_draw(frame_present, true);
		break;

	case DISPLAY_FRAME:
		if (is_frame_need_update(frame_present))
			gfx_frame_draw(frame_present, true);
	break;

	case DISPLAY_DASHBOARD:
		if (is_frame_need_update(dashboard))
			gfx_frame_draw(dashboard, true);
		break;
	default:
		break;
	}
}

static struct work update_screen_work = { .do_work = update_info };

#define UPDATE_SCREEN_TIME		1
static double screen_get_recur_period(void)
{
    return UPDATE_SCREEN_TIME;
}

struct scheduler_task screen_sec_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};
