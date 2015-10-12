/*
 * u8g.c
 *
 *  Created on: Oct 7, 2015
 *      Author: arkadi
 */
#include "u8g.h"

u8g_t u8g;
//const u8g_fntpgm_uint8_t  *font;

void u8g_init()
{
//	font = u8g_font_6x10;
	u8g_InitHWSPI(&u8g, &u8g_dev_ssd1309_128x64_hw_spi, PN(1, 0), PN(1, 1), PN(1,2));
}

void u8g_set_menu_font()
{
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_SetFontRefHeightText(&u8g);
	u8g_SetFontPosTop(&u8g);
}


void _u8g_draw_string(char *text)
{
	uint8_t  w = u8g_GetWidth(&u8g),
			 h = u8g_GetFontAscent(&u8g) - u8g_GetFontDescent(&u8g),
			 d = (w - u8g_GetStrWidth(&u8g, text))/2;
	u8g_DrawStr(&u8g, d, h, text);
}

void _u8g_draw_stringP(const u8g_pgm_uint8_t *text)
{
	uint8_t  w = u8g_GetWidth(&u8g),
			 h = u8g_GetFontAscent(&u8g) - u8g_GetFontDescent(&u8g),
			 d = (w-u8g_GetStrWidthP(&u8g, text))/2;
	u8g_DrawStrP(&u8g, d, h, text);
}


void _u8g_set_font()
{
	u8g_SetFont(&u8g, u8g_font_6x10);
}

void u8g_clear_screen()
{
	u8g_FirstPage(&u8g);
	while (u8g_NextPage(&u8g)) {};
}

void set_menu_size(struct menu *menu)
{
	uint8_t count = 0;
	struct menu_item_node *curr_node = menu->first_item;
	while (curr_node != 0){
		if (curr_node->visible)
			count++;
		curr_node = curr_node->next;
	}
	menu->size = count;
}

void u8g_draw_menu(struct menu *menu)
{
	u8g_FirstPage(&u8g);
	do {
		u8g_set_menu_font();
		uint8_t i = 2, h = u8g_GetFontAscent(&u8g) - u8g_GetFontDescent(&u8g);
		u8g_uint_t d, w = u8g_GetWidth(&u8g), item_index = 0;;

		d = (w-u8g_GetStrWidthP(&u8g, menu->title))/2;
		u8g_DrawStrP(&u8g, d, 0, menu->title);
		set_menu_size(menu);
		if (menu->curr_selected_index > menu->size)
			menu->curr_selected_index = 0;
		struct menu_item_node *curr_node = menu->first_item;
		while (curr_node != 0 && !curr_node->visible)
			curr_node = curr_node->next;
		while (curr_node != 0)
		{
			d = (w-u8g_GetStrWidthP(&u8g, curr_node->text))/2;
			u8g_SetDefaultForegroundColor(&u8g);
			if ( item_index == menu->curr_selected_index)
			{
				menu->curr_selected_item = curr_node;
				u8g_DrawBox(&u8g, 0, i*h+1, w, h);
				u8g_SetDefaultBackgroundColor(&u8g);
			}
			u8g_DrawStrP(&u8g, d, i*h, curr_node->text);
			curr_node = curr_node->next;
			while (curr_node != 0 && !curr_node->visible)
				curr_node = curr_node->next;
			i++;
			item_index++;
		}
	} while (u8g_NextPage(&u8g));
}

void u8g_draw_frame(struct gfx_frame *frame)
{
	u8g_FirstPage(&u8g);
	_u8g_set_font();
	do {
//		_u8g_draw_string("test");

//		struct gfx_label_node *label = frame->label_head;
//		while (label != 0){
////			_u8g_draw_stringP(label->label.text);
//			_u8g_draw_string("test");
//			label = label->next;
//		}
//		struct gfx_information_node *info = frame->information_head;
//		while (info != 0){
////			update_data_by_type(info->information.info_type, info->information.text, info->information.info_data);
//			info->information.text = "testing";
//			_u8g_draw_string(info->information.text);
//			info = info->next;
//		}
//		struct gfx_image_node *image = frame->image_head;
//		while (image != 0){
//			gfx_image_draw(&image->image);
//			image = image->next;
//		}
	} while (u8g_NextPage(&u8g));
}

void u8g_draw_string(uint8_t y, char *text)
{
	u8g_FirstPage(&u8g);
	do {
		_u8g_set_font();
		_u8g_draw_string(text);

	} while (u8g_NextPage(&u8g));
}


void u8g_draw_stringP(uint8_t y, const u8g_pgm_uint8_t *text)
{
	u8g_FirstPage(&u8g);
	do {
		_u8g_set_font();
		_u8g_draw_stringP(text);
	} while (u8g_NextPage(&u8g));
}
