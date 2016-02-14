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
#include "fonts/Bebas_Neue18x36_Numbers.h"
#include "fonts/Bebas_Neue20x36_Bold_Numbers.h"
#include "fonts/Earthbound_12x19_48to57.h"
#include "fonts/Liberation_Sans11x14_Numbers.h"
#include "fonts/Liberation_Sans15x21_Numbers.h"
#include "fonts/Liberation_Sans17x17_Alpha.h"
#include "fonts/Liberation_Sans20x28_Numbers.h"
#include "fonts/Liberation_Sans27x36_Numbers.h"
#include "fonts/arial_black_32_to_127_11X11.h"
#include "fonts/font5x7.h"
#include "fonts/times_new_roman_32_to_127_11X12.h"

#define NUM_DEFAULT_FONTS		11

enum glcd_font_type {
	GLCD_FONT_SYSFONT_5X7,
	GLCD_FONT_ARIAL_11X11_BOLD,
	GLCD_FONT_TIMES_NEW_ROMAN_11X12,
	GLCD_FONT_BEBAS_NEUE_18X36_NUM,
	GLCD_FONT_BEBAS_NEUE_20X36_BOLD_NUM,
	GLCD_FONT_EARTHBOUND_12X19_NUM,
	GLCD_FONT_LIBERATION_SANS_11X14_NUM,
	GLCD_FONT_LIBERATION_SANS_15X21_NUM,
	GLCD_FONT_LIBERATION_SANS_17X17_ALPHA,
	GLCD_FONT_LIBERATION_SANS_20X28_NUM,
	GLCD_FONT_LIBERATION_SANS_27X36_NUM,
};

struct glcd_FontConfig_t *get_font_by_type(enum glcd_font_type type);

void glcd_add_font(struct glcd_FontConfig_t *new_font, uint16_t font_id);

void glcd_fonts_init(uint8_t fonts_add_size);

#endif /* DISPLAY_GLCD_0_5_2_GLCD_FONT_H_ */
