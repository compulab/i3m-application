/*
 * screen_saver_enable.c
 *
 * Created: 3/27/2017 5:15:00 PM
 *  Author: Nikita
 */

#include <stdbool.h>
#include "gfx/gfx_utils.h"

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

	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, computer_data.packed.screen_saver_config);
	frame_present->draw(frame_present, true);
}

void sprintf_screen_saver_enable(struct gfx_information *info, char *output_str)
{
	frame_present->handle_buttons = handle_screen_saver_enable_buttons;
	sprintf(output_str, "DISABLE ENABLE");
	present_menu->is_active_frame = true;
}

static void draw_screen_saver_enable(struct gfx_information *info)
{
	gfx_information_draw_string(info);
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

int gfx_information_init_set_screen_saver_enable(struct gfx_information *info)
{
	info->to_string = sprintf_screen_saver_enable;
	info->draw = draw_screen_saver_enable;
	return 0;
}

void set_screen_saver_enable_draw_graphic_signs(void)
{
	draw_graphic_signs(computer_data.details.screen_saver_visible, 0, 1, false);
}
