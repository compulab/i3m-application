/*
 * display_render.h|
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */


#ifndef DISPLAY_DISPLAY_RENDER_H_
#define DISPLAY_DISPLAY_RENDER_H_

//#include "ssd1306xled/font6x8.h"
//#include "ssd1306xled/font8x16.h"

#include "../ASF/common/services/gfx_mono/gfx_mono.h"
#include "../gfx/gfx_components.h"
#include "../ASF/common/services/gfx_mono/gfx_mono.h"

void draw_string_in_buffer(char *str, uint8_t x, uint8_t y);

#endif
