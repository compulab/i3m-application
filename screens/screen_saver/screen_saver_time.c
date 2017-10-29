/*
 * screen_saver_time.c
 *
 * Created: 3/27/2017 5:18:59 PM
 *  Author: Nikita
 */

#include "screen_saver.h"
#include "gfx/gfx_components/gfx_information.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "gfx/gfx_assets.h"
#include "gfx/gfx_gui_control.h"
#include "eeprom/eeprom_layout.h"
#include <stdio.h>

static void handle_screen_saver_time_buttons(uint8_t key)
{
	if (!computer_data.details.screen_saver_visible &&
		(key == GFX_MONO_MENU_KEYCODE_DOWN || key == GFX_MONO_MENU_KEYCODE_UP)) {
		return;
	}

	switch (key) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		gfx_switch_to_current_menu();
		return;
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (computer_data.details.screen_saver_update_time <= SCREEN_SAVER_SECOND_MIN_VALUE)
			return;
		computer_data.details.screen_saver_update_time -= SCREEN_SAVER_SECOND_JUMP;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (computer_data.details.screen_saver_update_time >= SCREEN_SAVER_SECOND_MAX_VALUE)
			return;
		computer_data.details.screen_saver_update_time += SCREEN_SAVER_SECOND_JUMP;
		break;
	default:
		return;
	}

	eeprom_set_screen_saver_time(computer_data.packed.screen_saver_update_time);
	gfx_redraw_current_frame();
}

static void sprintf_screen_saver_time(struct gfx_information *info, char *output_str)
{
	if (computer_data.details.screen_saver_visible)
		sprintf(output_str, "%d", computer_data.details.screen_saver_update_time);
	else
		sprintf_disabled(output_str);
}

static void set_screen_saver_time_draw_graphic_signs(struct gfx_information *info)
{
	if (computer_data.details.screen_saver_visible)
		draw_control_signs_numeric(computer_data.details.screen_saver_update_time, SCREEN_SAVER_SECOND_MIN_VALUE, SCREEN_SAVER_SECOND_MAX_VALUE);
}

static bool screen_saver_time_is_valid(struct gfx_information *info)
{
	return computer_data.details.screen_saver_visible;
}

int gfx_information_init_set_screen_saver_time(struct gfx_information *info)
{
	info->to_string = sprintf_screen_saver_time;
	info->draw_controls = set_screen_saver_time_draw_graphic_signs;
	info->handle_buttons = handle_screen_saver_time_buttons;
	info->is_valid = screen_saver_time_is_valid;
	return 0;
}
