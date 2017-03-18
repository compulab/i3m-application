/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_

#ifndef GFX_MENU_HANDLER_H_
#include "gfx/menu-handler.h"
#endif

#ifndef GFX_ACTION_MENU_GRAPHIC_MENU_H_
#include "graphic_menu.h"
#endif

#define MAIN_MENU_ID 	0

extern struct gfx_action_menu *present_menu;

void show_current_menu(bool redraw);

void clear_screen(void);

void show_frame(struct gfx_frame *frame);

void gfx_action_menu_init(struct gfx_action_menu *action_menu, bool redraw);

void handle_button_preesed_by_display_mode(void);

void gfx_action_menu_process_key(struct gfx_action_menu *action_menu,
		uint8_t keycode, bool from_frame);

#endif /* GFX_GFX_ACTION_MENU_H_ */
