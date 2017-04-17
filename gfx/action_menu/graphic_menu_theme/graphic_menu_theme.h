/*
 * graphic_menu.h
 *
 *  Created on: Oct 18, 2015
 *      Author: arkadi
 */

#ifndef GFX_ACTION_MENU_GRAPHIC_MENU_H_
#define GFX_ACTION_MENU_GRAPHIC_MENU_H_

#include "gfx/action_menu/gfx_action_menu.h"
#include <stdio.h>

void graphic_menu_format(struct gfx_action_menu *action_menu);
void graphic_menu_deselect_item(struct gfx_action_menu *action_menu, uint8_t selection);
void graphic_menu_select_item(struct gfx_action_menu *action_menu, uint8_t selection);

#endif /* GFX_ACTION_MENU_GRAPHIC_MENU_H_ */
