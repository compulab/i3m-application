/*
 * gfx_item_action.h
 *
 * Created: 3/27/2017 6:24:56 PM
 *  Author: Nikita
 */


#ifndef GFX_ITEM_ACTION_H_
#define GFX_ITEM_ACTION_H_

#include "gfx/gfx_frame.h"

struct gfx_graphic_menu;

struct gfx_item_action {
	enum action_type type;
	uint8_t menu_id;
	bool visible;
	struct gfx_graphic_menu *menu;
	struct gfx_frame *frame;
};

#endif /* GFX_ITEM_ACTION_H_ */