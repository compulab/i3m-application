/*
 * hdd.c
 *
 * Created: 3/21/2017 1:59:51 PM
 *  Author: Nikita
 */

#include "screens/string_formats.h"
#include "gfx/gfx_components/gfx_information.h"
#include "lib/syntax.h"
#include <string.h>

static void sprintf_hdd_temp(struct gfx_information *info, char *output_str)
{
	uint8_t hdd_id = info->info_data;
	if (BIT_ON(computer_data.packed.hdd_temp_set, hdd_id))
		sprintf_temperature(output_str, computer_data.packed.hdd_temp[hdd_id]);
	else
		sprintf_inval_data(output_str);
}

static bool hdd_temp_is_valid(struct gfx_information *info)
{
	return BIT_ON(computer_data.packed.hdd_temp_set, info->info_data);
}

int gfx_information_init_show_hdd_temp(struct gfx_information *info)
{
	info->to_string = sprintf_hdd_temp;
	info->is_valid = hdd_temp_is_valid;
	return 0;
}

static void sprintf_hdd_size(struct gfx_information *info, char *output_str)
{
	uint8_t hdd_id = info->info_data;
	uint16_t size = computer_data.packed.hdd_size[hdd_id];
	bool is_tera = BIT_ON(computer_data.packed.hdd_units_tera, hdd_id);
	char *units = is_tera ? "TB" : "GB";

	if (BIT_ON(computer_data.packed.hdd_size_set, hdd_id))
		sprintf(output_str, "%d %s", size, units);
	else
		sprintf_inval_data(output_str);
}

static bool hdd_size_is_valid(struct gfx_information *info)
{
	return BIT_ON(computer_data.packed.hdd_size_set, info->info_data);
}

int gfx_information_init_show_hdd_size(struct gfx_information *info)
{
	info->to_string = sprintf_hdd_size;
	info->is_valid = hdd_size_is_valid;
	return 0;
}
