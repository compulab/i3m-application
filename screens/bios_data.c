/*
 * bios_data.c
 *
 * Created: 3/27/2017 8:34:19 PM
 *  Author: Nikita
 */

#include "gfx/gfx_information.h"
#include "twi/i2c_buffer.h"

static void sprintf_post_code(struct gfx_information *info, char *output_str)
{
	sprintf(output_str, "%04X", i2c_buffer.layout.bios_post_code);
}

int gfx_information_init_show_post_code(struct gfx_information *info)
{
	info->to_string = sprintf_post_code;
	return 0;
}