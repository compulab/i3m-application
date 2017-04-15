/*
 * ambient.c
 *
 * Created: 3/21/2017 2:06:36 PM
 *  Author: Nikita
 */

#include <string.h>
#include "screens/string_formats.h"
#include "gfx/gfx_components/gfx_information.h"

static void sprintf_ambient_temp(struct gfx_information *info, char *output_str)
{
	if (computer_data.details.ambient_temp_set)
		sprintf_temperature(output_str, computer_data.details.ambient_temp);
	else
		sprintf_inval_data(output_str);
}

static bool ambient_temp_is_valid(struct gfx_information *info)
{
	return computer_data.details.ambient_temp_set;
}

int gfx_information_init_show_ambient_temp(struct gfx_information *info)
{
	info->to_string = sprintf_ambient_temp;
	info->is_valid = ambient_temp_is_valid;
	return 0;
}
