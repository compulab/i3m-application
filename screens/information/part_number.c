/*
 * part_number.c
 *
 * Created: 3/29/2017 9:01:58 PM
 *  Author: Nikita
 */

#include <stdio.h>
#include <string.h>
#include "gfx/gfx_components/gfx_information.h"

static void sprintf_part_number(struct gfx_information *info, char *output_str)
{
	char part_number[PART_NUMBER_LENGTH + 1];
	uint8_t index = 0;
	uint8_t data;
	for (int i = 0; i < PART_NUMBER_OPT_LENGTH; i++) {
		data = eeprom_read_byte(PART_NUMBER_EEPROM_ADDRESS + i);
		if (data == '\0')
			break;
		part_number[index] = data;
		index++;
	}

	part_number[index] = '\n';
	index++;

	for (int j = 1; j < 4; j++) {
		for (int i = 0; i < PART_NUMBER_OPT_LENGTH; i++) {
			data = eeprom_read_byte(PART_NUMBER_EEPROM_ADDRESS + i + j * PART_NUMBER_OPT_LENGTH);

			if (data == '\0')
				break;
			part_number[index] = data;
			index++;
		}
	}

	part_number[index - 1] = '\0';
	strcpy(output_str, part_number);
}

int gfx_information_init_show_part_number(struct gfx_information *info)
{
	info->to_string = sprintf_part_number;
	return 0;
}