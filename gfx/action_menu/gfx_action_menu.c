#include "gfx_action_menu.h"


void gfx_action_menu_init(struct gfx_action_menu *action_menu, bool redraw)
{
	present_menu->visible = false;
	present_menu = action_menu;
	present_menu->is_active_frame = false;
	action_menu->visible = true;
	if (present_menu->is_graphic_view)
		graphic_menu_init(action_menu, redraw);
	else
		gfx_mono_menu_init(action_menu->menu, redraw, action_menu->is_progmem);
}

void show_menu(struct gfx_action_menu *menu, bool redraw)
{
	gfx_action_menu_init(menu, redraw);
}

void show_current_menu(bool redraw)
{
	gfx_action_menu_init(present_menu, redraw);
}

struct gfx_action_menu *menu;

struct gfx_action_menu dmi_menu = {.is_progmem = false };

bool is_dmi_set;

void clear_screen()
{
	for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
}

void set_dmi_mono_menu()
{
	is_dmi_set = false;
	uint8_t count = 0;
	struct direct_string_item * direct_item = computer_data.direct_string;
	while (direct_item != 0){
		count++;
		direct_item = direct_item->next;
	}
	if (count > 0){
		dmi_menu.menu = malloc(sizeof(struct gfx_mono_menu));
		dmi_menu.menu->num_elements = count;
		dmi_menu.menu->title = "DMI STRINGS";
		dmi_menu.menu->current_selection = 0;
		direct_item = computer_data.direct_string;
		for (uint8_t i = 0; i < count  && i < 5; i++){
			if (direct_item == 0)
				break;
			dmi_menu.menu->strings[i] = direct_item->type;
			direct_item = direct_item->next;
		}
		if (dmi_menu.menu->num_elements > 4){
			dmi_menu.menu->strings[5] = "Back To Main Menu";
			dmi_menu.menu->num_elements = 6;
		} else {
			dmi_menu.menu->strings[count] = "Back To Main Menu";
			dmi_menu.menu->num_elements++;
		}
		is_dmi_set = true;
	} else {
		free(dmi_menu.menu);
		dmi_menu.menu = 0;
	}
}

void set_dmi_position(struct gfx_item *pos)
{
	pos->x = 0;
	pos->y = 32;
	pos->height = 8;
	pos->width = 0;
	pos->visible = true;
}

struct direct_string_item * curr_item = 0;

void set_dmi_label(struct gfx_label_node *label_node, uint8_t index)
{
	struct direct_string_item * direct_item = computer_data.direct_string;
	while (direct_item != 0){
		index++;
		direct_item = direct_item->next;
	}
	set_dmi_position(&label_node->label.postion);
	label_node->label.text.font = fonts[0];
	label_node->label.text.is_progmem = false;
//	label_node->label.text.text = direct_item->type;
	label_node->next = 0;
}

void set_dmi_frame(struct gfx_frame *frame, uint8_t index)
{
	frame->information_head = 0;
	frame->image_head = 0;
	frame->label_head =  malloc(sizeof(struct gfx_label_node));;
	set_dmi_label(frame->label_head, index);
}

void set_dmi_label_text()
{
	struct direct_string_item * direct_item = computer_data.direct_string;
	for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
		dmi_menu.actions[i].frame->label_head->label.text.text = direct_item->content;
		direct_item = direct_item->next;
	}
}

void set_dmi_actions()
{
	if (is_dmi_set){
		dmi_menu.actions = malloc(sizeof(struct gfx_item_action) * dmi_menu.menu->num_elements);
		for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
			dmi_menu.actions[i].type =  ACTION_TYPE_SHOW_FRAME;
			dmi_menu.actions[i].frame = malloc(sizeof(struct gfx_frame));
			set_dmi_frame(dmi_menu.actions[i].frame, i);
		}
		set_dmi_label_text();
		dmi_menu.actions[dmi_menu.menu->num_elements -1].type = ACTION_TYPE_SHOW_MENU;
		dmi_menu.actions[dmi_menu.menu->num_elements -1].menu = action_menus[0];
	}
}

void set_dmi_menu()
{
	set_dmi_mono_menu();
	set_dmi_actions();
}

void gfx_action_menu_process_key(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	if (keycode == GFX_MONO_MENU_KEYCODE_ENTER){
		struct gfx_item_action selected_action = action_menu->actions[(action_menu->menu)->current_selection];
		enum action_type type = selected_action.type;
		present_menu->visible = false;
		frame_present = 0;
		clear_screen();
		switch (type){
		case ACTION_TYPE_SHOW_FRAME:
//			MSG("Show frame", 10)
			tc_no_button_pressed();
			disable_sleep_mode();
			gfx_frame_draw(selected_action.frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			if (from_frame && selected_action.menu_id == MAIN_MENU_ID)
				break;
			enable_sleep_mode();
			menu = selected_action.menu;
			show_menu(menu, true);
			break;
		case ACTION_TYPE_SHOW_DMI_MENU:
			disable_sleep_mode();
			if (computer_data.direct_string != 0){
				set_dmi_menu();
				if (is_dmi_set){
					show_menu(&dmi_menu, true);
				}
			} else {
				show_current_menu(true);
			}
			break;
		default:
			break;
		}
	} else if (keycode == GFX_MONO_MENU_KEYCODE_BACK) {
		if (!present_menu->visible)
			show_current_menu(true);
	}else {
		 gfx_mono_menu_process_key(action_menu->menu, keycode, action_menu->is_progmem);
		 if (from_frame)
			 gfx_action_menu_process_key(action_menu, GFX_MONO_MENU_KEYCODE_ENTER, true);
		 else
			 show_current_menu(false);
	}
}
