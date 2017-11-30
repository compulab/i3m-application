/*
 * eeprom_layout.c
 *
 * Created: 3/29/2017 5:20:27 PM
 *  Author: Nikita
 */

#include "eeprom.h"
#include "eeprom_layout.h"
#include <stdio.h>

#define ASCII_FIELD_LENGTH			16

static void eeprom_get_ascii_field(char *output_ascii_field, uint8_t addr)
{
	for (int i = 0; i < ASCII_FIELD_LENGTH; i++)
		output_ascii_field[i] = eeprom_read_byte(addr + i);

	output_ascii_field[ASCII_FIELD_LENGTH - 1] = '\0';
	return;
}

uint8_t eeprom_get_screen_saver_config(void)
{
	return eeprom_read_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS);
}

void eeprom_set_screen_saver_config(uint8_t new_config)
{
	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, new_config);
}

uint8_t eeprom_get_screen_saver_time(void)
{
	return eeprom_read_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS);
}

void eeprom_set_screen_saver_time(uint8_t new_time)
{
	eeprom_write_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS, new_time);
}

void eeprom_get_mac_address(uint8_t output_mac_addr[MAC_ADDRESS_LENGTH], uint8_t mac_address_index)
{
	uint8_t eeprom_addr = MAC_ADDRESS_EEPROM_ADDRESS + MAC_ADDRESS_LENGTH * mac_address_index;
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		output_mac_addr[i] = eeprom_read_byte(eeprom_addr + i);
}

void eeprom_get_product_name(char product_name[PRODUCT_NAME_LENGTH])
{
	eeprom_get_ascii_field(product_name, PRODUCT_NAME_EEPROM_ADDRESS);
}

void eeprom_get_product_options(char product_options[PART_NUMBER_OPT_LENGTH], uint8_t index)
{
	eeprom_get_ascii_field(product_options, PRODUCT_OPTIONS_EEPROM_ADDRESS + index * PART_NUMBER_OPT_LENGTH);
}