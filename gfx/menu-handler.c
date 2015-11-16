#include "menu-handler.h"

#define is_key_selected(var,key) (var & key) == key

struct gfx_mono_bitmap splash_bitmap;

enum key_Select selected_Key;
enum button_state left_button, right_button, ok_button;
bool left_pressed = false,
		right_pressed = false,
		ok_pressed = false;
uint8_t left_time = 0,
		right_time = 0,
		ok_time = 0;
uint8_t size_of_menus;
bool is_screen_saver_on;

//TODO: enter bootloader through SW
void enter_to_bootloader(){
    asm ("ldi r30, 0xFE\n"  /* Low byte to ZL */
            "ldi r31, 0x00\n" /* mid byte to ZH */
            "ldi r24, 0x01\n" /* high byte to EIND which lives */
            "out 0x3c, r24\n" /* at addr 0x3c in I/O space */
            "eijmp":  :: "r24", "r30", "r31");
}

void action_types_init()
{
	struct gfx_action_menu *menu;
	struct gfx_item_action *action;
	for (int i=0; i < size_of_menus; i++){
		menu = action_menus[i];
#ifdef DEBUG_MODE
		MSG_T_N("menu init in menu:",i)
#endif
		for (int j=0; j < menu->menu->num_elements; j++){
			action = &menu->actions[j];
			switch (action->type){
			case ACTION_TYPE_SHOW_MENU:
				set_menu_by_id(&(action->menu), action->menu_id);
#ifdef DEBUG_MODE
				MSG_T_N("Menu id",action->menu_id)
#endif
				break;
			default:
				break;
			}
		}
	}
}

void load_action(struct gfx_item_action *action, struct cnf_action config_action)
{
	action->type = config_action.type;
	switch(config_action.type){
	case ACTION_TYPE_SET_BIOS_STATE:
	case ACTION_TYPE_SHOW_FRAME:
#ifdef DEBUG_MODE_ACTION
		MSG("action is frame")
#endif
		action->frame = malloc (sizeof(struct gfx_frame));
		gfx_frame_init(action->frame, config_action.frame);
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
}

void show_splash()
{
	if (!is_screen_saver_on && sleep_mode_enabled) {
		is_screen_saver_on = true;
		clear_screen();
		present_menu->visible = false;
		gfx_mono_generic_put_bitmap(&splash_bitmap, 0, 0);
		gfx_mono_ssd1306_put_framebuffer();
	}
}

void graphic_item_init(struct gfx_image *menu_image, struct cnf_image * image_node)
{
	menu_image->bitmap = malloc(sizeof(struct gfx_mono_bitmap));
	menu_image->bitmap->height = image_node->height;
	menu_image->bitmap->width = image_node->width;
	menu_image->bitmap->data.progmem = image_node->bitmap_progmem;
	menu_image->bitmap->type = GFX_MONO_BITMAP_SECTION;
}

void splash_init(struct cnf_blk config_block)
{
	splash_bitmap.width = config_block.splash_width;
	splash_bitmap.height = config_block.splash_height;
	splash_bitmap.data.progmem = config_block.splash;
	splash_bitmap.type = GFX_MONO_BITMAP_SECTION;
//	show_splash();
}

void load_fonts(struct cnf_font_node *cnf_font_node, uint8_t size)
{
	struct cnf_font_node font_node;
	struct gfx_font *font;
	fonts = malloc(sizeof(struct gfx_font *) * size);
	while (cnf_font_node != 0) {
		memcpy_P(&font_node, cnf_font_node, sizeof(struct cnf_font_node));
		font = malloc(sizeof(struct gfx_font));
		font->source = font_node.font.source;
		font->width = font_node.font.width;
		font->height = font_node.font.height;
		fonts[font_node.id] = font;
		cnf_font_node = font_node.next;
	}
}



void set_graphic_view(struct gfx_action_menu *action_menu, struct cnf_image_node *cnf_graphic_item_node)
{
	struct cnf_image_node cnf_image;
	struct gfx_mono_menu *mono_menu = action_menu->menu;
	action_menu->is_graphic_view =  cnf_graphic_item_node != 0;
	if (action_menu->is_graphic_view){
		action_menu->graphic_items_head = malloc(sizeof(struct gfx_image_node));
		struct gfx_image_node *image_node = action_menu->graphic_items_head;
		for (uint8_t i = 0; i < mono_menu->num_elements; i++){
			if (cnf_graphic_item_node == 0)
				break;
			memcpy_P(&cnf_image, cnf_graphic_item_node, sizeof(struct cnf_image_node));
			graphic_item_init(&image_node->image, &cnf_image.image);
			cnf_graphic_item_node = cnf_image.next;
			if (i < mono_menu->num_elements - 1){
				image_node->next = malloc(sizeof(struct gfx_image_node));
				image_node = image_node->next;
			} else {
				image_node->next = 0;
			}
		}
	} else {
		action_menu->graphic_items_head = 0;
	}

}

void set_mono_menu(struct gfx_action_menu *action_menu, struct gfx_mono_menu *menu)
{
	struct gfx_mono_menu *mono_menu = malloc(sizeof(struct gfx_mono_menu));
	memcpy_P(mono_menu, menu, sizeof(struct gfx_mono_menu));
	action_menu->is_progmem = true;
	action_menu->menu= mono_menu;
	action_menu->actions = malloc (sizeof(struct gfx_item_action) * mono_menu->num_elements);
}

void set_actions(struct gfx_action_menu * menu, struct cnf_action_node *cnf_action_node)
{
	struct cnf_action_node action_node;
	memcpy_P(&action_node, cnf_action_node, sizeof(struct cnf_action_node));
	uint8_t action_index = 0;
	while (cnf_action_node != 0){
		memcpy_P(&action_node, cnf_action_node, sizeof(struct cnf_action_node));
		load_action(&(menu->actions[action_index]), action_node.action);
		action_index++;
		cnf_action_node = action_node.next;
	}
}

void set_visible_items(struct gfx_action_menu *action_menu)
{
	action_menu->visible_items.visible_actions = 0;
	action_menu->visible_items.visible_images = 0;
	action_menu->visible_items.visible_menu = 0;
}

void load_config_block()
{
	struct cnf_blk config_block;
	struct cnf_menu config_menu;
	struct cnf_menu_node cnf_menu;
	memcpy_P(&config_block,(void *) CONFIG_SECTION_ADDRESS, sizeof(struct cnf_blk));
	size_of_menus = config_block.size;
	if (config_block.fonts_head != 0)
		load_fonts(config_block.fonts_head, config_block.font_size);
	splash_init(config_block);
	action_menus = malloc(sizeof (struct gfx_action_menu *) * size_of_menus);
	struct cnf_menu_node *cnf_menu_node = config_block.menus_head;
	for (int i=0; i < size_of_menus; i++) 	action_menus[i] = malloc(sizeof(struct gfx_action_menu));
	for (int i=0; i < size_of_menus; i++){
		if (cnf_menu_node != 0){
			memcpy_P(&cnf_menu, cnf_menu_node, sizeof(struct cnf_menu_node));
			memcpy_P(&config_menu, cnf_menu.menu, sizeof(struct cnf_menu));
			action_menus[config_menu.id]->id = config_menu.id;
			set_mono_menu(action_menus[config_menu.id], config_menu.menu);
			set_visible_items(action_menus[config_menu.id]);
			set_graphic_view(action_menus[config_menu.id], config_menu.images_items_head);
			set_actions(action_menus[i], config_menu.actions_head);
			cnf_menu_node = cnf_menu.next;
		} else {
			break;
		}
	}
	action_types_init();
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

void update_button_state(bool pressed, uint8_t *time, enum button_state *state)
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

void update_button_pressed(bool *pressed, uint8_t *time, port_pin_t pin)
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

void hadle_back_to_menu()
{
	clear_screen();
	frame_present = 0;
//	information_present = 0;
	gfx_action_menu_init(present_menu, true);
}


void update_buttons_states()
{
	update_button_pressed(&left_pressed, &left_time, FP_LEFT_BUTTON);
	update_button_pressed(&right_pressed, &right_time,FP_RIGHT_BUTTON);
	update_button_pressed(&ok_pressed, &ok_time, FP_OK_BUTTON);

	update_button_state(left_pressed, &left_time, &left_button);
	update_button_state(right_pressed, &right_time, &right_button);
	update_button_state(ok_pressed, &ok_time, &ok_button);
}

void handle_buttons_update()
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
//		tc_button_pressed();
		if (present_menu->is_active_frame)
			frame_present->handle_buttons(GFX_MONO_MENU_KEYCODE_DOWN);
		else
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_DOWN, !present_menu->visible);
		break;
	default:
		break;
	}
	switch (right_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
//		tc_button_pressed();
		if (present_menu->is_active_frame)
			frame_present->handle_buttons(GFX_MONO_MENU_KEYCODE_UP);
		else
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_UP, !present_menu->visible);
		break;
	default:
		break;
	}
//	tc_no_button_pressed();
}

struct work button_work = { .do_work = handle_buttons_update, .data = NULL, .next = NULL, };

void handle_button_pressed()
{
	update_buttons_states();
	insert_work(&button_work);
}
