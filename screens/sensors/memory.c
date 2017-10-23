/*
 * memory.c
 *
 * Created: 3/21/2017 2:03:01 PM
 *  Author: Nikita
 */

#include "screens/string_formats.h"
#include "gfx/gfx_components/gfx_information.h"
#include "lib/syntax.h"
#include <string.h>

static void set_mem_size_str(char *str, uint8_t mem)
{
	switch (mem) {
	case 0x00:
		sprintf(str, "Empty");
		return;
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
		sprintf(str, "%d GB", 1 << (mem - 1));
		return;
	default:
		sprintf_inval_data(str);
		return;
	}
}

static void sprintf_memory_size(struct gfx_information *info, char *output_str)
{
	uint8_t mem_id = info->metadata;
	if (BIT_ON(computer_data.packed.mems, mem_id))
		set_mem_size_str(output_str, computer_data.packed.memsz[mem_id]);
	else
		sprintf_inval_data(output_str);
}

static bool memory_size_is_valid(struct gfx_information *info)
{
	return BIT_ON(computer_data.packed.mems, info->metadata);
}

int gfx_information_init_show_memory_size(struct gfx_information *info)
{
	info->to_string = sprintf_memory_size;
	info->is_valid = memory_size_is_valid;
	return 0;
}
