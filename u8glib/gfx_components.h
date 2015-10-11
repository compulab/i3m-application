/*
 * gfx_components.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_COMPONENTS_H_
#define GFX_GFX_COMPONENTS_H_
#include <stdbool.h>
#include <stdint.h>
#include "../Fp-utils.h"
#include "u8g.h"

struct gfx_item {
	uint8_t x, y;
	uint8_t width, height;
	bool visible;
	bool border_visible;
};

//struct gfx_text {
//	char *text;
//	bool is_progmem;
//	struct font *font;
//};

struct gfx_label {
	struct gfx_item postion;
	u8g_pgm_uint8_t *text;
};

struct gfx_information {
	struct gfx_item postion;
	enum information_type info_type;
	uint8_t info_data;
	char *text;
};

struct gfx_image {
	struct gfx_item postion;
	struct gfx_mono_bitmap *bitmap;
};

struct gfx_image_node {
	struct gfx_image image;
	struct gfx_image_node *next;
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
	struct gfx_image_node *image_head;
	struct gfx_label_node *label_head;
	struct gfx_information_node *information_head;
};

#endif /* GFX_GFX_COMPONENTS_H_ */
