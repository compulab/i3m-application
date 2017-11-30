/*
 * part_number.c
 *
 * Created: 3/29/2017 9:01:58 PM
 *  Author: Nikita
 */

#include "gfx/gfx_components/gfx_information.h"
#include "eeprom/eeprom.h"
#include "eeprom/eeprom_layout.h"
#include "information.h"
#include <stdio.h>
#include <string.h>

static void sprintf_part_number(struct gfx_information *info, char *output_str)
{
	char product_name[PRODUCT_NAME_LENGTH] = { 0 };
	char product_opts1[PRODUCT_OPTIONS_LENGTH] = { 0 };
	char product_opts2[PRODUCT_OPTIONS_LENGTH] = { 0 };
	char product_opts3[PRODUCT_OPTIONS_LENGTH] = { 0 };
	eeprom_read_str(product_name, PRODUCT_NAME_EEPROM_ADDRESS, ASCII_FIELD_LENGTH);
	eeprom_read_str(product_opts1, PRODUCT_OPTIONS_EEPROM_ADDRESS + 0 * PRODUCT_OPTIONS_LENGTH, ASCII_FIELD_LENGTH);
	eeprom_read_str(product_opts2, PRODUCT_OPTIONS_EEPROM_ADDRESS + 1 * PRODUCT_OPTIONS_LENGTH, ASCII_FIELD_LENGTH);
	eeprom_read_str(product_opts3, PRODUCT_OPTIONS_EEPROM_ADDRESS + 2 * PRODUCT_OPTIONS_LENGTH, ASCII_FIELD_LENGTH);
	sprintf(output_str, "%s\n%s\n%s\n%s", product_name, product_opts1, product_opts2, product_opts3);
}

int gfx_information_init_show_part_number(struct gfx_information *info)
{
	info->to_string = sprintf_part_number;
	return 0;
}