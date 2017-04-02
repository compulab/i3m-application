/*
 * gfx_utils.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#include "gfx_utils.h"
#include "gfx_frame.h"
#include "uart/uart.h"
#include "lib/syntax.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "screens/screen_saver/screen_saver.h"
#include "screens/settings/brightness.h"

#define PAGE_ADDRESS(y) floor (y/8)

/*Sign symbols*/
PROGMEM_DECLARE(gfx_mono_color_t, right [5]) = {
		0x00, 0x7E, 0x3C, 0x18, 0x00,
};

PROGMEM_DECLARE(gfx_mono_color_t, left [5]) = {
		0x00, 0x18, 0x3C, 0x7E, 0x00,
};

PROGMEM_DECLARE(gfx_mono_color_t, plus [5]) = {
		0x18, 0x18, 0xFF, 0x18, 0x18,
};

PROGMEM_DECLARE(gfx_mono_color_t, minus [5]) = {
		0x18, 0x18, 0x18, 0x18, 0x18,
};

static struct gfx_mono_bitmap minus_bitmap = {
		.width = 5,
		.height = 8,
		.data.progmem = minus,
		.type = GFX_MONO_BITMAP_PROGMEM
};

static struct gfx_mono_bitmap plus_bitmap = {
		.width = 5,
		.height = 8,
		.data.progmem = plus,
		.type = GFX_MONO_BITMAP_PROGMEM
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
		.x = 3,
		.y = 57,
	},
	.bitmap = &left_bitmap,
};

static struct gfx_image right_sign_image = {
	.postion = {
		.x = 120,
		.y = 57,
	},
	.bitmap = &right_bitmap,
};

static struct gfx_image minus_sign_image = {
	.postion = {
		.x = 3,
		.y = 57,
	},
	.bitmap = &minus_bitmap,
};

static struct gfx_image plus_sign_image = {
	.postion = {
		.x = 120,
		.y = 57,
	},
	.bitmap = &plus_bitmap,
};

void print_horizontal_line(uint8_t x, uint8_t y, uint8_t length)
{
	gfx_mono_draw_line(x, y, x + length, y, GFX_PIXEL_SET);
}

void print_vertical_line(uint8_t x, uint8_t y, uint8_t length)
{
	gfx_mono_draw_line(x, y, x, y + length, GFX_PIXEL_SET);
}

void gfx_item_init(struct gfx_item *item, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	item->x = x;
	item->y = y;
	item->width = width;
	item->height = height;
	item->visible = true;
}

static void gfx_label_init(struct gfx_label *label, char *text,
		uint8_t x, uint8_t y, uint8_t font_id)
{
		gfx_item_init(&label ->postion, x, y, 0, 0);
		label->text.is_progmem = true;
		label->text.textP = text;
		label->text.font = get_font_by_type(font_id);
}

static void print_data_P(char *text, struct glcd_FontConfig_t *font, uint8_t x, uint8_t y)
{
	draw_string_in_buffer_P(text, x, y, font);
}

static void gfx_label_draw(struct gfx_label *label)
{
	if (label->text.textP != NULL)
		print_data_P(label->text.textP, label->text.font, label->postion.x, label->postion.y);
}

static int gfx_image_init(struct gfx_image *image, gfx_mono_color_t PROGMEM_T *bitmap_progmem,
		uint8_t height, uint8_t width, uint8_t x, uint8_t y)
{
	struct gfx_mono_bitmap *bitmap = malloc_locked(sizeof(struct gfx_mono_bitmap));
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

static void gfx_image_draw(struct gfx_image *image)
{
	gfx_mono_generic_put_bitmap(image->bitmap, image->postion.x, image->postion.y);
}

static int gfx_frame_set_images(struct gfx_frame *frame, struct cnf_image_node *cnf_image_pgmem)
{
	struct gfx_image_node *frame_image_last = 0;
	while (cnf_image_pgmem != 0){
		struct cnf_image_node cnf_image_node;
		struct gfx_image_node *gfx_image_node = malloc_locked(sizeof(struct gfx_image_node));
		if (gfx_image_node == NULL){
			uart_send_string("gfx_image_node failed\n\r");
			return -1;
		}
		memcpy_config(&cnf_image_node, cnf_image_pgmem, sizeof(struct cnf_image_node));
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

static int gfx_frame_set_labels(struct gfx_frame *frame, struct cnf_label_node *cnf_label_pgmem)
{
	struct gfx_label_node *frame_label_last = 0;
	while (cnf_label_pgmem != 0){
		struct cnf_label_node cnf_label_node;
		struct gfx_label_node *gfx_label_node = malloc_locked(sizeof(struct gfx_label_node));
		if (gfx_label_node == NULL) {
			uart_send_string("label node fail \n\r");
			return -1;
		}
		memcpy_config(&cnf_label_node, cnf_label_pgmem, sizeof(struct cnf_label_node));
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

static int gfx_frame_set_infos(struct gfx_frame *frame, struct cnf_info_node *cnf_info_pgmem)
{
	struct gfx_information_node *frame_information_last = 0;
	while (cnf_info_pgmem) {
		struct cnf_info_node cnf_info_node;
		struct gfx_information_node *gfx_information_node = malloc_locked(sizeof(struct gfx_information_node));
		if (gfx_information_node == NULL)
			return -1;

		memcpy_config(&cnf_info_node, (void *)cnf_info_pgmem, sizeof(struct cnf_info_node));
		struct cnf_info cnf_info = cnf_info_node.info;
		if (gfx_information_init(&gfx_information_node->information, &cnf_info, cnf_info_node.font_id))
			return -1;

		gfx_information_node->next = 0;
		if (frame->information_head)
			frame_information_last->next = gfx_information_node;
		else
			frame->information_head = gfx_information_node;
		frame_information_last = gfx_information_node;
		cnf_info_pgmem = cnf_info_node.next;
	}
	return 0;
}

static void gfx_labels_draw(struct gfx_label_node *list)
{
	list_foreach(struct gfx_label_node *, list, curr_label_node)
		gfx_label_draw(&curr_label_node->label);
}

static void gfx_infos_draw(struct gfx_information_node *list)
{
	list_foreach(struct gfx_information_node *, list, curr_info_node)
		curr_info_node->information.draw_data(&curr_info_node->information);
}

static void gfx_images_draw(struct gfx_image_node *list)
{
	list_foreach(struct gfx_image_node *, list, image_node)
		gfx_image_draw(&image_node->image);
}

static void insert_graphic_signs(struct gfx_frame *frame)
{
	if (frame->information_head == NULL)
		return;

	switch (frame->information_head->information.info_type) {
	case SET_BRIGHTNESS:
		set_brightness_draw_graphic_signs();
		break;
	case SET_SCREEN_SAVER_ENABLE:
		set_screen_saver_enable_draw_graphic_signs();
		break;
	case SET_SCREEN_SAVER_TIME:
		set_screen_saver_time_draw_graphic_signs();
		break;
	case SET_SCREEN_SAVER_TYPE:
		set_screen_saver_type_draw_graphic_signs();
		break;
	default:
		draw_control_signs_arrows(present_menu->menu->current_selection, 0, present_menu->menu->num_elements - 2);
		break;
	}
}

//TODO: there's a lot of code duplication with the dashboard version. Fix later.
static void gfx_frame_draw(struct gfx_frame *frame, bool redraw)
{
	if (!frame)
		return;

	update_screen_timer();
	frame_present = frame;
	if (present_menu->is_active_frame || !redraw) {
		clear_screen();
		gfx_labels_draw(frame->label_head);
		gfx_images_draw(frame->image_head);
		if (present_menu->is_active_frame)
			display_state = DISPLAY_ACTION_FRAME;
		gfx_mono_generic_draw_horizontal_line(0, SEPERATE_LINE_Y, GFX_MONO_LCD_WIDTH, GFX_PIXEL_SET);
	}

	gfx_infos_draw(frame->information_head);
	insert_graphic_signs(frame);
	gfx_mono_ssd1306_put_framebuffer();
}

//TODO: there's a lot of code duplication with the frame version. Fix later.
static void gfx_dashboard_draw(struct gfx_frame *frame, bool redraw)
{
	if (!frame)
		return;

	update_screen_timer();
	frame_present = frame;
	if (!redraw) {
		clear_screen();
		gfx_labels_draw(frame->label_head);
		gfx_images_draw(frame->image_head);
	}

	gfx_infos_draw(frame->information_head);
	gfx_mono_ssd1306_put_framebuffer();
}

static void init_frame(struct gfx_frame *frame, bool is_dashboard)
{
	frame->image_head = 0;
	frame->information_head = 0;
	frame->label_head = 0;
	frame->draw = is_dashboard ? gfx_dashboard_draw : gfx_frame_draw;
}

int gfx_frame_init(struct gfx_frame *frame, struct cnf_frame *cnf_frame_pgmem, bool is_dashboard)
{
	struct cnf_frame cnf_frame;
	memcpy_config(&cnf_frame, cnf_frame_pgmem, sizeof(cnf_frame));
	init_frame(frame, is_dashboard);
	int retval = gfx_frame_set_images(frame, cnf_frame.images_head);
	retval |= gfx_frame_set_labels(frame, cnf_frame.labels_head);
	retval |= gfx_frame_set_infos(frame, cnf_frame.infos_head);
	return retval;
}

void draw_control_signs(uint8_t selection, uint8_t min_index, uint8_t max_index,
						struct gfx_image *left_sign, struct gfx_image *right_sign)
{
	if (selection > min_index)
		gfx_mono_put_bitmap(left_sign->bitmap, left_sign->postion.x, left_sign->postion.y);

	if (selection < max_index)
		gfx_mono_put_bitmap(right_sign->bitmap, right_sign->postion.x, right_sign->postion.y);
}

void draw_control_signs_arrows(uint8_t selection, uint8_t min_index, uint8_t max_index)
{
	draw_control_signs(selection, min_index, max_index, &left_sign_image, &right_sign_image);
}

void draw_control_signs_numeric(uint8_t selection, uint8_t min_index, uint8_t max_index)
{
	draw_control_signs(selection, min_index, max_index, &minus_sign_image, &plus_sign_image);
}