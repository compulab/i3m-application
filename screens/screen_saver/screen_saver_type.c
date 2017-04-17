/*
 * screen_saver_type.c
 *
 * Created: 3/27/2017 5:21:56 PM
 *  Author: Nikita
 */

#include "screen_saver.h"
#include "screen_saver/screen_saver.h"
#include "gfx/gfx_components/gfx_information.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "gfx/gfx_utils.h"
#include "eeprom/eeprom_layout.h"
#include "lib/syntax.h"
#include <stdio.h>

static const char *screen_saver_type_str[SCREEN_SAVER_TYPE_SIZE] = { "LOGO", "DASHBOARD", "CLOCK" };

static void handle_screen_saver_type_buttons(uint8_t key)
{
	if (!computer_data.details.screen_saver_visible &&
		(key == GFX_MONO_MENU_KEYCODE_DOWN || key == GFX_MONO_MENU_KEYCODE_UP)) {
		return;
	}

	switch (key) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		handle_back_to_menu();
		return;
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (computer_data.details.screen_saver_type == 0)
			return;
		computer_data.details.screen_saver_type--;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (computer_data.details.screen_saver_type == array_length(screen_saver_type_str) - 1)
			return;
		computer_data.details.screen_saver_type++;
		break;
	default:
		return;
	}

	eeprom_set_screen_saver_config(computer_data.packed.screen_saver_config);
	show_current_frame();
}

static void sprintf_screen_saver_type(struct gfx_information *info, char *output_str)
{
	if (computer_data.details.screen_saver_visible)
		sprintf(output_str, screen_saver_type_str[computer_data.details.screen_saver_type]);
	else
		sprintf_disabled(output_str);
}

static void set_screen_saver_type_draw_graphic_signs(struct gfx_information *info)
{
	if (computer_data.details.screen_saver_visible)
		draw_control_signs_arrows(computer_data.details.screen_saver_type, 0, SCREEN_SAVER_TYPE_SIZE - 1);
}

static bool screen_saver_type_is_valid(struct gfx_information *info)
{
	return computer_data.details.screen_saver_visible;
}

int gfx_information_init_set_screen_saver_type(struct gfx_information *info)
{
	info->to_string = sprintf_screen_saver_type;
	info->draw_controls = set_screen_saver_type_draw_graphic_signs;
	info->handle_buttons = handle_screen_saver_type_buttons;
	info->is_valid = screen_saver_type_is_valid;
	return 0;
}