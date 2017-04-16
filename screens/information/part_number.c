/*
 * part_number.c
 *
 * Created: 3/29/2017 9:01:58 PM
 *  Author: Nikita
 */

#include "gfx/gfx_components/gfx_information.h"
#include "eeprom/eeprom_layout.h"
#include <stdio.h>
#include <string.h>

static void sprintf_part_number(struct gfx_information *info, char *output_str)
{
	char product_name[PRODUCT_NAME_LENGTH] = { 0 };
	char product_opts1[PART_NUMBER_OPT_LENGTH] = { 0 };
	char product_opts2[PART_NUMBER_OPT_LENGTH] = { 0 };
	char product_opts3[PART_NUMBER_OPT_LENGTH] = { 0 };
	eeprom_get_product_name(product_name);
	eeprom_get_product_options(product_opts1, 0);
	eeprom_get_product_options(product_opts2, 1);
	eeprom_get_product_options(product_opts3, 2);
	sprintf(output_str, "%s\n%s\n%s\n%s", product_name, product_opts1, product_opts2, product_opts3);
}

int gfx_information_init_show_part_number(struct gfx_information *info)
{
	info->to_string = sprintf_part_number;
	return 0;
}