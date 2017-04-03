#include "menu-handler.h"
#include "gfx_components/gfx_information.h"
#include "gfx/gfx_item_action.h"
#include "gfx_components/gfx_label.h"
#include "gfx_components/gfx_image.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "screens/screen_saver/screen_saver.h"
#include "uart/uart.h"

#define is_key_selected(var,key) (var & key) == key

struct gfx_mono_bitmap splash_bitmap;

struct gfx_frame *dashboard;

struct gfx_frame *clock;

enum key_state selected_Key;

bool left_pressed = false;
bool right_pressed = false;
bool ok_pressed = false;
uint8_t left_time = 0;
uint8_t right_time = 0;
uint8_t ok_time = 0;
uint8_t size_of_menus;

uint8_t new_fonts_size;

enum button_state ok_button = BUTTON_NOT_PRESSED;
enum button_state left_button = BUTTON_NOT_PRESSED;
enum button_state right_button = BUTTON_NOT_PRESSED;

static void free_images(struct gfx_image_node *images_head)
{
	struct gfx_image_node *curr_graphic = images_head;
	struct gfx_image_node *next_graphic;
	while (curr_graphic != NULL) {
		next_graphic = curr_graphic->next;
		if (curr_graphic->image.bitmap != NULL)
			free(curr_graphic->image.bitmap);
		free(curr_graphic);
		curr_graphic = next_graphic;
	}
}


void memcpy_config(void *dst, void *src_addr, size_t size)
{
	memcpy_PF(dst, (uint_farptr_t) (0x00010000 + (uint16_t)src_addr), size);
}

static void free_infos(struct gfx_information_node *infos_head)
{
	struct gfx_information_node *curr_info = infos_head;
	struct gfx_information_node *next_info;
	while (curr_info != NULL) {
		next_info = curr_info->next;
		if (curr_info->information.text.text != NULL)
			free(curr_info->information.text.text );
		free(curr_info);
		curr_info = next_info;
	}
}

static void free_labels(struct gfx_label_node *labels_head)
{
	struct gfx_label_node *curr_label = labels_head;
	struct gfx_label_node *next_label;
	while (curr_label != NULL) {
		next_label = curr_label->next;
		free(curr_label);
		curr_label = next_label;
	}
}

static void free_menus(void)
{
	if (fonts != NULL) {
		for (int i = 0; i < new_fonts_size; i++) {
			if (fonts[i] != NULL)
				free(fonts[i]);
			else
				break;
		}
		for (int i = 0; i < size_of_menus; i++) {
			if (action_menus[i] != NULL) {
				if (action_menus[i]->menu != NULL) {
					if (action_menus[i]->graphic_items_head != NULL) {
						free_images(action_menus[i]->graphic_items_head);
					}
					for (int j = 0; j < action_menus[i]->menu->num_elements - 1; j++) {
						if (action_menus[i]->actions[j].type == ACTION_TYPE_SHOW_FRAME) {
							struct gfx_frame *frame = action_menus[i]->actions[j].frame;
							free_images(frame->image_head);
							free_infos(frame->information_head);
							free_labels(frame->label_head);
							free(frame);
						}
					}
					free(action_menus[i]->menu);
				}
				free (action_menus[i]);
			}
		}
		free (action_menus);
		free(fonts);
	}
}

static int config_block_error(void)
{
	uart_send_string("config block error handeling\n\r");
	free_menus();
	return -1;
}

static void action_types_init(void)
{
	struct gfx_action_menu *menu;
	struct gfx_item_action *action;
	for (int i = 0; i < size_of_menus; i++) {
		menu = action_menus[i];
		for (int j = 0; j < menu->menu->num_elements; j++) {
			action = &menu->actions[j];
			switch (action->type) {
			case ACTION_TYPE_SHOW_MENU:
				set_menu_by_id(&(action->menu), action->menu_id);
				break;
			default:
				break;
			}
		}
	}
}

static int load_action(struct gfx_item_action *action, struct cnf_action config_action, struct cnf_frame * cnf_dashboard)
{
	action->type = config_action.type;
	switch(config_action.type){
	case ACTION_TYPE_SET_BIOS_STATE:
	case ACTION_TYPE_SHOW_FRAME:
#ifdef DEBUG_MODE_ACTION
		MSG("action is frame")
#endif
		if (config_action.frame == cnf_dashboard) {
			action->frame = dashboard;
			return 0;
		}

		action->frame = malloc_locked(sizeof(struct gfx_frame));
		if (action->frame == NULL) {
			uart_send_string("action frame failed\n\r");
			return -1;
		}
		if (gfx_frame_init(action->frame, config_action.frame, false) != 0) {
			uart_send_string("frame init fail\n\r");
			return -1;
		}
	break;
	case ACTION_TYPE_SHOW_MENU:
#ifdef DEBUG_MODE_ACTION
		MSG("action is menu")
#endif
		action->menu_id = config_action.menu_id;
		break;
	default:
		break;
	}
	return 0;
}

void show_logo(void)
{
	if (reset_screen_saver_req) {
		reset_screen_saver();
		reset_screen_saver_req = false;
	} else {
		switch(display_state) {
		case DISPLAY_LOGO:
		case DISPLAY_DASHBOARD:
		case DISPLAY_CLOCK:
			break;
		default:
			clear_screen();
			frame_present = 0;
			present_menu->visible = false;
			display_state = DISPLAY_LOGO;
			gfx_mono_generic_put_bitmap(&splash_bitmap, 0, 0);
			gfx_mono_ssd1306_put_framebuffer();
			break;
		}
	}
}

static int graphic_item_init(struct gfx_image *menu_image, struct cnf_image * image_node)
{
	menu_image->bitmap = malloc_locked(sizeof(struct gfx_mono_bitmap));
	if (menu_image->bitmap == NULL) {
		uart_send_string("graphic image bitmap failed\n\r");
		return -1;
	}
	menu_image->bitmap->height = image_node->height;
	menu_image->bitmap->width = image_node->width;
	menu_image->bitmap->data.progmem = image_node->bitmap_progmem;
	menu_image->bitmap->type = GFX_MONO_BITMAP_SECTION;
	return 0;
}

static void splash_init(struct cnf_blk config_block)
{
	splash_bitmap.width = config_block.splash_width;
	splash_bitmap.height = config_block.splash_height;
	splash_bitmap.data.progmem = config_block.splash;
	splash_bitmap.type = GFX_MONO_BITMAP_SECTION;
}

static int load_fonts(struct cnf_font_node *cnf_font_node)
{
	struct cnf_font_node font_node;
	struct glcd_FontConfig_t *font;
	if (fonts == NULL) {
		uart_send_string("fonts array failed\n\r");
		return -1;
	}
	while (cnf_font_node != 0) {
		memcpy_config(&font_node, cnf_font_node, sizeof(struct cnf_font_node));
		font = malloc_locked(sizeof(struct glcd_FontConfig_t));
		if (font == NULL) {
			uart_send_string("font failed\n\r");
			return -1;
		}

		font->font_table = font_node.font.font_table;
		font->width = font_node.font.width;
		font->height = font_node.font.height;
		font->start_char = font_node.font.start_char;
		font->end_char = font_node.font.end_char;
		font->table_type = font_node.font.table_type;
		glcd_add_font(font, font_node.id);
		cnf_font_node = font_node.next;
	}
	return 0;
}



static int set_graphic_view(struct gfx_action_menu *action_menu, struct cnf_image_node *cnf_graphic_item_node)
{
	struct cnf_image_node cnf_image;
	struct gfx_mono_menu *mono_menu = action_menu->menu;
	action_menu->is_graphic_view =  cnf_graphic_item_node != 0;
	if (action_menu->is_graphic_view){
		action_menu->graphic_items_head = malloc_locked(sizeof(struct gfx_image_node));
		if (action_menu->graphic_items_head == NULL) {
			uart_send_string("graphic image head failed\n\r");
			return -1;
		}
		struct gfx_image_node *image_node = action_menu->graphic_items_head;
		for (uint8_t i = 0; i < mono_menu->num_elements; i++){
			if (cnf_graphic_item_node == 0)
				break;
			memcpy_config(&cnf_image, cnf_graphic_item_node, sizeof(struct cnf_image_node));
			if (graphic_item_init(&image_node->image, &cnf_image.image) != 0) {
				uart_send_string("grpahic error\n\r");
				return config_block_error();
			}
			cnf_graphic_item_node = cnf_image.next;
			if (i < mono_menu->num_elements - 1){
				image_node->next = malloc_locked(sizeof(struct gfx_image_node));
				if (image_node->next == NULL) {
					uart_send_string("graphic image node failed\n\r");
					return -1;
				}
				image_node = image_node->next;
			} else {
				image_node->next = 0;
			}
		}
	} else {
		action_menu->graphic_items_head = 0;
	}
	return 0;

}

static int set_mono_menu(struct gfx_action_menu *action_menu, struct gfx_mono_menu *menu)
{
	struct gfx_mono_menu *mono_menu = malloc_locked(sizeof(struct gfx_mono_menu));
	if (mono_menu == NULL) {
		uart_send_string("mono menu failed\n\r");
		return -1;
	}

	memcpy_config(mono_menu, menu, sizeof(struct gfx_mono_menu));
	action_menu->is_progmem = true;
	action_menu->menu= mono_menu;
	action_menu->actions = malloc_locked(sizeof(struct gfx_item_action) * mono_menu->num_elements);
	if (action_menu->actions == NULL) {
		uart_send_string("actions set failed\n\r");
		return -1;
	}

	return 0;
}

static int set_actions(struct gfx_action_menu * menu, struct cnf_action_node *cnf_action_node, struct cnf_frame * cnf_dashboard)
{
	struct cnf_action_node action_node;
	uint8_t action_index = 0;
	while (cnf_action_node != 0){
		memcpy_config(&action_node, cnf_action_node, sizeof(struct cnf_action_node));
		if (load_action(&(menu->actions[action_index]), action_node.action, cnf_dashboard) != 0) {
			uart_send_string("load action fail\n\r");
			return -1;
		}
		action_index++;
		cnf_action_node = action_node.next;
	}
	return 0;
}


int load_config_block(void)
{
	struct cnf_blk config_block;
	struct cnf_menu config_menu;
	struct cnf_menu_node cnf_menu;
	uart_send_string("start load config\n\r");
	memcpy_config(&config_block, (void *)CONFIG_SECTION_ADDRESS, sizeof(struct cnf_blk));
	size_of_menus = config_block.menu_size;
	new_fonts_size = config_block.font_size;
	glcd_fonts_init(new_fonts_size);

	if (new_fonts_size > 0 && config_block.fonts_head && load_fonts(config_block.fonts_head))
			return config_block_error();

	if (config_block.dashboard != NULL) {
		dashboard = malloc_locked(sizeof(struct gfx_frame));
		if (dashboard == NULL)
			return config_block_error();
		gfx_frame_init(dashboard, config_block.dashboard, true);
	} else {
		dashboard = NULL;
	}

	if (config_block.clock != NULL) {
		clock = malloc_locked(sizeof(struct gfx_frame));
		if (clock == NULL)
			return config_block_error();
		gfx_frame_init(clock, config_block.clock, true);
	} else {
		clock = NULL;
	}

	splash_init(config_block);
	action_menus = malloc_locked(sizeof(struct gfx_action_menu *) * size_of_menus);
	if (action_menus == NULL) {
		uart_send_string("action menus set fail\n\r");
		return -1;
	}

	struct cnf_menu_node *cnf_menu_node = config_block.menus_head;
	for (int i=0; i < size_of_menus; i++) {
		action_menus[i] = malloc_locked(sizeof(struct gfx_action_menu));
		if (action_menus[i] == NULL) {
			uart_send_string("action menu settings fail\n\r");
			return config_block_error();
		}
	}

	for (int i=0; i < size_of_menus; i++){
		if (cnf_menu_node != 0){
			memcpy_config(&cnf_menu, cnf_menu_node, sizeof(struct cnf_menu_node));
			memcpy_config(&config_menu, cnf_menu.menu, sizeof(struct cnf_menu));
			uart_send_num(config_menu.id, 16);
			action_menus[config_menu.id]->id = config_menu.id;
			if ((set_mono_menu(action_menus[config_menu.id], config_menu.menu) != 0) |
					(set_graphic_view(action_menus[config_menu.id], config_menu.images_items_head) != 0) |
					(set_actions(action_menus[i], config_menu.actions_head, config_block.dashboard) != 0)) {
				uart_send_string("set action_menu error\n\r");
				return config_block_error();
			}
			cnf_menu_node = cnf_menu.next;
		} else {
			break;
		}
	}
	action_types_init();
	return 0;
}

void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index)
{
	if (index < size_of_menus){
		*menu = action_menus[index];
		(*menu)->visible = false;
		#ifdef DEBUG_MODE
			MSG_T_N("setting menu number: ", index)
		#endif
	}
}

static void update_button_state(bool pressed, uint8_t *time, enum button_state *state)
{
#ifndef SUPPORT_HOLD_BUTTON
	if (pressed)
		*state = BUTTON_CLICK;
	else
		*state = BUTTON_NOT_PRESSED;

#else
	if (!pressed && *time > 0){
		if (*time < 10)
			*state = BUTTON_CLICK;
		else
			*state = BUTTON_HOLD;
		*time = 0;
	} else {
		*state = BUTTON_NOT_PRESSED;
	}
#endif
}

static void update_button_pressed(bool *pressed, uint8_t *time, port_pin_t pin)
{
	bool is_high = gpio_pin_is_high(pin);
	if (!(*pressed) && is_high){
		(*time) = tc_counter;
		(*pressed) = true;
	} else if (*pressed && (!is_high )) { //|| tc_counter - (*time) > MAX_CLICK_TIME)) {
		(*time) = tc_counter - (*time);
		(*pressed) = false;
	}
}

void hadle_back_to_menu(void)
{
	clear_screen();
	frame_present = 0;
	enable_screen_saver_mode();
	gfx_action_menu_init(present_menu, true);
}


static void update_buttons_states(void)
{
	update_button_pressed(&left_pressed, &left_time, FP_LEFT_BUTTON);
	update_button_pressed(&right_pressed, &right_time,FP_RIGHT_BUTTON);
	update_button_pressed(&ok_pressed, &ok_time, FP_OK_BUTTON);

	update_button_state(left_pressed, &left_time, &left_button);
	update_button_state(right_pressed, &right_time, &right_button);
	update_button_state(ok_pressed, &ok_time, &ok_button);
}

static void handle_buttons_update(void *data)
{
	switch (ok_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
//		tc_button_pressed();
		if (present_menu->visible)
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_ENTER, !present_menu->visible);
		else
			hadle_back_to_menu();
		return ;
		break;
	default:
		break;
	}
	switch (left_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
		switch(display_state) {
		case DISPLAY_DIM:
		case DISPLAY_LOGO:
		case DISPLAY_DASHBOARD:
		case DISPLAY_CLOCK:
			handle_button_preesed_by_display_mode();
			break;
		default:
			if (frame_present) {
				frame_present->information_head->information.handle_buttons(GFX_MONO_MENU_KEYCODE_DOWN);
			} else {
				gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_DOWN, !present_menu->visible);
			}
			break;
		}
		break;
	default:
		break;
	}
	switch (right_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
		switch(display_state) {
			case DISPLAY_DIM:
			case DISPLAY_LOGO:
			case DISPLAY_CLOCK:
			case DISPLAY_DASHBOARD:
				handle_button_preesed_by_display_mode();
				break;
			default:
				if (frame_present) {
					frame_present->information_head->information.handle_buttons(GFX_MONO_MENU_KEYCODE_UP);
				} else {
					gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_UP, !present_menu->visible);
				}
			break;
		}
		break;
	default:
		break;
	}
//	tc_no_button_pressed();
}

struct work button_work = { .do_work = handle_buttons_update, .data = NULL, .next = NULL, };

void handle_button_pressed(void)
{
	update_buttons_states();
	insert_work(&button_work);
}
