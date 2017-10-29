/*
 * menu-handler.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_MENU_HANDLER_H_
#define GFX_MENU_HANDLER_H_

#include "gfx/gfx_assets.h"
#include "display/display_render.h"
#include "display/glcd-0.5.2/glcd_font.h"

extern struct gfx_mono_bitmap splash_bitmap;

struct gfx_graphic_menu **graphic_menus;

int load_config_block(void);
void set_menu_by_id(struct gfx_graphic_menu **menu, uint8_t index);

#endif /* GFX_MENU_HANDLER_H_ */
