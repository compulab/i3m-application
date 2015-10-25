/*
 * gfx_utils.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */


#include "gfx_utils.h"

#define PAGE_ADDRESS(y) floor (y/8)

void print_horizontal_line(uint8_t x, uint8_t y, uint8_t length)
{
	gfx_mono_draw_line(x, y, x + length, y, GFX_PIXEL_SET);
}

void print_vertical_line(uint8_t x, uint8_t y, uint8_t length)
{
	gfx_mono_draw_line(x, y, x, y + length, GFX_PIXEL_SET);
}

void gfx_item_draw(struct gfx_item *item)
{
//	if (item->visible && item->border_visible){
//		print_horizontal_line(item->x, item->y, item->width);
//		print_vertical_line(item->x, item->y, item->height);
//		print_horizontal_line(item->x, item->y+item->height, item->width);
//		print_vertical_line(item->x + item->width, item->y, item->height);
//	}
}

void gfx_item_init(struct gfx_item *item, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	item->x = x;
	item->y = y;
	item->width = width;
	item->height = height;
	item->visible = true;
}

void gfx_label_init(struct gfx_label *label, char *text,
		uint8_t x, uint8_t y)
{
	struct font * font = &sysfont;
	uint8_t length = strlen_P(text);
	uint8_t width = strlen(text) * font->width + 2,
			height = font->height + 4;

		gfx_item_init(&label->postion, x, y, width, height);
		label->text.is_progmem = false;
		label->text.text = malloc(sizeof(char) * length);
		memcpy_P(label->text.text, text, sizeof(char) * length);
		label->text.font = font;
}

void set_size_by_text(char *text, struct font *font, struct gfx_item *item)
{
	uint8_t width = strlen(text) * font->width + 2,
			height = font->height + 4;
	item->width = width;
	item->height = height;
}

void gfx_information_init(struct gfx_information *info,
		enum information_type info_type, uint8_t info_data, uint8_t max_length, uint8_t x, uint8_t y)
{
	struct font *font = &sysfont;
	info->info_type = info_type;
	info->info_data = info_data;
	info->text.text = malloc (sizeof(char) * max_length);
	gfx_item_init(&info->postion, x, y, 0, 0);
	info->text.font = font;
}

void gfx_label_with_font_init(struct gfx_label *label, char *text, struct font *font,
		uint8_t x, uint8_t y)
{
	gfx_label_init(label, text, x, y);
	label->text.font = font;
}

void update_information_present(struct gfx_information *info)
{
	switch(info->info_type){
	case SHOW_SERIAL_NUMBER:
	case SHOW_PRODUCT_NAME:
	case SET_BRIGHTNESS:
	case SHOW_HDD_SIZE:
	case SHOW_MEMORY_SIZE:
			break;
	default:
		information_present = info;
		break;
	}
}

void gfx_information_draw(struct gfx_information *info)
{
	update_information_present(info);
	update_data_by_type(info->info_type, (info->text).text, info->info_data);
	int x = info->postion.x + 2,
			y = info->postion.y + 2;
	struct gfx_text data = info->text;
//	gfx_mono_draw_string(data.text, x, y, data.font);
	draw_string_in_buffer(data.text, x, y);
	gfx_mono_put_framebuffer();
}

void update_information()
{
//	u8g_draw_frame(frame_present);
	gfx_frame_draw(frame_present);
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
		uint8_t height, uint8_t width, uint8_t x, uint8_t y)
{
	struct gfx_mono_bitmap *bitmap = malloc(sizeof(struct gfx_mono_bitmap));
	bitmap->width = width;
	bitmap->height = height;
	bitmap->data.progmem = bitmap_progmem;
	bitmap->type = GFX_MONO_BITMAP_SECTION;
	image->bitmap = bitmap;
	gfx_item_init(&image->postion, x, y, (image->bitmap->width + 2),
			(image->bitmap->height + 2));
}

void gfx_image_draw(struct gfx_image *image)
{
	gfx_item_draw(&image->postion);
	gfx_mono_generic_put_bitmap(image->bitmap, image->postion.x, image->postion.y);
//	gfx_mono_put_framebuffer();
}

void init_frame(struct gfx_frame *frame)
{
	frame->image_head = 0;
	frame->information_head = 0;
	frame->label_head = 0;
}

void gfx_frame_init(struct gfx_frame *frame, struct cnf_frame *cnf_frame_pgmem)
{
	struct cnf_frame cnf_frame;
	memcpy_P(&cnf_frame, cnf_frame_pgmem, sizeof(cnf_frame));
	init_frame(frame);
	struct cnf_image_node *cnf_image_pgmem = cnf_frame.images_head;
	struct gfx_image_node *frame_image_last =0;
	while (cnf_image_pgmem != 0){
		struct cnf_image_node cnf_image_node;
		struct gfx_image_node *gfx_image_node = malloc(sizeof(struct gfx_image_node));
		memcpy_P(&cnf_image_node, cnf_image_pgmem, sizeof(struct cnf_image_node));
		gfx_image_init(&gfx_image_node->image, cnf_image_node.image.bitmap_progmem, cnf_image_node.image.height,
				cnf_image_node.image.width, cnf_image_node.image.x, cnf_image_node.image.y);
		gfx_image_node->next = 0;
		if (frame->image_head == 0)
			frame->image_head = gfx_image_node;
		else
			frame_image_last->next = gfx_image_node;
		frame_image_last = gfx_image_node;
		cnf_image_pgmem = cnf_image_node.next;
	}

	struct cnf_label_node *cnf_label_pgmem = cnf_frame.labels_head;
	struct gfx_label_node *frame_label_last = 0;
	while (cnf_label_pgmem != 0){
		struct cnf_label_node cnf_label_node;
		struct gfx_label_node *gfx_label_node = malloc(sizeof(struct gfx_label_node));
		memcpy_P(&cnf_label_node, cnf_label_pgmem, sizeof(struct cnf_label_node));
		gfx_label_init(&gfx_label_node->label, cnf_label_node.label.text, cnf_label_node.label.x, cnf_label_node.label.y);
		gfx_label_node->next = 0;
		if (frame->label_head == 0)
			frame->label_head = gfx_label_node;
		else
			frame_label_last->next = gfx_label_node;
		frame_label_last = gfx_label_node;
		cnf_label_pgmem = cnf_label_node.next;
	}

	struct cnf_info_node *cnf_info_pgmem = cnf_frame.infos_head;
	struct gfx_information_node *frame_information_last = 0;
	while (cnf_info_pgmem != 0){
		struct cnf_info_node cnf_info_node;
		struct gfx_information_node *gfx_information_node = malloc(sizeof(struct gfx_information_node));
		memcpy_P(&cnf_info_node, cnf_info_pgmem, sizeof(struct cnf_info_node));
		gfx_information_init(&gfx_information_node->information, cnf_info_node.info.info_type, cnf_info_node.info.information,
						cnf_info_node.info.max_length, cnf_info_node.info.x, cnf_info_node.info.y);
		gfx_information_node->next = 0;
		if (frame->information_head == 0)
			frame->information_head = gfx_information_node;
		else
			frame_information_last->next = gfx_information_node;
		frame_information_last = gfx_information_node;
		cnf_info_pgmem = cnf_info_node.next;
	}
}

void gfx_frame_draw(struct gfx_frame *frame)
{
	if (frame != 0){
		frame_present = frame;
		//	gfx_mono_draw_filled_rect(GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT, 0,
		//			0, GFX_PIXEL_CLR);
		struct gfx_label_node *label = frame->label_head;
		while (label != 0){
			gfx_label_draw(&label->label);
			label = label->next;
		}
		struct gfx_information_node *info = frame->information_head;
		while (info != 0){
			gfx_information_draw(&info->information);
			info = info->next;
		}
		struct gfx_image_node *image = frame->image_head;
		while (image != 0){
			gfx_image_draw(&image->image);
			image = image->next;
		}
		gfx_mono_put_framebuffer();
	}
}
