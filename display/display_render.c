/*
 * display_render.c
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */

#include "display_render.h"


bool is_valid_char(char ch)
{
	return ch != '\0' && ch != '\n';
}

void draw_char(char ch, uint8_t x, uint8_t y, struct gfx_font *font)
{
	if (is_valid_char(ch)) {
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
}


void clear_string_background(uint8_t length, uint8_t x, uint8_t y, struct gfx_font *font)
{
	gfx_mono_draw_filled_rect(x, y, length * 8, font->height, GFX_PIXEL_CLR);
}

uint8_t length_P(char *str)
{
	uint8_t count = 0;
	uint8_t temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	while (is_valid_char(temp_char)) {
		count++;
		temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	}
	return count;
}

void draw_string_in_buffer_P(char *str, uint8_t x, uint8_t y, struct gfx_font *font)
{
	uint8_t length = length_P(str);
//	if (x == 0)
		x = (GFX_MONO_LCD_WIDTH - length) / 16;
	clear_string_background(length, x, y, font);
	uint8_t temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)(str++));

	while (temp_char){
		if (temp_char == '\n'){
			draw_string_in_buffer_P(str, x, y + 8, font);
			break;
		}
		if (x > 120)
		{
			break;
		}
		draw_char(temp_char, x, y, font);
		x += 8;
		temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	}
}


void draw_string_in_buffer(char *str, uint8_t x, uint8_t y, struct gfx_font *font)
{
	uint8_t j = 0;
	uint8_t length = strlen(str);
	if (x == 0)
		x = (GFX_MONO_LCD_WIDTH - length) / 4;
	clear_string_background(length, x, y, font);
	while (str[j] != '\0')
	{
		if (x > 120)
		{
			break;
		}
		draw_char(str[j], x, y, font);
		x += 8;
		j++;
	}
}
