/*
 * memory.c
 *
 * Created: 3/21/2017 2:03:01 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"
#include "screens/string_formats.h"
#include "lib/syntax.h"

void set_mem_size_str(char *str, uint8_t mem)
{
	uint8_t size;
	switch (mem){
	case 0x00:
	case 0x01:
	case 0x02:
		size = mem;
		break;
	case 0x03:
		size = 4;
		break;
	case 0x04:
		size = 8;
		break;
	case 0x05:
		size = 16;
		break;
	default:
		size = -1;
		break;
	}

	if (size < 0)
		sprintf(str, "-");
	else if (size == 0)
		sprintf(str, EMPTY_SLOT);
	else
		sprintf(str,"%d GB",size);
}

void sprintf_memory_size(char *output_str, uint8_t mem_id)
{
	if (BIT_ON(computer_data.packed.mems, mem_id))
		set_mem_size_str(output_str, computer_data.packed.memsz[mem_id]);
	else
		sprintf_inval_data(output_str);
}

bool is_mem_size_need_update(struct gfx_information *info, bool is_visible)
{
	if (is_visible)
		return !BIT_ON(computer_data.packed.mems, info->info_data);

	return true;
}