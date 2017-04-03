/*
 * gfx_label.c
 *
 * Created: 4/2/2017 5:41:17 PM
 *  Author: Nikita
 */

#include "gfx_label.h"
#include "display/display_render.h"

static void gfx_label_draw(struct gfx_label *label)
{
	if (label->text.textP != NULL)
		draw_string_in_buffer_P(label->text.textP, label->postion.x, label->postion.y, label->text.font);
}

void gfx_label_init(struct gfx_label *label, char *text, uint8_t x, uint8_t y, uint8_t font_id)
{
	gfx_item_init(&label ->postion, x, y, 0, 0);
	label->text.is_progmem = true;
	label->text.textP = text;
	label->text.font = get_font_by_type(font_id);
	label->draw = gfx_label_draw;
}