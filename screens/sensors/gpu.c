/*
 * gpu.c
 *
 * Created: 3/21/2017 2:01:28 PM
 *  Author: Nikita
 */

#include <string.h>
#include "screens/string_formats.h"
#include "Fp-utils.h"

void sprintf_gpu_temp(char *output_str)
{
	if (computer_data.details.gpu_temp_set)
		sprintf_temperature(output_str, computer_data.details.gpu_temp);
	else
		sprintf_inval_data(output_str);
}

bool is_gpu_temp_need_update(struct gfx_information *info, bool is_visible)
{
	return is_visible && !computer_data.details.gpu_temp_set;
}
