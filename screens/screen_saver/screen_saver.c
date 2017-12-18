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

extern struct gfx_frame *dashboard;

static enum screen_saver_type {
	DASHBOARD, SCREEN_SAVER_MAX
} current_screen_saver = DASHBOARD;

static void draw_screen_saver(struct gfx_information *info)
{
	gfx_display_frame(dashboard);
}

int gfx_information_init_show_screen_saver(struct gfx_information *info)
{
	info->draw_data = draw_screen_saver;
	return 0;
}
