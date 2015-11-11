/*
 * display_render.h|
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */
#ifndef DISPLAY_DISPLAY_RENDER_H_
#define DISPLAY_DISPLAY_RENDER_H_

#include <string.h>

#ifndef GFX_GFX_COMPONENTS_H_
	#include "../gfx/gfx_components.h"
#endif

struct gfx_font {
	PROGMEM_BYTE_ARRAY_T source;
	uint8_t width;
	uint8_t height;
};

void draw_string_in_buffer_P(char *str, uint8_t x, uint8_t y, struct gfx_font *font);

void draw_string_in_buffer(char *ch, uint8_t x, uint8_t y, struct gfx_font *font);

#endif
