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

