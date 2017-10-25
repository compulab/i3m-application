#include "gfx_action_menu.h"
#include "gfx_item_action.h"
#include "gfx/gfx_components/gfx_label.h"
#include "gfx/action_menu/graphic_menu_theme/graphic_menu_theme.h"
#include "work-queue/work.h"
#include "scheduler/scheduler.h"
#include "lib/syntax.h"

enum display_state {
	DISPLAY_MENU,
	DISPLAY_FRAME,
	DISPLAY_ACTION_FRAME,
	DISPLAY_LOGO,
	DISPLAY_DASHBOARD,
	DISPLAY_WAIT_FOR_USER_ACK,
	DISPLAY_CLOCK,
};

#define MAIN_MENU_ID 	0

enum display_state display_state;

struct gfx_frame *frame_present;
struct gfx_action_menu *present_menu;

void gfx_action_menu_init(void)
{
	set_menu_by_id(&present_menu, 0);
	present_menu->draw(present_menu);
}

static void update_action_visibility(struct gfx_item_action *action)
{
	if (action->type == ACTION_TYPE_SHOW_DMI_MENU) {
		action->visible = computer_data.details.direct_string != 0;
		return;
	} else if (action->type != ACTION_TYPE_SHOW_FRAME) {
		action->visible = true;
		return;
	}

	bool visible = true;
	list_foreach(struct gfx_information_node *, action->frame->information_head, info_node) {
		visible = info_node->information.is_valid(&info_node->information);
		if (!visible)
			break;
	}

	action->visible = visible;
}

static void gfx_action_menu_move_cursor(struct gfx_action_menu *action_menu)
{
	action_menu->draw(action_menu);
	graphic_menu_deselect_item(action_menu, action_menu->menu->last_selection);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
}

void gfx_action_menu_display(struct gfx_action_menu *action_menu)
{
	frame_present = 0;
	present_menu = action_menu;
	for (int i = 0; i < action_menu->menu->num_elements; i++)
		update_action_visibility(&action_menu->actions[i]);
	clear_screen();
	graphic_menu_format(action_menu);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
	display_state = DISPLAY_MENU;
}

void gfx_redraw_current_frame(void)
{
	frame_present->draw(frame_present);
}

void switch_to_frame(struct gfx_frame *frame)
{
	frame_present = frame;
	frame->draw(frame);
	display_state = DISPLAY_FRAME;
}

void gfx_menu_handle_button(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	struct gfx_item_action *selected_action;
	switch(keycode) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		selected_action = &action_menu->actions[(action_menu->menu)->current_selection];
		frame_present = 0;
		switch (selected_action->type) {
		case ACTION_TYPE_SHOW_FRAME:
			switch_to_frame(selected_action->frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			if (from_frame && selected_action->menu_id == MAIN_MENU_ID)
				break;
			selected_action->menu->draw(selected_action->menu);
			break;
		default:
			break;
		}
		break;
	default:
		if (from_frame && ((keycode == GFX_MONO_MENU_KEYCODE_DOWN && action_menu->menu->current_selection == 0) ||
					(keycode == GFX_MONO_MENU_KEYCODE_UP && action_menu->menu->current_selection == action_menu->menu->num_elements - 2)))
			return;
		 gfx_mono_menu_process_key(action_menu->menu, keycode, action_menu->is_progmem);
		 if (from_frame)
			 gfx_menu_handle_button(action_menu, GFX_MONO_MENU_KEYCODE_ENTER, true);
		 else
			 gfx_action_menu_move_cursor(present_menu);
		 break;
	}
}

void handle_back_to_menu(void)
{
	clear_screen();
	frame_present = 0;
	present_menu->draw(present_menu);
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
	if (!frame_present) {
		gfx_menu_handle_button(present_menu, keycode, false);
		return;
	}

	if (frame_present->information_head && frame_present->information_head->information.handle_buttons)
		frame_present->information_head->information.handle_buttons(keycode);
	else
		frame_present->handle_buttons(keycode);
}

static void update_screen(void *data)
{
	if (display_state == DISPLAY_WAIT_FOR_USER_ACK)
		return;

	if (display_state == DISPLAY_MENU)
		present_menu->draw(present_menu);
	else
		frame_present->draw(frame_present);
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