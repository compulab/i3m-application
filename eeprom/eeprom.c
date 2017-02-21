/*
 * eeprom.c
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#include "eeprom_driver.h"

#define EEPROM_BYTE_ADDRESS_MASK    0x1f
void eeprom_write_byte(uint16_t reg_addr, uint8_t value)
{
	EEPROM_WriteByte((unsigned char)(reg_addr / EEPROM_PAGESIZE) , (unsigned char)(reg_addr & EEPROM_BYTE_ADDRESS_MASK), value);
}

uint8_t eeprom_read_byte(uint16_t reg_addr)
{
	return EEPROM_ReadByte((unsigned char)(reg_addr / EEPROM_PAGESIZE), (unsigned char)(reg_addr & EEPROM_BYTE_ADDRESS_MASK));
}

void eeprom_write_str(char *str, uint16_t addr)
{
	uint8_t i = 0;
	while (str[i] != '\0') {
		eeprom_write_byte(addr, str[i]);
		addr++;
		i++;
	}
}
