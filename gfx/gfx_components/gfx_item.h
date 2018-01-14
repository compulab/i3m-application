/*
 * gfx_item.h
 *
 * Created: 4/2/2017 4:02:56 PM
 *  Author: Nikita
 */


#ifndef GFX_ITEM_H_
#define GFX_ITEM_H_

#include <stdio.h>
#include <stdbool.h>

struct gfx_item {
	uint8_t x, y;
	uint8_t width, height;
};

void gfx_item_init(struct gfx_item *item, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

#endif /* GFX_ITEM_H_ */