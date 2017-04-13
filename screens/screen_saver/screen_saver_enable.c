/*
 * screen_saver_enable.c
 *
 * Created: 3/27/2017 5:15:00 PM
 *  Author: Nikita
 */

#include <stdbool.h>
#include "gfx/gfx_utils.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "eeprom/eeprom_layout.h"

bool screen_saver_mode_enabled;

void enable_screen_saver_mode(void)
{
	screen_saver_mode_enabled = true;
}

void disable_screen_saver_mode(void)
{
	screen_saver_mode_enabled = false;
}

static void handle_screen_saver_enable_buttons(uint8_t key)
{
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		handle_back_to_menu();
		return;
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (!computer_data.details.screen_saver_visible)
			return;
		computer_data.details.screen_saver_visible = 0;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (computer_data.details.screen_saver_visible)
			return;
		computer_data.details.screen_saver_visible = 1;
		break;
	}

	eeprom_set_screen_saver_config(computer_data.packed.screen_saver_config);
	frame_present->draw(frame_present, true);
}

static void sprintf_screen_saver_enable(struct gfx_information *info, char *output_str)
{
	sprintf(output_str, "DISABLE ENABLE");
}

static void draw_screen_saver_enable(struct gfx_information *info)
{
	info->to_string(info, info->text.text);
	info->last_length = draw_string_in_buffer(info->text.text, info->postion.x, info->postion.y, info->text.font, info->last_length);

	uint8_t enable_len = 7 * info->text.font->width;
	uint8_t	disable_len = 8 * info->text.font->width;
	uint8_t	enable_x = disable_len + info->text.font->width + info->postion.x;
	uint8_t	disable_x = info->postion.x;
	uint8_t set_x = computer_data.details.screen_saver_visible ? enable_x : disable_x;
	uint8_t	clear_x = computer_data.details.screen_saver_visible ? disable_x : enable_x;
	uint8_t	length_set = computer_data.details.screen_saver_visible ? enable_len : disable_len;
	uint8_t	length_clr = computer_data.details.screen_saver_visible ? disable_len : enable_len;

	gfx_mono_generic_draw_horizontal_line(set_x, info->postion.y + info->text.font->height + 2, length_set, GFX_PIXEL_SET);
	gfx_mono_generic_draw_horizontal_line(clear_x, info->postion.y + info->text.font->height + 2, length_clr, GFX_PIXEL_CLR);
}

static void set_screen_saver_enable_draw_graphic_signs(struct gfx_information *info)
{
	draw_control_signs_arrows(computer_data.details.screen_saver_visible, 0, 1);
}

int gfx_information_init_set_screen_saver_enable(struct gfx_information *info)
{
	info->to_string = sprintf_screen_saver_enable;
	info->draw_data = draw_screen_saver_enable;
	info->draw_controls = set_screen_saver_enable_draw_graphic_signs;
	info->handle_buttons = handle_screen_saver_enable_buttons;
	return 0;
}
