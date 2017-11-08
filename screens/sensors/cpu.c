/*
 * cpu.c
 *
 * Created: 3/21/2017 1:58:34 PM
 *  Author: Nikita
 */

#include "screens/string_formats.h"
#include "gfx/gfx_components/gfx_information.h"
#include "lib/syntax.h"
#include "sensors.h"
#include <string.h>

static void sprintf_cpu_temp(struct gfx_information *info, char *output_str)
{
	uint8_t cpu_id = info->metadata;
	if (BIT_ON(computer_data.packed.cpu_temp_set, cpu_id))
		sprintf_temperature(output_str, computer_data.details.cpu_temp[cpu_id]);
	else
		sprintf_inval_data(output_str);
}

int gfx_information_init_show_cpu_temp(struct gfx_information *info)
{
	info->to_string = sprintf_cpu_temp;
	return 0;
}

static void sprintf_cpu_freq(struct gfx_information *info, char *output_str)
{
	uint8_t cpu_id = info->metadata;
	if (BIT_ON(computer_data.packed.cpu_freq_set, cpu_id))
		sprintf_freq(output_str, computer_data.packed.cpu_freq[cpu_id]);
	else
		sprintf_inval_data(output_str);
}

int gfx_information_init_show_cpu_freq(struct gfx_information *info)
{
	info->to_string = sprintf_cpu_freq;
	return 0;
}