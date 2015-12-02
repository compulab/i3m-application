/*
 * gfx_utils.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#include "gfx_utils.h"

#define PAGE_ADDRESS(y) floor (y/8)

PROGMEM_DECLARE(gfx_mono_color_t, right [5]) = {
0x00, 0x7E, 0x3C, 0x18, 0x00,
};

PROGMEM_DECLARE(gfx_mono_color_t, left [5]) = {
0x00, 0x18, 0x3C, 0x7E, 0x00,
};

static struct gfx_mono_bitmap left_bitmap = {
		.width = 5,
		.height = 8,
		.data.progmem = left,
		.type = GFX_MONO_BITMAP_PROGMEM
};

static struct gfx_mono_bitmap right_bitmap = {
		.width = 5,
		.height = 8,
		.data.progmem = right,
		.type = GFX_MONO_BITMAP_PROGMEM
};

static struct gfx_image left_sign_image = {
	.postion = {
		.x = 0,
		.y = 57,
	},
	.bitmap = &left_bitmap,
};

static struct gfx_image right_sign_image = {
	.postion = {
		.x = 115,
		.y = 57,
	},
	.bitmap = &right_bitmap,
};
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
		uint8_t x, uint8_t y, uint8_t font_id)
{
		gfx_item_init(&label ->postion, x, y, 0, 0);
		label->text.is_progmem = true;
		label->text.textP = text;
		label->text.font = fonts[font_id];
}

void set_size_by_text(char *text, struct font *font, struct gfx_item *item)
{
	uint8_t width = strlen(text) * font->width + 2,
			height = font->height + 4;
	item->width = width;
	item->height = height;
}

int gfx_information_init(struct gfx_information *info,
		enum information_type info_type, uint8_t info_data, uint8_t max_length, uint8_t x, uint8_t y, uint8_t font_id)
{
	info->info_type = info_type;
	info->info_data = info_data;
	info->text.is_progmem = false;
	info->text.max_text_size = max_length;
	gfx_item_init(&info->postion, x, y, 0, 0);
	info->text.font = fonts[font_id];
	return 0;
}

void update_information_present(struct gfx_information *info)
{
	switch(info->info_type){
	case SHOW_SERIAL_NUMBER:
	case SHOW_PART_NUMBER:
	case SHOW_HDD_SIZE:
	case SHOW_MEMORY_SIZE:
			break;
	default:
		information_present = info;
		break;
	}
}

void print_data_P(char *text, struct gfx_font *font, uint8_t x, uint8_t y)
{
	draw_string_in_buffer_P(text, x, y, font);

}
void print_data(char *text, struct gfx_font *font, uint8_t x, uint8_t y)
{
	draw_string_in_buffer(text, x, y, font);
}

void gfx_information_draw(struct gfx_information *info)
{
	char *text_to_draw = malloc(info->text.max_text_size);
	if (text_to_draw == NULL)
		return ;
	update_information_present(info);
	update_data_by_type(info->info_type, text_to_draw, info->info_data);
	print_data(text_to_draw, info->text.font, info->postion.x, info->postion.y);
	free(text_to_draw);
}

void gfx_label_draw(struct gfx_label *label)
{
	if (label->text.textP != NULL)
		print_data_P(label->text.textP, label->text.font, label->postion.x, label->postion.y);
	else
		print_data(label->text.text, label->text.font, label->postion.x, label->postion.y);
}

int gfx_image_init(struct gfx_image *image, gfx_mono_color_t PROGMEM_T *bitmap_progmem,
		uint8_t height, uint8_t width, uint8_t x, uint8_t y)
{
	struct gfx_mono_bitmap *bitmap = malloc(sizeof(struct gfx_mono_bitmap));
	if (bitmap == NULL){
		uart_send_string("bitmap failed\n\r");
		return -1;
	}
	bitmap->width = width;
	bitmap->height = height;
	bitmap->data.progmem = bitmap_progmem;
	bitmap->type = GFX_MONO_BITMAP_SECTION;
	image->bitmap = bitmap;
	gfx_item_init(&image->postion, x, y, (image->bitmap->width + 2),
			(image->bitmap->height + 2));
	return 0;
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

int set_images(struct gfx_frame *frame, struct cnf_image_node *cnf_image_pgmem)
{
	struct gfx_image_node *frame_image_last = 0;
	while (cnf_image_pgmem != 0){
		struct cnf_image_node cnf_image_node;
		struct gfx_image_node *gfx_image_node = malloc(sizeof(struct gfx_image_node));
		if (gfx_image_node == NULL){
			uart_send_string("gfx_image_node failed\n\r");
			return -1;
		}
		memcpy_P(&cnf_image_node, cnf_image_pgmem, sizeof(struct cnf_image_node));
		if (gfx_image_init(&gfx_image_node->image, cnf_image_node.image.bitmap_progmem, cnf_image_node.image.height,
				cnf_image_node.image.width, cnf_image_node.image.x, cnf_image_node.image.y) != 0) {
			uart_send_string("gfx_image_node init failed\n\r");
			return -1;
		}
		gfx_image_node->next = 0;
		if (frame->image_head == 0)
			frame->image_head = gfx_image_node;
		else
			frame_image_last->next = gfx_image_node;
		frame_image_last = gfx_image_node;
		cnf_image_pgmem = cnf_image_node.next;
	}
	return 0;
}

int set_labels(struct gfx_frame *frame, struct cnf_label_node *cnf_label_pgmem)
{
	struct gfx_label_node *frame_label_last = 0;
	while (cnf_label_pgmem != 0){
		struct cnf_label_node cnf_label_node;
		struct gfx_label_node *gfx_label_node = malloc(sizeof(struct gfx_label_node));
		if (gfx_label_node == NULL) {
			uart_send_string("label node fail \n\r");
			return -1;
		}
		memcpy_P(&cnf_label_node, cnf_label_pgmem, sizeof(struct cnf_label_node));
		gfx_label_init(&gfx_label_node->label, cnf_label_node.label.text, cnf_label_node.label.x, cnf_label_node.label.y, cnf_label_node.font_id);
		gfx_label_node->next = 0;
		if (frame->label_head == 0)
			frame->label_head = gfx_label_node;
		else
			frame_label_last->next = gfx_label_node;
		frame_label_last = gfx_label_node;
		cnf_label_pgmem = cnf_label_node.next;
	}
	return 0;
}

int set_infos(struct gfx_frame *frame,	struct cnf_info_node *cnf_info_pgmem)
{
	struct gfx_information_node *frame_information_last = 0;
	while (cnf_info_pgmem != 0){
		struct cnf_info_node cnf_info_node;
		struct gfx_information_node *gfx_information_node = malloc(sizeof(struct gfx_information_node));
		if (gfx_information_node == NULL) {
			uart_send_string("information_node fail\n\r");
			return -1;
		}

		memcpy_P(&cnf_info_node, cnf_info_pgmem, sizeof(struct cnf_info_node));
		if (gfx_information_init(&gfx_information_node->information, cnf_info_node.info.info_type, cnf_info_node.info.information,
						cnf_info_node.info.max_length, cnf_info_node.info.x, cnf_info_node.info.y, cnf_info_node.font_id) != 0) {
			uart_send_string("information init fail\n\r");
			return -1;
		}

		gfx_information_node->next = 0;
		if (frame->information_head == 0)
			frame->information_head = gfx_information_node;
		else
			frame_information_last->next = gfx_information_node;
		frame_information_last = gfx_information_node;
		cnf_info_pgmem = cnf_info_node.next;
	}
	return 0;
}

int gfx_frame_init(struct gfx_frame *frame, struct cnf_frame *cnf_frame_pgmem)
{
	struct cnf_frame cnf_frame;
	memcpy_P(&cnf_frame, cnf_frame_pgmem, sizeof(cnf_frame));
	init_frame(frame);
	return ((set_images(frame, cnf_frame.images_head) != 0) | (set_labels(frame, cnf_frame.labels_head) != 0) | (set_infos(frame, cnf_frame.infos_head) != 0));
}

void gfx_labels_draw(struct gfx_label_node *curr_label_node)
{
	while (curr_label_node != 0){
		gfx_label_draw(&curr_label_node->label);
		curr_label_node = curr_label_node->next;
	}
}

void gfx_infos_draw(struct gfx_information_node *curr_info_node)
{
	while (curr_info_node != 0){
		gfx_information_draw(&curr_info_node->information);
		curr_info_node = curr_info_node->next;
	}
}

void gfx_images_draw(struct gfx_image_node *curr_image_node)
{
	while (curr_image_node != 0){
		gfx_image_draw(&curr_image_node->image);
		curr_image_node = curr_image_node->next;
	}
}

void draw_graphic_signs(uint8_t index, uint8_t max_index)
{
	gfx_mono_draw_filled_rect(left_sign_image.postion.x, left_sign_image.postion.y, left_sign_image.bitmap->width, left_sign_image.bitmap->height, GFX_PIXEL_CLR);
	gfx_mono_draw_filled_rect(right_sign_image.postion.x, right_sign_image.postion.y, right_sign_image.bitmap->width, right_sign_image.bitmap->height, GFX_PIXEL_CLR);
	if (index == 0) {
		gfx_mono_put_bitmap(right_sign_image.bitmap, right_sign_image.postion.x, right_sign_image.postion.y);
	} else if (index == max_index) {
		gfx_mono_put_bitmap(left_sign_image.bitmap, left_sign_image.postion.x, left_sign_image.postion.y);
	} else {
		gfx_mono_put_bitmap(right_sign_image.bitmap, right_sign_image.postion.x, right_sign_image.postion.y);
		gfx_mono_put_bitmap(left_sign_image.bitmap, left_sign_image.postion.x, left_sign_image.postion.y);
	}
}

void insert_graphic_signs(struct gfx_frame *frame)
{
	if (frame->information_head != NULL) {
		switch (frame->information_head->information.info_type) {
		case SET_BRIGHTNESS:
			draw_graphic_signs(get_brightness_level(), MAX_BRIGHTNESS_LEVEL);
			return ;
		case SET_SCREEN_SAVER_ENABLE:
			draw_graphic_signs(computer_data.details.screen_saver_visible, 1);
			return ;
		case SET_SCREEN_SAVER_TIME:
			if (computer_data.details.screen_saver_visible == 1)
				draw_graphic_signs((computer_data.details.screen_saver_update_time / 2) - 1, 4);
			return ;
		case SET_SCREEN_SAVER_TYPE:
			if (computer_data.details.screen_saver_visible == 1)
				draw_graphic_signs(computer_data.details.screen_saver_type, SCREEN_SAVER_TYPE_SIZE - 1);
			return ;
		default:
			break;
		}
	}
	draw_graphic_signs((present_menu->menu)->current_selection, (present_menu->menu)->num_elements - 2);
}


void gfx_frame_draw(struct gfx_frame *frame, bool redraw)
{
	if (frame != 0){
		frame_present = frame;
		gfx_infos_draw(frame->information_head);
		if (!redraw){
			gfx_labels_draw(frame->label_head);
			gfx_images_draw(frame->image_head);
			gfx_mono_generic_draw_horizontal_line(0, 54, GFX_MONO_LCD_WIDTH, GFX_PIXEL_SET);
		}
		insert_graphic_signs(frame);
		gfx_mono_ssd1306_put_framebuffer();
	}
}
