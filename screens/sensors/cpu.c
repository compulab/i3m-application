/*
 * cpu.c
 *
 * Created: 3/21/2017 1:58:34 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"
#include "lib/syntax.h"

void sprintf_cpu_temp(char *data, uint8_t cpu_id)
{
	if (BIT_ON(computer_data.packed.cputs, cpu_id))
		set_temp_string(data, computer_data.details.cput[cpu_id]);
	else
		sprintf_inval_data(data);
}

void sprintf_cpu_freq(char *output_str, uint8_t cpu_id)
{
	if (BIT_ON(computer_data.packed.cpufs, cpu_id))
		set_fq_string(output_str, computer_data.packed.cpuf[cpu_id]);
	else
		sprintf_inval_data(output_str);
}
