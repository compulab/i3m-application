/*
 * eeprom_layout.c
 *
 * Created: 3/29/2017 5:20:27 PM
 *  Author: Nikita
 */

#include "eeprom.h"
#include "eeprom_layout.h"
#include <stdio.h>

#define MAC_ADDRESS_EEPROM_ADDRESS			0x04
#define SCREEN_SAVER_TIME_EEPROM_ADDRESS	0x120
#define SCREEN_SAVER_CONFIG_EEPROM_ADDRESS	0x121
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

void eeprom_set_brigntness_value(uint8_t new_value)
{
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, new_value);
}

uint8_t eeprom_get_screen_saver_config(void)
{
	return eeprom_read_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS);
}

void eeprom_set_screen_saver_config(uint8_t new_config)
{
	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, new_config);
}

uint8_t eeprom_get_screen_saver_time(void)
{
	return eeprom_read_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS);
}

void eeprom_set_screen_saver_time(uint8_t new_time)
{
	eeprom_write_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS, new_time);
}

void eeprom_get_mac_address(uint8_t output_mac_addr[MAC_ADDRESS_LENGTH], uint8_t mac_address_index)
{
	uint8_t eeprom_addr = MAC_ADDRESS_EEPROM_ADDRESS + MAC_ADDRESS_LENGTH * mac_address_index;
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		output_mac_addr[i] = eeprom_read_byte(eeprom_addr + i);
}
