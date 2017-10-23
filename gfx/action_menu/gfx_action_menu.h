/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_

#include "gfx/action_menu/menu-handler.h"
#include "gfx/gfx_frame.h"

struct gfx_action_menu {
	struct gfx_mono_menu *menu;
	struct gfx_item_action *actions;
	struct gfx_image_node *graphic_items_head;
	uint8_t id;
	bool is_progmem;
	void (*draw)(struct gfx_action_menu *action_menu);
};

extern struct gfx_action_menu *present_menu;

void clear_screen(void);
void gfx_display_msg(char *msg);
void gfx_action_menu_init(void);
void switch_to_frame(struct gfx_frame *frame);
void gfx_redraw_current_frame(void);
void gfx_handle_key_pressed(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame);

#endif /* GFX_GFX_ACTION_MENU_H_ */
