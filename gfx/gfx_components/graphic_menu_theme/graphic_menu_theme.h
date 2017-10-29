/*
 * graphic_menu.h
 *
 *  Created on: Oct 18, 2015
 *      Author: arkadi
 */

#ifndef GFX_ACTION_MENU_GRAPHIC_MENU_H_
#define GFX_ACTION_MENU_GRAPHIC_MENU_H_

#include "gfx/gfx_components/gfx_graphic_menu.h"
#include <stdio.h>

void graphic_menu_format(struct gfx_graphic_menu *graphic_menu);
void graphic_menu_deselect_item(struct gfx_graphic_menu *graphic_menu, uint8_t selection);
void graphic_menu_select_item(struct gfx_graphic_menu *graphic_menu, uint8_t selection);

#endif /* GFX_ACTION_MENU_GRAPHIC_MENU_H_ */
