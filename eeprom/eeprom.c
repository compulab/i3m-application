/*
 * eeprom.c
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#include "eeprom_driver.h"
#include "eeprom.h"

#define EEPROM_BYTE_ADDRESS_MASK    0x1f
void eeprom_write_byte(uint16_t reg_addr, uint8_t value)
{
	uint8_t page_addr = reg_addr / EEPROM_PAGESIZE;
	uint8_t byte_addr = reg_addr & EEPROM_BYTE_ADDRESS_MASK;
	EEPROM_WriteByte(page_addr, byte_addr, value);
}

uint8_t eeprom_read_byte(uint16_t reg_addr)
{
	uint8_t page_addr = reg_addr / EEPROM_PAGESIZE;
	uint8_t byte_addr = reg_addr & EEPROM_BYTE_ADDRESS_MASK;
	return EEPROM_ReadByte(page_addr, byte_addr);
}

void eeprom_write_str(char *str, uint16_t addr)
{
	for (uint8_t i = 0; str[i] != '\0'; i++)
		eeprom_write_byte(addr++, str[i]);
}
