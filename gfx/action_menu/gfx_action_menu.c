#include "gfx_action_menu.h"

void switch_present_menu(struct gfx_action_menu *action_menu)
{
	present_menu->visible = false;
	present_menu = action_menu;
	present_menu->is_active_frame = false;
	action_menu->visible = true;
}

void update_action_visibility(struct gfx_item_action *action)
{
	if (action->type == ACTION_TYPE_SHOW_DMI_MENU) {
		action->visible = computer_data.details.direct_string != 0;
		return ;
	} else if (action->type != ACTION_TYPE_SHOW_FRAME){
		action->visible = true;
		return;
	}
	bool visible = true;
	struct gfx_information_node * info_node = action->frame->information_head;
	while (info_node != 0){
		switch(info_node->information.info_type){
		case SHOW_CPU_TEMPERTURE:
			if (!is_valid_cpu_temp(info_node->information.info_data))
				visible = false;
				break;
		case SHOW_CPU_FREQUENCY:
			if (!is_valid_cpu_fq(info_node->information.info_data))
				visible = false;
				break;
		case SHOW_AMBIENT_TEMPERATURE:
			if (!is_valid_ambient_temp())
				visible = false;
				break;
		case SHOW_GPU_TEMPERTURE:
			if (!is_valid_gpu_temp())
				visible = false;
				break;
		case SHOW_HDD_SIZE:
			if (!is_valid_hdd_size(info_node->information.info_data))
				visible = false;
				break;
		case SHOW_HDD_TEMPERTURE:
			if (!is_valid_hdd_temp(info_node->information.info_data))
				visible = false;
				break;
		case SHOW_MEMORY_SIZE:
			if (!is_valid_mem(info_node->information.info_data))
				visible = false;
				break;
		case SET_SCREEN_SAVER_TIME:
		case SET_SCREEN_SAVER_TYPE:
			visible = computer_data.details.screen_saver_visible == 1;
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

void update_actions_visibility()
{
	for (int i = 0; i < present_menu->menu->num_elements; i++){
		update_action_visibility(&present_menu->actions[i]);
	}
}

void set_present_menu(struct gfx_action_menu *action_menu)
{
	present_menu->visible = false;
	present_menu = action_menu;
	present_menu->is_active_frame = false;
	action_menu->visible = true;
	for (int i = 0; i < action_menu->menu->num_elements; i++)
		update_action_visibility(&action_menu->actions[i]);
}

void gfx_action_menu_init(struct gfx_action_menu *action_menu, bool redraw)
{
	if (redraw){
		clear_screen();
		set_present_menu(action_menu);
	}
	frame_present = 0;
	if (present_menu->is_graphic_view)
		graphic_menu_init(action_menu, redraw);
	else
		gfx_mono_menu_init(action_menu->menu, redraw, action_menu->is_progmem);
	gfx_mono_generic_draw_horizontal_line(0, 51, GFX_MONO_LCD_WIDTH, GFX_PIXEL_SET);
	gfx_mono_ssd1306_put_framebuffer();
}

void show_menu(struct gfx_action_menu *menu, bool redraw)
{
	gfx_action_menu_init(menu, redraw);
}

void show_current_menu(bool redraw)
{
	show_menu(present_menu, redraw);
}

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
	struct direct_string_item * direct_item = computer_data.details.direct_string;
	while (direct_item != 0){
		count++;
		direct_item = direct_item->next;
	}

	if (count > 0){
		cli();
		dmi_menu.menu = malloc(sizeof(struct gfx_mono_menu));
		sei();
		if (dmi_menu.menu == NULL) {
			free_dmi_menu();
			return ;
		}
		dmi_menu.menu->num_elements = count;
		dmi_menu.menu->title = "DMI STRINGS";
		dmi_menu.menu->current_selection = 0;
		direct_item = computer_data.details.direct_string;
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
		cli();
		free(dmi_menu.menu);
		sei();
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
	struct direct_string_item * direct_item = computer_data.details.direct_string;
	while (direct_item != 0){
		index++;
		direct_item = direct_item->next;
	}
	set_dmi_position(&label_node->label.postion);
	label_node->label.text.font = fonts[0];
	label_node->label.text.is_progmem = false;
	label_node->label.text.text = direct_item->type;
	label_node->next = 0;
}

void free_dmi_frame(struct gfx_frame *frame)
{
	struct gfx_label_node *curr_label = frame->label_head;
	struct gfx_label_node *next_label;
	while (curr_label) {
		next_label = curr_label->next;
		cli();
		free(curr_label);
		sei();
		curr_label = next_label;
	}
}

void free_dmi_menu()
{
	if (dmi_menu.actions != NULL) {
		for (int i = 0; i < dmi_menu.menu->num_elements -1; i++) {
			if (dmi_menu.actions[i].type == ACTION_TYPE_SHOW_FRAME) {
				if (dmi_menu.actions[i].frame != NULL) {
					free_dmi_frame(dmi_menu.actions[i].frame);
					cli();
					free(dmi_menu.actions[i].frame);
					sei();
				} else {
					break;
				}
			}
		}
	}
	cli();
	free(dmi_menu.menu );
	sei();
	is_dmi_set = false;
}

void set_dmi_frame(struct gfx_frame *frame, uint8_t index)
{
	frame->information_head = 0;
	frame->image_head = 0;
	cli();
	frame->label_head =  malloc(sizeof(struct gfx_label_node));
	sei();
	if (frame->label_head == NULL) {
		free_dmi_menu();
		return ;
	}
	set_dmi_label(frame->label_head, index);
}

void set_dmi_label_text()
{
	struct direct_string_item * direct_item = computer_data.details.direct_string;
	for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
		dmi_menu.actions[i].frame->label_head->label.text.text = direct_item->content;
		direct_item = direct_item->next;
	}
}

void set_dmi_actions()
{
	if (is_dmi_set){
		cli();
		dmi_menu.actions = malloc(sizeof(struct gfx_item_action) * dmi_menu.menu->num_elements);
		sei();
		if (dmi_menu.actions == NULL) {
			free_dmi_menu();
			return ;
		}
		for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
			dmi_menu.actions[i].type =  ACTION_TYPE_SHOW_FRAME;
			cli();
			dmi_menu.actions[i].frame = malloc(sizeof(struct gfx_frame));
			sei();
			if (dmi_menu.actions[i].frame == NULL) {
				free_dmi_menu();
				return ;
			}
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
	reset_screen_saver();
	enable_screen_saver_mode();
	if (is_screen_saver_on) {
		is_screen_saver_on = false;
		show_current_menu(true);
	} else {
		if (keycode == GFX_MONO_MENU_KEYCODE_ENTER){
			struct gfx_item_action *selected_action = &action_menu->actions[(action_menu->menu)->current_selection];
			enum action_type type = selected_action->type;
			present_menu->visible = false;
			frame_present = 0;
			clear_screen();
			switch (type){
			case ACTION_TYPE_SHOW_FRAME:
	//			tc_no_button_pressed();
				disable_screen_saver_mode();
				gfx_frame_draw(selected_action->frame, false);
				break;
			case ACTION_TYPE_SHOW_MENU:
				if (from_frame && selected_action->menu_id == MAIN_MENU_ID)
					break;
				show_menu(selected_action->menu, true);
				break;
			case ACTION_TYPE_SHOW_DMI_MENU:
//				disable_sleep_mode();
				if (computer_data.details.direct_string != 0){
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
		} else {
			if (from_frame && ((keycode == GFX_MONO_MENU_KEYCODE_DOWN && action_menu->menu->current_selection == 0) ||
						(keycode == GFX_MONO_MENU_KEYCODE_UP && action_menu->menu->current_selection == action_menu->menu->num_elements - 2)))
					return ;
			 gfx_mono_menu_process_key(action_menu->menu, keycode, action_menu->is_progmem);
			 if (from_frame)
				 gfx_action_menu_process_key(action_menu, GFX_MONO_MENU_KEYCODE_ENTER, true);
			 else
				 show_current_menu(false);
		}
	}
}
