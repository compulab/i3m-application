#include "gfx_action_menu.h"
#include "gfx/gfx_item_action.h"
#include "gfx/gfx_components/gfx_label.h"
#include "gfx/action_menu/gfx_action_menu_dmi.h"
#include "screens/screen_saver/screen_saver.h"
#include "lib/syntax.h"

struct gfx_action_menu *present_menu;

static void update_action_visibility(struct gfx_item_action *action)
{
	if (action->type == ACTION_TYPE_SHOW_DMI_MENU) {
		action->visible = computer_data.details.direct_string != 0;
		return ;
	} else if (action->type != ACTION_TYPE_SHOW_FRAME){
		action->visible = true;
		return;
	}

	if (action->frame == dashboard) {
		action->visible = true;
		return ;
	}

	bool visible = true;
	struct gfx_information_node * info_node = action->frame->information_head;
	while (info_node != 0) {
		switch(info_node->information.info_type) {
		case SHOW_CPU_TEMPERTURE:
			visible = BIT_ON(computer_data.packed.cpu_temp_set, info_node->information.info_data);
			break;
		case SHOW_CPU_FREQUENCY:
			visible = BIT_ON(computer_data.packed.cpu_freq_set, info_node->information.info_data);
			break;
		case SHOW_AMBIENT_TEMPERATURE:
			visible = computer_data.details.ambient_temp_set;
			break;
		case SHOW_GPU_TEMPERTURE:
			visible = computer_data.details.gpu_temp_set;
			break;
		case SHOW_HDD_SIZE:
			visible = BIT_ON(computer_data.packed.hdd_size_set, info_node->information.info_data);
			break;
		case SHOW_HDD_TEMPERTURE:
			visible = BIT_ON(computer_data.packed.hdd_temp_set, info_node->information.info_data);
			break;
		case SHOW_MEMORY_SIZE:
			visible = BIT_ON(computer_data.packed.mems, info_node->information.info_data);
			break;
		case SET_SCREEN_SAVER_TIME:
		case SET_SCREEN_SAVER_TYPE:
			visible = computer_data.details.screen_saver_visible;
			break;
		default:
			break;
		}
		if (visible)
			info_node = info_node->next;
		else
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

void gfx_action_menu_init(struct gfx_action_menu *action_menu)
{
	display_state = DISPLAY_MENU;
	update_screen_timer();
	clear_screen();
	set_present_menu(action_menu);
	frame_present = 0;
	graphic_menu_draw(action_menu);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	draw_standard_separator_line();
	gfx_mono_ssd1306_put_framebuffer();
}

void gfx_action_menu_move_cursor(struct gfx_action_menu *action_menu)
{
	display_state = DISPLAY_MENU;
	update_screen_timer();
	graphic_menu_deselect_item(action_menu, action_menu->menu->last_selection);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	draw_standard_separator_line();
	gfx_mono_ssd1306_put_framebuffer();
}

void show_frame(struct gfx_frame *frame)
{
	display_state = (frame == dashboard) ? DISPLAY_DASHBOARD : DISPLAY_FRAME;
	clear_screen();
	disable_screen_saver_mode();
	frame->draw(frame, false);
}

void gfx_handle_key_pressed(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	enum action_type type;
	struct gfx_item_action *selected_action;
	switch(keycode) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		selected_action = &action_menu->actions[(action_menu->menu)->current_selection];
		type = selected_action->type;
		frame_present = 0;
		clear_screen();
		switch (type) {
		case ACTION_TYPE_SHOW_FRAME:
			show_frame(selected_action->frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			if (from_frame && selected_action->menu_id == MAIN_MENU_ID)
				break;
			gfx_action_menu_init(selected_action->menu);
			break;
		case ACTION_TYPE_SHOW_DMI_MENU:
			if (!computer_data.details.direct_string) {
				gfx_action_menu_init(present_menu);
				break;
			}

			set_dmi_menu();
			if (is_dmi_set)
				gfx_action_menu_init(&dmi_menu);
			break;
		default:
			break;
		}
		break;
	case GFX_MONO_MENU_KEYCODE_BACK:
		gfx_action_menu_init(present_menu);
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
	switch (display_state) {
	case DISPLAY_DIM:
		exit_sleep_mode();
		break;
	default:
		gfx_handle_key_pressed(action_menu, keycode, from_frame);
		break;
	}
}
