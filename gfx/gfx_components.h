
/*
 * gfx_components.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_COMPONENTS_H_
#define GFX_GFX_COMPONENTS_H_

#include "asf.h"
#include "def.h"

struct gfx_item {
	uint8_t x, y;
	uint8_t width, height;
	bool visible;
};

struct gfx_text {
	char *text;
	char *textP;
	uint8_t max_text_size;
	bool is_progmem;
	struct glcd_FontConfig_t *font;
};

struct gfx_label {
	struct gfx_item postion;
	struct gfx_text text;
};

struct gfx_label_node {
	struct gfx_label label;
	struct gfx_label_node *next;
};

struct gfx_image {
	struct gfx_item postion;
	struct gfx_mono_bitmap *bitmap;
};

struct gfx_image_node {
	struct gfx_image image;
	struct gfx_image_node *next;
};

#endif /* GFX_GFX_COMPONENTS_H_ */
