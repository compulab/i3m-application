/*
 * memory.c
 *
 * Created: 3/21/2017 2:03:01 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"
#include "lib/syntax.h"

void sprintf_memory_size(char *output_str, uint8_t mem_id)
{
	if (BIT_ON(computer_data.packed.mems, mem_id))
		set_mem_size_str(output_str, computer_data.packed.memsz[mem_id]);
	else
		sprintf_inval_data(output_str);
}

