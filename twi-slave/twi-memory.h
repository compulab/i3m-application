/*
 * twi-memory.h
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#ifndef TWI_MEMORY_H_
#define TWI_MEMORY_H_
#include <stdint.h>
#include "defines.h"
#include "eeprom_driver.h"
#include "sp_driver.h"

uint8_t twi_EEPROM_ReadByte(uint8_t regAddr);
void twi_EEPROM_WriteByte(uint8_t regAddr, uint8_t value);
//void twi_Flash_WriteWord(uint8_t regAddr, uint8_t high, uint8_t low);
//void twi_Flash_WritePage(uint8_t regAddr, uint8_t* bufferPage);


#endif /* TWI_MEMORY_H_ */
