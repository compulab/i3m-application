/*
 * gfx_image.h
 *
 * Created: 4/2/2017 6:10:24 PM
 *  Author: Nikita
 */


#ifndef GFX_IMAGE_H_
#define GFX_IMAGE_H_

#include <stdio.h>
#include <stdbool.h>
#include "gfx_item.h"
#include "config/cnf_blk_components.h"

struct gfx_image {
	struct gfx_item postion;
	struct gfx_mono_bitmap *bitmap;
	void (*draw)(struct gfx_image *image);
};

struct gfx_image_node {
	struct gfx_image image;
	struct gfx_image_node *next;
};

int gfx_image_init(struct gfx_image *image, gfx_mono_color_t PROGMEM_T *bitmap_progmem,
		uint8_t height, uint8_t width, uint8_t x, uint8_t y);

#endif /* GFX_IMAGE_H_ */