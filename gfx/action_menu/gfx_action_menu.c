#include "gfx_action_menu.h"
#include "gfx/action_menu/gfx_item_action.h"
#include "gfx/gfx_components/gfx_label.h"
#include "gfx/action_menu/gfx_action_menu_dmi.h"
#include "screen_saver/screen_saver.h"
#include "lib/syntax.h"

struct gfx_action_menu *present_menu;

static void update_action_visibility(struct gfx_item_action *action)
{
	if (action->type == ACTION_TYPE_SHOW_DMI_MENU) {
		action->visible = computer_data.details.direct_string != 0;
		return;
	} else if (action->type != ACTION_TYPE_SHOW_FRAME) {
		action->visible = true;
		return;
	}

	if (action->frame == dashboard) {
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

static void set_present_menu(struct gfx_action_menu *action_menu)
{
	present_menu = action_menu;
	for (int i = 0; i < action_menu->menu->num_elements; i++)
		update_action_visibility(&action_menu->actions[i]);
}

void gfx_action_menu_display(struct gfx_action_menu *action_menu)
{
	display_state = DISPLAY_MENU;
	update_screen_timer();
	clear_screen();
	set_present_menu(action_menu);
	frame_present = 0;
	graphic_menu_format(action_menu);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
}

void gfx_action_menu_move_cursor(struct gfx_action_menu *action_menu)
{
	display_state = DISPLAY_MENU;
	update_screen_timer();
	graphic_menu_deselect_item(action_menu, action_menu->menu->last_selection);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
}

void show_frame(struct gfx_frame *frame)
{
	display_state = (frame == dashboard) ? DISPLAY_DASHBOARD : DISPLAY_FRAME;
	clear_screen();
	disable_screen_saver_mode();
	frame->draw(frame);
}

void gfx_handle_key_pressed(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	struct gfx_item_action *selected_action;
	switch(keycode) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		selected_action = &action_menu->actions[(action_menu->menu)->current_selection];
		frame_present = 0;
		switch (selected_action->type) {
		case ACTION_TYPE_SHOW_FRAME:
			show_frame(selected_action->frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			if (from_frame && selected_action->menu_id == MAIN_MENU_ID)
				break;
			selected_action->menu->draw(selected_action->menu);
			break;
		case ACTION_TYPE_SHOW_DMI_MENU:
			if (!computer_data.details.direct_string) {
				present_menu->draw(present_menu);
				break;
			}

			set_dmi_menu();
			if (is_dmi_set)
				gfx_action_menu_display(&dmi_menu);
			break;
		default:
			break;
		}
		break;
	case GFX_MONO_MENU_KEYCODE_BACK:
		present_menu->draw(present_menu);
		break;
	default:
		if (from_frame && ((keycode == GFX_MONO_MENU_KEYCODE_DOWN && action_menu->menu->current_selection == 0) ||
					(keycode == GFX_MONO_MENU_KEYCODE_UP && action_menu->menu->current_selection == action_menu->menu->num_elements - 2)))
			return;
		 gfx_mono_menu_process_key(action_menu->menu, keycode, action_menu->is_progmem);
		 if (from_frame)
			 gfx_action_menu_process_key(action_menu, GFX_MONO_MENU_KEYCODE_ENTER, true);
		 else
			 gfx_action_menu_move_cursor(present_menu);
		 break;
	}
}

void gfx_action_menu_process_key(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	reset_screen_saver();
	enable_screen_saver_mode();
	if (display_state == DISPLAY_DIM)
		exit_sleep_mode();
	else
		gfx_handle_key_pressed(action_menu, keycode, from_frame);
}
