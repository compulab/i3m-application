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

struct gfx_item {
	uint8_t x, y;
	uint8_t width, height;
	bool visible;
	bool border_visible;
};

struct gfx_text {
	char *text;
	bool is_progmem;
	struct font *font;
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

struct gfx_frame {
	struct gfx_image **images;
	struct gfx_information  **informations;
	struct gfx_label  **labels;
	uint8_t image_size, information_size, label_size;
};

#endif /* GFX_GFX_COMPONENTS_H_ */
