/*
 * eeprom_layout.c
 *
 * Created: 3/29/2017 5:20:27 PM
 *  Author: Nikita
 */

#include <stdio.h>
#include "eeprom_layout.h"

#define BRIGHTNESS_EEPROM_ADDRESS 			0x122

#define BRIGHTNESS_STEP				25

uint8_t eeprom_get_brightness_value(void)
{
	return eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS);
}

uint8_t eeprom_get_brightness_level(void)
{
	uint8_t brightness = eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS);

	if (brightness > (MAX_BRIGHTNESS_LEVEL -1) * BRIGHTNESS_STEP)
		return MAX_BRIGHTNESS_LEVEL;
	else if (brightness < BRIGHTNESS_STEP)
		return 0;

	return brightness / BRIGHTNESS_STEP;
}

void eeprom_increse_brightness_level(void)
{
	uint8_t brightness_level = eeprom_get_brightness_level();
	if (brightness_level < MAX_BRIGHTNESS_LEVEL)
		brightness_level++;
	else
		return;

	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

void eeprom_decrese_brightness_level(void)
{
	uint8_t brightness_level = eeprom_get_brightness_level();
	if (brightness_level > 0)
		brightness_level--;
	else
		return;

	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

void eeprom_set_brigntness_value(unsigned char new_value)
{
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, new_value);
}