/*
 * brightness.c
 *
 * Created: 3/18/2017 9:28:35 PM
 *  Author: Nikita
 */

#include "layout.h"
#include "Fp-utils.h"

#define BRIGHTNESS_STEP				25
#define MIN_BRIGHTNESS_LEVEL		0
#define MAX_BRIGHTNESS_LEVEL		10

void update_brightness(void)
{
	ssd1306_set_contrast(eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS));
}

uint8_t get_brightness_level(void)
{
	uint8_t brightness = eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS);

	if (brightness > (MAX_BRIGHTNESS_LEVEL -1) * BRIGHTNESS_STEP)
		return MAX_BRIGHTNESS_LEVEL;
	else if (brightness < BRIGHTNESS_STEP)
		return 0;

	return brightness / BRIGHTNESS_STEP;
}

static void increse_brightness_level(void)
{
	uint8_t brightness_level = get_brightness_level();
	if (brightness_level < MAX_BRIGHTNESS_LEVEL)
		brightness_level++;
	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

static void decrese_brightness_level(void)
{
	uint8_t brightness_level = get_brightness_level();
	if (brightness_level > 0)
		brightness_level--;
	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

static void handle_brightness_buttons(uint8_t key)
{
	uint8_t brightness_level = get_brightness_level();
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (brightness_level > 0)
			decrese_brightness_level();
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (brightness_level < MAX_BRIGHTNESS_LEVEL)
			increse_brightness_level();
		break;
	}
	update_brightness();
	gfx_frame_draw(frame_present, true);
}

void sprintf_brightness(char *str)
{
	frame_present->handle_buttons = handle_brightness_buttons;
	sprintf(str, "%d ", eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS) / BRIGHTNESS_STEP);
}

void set_brightness_draw_graphic_signs(void)
{
	draw_graphic_signs(get_brightness_level(), MIN_BRIGHTNESS_LEVEL, MAX_BRIGHTNESS_LEVEL, true);
}