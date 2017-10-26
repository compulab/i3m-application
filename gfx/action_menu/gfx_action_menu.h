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
};

extern struct gfx_action_menu *current_menu;

void clear_screen(void);
void gfx_gui_init(void);
void gfx_switch_to_frame(struct gfx_frame *frame);
void gfx_redraw_current_frame(void);
void gfx_menu_handle_button(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame);
void gfx_handle_button(uint8_t keycode);
void gfx_switch_to_current_menu(void);

#endif /* GFX_GFX_ACTION_MENU_H_ */
