/*
 * eeprom.h
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#ifndef EEPROM_H
#define EEPROM_H
#include <stdint.h>

#include "../config/conf-twi.h"
#include "../twi/eeprom_driver.h"


uint8_t twi_EEPROM_ReadByte(uint8_t regAddr);
void twi_EEPROM_WriteByte(uint8_t regAddr, uint8_t value);

#endif /* EEPROM_H */
