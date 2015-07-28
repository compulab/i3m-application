/*
 * twi-memory.c
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */
#include "twi-memory.h"

void twi_EEPROM_WriteByte(uint8_t regAddr, uint8_t value)
{
	EEPROM_WriteByte((unsigned char)(regAddr / EEPROM_BYTES_IN_PAGE) , (unsigned char)(regAddr & EEPROM_BYTE_ADDRESS_MASK), value );
}

void twi_Flash_WriteWord(uint8_t regAddr, uint8_t high, uint8_t low){
	SP_WaitForSPM();
	SP_LoadFlashWord(regAddr,0x0000);
}


void twi_Flash_WritePage(uint8_t regAddr, uint8_t* bufferPage)
{
	for (uint8_t i=0x00; i < FLASH_PAGE_SIZE; i=i+2){
		uint8_t lowByte = bufferPage[i];
		uint8_t highByte = bufferPage[i+1];
		uint32_t address = (uint32_t) (i + regAddr);
		SP_LoadFlashWord(address, ((uint16_t) highByte << 8) | lowByte);
		SP_WaitForSPM();
	}
}

uint8_t twi_ReadByte(uint8_t regAddr)
{
	return EEPROM_ReadByte((unsigned char)(regAddr / EEPROM_BYTES_IN_PAGE), (unsigned char)(regAddr & EEPROM_BYTE_ADDRESS_MASK));
}
