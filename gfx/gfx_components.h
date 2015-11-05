
/*
 * gfx_components.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_COMPONENTS_H_
#define GFX_GFX_COMPONENTS_H_


#ifndef FP_UTILS_H_
#include "../Fp-utils.h"
#endif

#ifndef DEF_H
#include "../def.h"
#endif

struct gfx_item {
	uint8_t x, y;
	uint8_t width, height;
	bool visible;
};

struct gfx_item_data {
	PROGMEM_STRING_T title;
	char *text;
};

struct gfx_item_action {
	enum action_type type;
	uint8_t menu_id;
	bool visible;
	struct gfx_action_menu *menu;
	struct gfx_frame *frame;
};

struct gfx_text {
	char *text;
	char *textP;
	bool is_progmem;
	struct gfx_font *font;
};

struct gfx_label {
	struct gfx_item postion;
	struct gfx_text text;
};

struct gfx_information {
	struct gfx_item postion;
	enum information_type info_type;
	uint8_t info_data;
	struct gfx_text text;
};

struct gfx_image {
	struct gfx_item postion;
	struct gfx_mono_bitmap *bitmap;
};

struct gfx_image_node {
	struct gfx_image image;
	struct gfx_image_node *next;
};

struct gfx_visible_items {
	struct gfx_item_action **visible_actions;
	struct gfx_mono_menu *visible_menu;
	struct gfx_image **visible_images;
};

struct gfx_action_menu {
	struct gfx_mono_menu *menu;
	struct gfx_item_action *actions;
	struct gfx_image_node *graphic_items_head;
	struct gfx_visible_items visible_items;
	uint8_t id;
	bool is_active_frame;
	bool is_progmem;
	bool is_graphic_view;
	bool visible;
};

struct gfx_label_node {
	struct gfx_label label;
	struct gfx_label_node *next;
};

struct gfx_information_node {
	struct gfx_information information;
	struct gfx_information_node *next;
};

struct gfx_frame {
	void (*handle_buttons)(uint8_t);
	struct gfx_image_node *image_head;
	struct gfx_label_node *label_head;
	struct gfx_information_node *information_head;
};

#endif /* GFX_GFX_COMPONENTS_H_ */
