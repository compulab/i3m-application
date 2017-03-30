/*
 * screen_saver_time.c
 *
 * Created: 3/27/2017 5:18:59 PM
 *  Author: Nikita
 */

#include <stdio.h>
#include "gfx/gfx_information.h"
#include "gfx/gfx_utils.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "eeprom/eeprom_layout.h"
#include "Fp-utils.h"
#include "screen_saver.h"

static void handle_screen_saver_time_buttons(uint8_t key)
{
	if (!computer_data.details.screen_saver_visible)
		return;

	switch (key) {
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
	frame_present->draw(frame_present, true);
}

void sprintf_screen_saver_time(struct gfx_information *info, char *output_str)
{
	frame_present->handle_buttons = handle_screen_saver_time_buttons;
	if (computer_data.details.screen_saver_visible)
		sprintf(output_str, "%d", computer_data.details.screen_saver_update_time);
	else
		sprintf_disabled(output_str);

	present_menu->is_active_frame = true;
}

int gfx_information_init_set_screen_saver_time(struct gfx_information *info)
{
	info->to_string = sprintf_screen_saver_time;
	return 0;
}

void set_screen_saver_time_draw_graphic_signs(void)
{
	if (computer_data.details.screen_saver_visible)
		draw_control_signs_numeric(computer_data.details.screen_saver_update_time, SCREEN_SAVER_SECOND_MIN_VALUE, SCREEN_SAVER_SECOND_MAX_VALUE);
}
