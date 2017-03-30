/*
 * gfx_information.c
 *
 * Created: 3/27/2017 3:40:46 PM
 *  Author: Nikita
 */

#include "gfx_information.h"
#include "action_menu/gfx_action_menu.h"

void gfx_information_draw_string(struct gfx_information *info)
{
	//TODO: kill this is_active_frame nonesense once we stop making this distinction.
	present_menu->is_active_frame = false;
	info->to_string(info, info->text.text);
	//TODO: this is fucked up. This is data saved for an algorithm. It should not be here.
	info->last_length = draw_string_in_buffer(info->text.text, info->postion.x, info->postion.y, info->text.font, info->last_length);
}

static void sprintf_sprintf(struct gfx_information *info, char *output_str) {
	sprintf(output_str, info->text.text);
}

static int gfx_information_init_generic(struct gfx_information *info, enum information_type info_type,
										uint8_t info_data, uint8_t max_length, uint8_t x, uint8_t y, uint8_t font_id)
{
	info->info_type = info_type;
	info->info_data = info_data;
	info->last_length = 0;
	info->text.is_progmem = false;
	info->text.max_text_size = max_length;
	info->text.text = malloc_locked(info->text.max_text_size);
	if (info->text.text == NULL)
		return -1;

	gfx_item_init(&info->postion, x, y, 0, 0);
	info->text.font = get_font_by_type(font_id);
	info->to_string = sprintf_sprintf;
	info->draw = gfx_information_draw_string;

	return 0;
}

static int gfx_information_init_type(struct gfx_information *info, enum information_type info_type)
{
	switch(info_type) {
	case SET_BRIGHTNESS:
		return gfx_information_init_set_brightness(info);
	case SET_SCREEN_SAVER_ENABLE:
		return gfx_information_init_set_screen_saver_enable(info);
	case SET_SCREEN_SAVER_TIME:
		return gfx_information_init_set_screen_saver_time(info);
	case SET_SCREEN_SAVER_TYPE:
		return gfx_information_init_set_screen_saver_type(info);
	case SHOW_SERIAL_NUMBER:
		return gfx_information_init_show_serial_number(info);
	case SHOW_APP_VERSION:
		return gfx_information_init_show_app_version(info);
	case SHOW_PART_NUMBER:
		return gfx_information_init_show_part_number(info);
	case SHOW_MAC_ADDRESS:
		return gfx_information_init_show_mac_address(info);
	case SHOW_POWER_STATE:
		return gfx_information_init_show_power_state(info);
	case SHOW_COMPUTER_POWER:
		return gfx_information_init_show_power_data(info);
	case SHOW_CPU_FREQUENCY:
		return gfx_information_init_show_cpu_freq(info);
	case SHOW_HDD_SIZE:
		return gfx_information_init_show_hdd_size(info);
	case SHOW_MEMORY_SIZE:
		return gfx_information_init_show_memory_size(info);
	case SHOW_HDD_TEMPERTURE:
		return gfx_information_init_show_hdd_temp(info);
	case SHOW_CPU_TEMPERTURE:
		return gfx_information_init_show_cpu_temp(info);
	case SHOW_GPU_TEMPERTURE:
		return gfx_information_init_show_gpu_temp(info);
	case SHOW_AMBIENT_TEMPERATURE:
		return gfx_information_init_show_ambient_temp(info);
	case SHOW_POST_CODE:
		return gfx_information_init_show_post_code(info);
	case SHOW_RTC_HOUR:
		return gfx_information_init_show_rtc_hour(info);
	case SHOW_RTC_MIN:
		return gfx_information_init_show_rtc_min(info);
	case SHOW_RTC_SEC:
		return gfx_information_init_show_rtc_sec(info);
	case SHOW_USB_SERIAL_INPUT://TODO:THIS IS TEMPORARY, KILL THIS WHEN NO LONGER NEEDED
		//info->to_string = set_usb_serial_string;
		return 0;
	default:
		return -1;
	}
};

//TODO: for some reason font_id is part of cnf_info_node, instead of cnf_info.
//Fix this later.
int gfx_information_init(struct gfx_information *info, struct cnf_info *cnf_info, uint8_t font_id)
{
	int retval = gfx_information_init_generic(info,
											  cnf_info->info_type,
											  cnf_info->information,
											  cnf_info->max_length,
											  cnf_info->x,
											  cnf_info->y,
											  font_id);
	if (retval)
		return -1;

	return gfx_information_init_type(info, cnf_info->info_type);
}
