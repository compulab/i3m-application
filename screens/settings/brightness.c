/*
 * brightness.c
 *
 * Created: 3/18/2017 9:28:35 PM
 *  Author: Nikita
 */

#include "gfx/gfx_utils.h"
#include "gfx/gfx_gui_control.h"
#include "gfx/action_menu/gfx_graphic_menu.h"
#include "gfx/gfx_components/gfx_information.h"
#include "eeprom/eeprom_layout.h"

static void handle_brightness_buttons(uint8_t key)
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
	sprintf(output_str, "%d ", eeprom_get_brightness_level());
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