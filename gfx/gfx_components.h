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

typedef struct GFX_ITEM {
	uint8_t x,y;
	uint8_t width,height;
	bool visible;
	bool borderVisible;
} gfx_item ;

typedef struct GFX_TEXT {
	char * text;
	struct font * font;
} gfx_text;

typedef struct GFX_LABEL {
	gfx_item postion;
	gfx_text text;
} gfx_label;

typedef struct GFX_INFORMATION_LABEL {
	gfx_item postion;
	information_type info_type;
	gfx_text text;
} gfx_information_label;

typedef struct GFX_BITMAP {
	gfx_item postion;
	struct gfx_mono_bitmap * bitmap;
} gfx_image;

typedef struct GFX_FRAME {
	gfx_image ** images;
	gfx_information_label ** informationLabels;
	gfx_label ** labels;
	uint8_t image_size, information_size, label_size;
} gfx_frame;

#endif /* GFX_GFX_COMPONENTS_H_ */
