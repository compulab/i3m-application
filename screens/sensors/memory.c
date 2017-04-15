/*
 * memory.c
 *
 * Created: 3/21/2017 2:03:01 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"
#include "screens/string_formats.h"
#include "gfx/gfx_components/gfx_information.h"
#include "lib/syntax.h"

static void set_mem_size_str(char *str, uint8_t mem)
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
		sprintf_inval_data(str);
	else if (size == 0)
		sprintf(str, "Empty");
	else
		sprintf(str, "%d GB", size);
}

static void sprintf_memory_size(struct gfx_information *info, char *output_str)
{
	uint8_t mem_id = info->info_data;
	if (BIT_ON(computer_data.packed.mems, mem_id))
		set_mem_size_str(output_str, computer_data.packed.memsz[mem_id]);
	else
		sprintf_inval_data(output_str);
}

static bool memory_size_is_valid(struct gfx_information *info)
{
	return BIT_ON(computer_data.packed.mems, info->info_data);
}

int gfx_information_init_show_memory_size(struct gfx_information *info)
{
	info->to_string = sprintf_memory_size;
	info->is_valid = memory_size_is_valid;
	return 0;
}
