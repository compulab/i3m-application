/*
 * gfx_frame.c
 *
 * Created: 03-Apr-17 4:00:49 PM
 *  Author: Nikita
 */

#include "gfx_frame.h"
#include "gfx_utils.h"
#include "action_menu/gfx_action_menu.h"
#include "lib/syntax.h"
#include <stdio.h>

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
	clear_screen();
	gfx_labels_draw(frame->label_head);
	gfx_images_draw(frame->image_head);
	gfx_infos_draw(frame->information_head);
}

static void gfx_frame_draw(struct gfx_frame *frame)
{
	gfx_frame_format_generic_frame(frame);
	gfx_mono_ssd1306_put_framebuffer();
}

static void gfx_functional_frame_draw(struct gfx_frame *frame)
{
	gfx_frame_format_generic_frame(frame);
	draw_standard_separator_line();
	frame->draw_controls(frame);
	gfx_mono_ssd1306_put_framebuffer();
}

static void gfx_frame_draw_control_arrows(struct gfx_frame *info)
{
	draw_control_signs_arrows(present_menu->menu->current_selection, 0, present_menu->menu->num_elements - 2);
}

static void gfx_frame_draw_control_from_info(struct gfx_frame *frame)
{
	if (frame->information_head->information.draw_controls)
		frame->information_head->information.draw_controls(&frame->information_head->information);
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

/*
 * BASIC FRAME: displays images, labels, and dynamic info.
 * Does not respond functionally to user actions. Key presses just return user to
 * current menu. Since it has no functionality, it does not display controls.
 *
 * This frame is suitable for invocation in any situation, because it has no
 * prerequisites. Thus, it can be used for splash screens, dashboards, random
 * messages to the user, as well as being invoked from a menu.
 */
void gfx_frame_init(struct gfx_frame *frame, struct gfx_image_node *image_head,
					struct gfx_label_node *label_head, struct gfx_information_node *info_head)
{
	frame->image_head = image_head;
	frame->label_head = label_head;
	frame->information_head = info_head;
	frame->draw = gfx_frame_draw;
	frame->handle_buttons = handle_buttons_back_to_menu;
	frame->draw_controls = NULL;
}

/*
 * CONTEXT FRAME: same abilities as BASIC FRAME, but exists within the context of a menu.
 * Thus, it is not suitable for spontaneous invocation (like a user message or a splash
 * screen), and must be invoked from a specific menu.
 *
 * Its context awareness means that pressing left and right scrolls display to next or
 * previous frame in the current menu. OK takes us back to the menu. Appropriate controls
 * are displayed.
 */
void gfx_context_frame_init(struct gfx_frame *frame, struct gfx_image_node *image_head,
							struct gfx_label_node *label_head, struct gfx_information_node *info_head)
{
	gfx_frame_init(frame, image_head, label_head, info_head);
	frame->draw = gfx_functional_frame_draw;
	frame->handle_buttons = handle_buttons_scroll_to_frame;
	frame->draw_controls = gfx_frame_draw_control_arrows;
}

/*
 * ACTION FRAME: similar in appearance to the CONTEXT FRAME, it differs from the CONTEXT
 * FRAME in functionality. The functionality of the CONTEXT FRAME is that pressing left or
 * right moves the view within the context of the current menu. ACTION FRAME replaces this
 * functionality with a custom function that depends on the assigned info artifact.
 *
 * The info artifact is responsible for implementing the functionality, so it should provide
 * a method for reacting to buttons. If no such method is provided, the default functionality
 * of the standard frame (back to menu) is invoked. The info artifact may also provide a
 * draw_controls method. Otherwise, no controls will be drawn.
 */
void gfx_action_frame_init(struct gfx_frame *frame, struct gfx_image_node *image_head,
						   struct gfx_label_node *label_head, struct gfx_information_node *info_head)
{
	gfx_frame_init(frame, image_head, label_head, info_head);
	frame->draw = gfx_functional_frame_draw;
	if (frame->information_head->information.handle_buttons)
		frame->handle_buttons = frame->information_head->information.handle_buttons;
	frame->draw_controls = gfx_frame_draw_control_from_info;
}
