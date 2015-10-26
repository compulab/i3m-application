/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_

#ifndef GFX_MENU_HANDLER_H_
#include "../menu-handler.h"
#endif

#ifndef GFX_ACTION_MENU_GRAPHIC_MENU_H_
#include "graphic_menu.h"
#endif

struct gfx_action_menu *present_menu;

void show_current_menu(bool redraw);

void clear_screen();

void gfx_action_menu_init(struct gfx_action_menu *action_menu, bool redraw);

void gfx_action_menu_process_key(struct gfx_action_menu *action_menu, uint8_t keycode);

#endif /* GFX_GFX_ACTION_MENU_H_ */
