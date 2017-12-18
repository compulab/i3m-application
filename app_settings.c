/*
 * app_settings.c
 *
 * Created: 16-Nov-17 7:36:28 PM
 *  Author: Nikita
 */

#include "eeprom/eeprom_layout.h"
#include "eeprom/eeprom.h"
#include "layout.h"

#ifndef APPLICATION_VER_MSB
#define APPLICATION_VER_MSB 0
#endif

#ifndef APPLICATION_VER_LSB
#define APPLICATION_VER_LSB 1
#endif

static void setup_brightness_settings(void)
{
	if (eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS) == 0x00)
		eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, BRIGHTNESS_DEFAULT);
}

static void setup_app_version(void)
{
	if (eeprom_read_byte(APPLICATION_VER_MSB_EEPROM_ADDRESS) != APPLICATION_VER_MSB)
		eeprom_write_byte(APPLICATION_VER_MSB_EEPROM_ADDRESS, APPLICATION_VER_MSB);
	if (eeprom_read_byte(APPLICATION_VER_LSB_EEPROM_ADDRESS) != APPLICATION_VER_LSB)
		eeprom_write_byte(APPLICATION_VER_LSB_EEPROM_ADDRESS, APPLICATION_VER_LSB);
}

static void init_updateable_data(void)
{
	uint8_t *p_computer_data = (uint8_t *)&computer_data;
	for (uint8_t i = 0; i < (sizeof(computer_data) / 8); i++)
		p_computer_data[i] = 0;
}

void apply_app_settings(void)
{
	setup_brightness_settings();
	setup_app_version();
	init_updateable_data();
}