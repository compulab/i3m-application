/*
 * screen_saver_type.c
 *
 * Created: 3/27/2017 5:21:56 PM
 *  Author: Nikita
 */

#include <stdio.h>
#include "lib/syntax.h"
#include "gfx/gfx_information.h"
#include "eeprom/eeprom_layout.h"
#include "screen_saver.h"

const char *screen_saver_type_str[SCREEN_SAVER_TYPE_SIZE] = { "LOGO", "DASHBOARD", "CLOCK" };

static void handle_screen_saver_type_buttons(uint8_t key)
{
	if (!computer_data.details.screen_saver_visible)
		return;

	switch (key) {
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
	frame_present->draw(frame_present, true);
}

static void sprintf_screen_saver_type(struct gfx_information *info, char *output_str)
{
	frame_present->handle_buttons = handle_screen_saver_type_buttons;
	if (computer_data.details.screen_saver_visible)
		sprintf(output_str, screen_saver_type_str[computer_data.details.screen_saver_type]);
	else
		sprintf_disabled(output_str);

	present_menu->is_active_frame = true;
}

int gfx_information_init_set_screen_saver_type(struct gfx_information *info)
{
	info->to_string = sprintf_screen_saver_type;
	return 0;
}

void set_screen_saver_type_draw_graphic_signs(void)
{
	if (computer_data.details.screen_saver_visible)
		draw_graphic_signs(computer_data.details.screen_saver_type, 0, SCREEN_SAVER_TYPE_SIZE - 1, false);
}