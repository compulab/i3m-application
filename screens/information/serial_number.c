/*
 * serial_number.c
 *
 * Created: 3/18/2017 11:42:46 PM
 *  Author: Nikita
 */

#include "gfx/gfx_components/gfx_information.h"
#include "eeprom/eeprom_layout.h"
#include "information.h"
#include <stdio.h>

static void sprintf_serial_number(struct gfx_information *info, char *output_str)
{
	char serial[SERIAL_NUMBER_LENGTH * 2  + 1];
	bool start_parsing = false;
	uint8_t index, serial_index = 0;
	uint8_t serial_byte;

	for (index = SERIAL_NUMBER_LENGTH; index > 0 ; index--) {
		serial_byte = eeprom_read_byte(SERIAL_NUMBER_EEPROM_ADDRESS + index - 1);
		if (!start_parsing) {
			if (serial_byte)
				start_parsing = true;
			else
				continue;
		}
		sprintf(&serial[serial_index], "%02x", serial_byte);
		serial_index += 2;
	}

	for (index = serial_index; index > serial_index - 6; index--)
		serial[index] = serial[index - 1];

	serial[serial_index - 5] = '-';
	serial[serial_index] = '\0';

	sprintf(output_str, "S/N: %s", serial);
}

int gfx_information_init_show_serial_number(struct gfx_information *info)
{
	info->to_string = sprintf_serial_number;
	return 0;
}