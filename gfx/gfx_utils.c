/*
 * gfx_utils.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#include "gfx_components.h"
#include "../asf.h"
#include <math.h>
#include <string.h>

#define PAGE_ADDRESS(y) floor (y/8)

void printHorizontalLine(uint8_t x, uint8_t y, uint8_t length){
	gfx_mono_draw_line(x,y,x+length,y,GFX_PIXEL_SET);
}

void printVerticalLine(uint8_t x, uint8_t y, uint8_t length){
	gfx_mono_draw_line(x,y,x,y+length,GFX_PIXEL_SET);
}

void gfx_item_draw(gfx_item * item){
	if (item->visible && item->borderVisible){
		printHorizontalLine(item->x,item->y,item->width);
		printVerticalLine(item->x,item->y,item->height);
		printHorizontalLine(item->x,item->y+item->height,item->width);
		printVerticalLine(item->x+item->width,item->y,item->height);
	}
}

void gfx_item_init(gfx_item * item, uint8_t x, uint8_t y, bool borderVisible, uint8_t width,uint8_t height){
	item->x = x;
	item->y = y;
	item->width = width;
	item->height = height;
	item->visible = true;
	item->borderVisible = borderVisible;
}

void gfx_label_init(gfx_label * label,char * text, uint8_t x, uint8_t y, bool borderVisible){
	struct font * font = &sysfont;
	uint8_t width = strlen(text)*font->width +2,
			height = font->height +4;
//	if (width -x >= GFX_MONO_LCD_WIDTH){
		gfx_item_init(&label->postion,x,y,borderVisible,width,height);
		label->text.text = text;
		label->text.font = font;
//	}
}

void set_size_by_text(char * text,struct font* font,gfx_item * item){
	uint8_t width = strlen(text)*font->width +2,
			height = font->height +4;
	item->width = width;
	item->height = height;
}

void gfx_information_label_init(gfx_information_label * label,information_type info_type, uint8_t x, uint8_t y, bool borderVisible){
	struct font * font = &sysfont;
	label->info_type = info_type;
	gfx_item_init(&label->postion,x,y,borderVisible,0,0);
	label->text.font = font;
}

void gfx_label_with_font_init(gfx_label * label,char * text, struct font * font, uint8_t x, uint8_t y, bool borderVisible){
	gfx_label_init(label,text,x,y,borderVisible);
	label->text.font = font;
}

void gfx_information_label_draw(gfx_information_label * info_label){
	updateDataByType(info_label->info_type, &((info_label->text).text));
	set_size_by_text(info_label->text.text,info_label->text.font, &info_label->postion);
	gfx_item_draw(&info_label->postion);
	int x = info_label->postion.x +2,
			y = info_label->postion.y +2;
	gfx_text data = info_label->text;
	gfx_mono_draw_string(data.text,x,y,data.font);
}

void gfx_label_draw(gfx_label * label){
	gfx_item_draw(&label->postion);
	int x = label->postion.x +2,
			y = label->postion.y +2;
	gfx_text data = label->text;
	gfx_mono_draw_string(data.text,x,y,data.font);
}

void gfx_image_init(gfx_image * image, struct gfx_mono_bitmap * bitmap, uint8_t x, uint8_t y, bool borderVisible){
	image->bitmap = bitmap;
	gfx_item_init(&image->postion,x,y,borderVisible,(bitmap->width+2),(bitmap->height+2));
}

void gfx_image_draw(gfx_image * image){
	gfx_item_draw(&image->postion);
	gfx_mono_generic_put_bitmap(image->bitmap,image->postion.x,image->postion.y);
}

