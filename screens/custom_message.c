/*
 * custom_message.c
 *
 * Created: 09-Nov-17 1:13:42 PM
 *  Author: Nikita
 */

#include "display/display_render.h"
#include "gfx/gfx_components/gfx_information.h"
#include "display/glcd-0.5.2/glcd_font.h"
#include "ASF/common/services/gfx_mono/gfx_mono.h"
#include <string.h>

static void draw_custom_message(struct gfx_information *info)
{
	char *message = info->text.text;
	if (message == NULL)
		return;

	uint16_t font_id = fonts_size > 1 ? 2 : GLCD_FONT_SYSFONT_5X7;
	clear_screen();
	uint8_t msg_x = GFX_MONO_LCD_WIDTH / 2 - ((strlen(message) * (get_font_by_type(font_id))->width) / 2);
	uint8_t msg_y = 20;
	draw_string_in_buffer(message, msg_x, msg_y, get_font_by_type(font_id));
	gfx_mono_ssd1306_put_framebuffer();
}

int gfx_information_init_custom_message(struct gfx_information *info)
{
	info->draw_data = draw_custom_message;
	return 0;
}