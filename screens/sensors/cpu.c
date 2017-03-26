/*
 * cpu.c
 *
 * Created: 3/21/2017 1:58:34 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"
#include "screens/string_formats.h"
#include "lib/syntax.h"

void sprintf_cpu_temp(char *data, uint8_t cpu_id)
{
	if (BIT_ON(computer_data.packed.cpu_temp_set, cpu_id))
		sprintf_temperature(data, computer_data.details.cpu_temp[cpu_id]);
	else
		sprintf_inval_data(data);
}

void sprintf_cpu_freq(char *output_str, uint8_t cpu_id)
{
	if (BIT_ON(computer_data.packed.cpu_freq_set, cpu_id))
		sprintf_freq(output_str, computer_data.packed.cpu_freq[cpu_id]);
	else
		sprintf_inval_data(output_str);
}
