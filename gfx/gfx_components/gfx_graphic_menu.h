/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_

#include "gfx_frame.h"

struct gfx_graphic_menu {
	struct gfx_mono_menu *menu;
	struct gfx_graphic_menu_action {
		enum action_type type;
		uint8_t menu_id;
		bool visible;
		struct gfx_graphic_menu *menu;
		struct gfx_frame *frame;
	} *actions;
	struct gfx_image_node *graphic_items_head;
	uint8_t id;

	void (*draw)(struct gfx_graphic_menu *graphic_menu);
	void (*handle_button)(struct gfx_graphic_menu *graphic_menu, uint8_t keycode);
};

extern struct gfx_graphic_menu *current_menu;

void gfx_graphic_menu_move_cursor(struct gfx_graphic_menu *graphic_menu);
void gfx_graphic_menu_init(struct gfx_graphic_menu *graphic_menu, uint8_t id,
						  struct gfx_mono_menu *menu, struct gfx_graphic_menu_action *actions,
						  struct gfx_image_node *graphic_items_head);

#endif /* GFX_GFX_ACTION_MENU_H_ */
