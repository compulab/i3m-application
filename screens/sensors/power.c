/*
 * power.c
 *
 * Created: 15-Apr-17 8:23:20 PM
 *  Author: Nikita
 */

#include "gfx/gfx_components/gfx_information.h"
#include "power/power.h"
#include "sensors.h"
#include <string.h>

static void sprintf_power_state(struct gfx_information *info, char *output_str)
{
	switch (current_power_state) {
	case POWER_ON:
		strcpy(output_str, "Airtop on");
		break;
	case POWER_STR:
		strcpy(output_str, "Sleep");
		break;
	case POWER_STD:
		strcpy(output_str, "Hibernate");
		break;
	case POWER_OFF:
		strcpy(output_str, "Airtop off");
		break;
	default:
		strcpy(output_str, "");
		break;
	}
}

int gfx_information_init_show_power_state(struct gfx_information *info)
{
	info->to_string = sprintf_power_state;
	return 0;
}

static void sprintf_power_data(struct gfx_information *info, char *output_str)
{
	long power = computer_data.details.adc * 0.10137 + 2.9;

	if (!computer_data.details.adc_set)
		sprintf(output_str, "-");
	else if (power >= 6 && power <= 300)
		sprintf(output_str, "%ldW", power);
	else
		sprintf(output_str, "<5W");
}

int gfx_information_init_show_power_data(struct gfx_information *info)
{
	info->to_string = sprintf_power_data;
	return 0;
}