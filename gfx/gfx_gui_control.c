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
struct gfx_frame *splash;

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

void gfx_gui_init(struct gfx_graphic_menu **graphic_menus, uint8_t num_elements, struct gfx_frame *logo)
{
	gfx_information_init(&custom_message_info.information, SHOW_CUSTOM_MESSAGE,
						 0, 64, 0, 0, GLCD_FONT_SYSFONT_5X7);
	gfx_frame_init(&custom_message, NULL, NULL, &custom_message_info);
	current_menu = graphic_menus[MAIN_MENU_ID];
	splash = logo;
}

void gfx_show_splash_screen(uint16_t mdelay)
{
	struct gfx_frame *save_current_frame = current_frame;

	gfx_switch_to_frame(splash);
	if (mdelay) {
		delay_ms(mdelay);
		if (!save_current_frame)
			gfx_switch_to_current_menu();
		else
			gfx_switch_to_frame(save_current_frame);
	}
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

void gfx_display_frame(struct gfx_frame *frame)
{
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
    return 0.1;
}

struct scheduler_task screen_tick_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};