/*
 * eeprom.c
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#include "eeprom_driver.h"
#include "eeprom.h"

#define EEPROM_BYTE_ADDRESS_MASK    0x1f
#define PAGE_ADDR(reg_addr)			(reg_addr / EEPROM_PAGESIZE)
#define BYTE_OFFSET(reg_addr)		(reg_addr & EEPROM_BYTE_ADDRESS_MASK)

void eeprom_write_byte(uint16_t reg_addr, uint8_t value)
{
	EEPROM_WriteByte(PAGE_ADDR(reg_addr), BYTE_OFFSET(reg_addr), value);
}

uint8_t eeprom_read_byte(uint16_t reg_addr)
{
	return EEPROM_ReadByte(PAGE_ADDR(reg_addr), BYTE_OFFSET(reg_addr));
}

void eeprom_write_str(char *str, uint16_t addr)
{
	for (uint8_t i = 0; str[i] != '\0'; i++)
		eeprom_write_byte(addr++, str[i]);
}
