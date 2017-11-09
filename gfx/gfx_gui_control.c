/*
 * gfx_gui.c
 *
 * Created: 25-Oct-17 7:33:58 PM
 *  Author: Nikita
 */

#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "gfx/gfx_gui_control.h"
#include "config/config_block.h"
#include "work-queue/work.h"
#include "scheduler/scheduler.h"
#include "lib/syntax.h"

#define MAIN_MENU_ID 	0

struct gfx_frame *current_frame;
struct gfx_graphic_menu *current_menu;

static struct gfx_frame custom_message;
static struct gfx_information_node custom_message_info = {
	.next = NULL
};

void gfx_display_msg(char *msg)
{
	custom_message.information_head->information.text.text = msg;
	gfx_switch_to_frame(&custom_message);
}

static bool gfx_in_menu(void)
{
	return !current_frame;
}

void gfx_gui_init(void)
{
	gfx_information_init(&custom_message_info.information, SHOW_CUSTOM_MESSAGE,
						 0, 64, 0, 0, GLCD_FONT_SYSFONT_5X7);
	gfx_frame_init(&custom_message, NULL, NULL, &custom_message_info);
	set_menu_by_id(&current_menu, MAIN_MENU_ID);
	gfx_switch_to_menu(current_menu);
}

void gfx_switch_to_current_menu(void)
{
	gfx_switch_to_menu(current_menu);
}

void gfx_redraw_current_frame(void)
{
	if (current_frame)
		current_frame->draw(current_frame);
}

void gfx_switch_to_frame(struct gfx_frame *frame)
{
	current_frame = frame;
	frame->draw(frame);
}

void gfx_switch_to_menu(struct gfx_graphic_menu *graphic_menu)
{
	current_frame = 0;
	current_menu = graphic_menu;
	graphic_menu->draw(graphic_menu);
}

void gfx_handle_button(uint8_t keycode)
{
	if (gfx_in_menu())
		current_menu->handle_button(current_menu, keycode);
	else
		current_frame->handle_buttons(current_frame, keycode);
}

static void update_screen(void *data)
{
	if (gfx_in_menu())
		current_menu->draw(current_menu);
	else
		current_frame->draw(current_frame);
}

static struct work update_screen_work = { .do_work = update_screen };

static double screen_get_recur_period(void)
{
    return 1;
}

struct scheduler_task screen_tick_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};