/*
 * screen_off.c
 *
 * Created: 13-Jan-18 5:08:34 PM
 *  Author: Nikita
 */

#include "gfx/gfx_gui_control.h"
#include "gfx/gfx_components/gfx_information.h"
#include "settings.h"

static void draw_screen_off(struct gfx_information *info)
{
}

int gfx_information_init_screen_off(struct gfx_information *info)
{
	info->draw_data = draw_screen_off;
	return 0;
}