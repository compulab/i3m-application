
/*
 * gfx_components.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_COMPONENTS_H_
#define GFX_GFX_COMPONENTS_H_

#include "asf.h"
#include "def.h"
#include "gfx_item.h"

struct gfx_text {
	char *text;
	char *textP;
	uint8_t max_text_size;
	bool is_progmem;
	struct glcd_FontConfig_t *font;
};



#endif /* GFX_GFX_COMPONENTS_H_ */
