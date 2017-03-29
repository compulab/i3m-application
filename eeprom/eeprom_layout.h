/*
 * eeprom_layout.h
 *
 * Created: 3/29/2017 5:23:59 PM
 *  Author: Nikita
 */


#ifndef EEPROM_LAYOUT_H_
#define EEPROM_LAYOUT_H_

#define MIN_BRIGHTNESS_LEVEL		0
#define MAX_BRIGHTNESS_LEVEL		10

uint8_t eeprom_get_brightness_value(void);
uint8_t eeprom_get_brightness_level(void);
void eeprom_increse_brightness_level(void);
void eeprom_decrese_brightness_level(void);
void eeprom_set_brigntness_value(unsigned char new_value);

#endif /* EEPROM_LAYOUT_H_ */