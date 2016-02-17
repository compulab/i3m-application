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

static  struct glcd_FontConfig_t courier_new_14x18_num_font = {
		.font_table = Courier_New14x18_NUM,
		.width = 14,
		.height = 18,
		.start_char = 48,
		.end_char = 57,
		.table_type = MIKRO,
};

static  struct glcd_FontConfig_t courier_new_8x13_bold_font = {
		.font_table = Courier_New8x13,
		.width = 8,
		.height = 13,
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

struct glcd_FontConfig_t *get_font_by_type(uint8_t font_id)
{
	if (font_id > font_size)
		return NULL;
	else if (font_id > NUM_DEFAULT_FONTS)
		return fonts[font_id];
	else {
		struct glcd_FontConfig_t *font;
		switch (font_id) {
		case GLCD_FONT_ARIAL_11X11_BOLD:
			font = &arial_black_11x11_font;
			break;
		case GLCD_FONT_COURIER_NEW_8X13_BOLD:
			font = &courier_new_8x13_bold_font;
			break;
		case GLCD_FONT_COURIER_NEW_14X18_NUM:
			font = &courier_new_14x18_num_font;
			break;
		case GLCD_FONT_SYSFONT_5X7:
			font = &sysfont_5x7_font;
			break;
		default:
			font = &sysfont_5x7_font;
			break;
		}
		return font;
	}
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
