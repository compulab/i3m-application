/*
 * gfx_text.c
 *
 * Created: 11-Apr-17 10:37:52 PM
 *  Author: Nikita
 */

#include <stdbool.h>
#include "display/glcd-0.5.2/glcd_font.h"
#include "gfx_text.h"

void gfx_text_init(struct gfx_text *text, char *str, uint8_t max_text_size, bool is_progmem, uint8_t font_id)
{
	text->is_progmem = is_progmem;
	if (is_progmem) {
		text->textP = str;
		text->text = NULL;
	} else {
		text->textP = NULL;
		text->text = str;
	}

	text->font = get_font_by_type(font_id);
	text->max_text_size = max_text_size;
}