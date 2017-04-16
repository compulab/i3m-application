/*
 * clock.c
 *
 * Created: 15-Apr-17 8:17:41 PM
 *  Author: Nikita
 */

#include "screens/string_formats.h"
#include "gfx/gfx_components/gfx_information.h"
#include "calendar/calendar.h"

void sprintf_rtc_hour(struct gfx_information *info, char *output_str)
{
	if (calendar_is_date_valid(&computer_date_time))
		sprintf(output_str, "%d" ,computer_date_time.hour);
	else
		sprintf_inval_data(output_str);
}

int gfx_information_init_show_rtc_hour(struct gfx_information *info)
{
	info->to_string = sprintf_rtc_hour;
	return 0;
}

void sprintf_rtc_min(struct gfx_information *info, char *output_str)
{
	if (calendar_is_date_valid(&computer_date_time))
		sprintf(output_str, "%02d" ,computer_date_time.minute);
	else
		sprintf_inval_data(output_str);
}

int gfx_information_init_show_rtc_min(struct gfx_information *info)
{
	info->to_string = sprintf_rtc_min;
	return 0;
}

void sprintf_rtc_sec(struct gfx_information *info, char *output_str)
{
	if (calendar_is_date_valid(&computer_date_time) && computer_date_time.second % 2)
		sprintf(output_str, ":");
	else
		sprintf(output_str, " ");
}

int gfx_information_init_show_rtc_sec(struct gfx_information *info)
{
	info->to_string = sprintf_rtc_sec;
	return 0;
}