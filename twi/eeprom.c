/*
 * eeprom.c
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#include "eeprom.h"

void twi_EEPROM_WriteByte(uint8_t regAddr, uint8_t value)
{
	EEPROM_WriteByte((unsigned char)(regAddr / EEPROM_BYTES_IN_PAGE) , (unsigned char)(regAddr & EEPROM_BYTE_ADDRESS_MASK), value );
}

uint8_t twi_EEPROM_ReadByte(uint8_t regAddr)
{
	return EEPROM_ReadByte((unsigned char)(regAddr / EEPROM_BYTES_IN_PAGE), (unsigned char)(regAddr & EEPROM_BYTE_ADDRESS_MASK));
}
