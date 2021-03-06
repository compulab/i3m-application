/*
 * eeprom.c
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#include "ASF/xmega/drivers/nvm/nvm.h"
#include "eeprom.h"

void eeprom_write_byte(uint16_t reg_addr, uint8_t value)
{
	nvm_eeprom_write_byte(reg_addr, value);
}

uint8_t eeprom_read_byte(uint16_t reg_addr)
{
	return nvm_eeprom_read_byte(reg_addr);
}

void eeprom_write_str(char *str, uint16_t addr)
{
	for (uint8_t i = 0; str[i] != '\0'; i++)
		eeprom_write_byte(addr++, str[i]);
}

void eeprom_read_str(char *output_str, uint8_t addr, uint8_t str_length)
{
	for (int i = 0; i < str_length; i++)
		output_str[i] = eeprom_read_byte(addr + i);

	output_str[str_length - 1] = '\0';
}

void eeprom_read_byte_array(uint16_t reg_addr, uint8_t *output_array, uint8_t array_length)
{
	for (int i = 0; i < array_length; i++)
		output_array[i] = eeprom_read_byte(reg_addr + i);
}
