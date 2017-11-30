/*
 * eeprom_layout.h
 *
 * Created: 3/29/2017 5:23:59 PM
 *  Author: Nikita
 */


#ifndef EEPROM_LAYOUT_H_
#define EEPROM_LAYOUT_H_

#include <stdio.h>

#define LAYOUT_VERSION_EEPROM_ADDRESS 		0x2c
#define SERIAL_NUMBER_EEPROM_ADDRESS 		0x14

#define SERIAL_NUMBER_LENGTH				12
#define PRODUCT_NAME_LENGTH					16
#define PART_NUMBER_OPT_LENGTH				16
#define MAC_ADDRESS_LENGTH 					6

#define MIN_BRIGHTNESS_LEVEL		0
#define MAX_BRIGHTNESS_LEVEL		10

#define MAC_ADDRESS_EEPROM_ADDRESS			0x04

#define PRODUCT_NAME_EEPROM_ADDRESS			0x80
#define PRODUCT_OPTIONS_EEPROM_ADDRESS		0x90

#define SCREEN_SAVER_TIME_EEPROM_ADDRESS	0x120
#define SCREEN_SAVER_CONFIG_EEPROM_ADDRESS	0x121
#define BRIGHTNESS_EEPROM_ADDRESS 			0x122

void eeprom_get_mac_address(uint8_t output_mac_addr[MAC_ADDRESS_LENGTH], uint8_t mac_address_index);
void eeprom_get_product_name(char product_name[PRODUCT_NAME_LENGTH]);
void eeprom_get_product_options(char product_options[PART_NUMBER_OPT_LENGTH], uint8_t index);

#endif /* EEPROM_LAYOUT_H_ */