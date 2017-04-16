/*
 * eeprom.h
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#ifndef EEPROM_H
#define EEPROM_H
#include <stdint.h>

#define SERIAL_NUMBER_EEPROM_ADDRESS 		0x14
#define LAYOUT_VERSION_EEPROM_ADDRESS 		0x2c
#define PART_NUMBER_EEPROM_ADDRESS			0x80

#define SERIAL_NUMBER_LENGTH				12
#define PART_NUMBER_LENGTH					64
#define PART_NUMBER_OPT_LENGTH				16

void eeprom_write_str(char *str, uint16_t addr);
void eeprom_write_byte(uint16_t regAddr, uint8_t value);
uint8_t eeprom_read_byte(uint16_t regAddr);

#endif /* EEPROM_H */
