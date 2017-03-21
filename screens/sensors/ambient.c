/*
 * ambient.c
 *
 * Created: 3/21/2017 2:06:36 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"

void sprintf_ambient_temp(char *output_str)
{
	if (computer_data.details.ambs)
		set_temp_string(output_str, computer_data.details.ambt);
	else
		sprintf_inval_data(output_str);
}

bool is_ambient_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = is_visible && !computer_data.details.ambs;
	if (!need_update){
		char temp_str[3];
		char curr_str[3];
		sprintf_ambient_temp(temp_str);
		set_curr_str(curr_str, SHOW_AMBIENT_TEMPERATURE);
		need_update = strcmp(temp_str, curr_str) != 0;
	}
	return need_update;
}
