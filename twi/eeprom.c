/*
 * eeprom.c
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#include "eeprom.h"

void eeprom_write_byte(uint8_t reg_addr, uint8_t value)
{
	EEPROM_WriteByte((unsigned char)(reg_addr / EEPROM_BYTES_IN_PAGE) , (unsigned char)(reg_addr & EEPROM_BYTE_ADDRESS_MASK), value);
}

uint8_t eeprom_read_byte(uint8_t reg_addr)
{
	return EEPROM_ReadByte((unsigned char)(reg_addr / EEPROM_BYTES_IN_PAGE), (unsigned char)(reg_addr & EEPROM_BYTE_ADDRESS_MASK));
}
