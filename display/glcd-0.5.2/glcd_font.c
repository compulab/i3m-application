/*
 * glcd_font.c
 *
 *  Created on: Feb 11, 2016
 *      Author: arkadi
 */

#include "glcd_font.h"
#include "lib/memory.h"

struct glcd_FontConfig_t **fonts;
uint16_t fonts_size;
static uint16_t insert_index;


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
	if (font_id > fonts_size) {
		return NULL;
	} else if (font_id >= NUM_DEFAULT_FONTS) {
		return fonts[font_id];
	} else {
		struct glcd_FontConfig_t *font;
		switch (font_id) {
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
	if (font_id >= NUM_DEFAULT_FONTS && font_id < fonts_size)
		fonts[font_id] = new_font;
}

void glcd_fonts_init(uint8_t fonts_add_size)
{
	fonts_size = fonts_add_size + NUM_DEFAULT_FONTS;
	fonts = malloc_locked(sizeof(struct glcd_FontConfig_t *) * fonts_size);
	for (insert_index = 0; insert_index < NUM_DEFAULT_FONTS; insert_index++)
		fonts[insert_index] = get_font_by_type(insert_index);

}
