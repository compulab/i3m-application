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
	bool need_update = is_visible && !computer_data.details.gpus;
	if (!need_update){
		char temp_str[3];
		char curr_str[3];
		set_curr_str(curr_str, SHOW_GPU_TEMPERTURE);
		sprintf_gpu_temp(temp_str);
		need_update = strcmp(temp_str, curr_str);
	}
	return need_update;
}
