/*
 * gfx_item.c
 *
 * Created: 4/2/2017 4:03:29 PM
 *  Author: Nikita
 */

#include <stdio.h>
#include <stdbool.h>
#include "gfx_item.h"

void gfx_item_init(struct gfx_item *item, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	item->x = x;
	item->y = y;
	item->width = width;
	item->height = height;
	item->visible = true;
}