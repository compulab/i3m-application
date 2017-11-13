/*
 * display_render.c
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */

#include "display_render.h"

void draw_string_in_buffer(char *str, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font)
{
	uint16_t str_len = strlen(str);
	struct glcd_FontConfig_t *best_font = font;

	if (str_len * (font->width + 1) > GFX_MONO_LCD_WIDTH - x) {
		x = 0;
		best_font = glcd_get_best_font(font, str_len + 1, GFX_MONO_LCD_WIDTH);

		if (best_font == NULL) {
			gfx_mono_draw_string(str, 0, y, &sysfont); //clear all line
			return;
		}
	}

	glcd_set_font_from_font(best_font);

	for (int j = 0; str[j] != '\0'; j++) {
		if (x > 120 || str[j] == '\n') {
			y += best_font->height + 2;
			x = 0;
			if (str[j] == '\n')
				continue;
		}

		glcd_draw_char_xy(x, y, str[j]);

		x += best_font->width + 1;
	}

	return;
}

void clear_screen(void)
{
	for (int i = 0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
}