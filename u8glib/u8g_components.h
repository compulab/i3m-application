/*
 * u8g_components.h
 *
 *  Created on: Oct 7, 2015
 *      Author: arkadi
 */

#ifndef U8GLIB_U8G_COMPONENTS_H_
#define U8GLIB_U8G_COMPONENTS_H_
#include <stdbool.h>
#include "csrc/u8g.h"
#include "../Fp-utils.h"

struct u8g_xbmp {
	uint8_t height;
	uint8_t width;
	u8g_pgm_uint8_t *bitmap;
};

struct item_action {
	enum action_type type;
	uint8_t menu_id;
	struct menu *menu;
	struct gfx_frame *frame;
};

struct menu_item_node {
	const u8g_pgm_uint8_t *text;
	struct item_action *action;
	bool visible;
	struct menu_item_node *prev;
	struct menu_item_node *next;
};

struct menu {
	bool visible;
	const u8g_pgm_uint8_t *title;
	struct menu_item_node *first_item;
	struct menu_item_node *last_item;
	struct menu_item_node * curr_selected_item;
};


#endif /* U8GLIB_U8G_COMPONENTS_H_ */
