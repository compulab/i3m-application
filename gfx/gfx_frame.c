/*
 * gfx_frame.c
 *
 * Created: 03-Apr-17 4:00:49 PM
 *  Author: Nikita
 */

#include <stdio.h>
#include "gfx_frame.h"
#include "gfx_utils.h"
#include "Fp-utils.h"
#include "action_menu/gfx_action_menu.h"
#include "lib/syntax.h"

static int gfx_frame_set_images(struct gfx_frame *frame, struct cnf_image_node *cnf_image_pgmem)
{
	struct gfx_image_node *frame_image_last = 0;
	while (cnf_image_pgmem) {
		struct cnf_image_node cnf_image_node;
		struct gfx_image_node *gfx_image_node = malloc_locked(sizeof(struct gfx_image_node));
		if (gfx_image_node == NULL)
			return -1;

		memcpy_config(&cnf_image_node, cnf_image_pgmem, sizeof(struct cnf_image_node));
		gfx_image_node->image.bitmap = malloc_locked(sizeof(struct gfx_mono_bitmap));
		//TODO: Possible memory leak here fix later
		if (gfx_image_node->image.bitmap == NULL)
			return -1;

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
	return 0;
}

static int gfx_frame_set_labels(struct gfx_frame *frame, struct cnf_label_node *cnf_label_pgmem)
{
	struct gfx_label_node *frame_label_last = 0;
	while (cnf_label_pgmem) {
		struct cnf_label_node cnf_label_node;
		struct gfx_label_node *gfx_label_node = malloc_locked(sizeof(struct gfx_label_node));
		if (gfx_label_node == NULL)
			return -1;

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
		curr_label_node->label.draw(&curr_label_node->label);
}

static void gfx_infos_draw(struct gfx_information_node *list)
{
	list_foreach(struct gfx_information_node *, list, curr_info_node)
		curr_info_node->information.draw_data(&curr_info_node->information);
}

static void gfx_images_draw(struct gfx_image_node *list)
{
	list_foreach(struct gfx_image_node *, list, image_node)
		image_node->image.draw(&image_node->image);
}

static void gfx_frame_format_generic_frame(struct gfx_frame *frame)
{
	update_screen_timer();
	frame_present = frame;
	clear_screen();
	gfx_labels_draw(frame->label_head);
	gfx_images_draw(frame->image_head);
	gfx_infos_draw(frame->information_head);
}

static void gfx_frame_draw(struct gfx_frame *frame)
{
	gfx_frame_format_generic_frame(frame);
	draw_standard_separator_line();
	if (frame->information_head->information.draw_controls)
		frame->information_head->information.draw_controls(&frame->information_head->information);
	else
		frame->draw_controls(frame);
	gfx_mono_ssd1306_put_framebuffer();
}

static void gfx_dashboard_draw(struct gfx_frame *frame)
{
	gfx_frame_format_generic_frame(frame);
	gfx_mono_ssd1306_put_framebuffer();
}

static void gfx_frame_draw_control_arrows(struct gfx_frame *info)
{
	draw_control_signs_arrows(present_menu->menu->current_selection, 0, present_menu->menu->num_elements - 2);
}

static void handle_buttons_scroll_to_frame(uint8_t key)
{
	if (key == GFX_MONO_MENU_KEYCODE_ENTER) {
		handle_back_to_menu();
		return;
	}

	if ((key == GFX_MONO_MENU_KEYCODE_DOWN && present_menu->menu->current_selection == 0) ||
		(key == GFX_MONO_MENU_KEYCODE_UP && present_menu->menu->current_selection == present_menu->menu->num_elements - 2))
				return;
	 gfx_mono_menu_process_key(present_menu->menu, key, present_menu->is_progmem);
	 //Invoke "display new frame" by simulating a KECODE ENTER event
	 gfx_handle_key_pressed(present_menu, GFX_MONO_MENU_KEYCODE_ENTER, true);
}

static void handle_buttons_back_to_menu(uint8_t keycode)
{
	handle_back_to_menu();
}

int gfx_frame_init(struct gfx_frame *frame, struct cnf_frame *cnf_frame_pgmem, bool is_dashboard)
{
	struct cnf_frame cnf_frame;
	frame->image_head = 0;
	frame->information_head = 0;
	frame->label_head = 0;
	frame->draw = is_dashboard ? gfx_dashboard_draw : gfx_frame_draw;
	frame->handle_buttons = is_dashboard ? handle_buttons_back_to_menu : handle_buttons_scroll_to_frame;
	frame->draw_controls = is_dashboard ?  NULL : gfx_frame_draw_control_arrows;
	if (cnf_frame_pgmem == NULL)
		return 0;

	memcpy_config(&cnf_frame, cnf_frame_pgmem, sizeof(cnf_frame));
	int retval = gfx_frame_set_images(frame, cnf_frame.images_head);
	retval |= gfx_frame_set_labels(frame, cnf_frame.labels_head);
	retval |= gfx_frame_set_infos(frame, cnf_frame.infos_head);
	return retval;
}
