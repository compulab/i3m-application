/*
 * display_render.c
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */

#include "display_render.h"

uint8_t draw_string_in_buffer(char *str, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font)
{
	uint8_t print_length = 0;
	uint8_t j = 0;
	uint16_t str_len = strlen(str);
	uint8_t length = str_len * (font->width + 1);
	struct glcd_FontConfig_t *best_font = font;

	if (length > GFX_MONO_LCD_WIDTH - x) {
		x = 0;
		best_font = glcd_get_best_font(font, str_len + 1, GFX_MONO_LCD_WIDTH);

		if (best_font == NULL) {
			gfx_mono_draw_string(str, 0, y, &sysfont); /* clear all line */
			return str_len * 8;
		}

		length = str_len * (best_font->width + 1);
	}

	glcd_set_font_from_font(best_font);

	length += best_font->width;

	while (str[j] != '\0') {
		if (x > 120 || str[j] == '\n') {
			y += best_font->height + 2;
			x = 0;
			if (str[j] == '\n')
				j++;
			continue;
		}

		glcd_draw_char_xy(x, y, str[j]);

		print_length += 8;
		x += best_font->width + 1;
		j++;
	}

	return print_length;
}

void clear_screen(void)
{
	for (int i = 0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
}