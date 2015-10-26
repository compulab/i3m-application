/*
 * eeprom.h
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#ifndef EEPROM_H
#define EEPROM_H
#include <stdint.h>

#include "../config/conf_twi.h"
#include "eeprom_driver.h"


uint8_t eeprom_read_byte(uint8_t regAddr);

void eeprom_write_byte(uint8_t regAddr, uint8_t value);

#endif /* EEPROM_H */
