/*
 * gfx_utils.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */


#include "gfx_utils.h"

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
	#ifdef DEBUG_MODE
		MSG_T_T("string to print:",data.text)
		MSG_T_N("first char of font", data.font->first_char)
	#endif
}

void gfx_image_init(gfx_image * image, gfx_mono_color_t PROGMEM_T * bitmapProgmem,uint8_t height,uint8_t width, uint8_t x, uint8_t y, bool borderVisible){
	image->bitmap->width = width;
	image->bitmap->height = height;
	image->bitmap->data.progmem = bitmapProgmem;
	image->bitmap->type = GFX_MONO_BITMAP_PROGMEM;
	gfx_item_init(&image->postion,x,y,borderVisible,(image->bitmap->width+2),(image->bitmap->height+2));
}

void gfx_image_draw(gfx_image * image){
	gfx_item_draw(&image->postion);
	gfx_mono_generic_put_bitmap(image->bitmap,image->postion.x,image->postion.y);
}

void setFrameSizes(gfx_frame * frame, cnf_frame * cnf_frame){
	frame->image_size = cnf_frame->image_size;
	frame->information_size= cnf_frame->information_size;
	frame->label_size= cnf_frame->label_size;

	#ifdef DEBUG_MODE
		MSG_T_N("images in frame: ", frame->image_size)
		MSG_T_N("labels in frame: ", frame->label_size)
		MSG_T_N("infos in frame: ", frame->information_size)
	#endif
}

void gfx_frame_init(gfx_frame * frame, cnf_frame * cnf_frame_pgmem){
	cnf_gfx_image cnf_image;
	cnf_gfx_information_label cnf_information;
	cnf_gfx_label cnf_label;
	cnf_frame cnf_frame;
	memcpy_P(&cnf_frame,cnf_frame_pgmem,sizeof(cnf_frame));
	setFrameSizes(frame,&cnf_frame);
	frame->images = malloc (sizeof(gfx_image *) * frame->image_size);
	for (int i=0; i < frame->image_size; i++) {
		frame->images[i] = malloc (sizeof(gfx_image));
		memcpy_P(&cnf_image,(cnf_frame.image[i]),sizeof(cnf_gfx_image));
		gfx_image_init(frame->images[i],cnf_image.bitmapProgmem, cnf_image.height,cnf_image.width,cnf_image.x,cnf_image.y,cnf_image.borderVisible);
		#ifdef DEBUG_MODE
			MSG_T_N("image height:",cnf_image.height)
			MSG_T_N("image width:",cnf_image.width)
		#endif
	}
	frame->labels = malloc (sizeof(gfx_label *) * frame->label_size);
	for (int i=0; i < frame->label_size; i++) {
		frame->labels[i] = malloc (sizeof(gfx_label));
		memcpy_P(&cnf_label,cnf_frame.label[i],sizeof(cnf_gfx_label));
		gfx_label_init(frame->labels[i],cnf_label.text,cnf_label.x,cnf_label.y,cnf_label.borderVisible);
		#ifdef DEBUG_MODE
			MSG_T_T("label title:",cnf_label.text)
		#endif
	}
	frame->informationLabels = malloc (sizeof(gfx_information_label *) * frame->information_size);
	for (int i=0; i < frame->information_size; i++) {
		frame->informationLabels[i] = malloc (sizeof(gfx_information_label));
		memcpy_P(&cnf_information,(cnf_frame.information_label[i]),sizeof(cnf_gfx_information_label));
		gfx_information_label_init(frame->informationLabels[i],cnf_information.info_type,cnf_information.x,cnf_information.y,cnf_information.borderVisible);
#		ifdef DEBUG_MODE
			MSG_T_N("[0] powerState [1]voltage :",cnf_information.info_type)
		#endif
	}
}

void gfx_frame_draw(gfx_frame * frame){
	gfx_mono_draw_filled_rect(
				GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT, 0, 0,GFX_PIXEL_CLR);
	for (int i=0; i < frame->label_size; i++)
		gfx_label_draw(frame->labels[i]);
	for (int i=0; i < frame->information_size; i++)
			gfx_information_label_draw(frame->informationLabels[i]);
	for (int i=0; i < frame->image_size; i++)
			gfx_image_draw(frame->images[i]);
}
