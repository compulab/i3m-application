/*
 * usb_input.c
 *
 * Created: 3/27/2017 8:33:27 PM
 *  Author: Nikita
 */

#include <string.h>
#include "gfx/gfx_components/gfx_information.h"

extern char buf[1];
void set_usb_serial_string(struct gfx_information *info, char *output_str)
{
	strncpy(output_str, buf, 1);
}