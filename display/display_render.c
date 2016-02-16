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

void draw_char(char ch, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font)
{
//	if (is_valid_char(ch)) {
//		uint8_t inc_y,
//				glyph_byte,
//				c = ch - ' ';
//		for (uint8_t l = 0; l < font->height; l += 8){
//			for (uint8_t i = 0; i < font->width; i++)
//			{
//				inc_y = y + 8 + l;
////				if (font->is_numeric_only) {  /* set offset for numeric number */
////					if (ch >= '0' &&ch <= '9')
////						c = ch - '0';
////					else
////						font = fonts[1];
////				}
//
////				glyph_byte = PROGMEM_READ_FAR_BYTE((uint16_t)font-> + (c * font->height + i + l));
//
//				for (uint8_t z = 0; z < 8 ; z++){
//					if ((glyph_byte & 0x80)) {
//						gfx_mono_draw_pixel(x + i, inc_y,
//								GFX_PIXEL_SET);
//					}
//					inc_y--;
//					glyph_byte <<= 1;
//				}
//			}
//		}
//	}
}

#include "glcd-0.5.2/glcd_text.h"
#include "glcd-0.5.2/glcd_font.h"


void clear_string_background(uint8_t length, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font)
{
	gfx_mono_draw_filled_rect(x, y, length, font->height + 3, GFX_PIXEL_CLR);
}

uint8_t length_P(char *str)
{
	uint8_t count = 0;
	uint8_t temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	while (is_valid_char(temp_char)) {
		count++;
		temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	}
	return count;
}

uint8_t draw_string_in_buffer_P(char *str, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font)
{
	uint8_t print_length = 0;
	uint8_t length = length_P(str) * font->width;
	if (length >= GFX_MONO_LCD_WIDTH) {
			gfx_mono_draw_string(str, 0, y, &sysfont);
	} else {
		if (x == 0)
			x = (GFX_MONO_LCD_WIDTH - length - 30) / 2;

//		clear_string_background(length, x, y, font);
		uint8_t temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));

		glcd_set_font_from_font(font);

		while (temp_char){
			if (temp_char == '\n' || x > 120){
				draw_string_in_buffer_P(str, x, y + font->height + 2, font);
				break;
			}

			glcd_draw_char_xy(x, y, temp_char);

			x += 8;
			print_length += 8;
			temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));
		}

		glcd_write();
	}
	return print_length;
}

uint8_t draw_string_in_buffer(char *str, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font, uint8_t old_len)
{
	uint8_t print_length = 0;
	uint8_t j = 0;
	uint8_t length = strlen(str) * font->width;
	if (length < old_len)
		length = old_len;
	if (length >= GFX_MONO_LCD_WIDTH) {
		gfx_mono_draw_string(str, 0, y, &sysfont); /* clear all line */
	} else {
		if (x == 0)
			x = (GFX_MONO_LCD_WIDTH - length - 10) / 4;
		clear_string_background(length, x, y, font);

//		struct glcd_FontConfig_t *font_config;
//		if (font->width < 7)
////			glcd_tiny_set_font(Font5x7,5,7,32,127);
//			font_config = get_font_by_type(GLCD_FONT_SYSFONT_5X7);
//		else
////			glcd_set_font(Arial_Black_32_to_127_11x11,11,11,32,127);
//			font_config = get_font_by_type(GLCD_FONT_ARIAL_11X11_BOLD);

		glcd_set_font_from_font(font);

		while (str[j] != '\0')
		{
			if (x > 120 || str[j] == '\n')
			{
				y += font->height + 2;
				x = 0;
				if (str[j] == '\n')
					j ++;
				continue;
			}

			glcd_draw_char_xy(x, y, str[j]);

//			draw_char(str[j], x, y, font);
			print_length += 8;
			x += 8;
			j++;
		}
	}
	return print_length;
}
