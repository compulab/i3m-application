/*
 * glcd_font.h
 *
 *  Created on: Feb 11, 2016
 *      Author: arkadi
 */

#ifndef DISPLAY_GLCD_0_5_2_GLCD_FONT_H_
#define DISPLAY_GLCD_0_5_2_GLCD_FONT_H_

#include "../../work-queue/work.h"

//#include "glcd.h"
#include "glcd_text.h"
#include "fonts/Courier_New_32_to_127_8X13.h"
#include "fonts/Courier_New_48_to_57_14X18_Num.h"
#include "fonts/arial_black_32_to_127_11X11.h"
#include "fonts/font5x7.h"

#define NUM_DEFAULT_FONTS		4

enum glcd_font_type {
	GLCD_FONT_SYSFONT_5X7,
	GLCD_FONT_COURIER_NEW_8X13_BOLD,
	GLCD_FONT_COURIER_NEW_14X18_NUM,
	GLCD_FONT_ARIAL_11X11_BOLD,
};

struct glcd_FontConfig_t *get_font_by_type(uint8_t font_id);

void glcd_add_font(struct glcd_FontConfig_t *new_font, uint16_t font_id);

void glcd_fonts_init(uint8_t fonts_add_size);

#endif /* DISPLAY_GLCD_0_5_2_GLCD_FONT_H_ */
