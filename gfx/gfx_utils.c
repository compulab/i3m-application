/*
 * gfx_utils.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#include "gfx_utils.h"
#include "gfx_components/gfx_image.h"

/*Sign symbols*/
PROGMEM_DECLARE(gfx_mono_color_t, right[5]) = {
	0x00, 0x7E, 0x3C, 0x18, 0x00,
};

PROGMEM_DECLARE(gfx_mono_color_t, left[5]) = {
	0x00, 0x18, 0x3C, 0x7E, 0x00,
};

PROGMEM_DECLARE(gfx_mono_color_t, plus[5]) = {
	0x18, 0x18, 0xFF, 0x18, 0x18,
};

PROGMEM_DECLARE(gfx_mono_color_t, minus[5]) = {
	0x18, 0x18, 0x18, 0x18, 0x18,
};

static struct gfx_mono_bitmap minus_bitmap = {
	.width = 5,
	.height = 8,
	.data.progmem = minus,
	.type = GFX_MONO_BITMAP_PROGMEM
};

static struct gfx_mono_bitmap plus_bitmap = {
	.width = 5,
	.height = 8,
	.data.progmem = plus,
	.type = GFX_MONO_BITMAP_PROGMEM
};

static struct gfx_mono_bitmap left_bitmap = {
	.width = 5,
	.height = 8,
	.data.progmem = left,
	.type = GFX_MONO_BITMAP_PROGMEM
};

static struct gfx_mono_bitmap right_bitmap = {
	.width = 5,
	.height = 8,
	.data.progmem = right,
	.type = GFX_MONO_BITMAP_PROGMEM
};

static struct gfx_image left_sign_image = {
	.postion = {
		.x = 3,
		.y = 57,
	},
	.bitmap = &left_bitmap,
};

static struct gfx_image right_sign_image = {
	.postion = {
		.x = 120,
		.y = 57,
	},
	.bitmap = &right_bitmap,
};

static struct gfx_image minus_sign_image = {
	.postion = {
		.x = 3,
		.y = 57,
	},
	.bitmap = &minus_bitmap,
};

static struct gfx_image plus_sign_image = {
	.postion = {
		.x = 120,
		.y = 57,
	},
	.bitmap = &plus_bitmap,
};

void draw_standard_separator_line(void)
{
	gfx_mono_generic_draw_horizontal_line(0, 50, GFX_MONO_LCD_WIDTH, GFX_PIXEL_SET);
}

static void draw_control_signs(uint8_t selection, uint8_t min_index, uint8_t max_index,
							   struct gfx_image *left_sign, struct gfx_image *right_sign)
{
	if (selection > min_index)
		gfx_mono_put_bitmap(left_sign->bitmap, left_sign->postion.x, left_sign->postion.y);

	if (selection < max_index)
		gfx_mono_put_bitmap(right_sign->bitmap, right_sign->postion.x, right_sign->postion.y);
}

void draw_control_signs_arrows(uint8_t selection, uint8_t min_index, uint8_t max_index)
{
	draw_control_signs(selection, min_index, max_index, &left_sign_image, &right_sign_image);
}

void draw_control_signs_numeric(uint8_t selection, uint8_t min_index, uint8_t max_index)
{
	draw_control_signs(selection, min_index, max_index, &minus_sign_image, &plus_sign_image);
}