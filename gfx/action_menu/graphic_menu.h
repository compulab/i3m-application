/*
 * graphic_menu.h
 *
 *  Created on: Oct 18, 2015
 *      Author: arkadi
 */

#ifndef GFX_ACTION_MENU_GRAPHIC_MENU_H_
#define GFX_ACTION_MENU_GRAPHIC_MENU_H_

#include <stdbool.h>
#include <math.h>
#include "gfx/action_menu/gfx_action_menu.h"
#include "gfx/gfx_components/gfx_text.h"

#define FRAME_WIDTH	38
#define FRAME_HEIGHT 22

void graphic_menu_init(struct gfx_action_menu *action_menu, bool redraw);

#endif /* GFX_ACTION_MENU_GRAPHIC_MENU_H_ */
