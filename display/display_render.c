/*
 * display_render.c
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */

#include "display_render.h"


void draw_char(char ch, uint8_t x, uint8_t y, struct gfx_font *font)
{
	uint8_t inc_y,
			glyph_byte,
			c = ch - ' ';
	for (uint8_t l = 0; l < font->height; l += 8){
		for (uint8_t i = 0; i < font->width; i++)
		{
			inc_y = y + 8 + l;
			glyph_byte = PROGMEM_READ_BYTE(font->source + (c * font->height + i + l));

			for (uint8_t z = 0; z < 8 ; z++){
				if ((glyph_byte & 0x80)) {
					gfx_mono_draw_pixel(x + i, inc_y,
							GFX_PIXEL_SET);
				}
				inc_y--;
				glyph_byte <<= 1;
			}
		}
	}
}


void clear_string_background(uint8_t length, uint8_t x, uint8_t y, struct gfx_font *font)
{
	gfx_mono_draw_filled_rect(x, y, length * 8, font->height, GFX_PIXEL_CLR);
}

void draw_string_in_buffer_P(char *str, uint8_t x, uint8_t y, struct gfx_font *font)
{
	clear_string_background(strlen_P(str), x, y, font);
	uint8_t temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)(++str));
	while (temp_char){
		if (x > 120)
		{
			x = 0;
			y++;
		}
		draw_char(temp_char, x, y, font);
		x += 8;
		temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)(++str));
	}
}


void draw_string_in_buffer(char *ch, uint8_t x, uint8_t y, struct gfx_font *font)
{
	uint8_t j = 0;
	clear_string_background(strlen(ch), x, y, font);
	while (ch[j] != '\0')
	{
		if (x > 120)
		{
			x = 0;
			y++;
		}
		draw_char(ch[j], x, y, font);
		x += 8;
		j++;
	}
}
