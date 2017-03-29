/*
 * static_data.c
 *
 * Created: 3/18/2017 11:42:46 PM
 *  Author: Nikita
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "eeprom/eeprom.h"
#include "twi/i2c_buffer.h"
#include "gfx/gfx_information.h"
#include "auto_generated.h"

#ifndef VERSION
#define VERSION "Version not set"
#endif

#ifndef BUILD_DATE
#define BUILD_DATE "Some day"
#endif

#ifndef BUILD_TIME
#define BUILD_TIME "hh:mm:ss"
#endif

void sprintf_mac_address(struct gfx_information *info, char *output_str)
{
	uint8_t mac_address_index = info->info_data;
	uint8_t eeprom_addr = MAC_ADDRESS_EEPROM_ADDRESS + MAC_ADDRESS_LENGTH * mac_address_index;
	uint8_t mac_addr[MAC_ADDRESS_LENGTH];
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		mac_addr[i] = eeprom_read_byte(eeprom_addr + i);

	sprintf(output_str, "[%d] %02X:%02X:%02X:%02X:%02X:%02X", (mac_address_index + 1),
			mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

int gfx_information_init_show_mac_address(struct gfx_information *info)
{
	info->to_string = sprintf_mac_address;
	return 0;
}

void sprintf_app_version(struct gfx_information *info, char *output_str)
{
	switch (info->info_data) {
	case 0:
		strcpy(output_str, BUILD_TIME);
		break;

	case 1:
		strcpy(output_str, BUILD_DATE);
		break;

	case 2:
		strcpy(output_str, "VER: " VERSION);
		break;

	default:
		strcpy(output_str, "-");
		break;
	}
}

int gfx_information_init_show_app_version(struct gfx_information *info)
{
	info->to_string = sprintf_app_version;
	return 0;
}

void sprintf_part_number(struct gfx_information *info, char *output_str)
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

void sprintf_serial_number(struct gfx_information *info, char *output_str)
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

	sprintf(output_str, serial);
}
<<<<<<< HEAD
=======

int gfx_information_init_show_serial_number(struct gfx_information *info)
{
	info->to_string = sprintf_serial_number;
	return 0;
}
>>>>>>> cd2fb6b... Refactor information and frame code
