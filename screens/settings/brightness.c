/*
 * brightness.c
 *
 * Created: 3/18/2017 9:28:35 PM
 *  Author: Nikita
 */

#include "gfx/gfx_assets.h"
#include "gfx/gfx_gui_control.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "gfx/gfx_components/gfx_information.h"
#include "eeprom/eeprom_layout.h"
#include "settings.h"

#define BRIGHTNESS_STEP				25
#define MIN_BRIGHTNESS_LEVEL		0
#define MAX_BRIGHTNESS_LEVEL		10

static uint8_t eeprom_get_brightness_value(void)
{
	return eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS);
}

static uint8_t eeprom_get_brightness_level(void)
{
	uint8_t brightness = eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS);

	if (brightness > (MAX_BRIGHTNESS_LEVEL -1) * BRIGHTNESS_STEP)
		return MAX_BRIGHTNESS_LEVEL;
	else if (brightness < BRIGHTNESS_STEP)
		return 0;

	return brightness / BRIGHTNESS_STEP;
}

static void eeprom_increse_brightness_level(void)
{
	uint8_t brightness_level = eeprom_get_brightness_level();
	if (brightness_level < MAX_BRIGHTNESS_LEVEL)
		brightness_level++;
	else
		return;

	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

static void eeprom_decrese_brightness_level(void)
{
	uint8_t brightness_level = eeprom_get_brightness_level();
	if (brightness_level > 0)
		brightness_level--;
	else
		return;

	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

static void handle_brightness_buttons(struct gfx_information *info, uint8_t key)
{
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		gfx_switch_to_current_menu();
		return;
	case GFX_MONO_MENU_KEYCODE_DOWN:
		eeprom_decrese_brightness_level();
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		eeprom_increse_brightness_level();
		break;
	}

	//Yes it is kind of fucked up that we update brightness by changing contrast.
	//TODO: figure out why this is implemented this way and hopefully fix it.
	ssd1306_set_contrast(eeprom_get_brightness_value());
	gfx_redraw_current_frame();
}

static void sprintf_brightness(struct gfx_information *info, char *output_str)
{
	sprintf(output_str, "%2d ", eeprom_get_brightness_level());
}

static void set_brightness_draw_graphic_signs(struct gfx_information *info)
{
	draw_control_signs_numeric(eeprom_get_brightness_level(), MIN_BRIGHTNESS_LEVEL, MAX_BRIGHTNESS_LEVEL);
}

int gfx_information_init_set_brightness(struct gfx_information *info)
{
	info->to_string = sprintf_brightness;
	info->draw_controls = set_brightness_draw_graphic_signs;
	info->handle_buttons = handle_brightness_buttons;
	return 0;
}