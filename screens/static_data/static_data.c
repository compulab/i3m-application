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

void sprintf_mac_address(char *output_str, uint8_t mac_address_index)
{
	uint8_t eeprom_addr = MAC_ADDRESS_EEPROM_ADDRESS + MAC_ADDRESS_LENGTH * mac_address_index;
	uint8_t mac_addr[MAC_ADDRESS_LENGTH];
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		mac_addr[i] = eeprom_read_byte(eeprom_addr + i);

	sprintf(output_str, "[%d] %02X:%02X:%02X:%02X:%02X:%02X", (mac_address_index + 1),
			mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

void sprintf_app_version(char *output_str, uint8_t type)
{
	switch (type) {
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

void sprintf_part_number(char *output_str)
{
	char part_number[PART_NUMBER_LENGTH + 1];
	uint8_t index = 0;
	uint8_t info;
	for (int i = 0; i < PART_NUMBER_OPT_LENGTH; i++) {
		info = eeprom_read_byte(PART_NUMBER_EEPROM_ADDRESS + i);
		if (info == '\0')
			break;
		part_number[index] = info;
		index++;
	}

	part_number[index] = '\n';
	index++;

	for (int j = 1; j < 4; j++) {
		for (int i = 0; i < PART_NUMBER_OPT_LENGTH; i++) {
			info = eeprom_read_byte(PART_NUMBER_EEPROM_ADDRESS + i + j * PART_NUMBER_OPT_LENGTH);

			if (info == '\0')
				break;
			part_number[index] = info;
			index++;
		}
	}

	part_number[index] = '\0';
	strcpy(output_str, part_number);
}

void sprintf_serial_number(char *output_str)
{
	char serial[SERIAL_NUMBER_LENGTH * 2  + 1];
	bool start_parsing = false;
	uint8_t index, serial_index = 0;;
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
	serial[serial_index + 1] = '\0';

	sprintf(output_str, serial);
}
