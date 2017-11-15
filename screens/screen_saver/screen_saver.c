/*
 * screen_saver.c
 *
 * Created: 15-Nov-17 5:44:50 PM
 *  Author: Nikita
 */

#include "display/display_render.h"
#include "gfx/gfx_assets.h"
#include "gfx/gfx_gui_control.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "gfx/gfx_components/gfx_information.h"
#include "screen_saver.h"
#include <stdbool.h>

extern struct gfx_frame *splash;
extern struct gfx_frame *dashboard;

#define SCREEN_SAVER_CONTROLS_DISPLAY_TIME	5

static enum screen_saver_type {
	LOGO, DASHBOARD, SCREEN_SAVER_MAX
} current_screen_saver = LOGO;

static int display_controls_countdown = SCREEN_SAVER_CONTROLS_DISPLAY_TIME;

static void handle_screen_saver_buttons(struct gfx_information *info, uint8_t key)
{
	display_controls_countdown = SCREEN_SAVER_CONTROLS_DISPLAY_TIME;
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		gfx_switch_to_current_menu();
		return;
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (current_screen_saver > 0)
			current_screen_saver--;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (current_screen_saver < SCREEN_SAVER_MAX - 1)
			current_screen_saver++;
		break;
	}

	gfx_redraw_current_frame();
}

static void screen_saver_draw_graphic_signs(struct gfx_information *info)
{
	if (display_controls_countdown > 0)
		draw_control_signs_arrows(current_screen_saver, 0, SCREEN_SAVER_MAX - 1);
}

static void draw_screen_saver(struct gfx_information *info)
{
	if (current_screen_saver == LOGO)
		gfx_display_frame(splash);
	else
		gfx_display_frame(dashboard);

	display_controls_countdown--;
	info->draw_controls(info);
}

int gfx_information_init_show_screen_saver(struct gfx_information *info)
{
	info->draw_data = draw_screen_saver;
	info->draw_controls = screen_saver_draw_graphic_signs;
	info->handle_buttons = handle_screen_saver_buttons;
	return 0;
}
