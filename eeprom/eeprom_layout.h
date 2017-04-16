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

uint8_t eeprom_get_brightness_value(void);
uint8_t eeprom_get_brightness_level(void);
void eeprom_increse_brightness_level(void);
void eeprom_decrese_brightness_level(void);
void eeprom_set_brigntness_value(unsigned char new_value);
uint8_t eeprom_get_screen_saver_config(void);
void eeprom_set_screen_saver_config(uint8_t new_config);
uint8_t eeprom_get_screen_saver_time(void);
void eeprom_set_screen_saver_time(uint8_t new_config);
void eeprom_get_mac_address(uint8_t output_mac_addr[MAC_ADDRESS_LENGTH], uint8_t mac_address_index);
void eeprom_get_product_name(char product_name[PRODUCT_NAME_LENGTH]);
void eeprom_get_product_options(char product_options[PART_NUMBER_OPT_LENGTH], uint8_t index);

#endif /* EEPROM_LAYOUT_H_ */