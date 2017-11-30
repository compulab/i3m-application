/*
 * eeprom.h
 *
 *  Created on: Jul 26, 2015
 *      Author: arkadi
 */

#ifndef EEPROM_H
#define EEPROM_H
#include <stdint.h>

void eeprom_write_str(char *str, uint16_t addr);
void eeprom_read_str(char *output_str, uint8_t addr, uint8_t str_length);
void eeprom_write_byte(uint16_t regAddr, uint8_t value);
uint8_t eeprom_read_byte(uint16_t regAddr);

#endif /* EEPROM_H */
