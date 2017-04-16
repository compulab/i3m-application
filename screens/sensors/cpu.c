/*
 * cpu.c
 *
 * Created: 3/21/2017 1:58:34 PM
 *  Author: Nikita
 */

#include "screens/string_formats.h"
#include "gfx/gfx_components/gfx_information.h"
#include "lib/syntax.h"
#include <string.h>

static void sprintf_cpu_temp(struct gfx_information *info, char *output_str)
{
	uint8_t cpu_id = info->info_data;
	if (BIT_ON(computer_data.packed.cpu_temp_set, cpu_id))
		sprintf_temperature(output_str, computer_data.details.cpu_temp[cpu_id]);
	else
		sprintf_inval_data(output_str);
}

static bool cpu_temp_is_valid(struct gfx_information *info)
{
	return BIT_ON(computer_data.packed.cpu_temp_set, info->info_data);
}

int gfx_information_init_show_cpu_temp(struct gfx_information *info)
{
	info->to_string = sprintf_cpu_temp;
	info->is_valid = cpu_temp_is_valid;
	return 0;
}

static void sprintf_cpu_freq(struct gfx_information *info, char *output_str)
{
	uint8_t cpu_id = info->info_data;
	if (BIT_ON(computer_data.packed.cpu_freq_set, cpu_id))
		sprintf_freq(output_str, computer_data.packed.cpu_freq[cpu_id]);
	else
		sprintf_inval_data(output_str);
}

static bool cpu_freq_is_valid(struct gfx_information *info)
{
	return BIT_ON(computer_data.packed.cpu_freq_set, info->info_data);
}

int gfx_information_init_show_cpu_freq(struct gfx_information *info)
{
	info->to_string = sprintf_cpu_freq;
	info->is_valid = cpu_freq_is_valid;
	return 0;
}