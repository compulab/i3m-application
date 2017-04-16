
/*
 * gfx_components.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_COMPONENTS_H_
#define GFX_GFX_COMPONENTS_H_

#include "asf.h"
#include <stdio.h>

struct gfx_text {
	char *text;
	char *textP;
	uint8_t max_text_size;
	bool is_progmem;
	struct glcd_FontConfig_t *font;
};

void gfx_text_init(struct gfx_text *text, char *str, uint8_t max_text_size, bool is_progmem, uint8_t font_id);

#endif /* GFX_GFX_COMPONENTS_H_ */
