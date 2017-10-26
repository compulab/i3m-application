/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_

#include "gfx/gfx_frame.h"

struct gfx_action_menu {
	struct gfx_mono_menu *menu;
	struct gfx_item_action *actions;
	struct gfx_image_node *graphic_items_head;
	uint8_t id;
	bool is_progmem;
	void (*draw)(struct gfx_action_menu *action_menu);
	void (*handle_button)(struct gfx_action_menu *action_menu, uint8_t keycode);
};

extern struct gfx_action_menu *current_menu;

void gfx_action_menu_move_cursor(struct gfx_action_menu *action_menu);
void gfx_action_menu_display(struct gfx_action_menu *action_menu);
void gfx_menu_handle_button(struct gfx_action_menu *action_menu, uint8_t keycode);

#endif /* GFX_GFX_ACTION_MENU_H_ */
