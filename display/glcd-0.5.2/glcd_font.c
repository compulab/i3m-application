/*
 * glcd_font.c
 *
 *  Created on: Feb 11, 2016
 *      Author: arkadi
 */

#include "glcd_font.h"

static struct glcd_FontConfig_t **fonts;
static uint16_t font_size;
static uint16_t insert_index;

static  struct glcd_FontConfig_t arial_black_11x11_font = {
		.font_table = Arial_Black_32_to_127_11x11,
		.width = 11,
		.height = 11,
		.start_char = 32,
		.end_char = 127,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t bebas_neue_18x36_num_font = {
		.font_table = Bebas_Neue18x36_Numbers,
		.width = 18,
		.height = 36,
		.start_char = 48,
		.end_char = 57,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t bebas_neue_20x36_bold_num_font = {
		.font_table = Bebas_Neue20x36_Bold_Numbers,
		.width = 20,
		.height = 36,
		.start_char = 48,
		.end_char = 57,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t earthbound_12x19_num_font = {
		.font_table = Earthbound_12x19_48to57,
		.width = 12,
		.height = 19,
		.start_char = 48,
		.end_char = 57,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t liberation_sans_11x14_num_font = {
		.font_table = Liberation_Sans11x14_Numbers,
		.width = 11,
		.height = 14,
		.start_char = 48,
		.end_char = 57,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t liberation_sans_15x21_num_font = {
		.font_table = Liberation_Sans15x21_Numbers,
		.width = 15,
		.height = 21,
		.start_char = 48,
		.end_char = 57,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t liberation_sans_17x17_alpha_font = {
		.font_table = Liberation_Sans17x17_Alpha,
		.width = 17,
		.height = 17,
		.start_char = 65,
		.end_char = 90,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t liberation_sans_20x28_num_font = {
		.font_table = Liberation_Sans20x28_Numbers,
		.width = 20,
		.height = 28,
		.start_char = 48,
		.end_char = 57,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t liberation_sans_27x36_num_font = {
		.font_table = Liberation_Sans27x36_Numbers,
		.width = 27,
		.height = 36,
		.start_char = 48,
		.end_char = 57,
		.table_type = MIKRO,
};


static  struct glcd_FontConfig_t times_new_roman_11x12_font = {
		.font_table = Times_New_Roman11x12,
		.width = 11,
		.height = 12,
		.start_char = 32,
		.end_char = 127,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t sysfont_5x7_font = {
		.font_table = Font5x7,
		.width = 5,
		.height = 7,
		.start_char = 32,
		.end_char = 127,
		.table_type = MIKRO,
};

struct glcd_FontConfig_t *get_font_by_type(enum glcd_font_type type)
{
	struct glcd_FontConfig_t *font;
	switch (type) {
	case GLCD_FONT_ARIAL_11X11_BOLD:
		font = &arial_black_11x11_font;
		break;
	case GLCD_FONT_BEBAS_NEUE_18X36_NUM:
		font = &bebas_neue_18x36_num_font;
		break;
	case GLCD_FONT_BEBAS_NEUE_20X36_BOLD_NUM:
		font = &bebas_neue_20x36_bold_num_font;
		break;
	case GLCD_FONT_EARTHBOUND_12X19_NUM:
		font = &earthbound_12x19_num_font;
		break;
	case GLCD_FONT_LIBERATION_SANS_11X14_NUM:
		font = &liberation_sans_11x14_num_font;
		break;
	case GLCD_FONT_LIBERATION_SANS_15X21_NUM:
		font = &liberation_sans_15x21_num_font;
		break;
	case GLCD_FONT_LIBERATION_SANS_17X17_ALPHA:
		font = &liberation_sans_17x17_alpha_font;
		break;
	case GLCD_FONT_LIBERATION_SANS_20X28_NUM:
		font = &liberation_sans_20x28_num_font;
		break;
	case GLCD_FONT_LIBERATION_SANS_27X36_NUM:
		font = &liberation_sans_27x36_num_font;
		break;
	case GLCD_FONT_TIMES_NEW_ROMAN_11X12:
		font = &times_new_roman_11x12_font;
		break;
	case GLCD_FONT_SYSFONT_5X7:
		font = &sysfont_5x7_font;
		break;
	default:
		font = &sysfont_5x7_font;
		break;
	}
//	if (font != NULL)
//		glcd_set_font_from_font(font);
	return font;
}

void glcd_add_font(struct glcd_FontConfig_t *new_font, uint16_t font_id)
{
	if (font_id >= NUM_DEFAULT_FONTS && font_id < font_size)
		fonts[font_id] = new_font;
}

void glcd_fonts_init(uint8_t fonts_add_size)
{
	font_size = fonts_add_size + NUM_DEFAULT_FONTS;
	fonts = malloc_locked(sizeof(struct glcd_FontConfig_t *) * font_size);
	for (insert_index = 0; insert_index < NUM_DEFAULT_FONTS; insert_index++)
		fonts[insert_index] = get_font_by_type(insert_index);

}
