#include "gfx_action_menu.h"
#include "screen_saver/screen_saver.h"
#include "lib/syntax.h"

struct gfx_action_menu *present_menu;
struct gfx_action_menu dmi_menu = {.is_progmem = false };

bool is_dmi_set;

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
			visible = BIT_ON(computer_data.packed.hdds, info_node->information.info_data);
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
	present_menu->visible = false;
	present_menu = action_menu;
	present_menu->is_active_frame = false;
	action_menu->visible = true;
	for (int i = 0; i < action_menu->menu->num_elements; i++)
		update_action_visibility(&action_menu->actions[i]);
}

void gfx_action_menu_init(struct gfx_action_menu *action_menu, bool redraw)
{
	display_state = DISPLAY_MENU;
	update_screen_timer();
	if (redraw){
		clear_screen();
		set_present_menu(action_menu);
	}
	frame_present = 0;
	if (present_menu->is_graphic_view)
		graphic_menu_init(action_menu, redraw);
	else
		gfx_mono_menu_init(action_menu->menu, redraw, action_menu->is_progmem);
	gfx_mono_generic_draw_horizontal_line(0, SEPERATE_LINE_Y, GFX_MONO_LCD_WIDTH, GFX_PIXEL_SET);
	gfx_mono_ssd1306_put_framebuffer();
}


static void free_dmi_frame(struct gfx_frame *frame)
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

static void free_dmi_menu(void)
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

static void show_menu(struct gfx_action_menu *menu, bool redraw)
{
	gfx_action_menu_init(menu, redraw);
}

void show_current_menu(bool redraw)
{
	show_menu(present_menu, redraw);
}

void clear_screen(void)
{
	for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
}

static void set_dmi_mono_menu(void)
{
	is_dmi_set = false;
	uint8_t count = 0;
	struct direct_string_item * direct_item = computer_data.details.direct_string;
	while (direct_item != 0){
		count++;
		direct_item = direct_item->next;
	}

	if (count > 0){
		dmi_menu.menu = malloc_locked(sizeof(struct gfx_mono_menu));
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
		if (dmi_menu.menu->num_elements > 3){
			dmi_menu.menu->strings[4] = "Back To Main Menu";
			dmi_menu.menu->num_elements = 5;
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

static void set_dmi_name_position(struct gfx_item* pos)
{
	pos->x = 0;
	pos->y = 12;
	pos->height = 8;
	pos->width = 0;
	pos->visible = true;
}

static void set_dmi_content_position(struct gfx_item* pos)
{
	pos->x = 0;
	pos->y = 32;
	pos->height = 8;
	pos->width = 0;
	pos->visible = true;
}

static void set_dmi_name_label(struct gfx_label_node **label_node, struct direct_string_item * direct_item)
{
	set_dmi_name_position(&(*label_node)->label.postion);
	(*label_node)->label.text.font = get_font_by_type(GLCD_FONT_SYSFONT_5X7);
	(*label_node)->label.text.is_progmem = false;
	(*label_node)->label.text.text = direct_item->content;
}

static void set_dmi_content_label(struct gfx_label_node *label_node, struct direct_string_item * direct_item)
{
	set_dmi_content_position(&label_node->label.postion);
	label_node->label.text.font = get_font_by_type(GLCD_FONT_SYSFONT_5X7);
	label_node->label.text.is_progmem = false;
	label_node->label.text.text = direct_item->type;
}

static int set_dmi_label(struct gfx_label_node *label_node, uint8_t index)
{
	struct direct_string_item * direct_item = computer_data.details.direct_string;
	while (direct_item != 0){
		index++;
		direct_item = direct_item->next;
	}
	label_node->next = malloc_locked(sizeof(struct gfx_label_node));
	if (label_node->next == NULL)
		return -1;
	set_dmi_content_label(label_node, direct_item);
	set_dmi_name_label(&(label_node->next), direct_item);
	return 0;
}

static void set_dmi_frame(struct gfx_frame *frame, uint8_t index)
{
	frame->information_head = 0;
	frame->image_head = 0;
	frame->label_head =  malloc_locked(sizeof(struct gfx_label_node));
	if (frame->label_head == NULL || set_dmi_label(frame->label_head, index) != 0) {
		free_dmi_menu();
		return ;
	}
}

static void set_dmi_label_text(void)
{
	struct direct_string_item *direct_item = computer_data.details.direct_string;
	struct gfx_label_node *label;
	for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
		label = dmi_menu.actions[i].frame->label_head;
		label->label.text.text = direct_item->content;
		label->label.text.textP = NULL;
		label = label->next;
		if (label != NULL) {
			label->label.text.text = direct_item->type;
			label->label.text.textP = NULL;
		}
		direct_item = direct_item->next;
	}
}

static void set_dmi_actions(void)
{
	if (is_dmi_set){
		dmi_menu.actions = malloc_locked(sizeof(struct gfx_item_action) * dmi_menu.menu->num_elements);
		if (dmi_menu.actions == NULL) {
			free_dmi_menu();
			return ;
		}
		for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
			dmi_menu.actions[i].type =  ACTION_TYPE_SHOW_FRAME;
			dmi_menu.actions[i].frame = malloc_locked(sizeof(struct gfx_frame));
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

static void set_dmi_menu(void)
{
	set_dmi_mono_menu();
	set_dmi_actions();
}

void show_frame(struct gfx_frame *frame)
{
	present_menu->visible = false;
	if (frame == dashboard)
		display_state = DISPLAY_DASHBOARD;
	else
		display_state = DISPLAY_FRAME;
	clear_screen();
	disable_screen_saver_mode();
	gfx_frame_draw(frame, false);

}

static void gfx_handle_key_pressed(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	enum action_type type;
	struct gfx_item_action *selected_action;
	switch(keycode) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		selected_action = &action_menu->actions[(action_menu->menu)->current_selection];
		type = selected_action->type;
		present_menu->visible = false;
		frame_present = 0;
		clear_screen();
		switch (type){
		case ACTION_TYPE_SHOW_FRAME:
			show_frame(selected_action->frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			if (from_frame && selected_action->menu_id == MAIN_MENU_ID)
				break;
			show_menu(selected_action->menu, true);
			break;
		case ACTION_TYPE_SHOW_DMI_MENU:
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
		break;
		case GFX_MONO_MENU_KEYCODE_BACK:
			if (!present_menu->visible)
				show_current_menu(true);
			break;
		default:
			if (from_frame && ((keycode == GFX_MONO_MENU_KEYCODE_DOWN && action_menu->menu->current_selection == 0) ||
						(keycode == GFX_MONO_MENU_KEYCODE_UP && action_menu->menu->current_selection == action_menu->menu->num_elements - 2)))
					return ;
			 gfx_mono_menu_process_key(action_menu->menu, keycode, action_menu->is_progmem);
			 if (from_frame)
				 gfx_action_menu_process_key(action_menu, GFX_MONO_MENU_KEYCODE_ENTER, true);
			 else
				 show_current_menu(false);
			 break;
	}
}

void handle_button_preesed_by_display_mode()
{
	switch (display_state) {
	case DISPLAY_LOGO:
	case DISPLAY_DASHBOARD:
	case DISPLAY_CLOCK:
		reset_screen_saver();
		hadle_back_to_menu();
//		return show_current_menu(true);
		break;

	case DISPLAY_DIM:
		exit_sleep_mode();
		break;
	default:
		break;
	}
}

void gfx_action_menu_process_key(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	reset_screen_saver();
	enable_screen_saver_mode();
	switch (display_state) {
	case DISPLAY_LOGO:
	case DISPLAY_CLOCK:
	case DISPLAY_DIM:
		handle_button_preesed_by_display_mode();
		break;

	default:
		gfx_handle_key_pressed(action_menu, keycode, from_frame);
		break;
	}
}
