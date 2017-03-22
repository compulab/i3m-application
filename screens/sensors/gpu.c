/*
 * gpu.c
 *
 * Created: 3/21/2017 2:01:28 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"

void sprintf_gpu_temp(char *output_str)
{
	if (computer_data.details.gpus)
		set_temp_string(output_str, computer_data.details.gput);
	else
		sprintf_inval_data(output_str);
}

bool is_gpu_temp_need_update(struct gfx_information *info, bool is_visible)
{
	return is_visible && !computer_data.details.gpus;
}
