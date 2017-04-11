/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_

#include "gfx/menu-handler.h"
#include "graphic_menu.h"

struct gfx_action_menu {
	struct gfx_mono_menu *menu;
	struct gfx_item_action *actions;
	struct gfx_image_node *graphic_items_head;
	uint8_t id;
	bool is_progmem;
};

#define MAIN_MENU_ID 	0

extern struct gfx_action_menu *present_menu;

void clear_screen(void);

void show_frame(struct gfx_frame *frame);

void gfx_action_menu_display(struct gfx_action_menu *action_menu);

void handle_button_pressed_by_display_mode(void);

void gfx_action_menu_process_key(struct gfx_action_menu *action_menu,
		uint8_t keycode, bool from_frame);

#endif /* GFX_GFX_ACTION_MENU_H_ */
