/*
 * gfx_utils.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */


#include "gfx_utils.h"

#define PAGE_ADDRESS(y) floor (y/8)

void printHorizontalLine(uint8_t x, uint8_t y, uint8_t length)
{
	gfx_mono_draw_line(x, y, x + length, y, GFX_PIXEL_SET);
}

void printVerticalLine(uint8_t x, uint8_t y, uint8_t length)
{
	gfx_mono_draw_line(x, y, x, y + length, GFX_PIXEL_SET);
}

void gfx_item_draw(struct gfx_item *item)
{
	if (item->visible && item->border_visible){
		printHorizontalLine(item->x, item->y, item->width);
		printVerticalLine(item->x, item->y, item->height);
		printHorizontalLine(item->x, item->y+item->height, item->width);
		printVerticalLine(item->x + item->width, item->y, item->height);
	}
}

void gfx_item_init(struct gfx_item *item, uint8_t x, uint8_t y,
		bool borderVisible, uint8_t width, uint8_t height)
{
	item->x = x;
	item->y = y;
	item->width = width;
	item->height = height;
	item->visible = true;
	item->border_visible = borderVisible;
}

void gfx_label_init(struct gfx_label *label, char *text,
		uint8_t x, uint8_t y, bool borderVisible)
{
	struct font * font = &sysfont;
	uint8_t width = strlen(text) * font->width + 2,
			height = font->height + 4;
//	if (width -x >= GFX_MONO_LCD_WIDTH){
		gfx_item_init(&label->postion, x, y, borderVisible, width, height);
		label->text.is_progmem = true;
		label->text.text = text;
		label->text.font = font;
//	}
}

void set_size_by_text(char *text, struct font *font, struct gfx_item *item)
{
	uint8_t width = strlen(text) * font->width + 2,
			height = font->height + 4;
	item->width = width;
	item->height = height;
}

void gfx_information_label_init(struct gfx_information *info,
		enum information_type info_type, uint8_t info_data, uint8_t x, uint8_t y, bool borderVisible)
{
	struct font *font = &sysfont;
	info->info_type = info_type;
	info->info_data = info_data;
	gfx_item_init(&info->postion, x, y, borderVisible, 0, 0);
	info->text.font = font;
}

void gfx_label_with_font_init(struct gfx_label *label, char *text, struct font *font,
		uint8_t x, uint8_t y, bool borderVisible)
{
	gfx_label_init(label, text, x, y, borderVisible);
	label->text.font = font;
}

void gfx_information_label_draw(struct gfx_information *info)
{
	update_data_by_type(info->info_type, &((info->text).text));
	set_size_by_text(info->text.text, info->text.font, &info->postion);
	gfx_item_draw(&info->postion);
	int x = info->postion.x + 2,
			y = info->postion.y + 2;
	struct gfx_text data = info->text;
	gfx_mono_draw_string(data.text, x, y, data.font);
}

void gfx_label_draw(struct gfx_label *label)
{
	gfx_item_draw(&label->postion);
	int x = label->postion.x + 2,
			y = label->postion.y + 2;
	struct gfx_text data = label->text;
	if (data.is_progmem)
		gfx_mono_draw_progmem_string(data.text, x, y, data.font);
	else
		gfx_mono_draw_string(data.text, x, y, data.font);
	#ifdef DEBUG_MODE
		MSG_T_T("string to print:",data.text)
		MSG_T_N("first char of font", data.font->first_char)
	#endif
}

void gfx_image_init(struct gfx_image *image, gfx_mono_color_t PROGMEM_T *bitmap_progmem,
		uint8_t height, uint8_t width, uint8_t x, uint8_t y, bool border_visible)
{
	struct gfx_mono_bitmap *bitmap = malloc(sizeof(struct gfx_mono_bitmap));
	bitmap->width = width;
	bitmap->height = height;
	bitmap->data.progmem = bitmap_progmem;
	bitmap->type = GFX_MONO_BITMAP_SECTION;
	image->bitmap = bitmap;
	gfx_item_init(&image->postion, x, y, border_visible, (image->bitmap->width + 2),
			(image->bitmap->height + 2));
}

void gfx_image_draw(struct gfx_image *image)
{
	gfx_item_draw(&image->postion);
	gfx_mono_generic_put_bitmap(image->bitmap, image->postion.x, image->postion.y);
}

void set_frame_sizes(struct gfx_frame *frame, struct cnf_frame *cnf_frame)
{
	frame->image_size = cnf_frame->image_size;
	frame->information_size = cnf_frame->information_size;
	frame->label_size = cnf_frame->label_size;

	#ifdef DEBUG_MODE
		MSG_T_N("images in frame: ", frame->image_size)
		MSG_T_N("labels in frame: ", frame->label_size)
		MSG_T_N("infos in frame: ", frame->information_size)
	#endif
}

void gfx_frame_init(struct gfx_frame *frame, struct cnf_frame *cnf_frame_pgmem)
{
	struct cnf_image cnf_image;
	struct cnf_info cnf_information;
	struct cnf_label cnf_label;
	struct cnf_frame cnf_frame;
	memcpy_P(&cnf_frame, cnf_frame_pgmem, sizeof(cnf_frame));
	set_frame_sizes(frame, &cnf_frame);
	frame->images = malloc (sizeof(struct gfx_image *) * frame->image_size);
	for (int i=0; i < frame->image_size; i++) {
		frame->images[i] = malloc (sizeof(struct gfx_image));
		memcpy_P(&cnf_image, (cnf_frame.images[i]), sizeof(struct cnf_image));
		gfx_image_init(frame->images[i], cnf_image.bitmapProgmem, cnf_image.height,
		cnf_image.width, cnf_image.x, cnf_image.y, cnf_image.border_visible);
		#ifdef DEBUG_MODE
			MSG_T_N("image height:",cnf_image.height)
			MSG_T_N("image width:",cnf_image.width)
		#endif
	}
	frame->labels = malloc (sizeof(struct gfx_label *) * frame->label_size);
	for (int i=0; i < frame->label_size; i++) {
		frame->labels[i] = malloc (sizeof(struct gfx_label));
		memcpy_P(&cnf_label, cnf_frame.labels[i], sizeof(struct cnf_label));
		gfx_label_init(frame->labels[i], cnf_label.text, cnf_label.x, cnf_label.y,
				cnf_label.border_visible);
		#ifdef DEBUG_MODE
			MSG_T_T("label title:",cnf_label.text)
		#endif
	}
	frame->informations = malloc (sizeof(struct gfx_information *) * frame->information_size);
	for (int i=0; i < frame->information_size; i++) {
		frame->informations[i] = malloc (sizeof(struct gfx_information));
		memcpy_P(&cnf_information, (cnf_frame.informations[i]), sizeof(struct cnf_info));
		gfx_information_label_init(frame->informations[i], cnf_information.info_type,cnf_information.information,
				cnf_information.x, cnf_information.y, cnf_information.border_visible);
#		ifdef DEBUG_MODE
			MSG_T_N("[0] powerState [1]voltage :",cnf_information.info_type)
		#endif
	}
}

void gfx_frame_draw(struct gfx_frame *frame)
{
	gfx_mono_draw_filled_rect(GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT, 0,
			0, GFX_PIXEL_CLR);
	for (int i=0; i < frame->label_size; i++)
		gfx_label_draw(frame->labels[i]);
	for (int i=0; i < frame->information_size; i++)
		gfx_information_label_draw(frame->informations[i]);
	for (int i=0; i < frame->image_size; i++)
		gfx_image_draw(frame->images[i]);
}
