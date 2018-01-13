/*
 * show_logo.c
 *
 * Created: 13-Jan-18 4:57:52 PM
 *  Author: Nikita
 */

#include "gfx/gfx_gui_control.h"
#include "gfx/gfx_components/gfx_information.h"
#include "settings.h"

static void draw_logo(struct gfx_information *info)
{
	gfx_show_splash_screen(0);
}

int gfx_information_init_show_logo(struct gfx_information *info)
{
	info->draw_data = draw_logo;
	return 0;
}