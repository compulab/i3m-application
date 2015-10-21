/*
 * u8g.c
 *
 *  Created on: Oct 7, 2015
 *      Author: arkadi
 */
#include "u8g.h"

u8g_t u8g;
//const u8g_fntpgm_uint8_t  *font;
uint8_t  screen_width;
uint8_t font_size;
uint8_t y;

void u8g_init()
{
//	font = u8g_font_6x10;
	u8g_InitHWSPI(&u8g, &u8g_dev_ssd1309_128x64_hw_spi, PN(1, 0), PN(1, 1), PN(1,2));
}

void _u8g_draw_string(char *text)
{
	uint8_t  d = (screen_width - u8g_GetStrWidth(&u8g, text))/2;
	u8g_DrawStr(&u8g, d, y, text);
	y += 10 + font_size;
}

void _u8g_draw_stringP(const u8g_pgm_uint8_t *text)
{
	uint8_t  d = (screen_width - u8g_GetStrWidthP(&u8g, text))/2;
	u8g_DrawStrP(&u8g, d, y, text);
	y += 10 + font_size;
}


void _u8g_set_font(const u8g_fntpgm_uint8_t  *font)
{
	u8g_SetFont(&u8g, font);
	font_size = u8g_GetFontAscent(&u8g) - u8g_GetFontDescent(&u8g);
	screen_width = u8g_GetWidth(&u8g);
}

void u8g_clear_screen()
{
	u8g_FirstPage(&u8g);
	while (u8g_NextPage(&u8g)) {};
}


uint8_t get_string_start(char * text)
{
	return 	(screen_width - u8g_GetStrWidth(&u8g, text))/2;
}

void u8g_draw_frame(struct gfx_frame *frame)
{
	u8g_FirstPage(&u8g);
		do {
			_u8g_set_font(u8g_font_6x10);
			y = 20;
		struct gfx_label_node *label = frame->label_head;
		while (label != 0){
			_u8g_draw_string(label->label.text.text);
			label = label->next;
		}

		y = 45;
		_u8g_set_font(u8g_font_9x18);
		struct gfx_information_node *info = frame->information_head;
		while (info != 0){
			update_data_by_type(info->information.info_type, info->information.text.text, info->information.info_data);
//			info->information.text.text = "testing";
			_u8g_draw_string(info->information.text.text);
			info = info->next;
		}
		struct gfx_image_node *image = frame->image_head;
		while (image != 0){
//			gfx_image_draw(image->image.bitmap);
			image = image->next;
		}
	} while (u8g_NextPage(&u8g));
}

void u8g_draw_string(uint8_t y, char *text)
{
//	u8g_FirstPage(&u8g);
//	do {
//		_u8g_set_font();
//		_u8g_draw_string(text);
//
//	} while (u8g_NextPage(&u8g));
}


void u8g_draw_stringP(uint8_t y, const u8g_pgm_uint8_t *text)
{
//	u8g_FirstPage(&u8g);
//	do {
//		_u8g_set_font();
//		_u8g_draw_stringP(text);
//	} while (u8g_NextPage(&u8g));
}
