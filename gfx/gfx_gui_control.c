/*
 * gfx_gui.c
 *
 * Created: 25-Oct-17 7:33:58 PM
 *  Author: Nikita
 */

#include "gfx/gfx_graphic_menu.h"
#include "gfx/gfx_item_action.h"
#include "gfx/gfx_gui_control.h"
#include "config/config_block.h"
#include "work-queue/work.h"
#include "scheduler/scheduler.h"
#include "lib/syntax.h"

enum display_state display_state;
#define MAIN_MENU_ID 	0

struct gfx_frame *current_frame;
struct gfx_graphic_menu *current_menu;

static bool gfx_in_menu(void)
{
	return !current_frame;
}

void gfx_gui_init(void)
{
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
	display_state = DISPLAY_FRAME;
	frame->draw(frame);
}

void gfx_switch_to_menu(struct gfx_graphic_menu *graphic_menu)
{
	current_frame = 0;
	current_menu = graphic_menu;
	display_state = DISPLAY_MENU;
	graphic_menu->draw(graphic_menu);
}

void gfx_display_msg(char *msg)
{
	uint16_t font_id = fonts_size > 1 ? 2 : GLCD_FONT_SYSFONT_5X7;
	display_state = DISPLAY_WAIT_FOR_USER_ACK;
	clear_screen();
	uint8_t msg_x = GFX_MONO_LCD_WIDTH / 2 - ((strlen(msg) * (get_font_by_type(font_id))->width) / 2);
	uint8_t msg_y = 20;
	draw_string_in_buffer(msg, msg_x, msg_y, get_font_by_type(font_id));
	gfx_mono_ssd1306_put_framebuffer();
}

void gfx_handle_button(uint8_t keycode)
{
	if (gfx_in_menu())
		current_menu->handle_button(current_menu, keycode);
	else
		current_frame->handle_buttons(keycode);
}

static void update_screen(void *data)
{
	if (display_state == DISPLAY_WAIT_FOR_USER_ACK)
		return;

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

struct scheduler_task screen_sec_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};