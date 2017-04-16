/*
 * display_render.h|
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */
#ifndef DISPLAY_DISPLAY_RENDER_H_
#define DISPLAY_DISPLAY_RENDER_H_

#include "glcd-0.5.2/glcd_font.h"
#include "glcd-0.5.2/glcd_text.h"
#include "gfx/gfx_components/gfx_text.h"
#include "gfx/menu-handler.h"
#include <string.h>

#ifndef GLCD_FONT_SUPPORT
//struct gfx_font {
//	PROGMEM_BYTE_ARRAY_T source;
//	uint8_t width;
//	uint8_t height;
//	bool is_numeric_only;
//};
#else
struct gfx_font {
	PROGMEM_BYTE_ARRAY_T source;
	uint8_t width;
	uint8_t height;
	bool is_numeric_only;
};
#endif
void clear_string_background(uint8_t length, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font);

uint8_t draw_string_in_buffer_P(char *str, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font);

uint8_t draw_string_in_buffer(char *ch, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font, uint8_t old_len);

void clear_screen(void);

#endif
