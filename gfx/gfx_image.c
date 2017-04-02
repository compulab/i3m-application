/*
 * gfx_image.c
 *
 * Created: 4/2/2017 6:10:10 PM
 *  Author: Nikita
 */

#include "gfx/gfx_image.h"
#include "asf.h"

static void gfx_image_draw(struct gfx_image *image)
{
	gfx_mono_generic_put_bitmap(image->bitmap, image->postion.x, image->postion.y);
}

int gfx_image_init(struct gfx_image *image, gfx_mono_color_t PROGMEM_T *bitmap_progmem,
		uint8_t height, uint8_t width, uint8_t x, uint8_t y)
{
	gfx_item_init(&image->postion, x, y, width + 2, height + 2);
	image->bitmap->width = width;
	image->bitmap->height = height;
	image->bitmap->data.progmem = bitmap_progmem;
	image->bitmap->type = GFX_MONO_BITMAP_SECTION;
	image->draw = gfx_image_draw;
}