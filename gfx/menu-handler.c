#include "menu-handler.h"
#include "../timer/tc.h"

#define is_key_selected(var,key) (var & key) == key
enum key_Select selected_Key;

enum button_state left_button, right_button, ok_button;

bool left_pressed = false,
		right_pressed = false,
		ok_pressed = false;

uint8_t left_time = 0,
		right_time = 0,
		ok_time = 0;

//TODO: enter bootloader threw SW
void enter_bootloader_mode()
{
//	udc_detach();
//	udc_stop();
//	asm ("ldi r30, 0xFE\n"  /* Low byte to ZL */
//		  "ldi r31, 0x40\n" /* mid byte to ZH */
//		  "ldi r24, 0x00\n" /* high byte to EIND which lives */
//		  "out 0x3c, r24\n" /* at addr 0x3c in I/O space Extended Indirect register */
//		  "eijmp":  :: "r24", "r30", "r31");
}


void menu_handler()
{
	getPressedKey(&selected_Key);
	if (present_menu->visible){
		if (selected_Key == KEY_NONE) return;
		else if (selected_Key == KEY_HOLD)
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_BACK);
		else if (is_key_selected(selected_Key, KEY_SELECT))
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_ENTER);
		else if (is_key_selected(selected_Key, KEY_LEFT))
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_UP);
		else
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_DOWN);
	} else {
		if (is_key_selected(selected_Key, KEY_HOLD))
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_BACK);
	}
}

void delay()
{
	delay_ms(100);
}

bool is_bootloader_state()
{
	delay();
//	return ioport_get_value(FP_RIGHT_BUTTON) && ioport_get_value(FP_LEFT_BUTTON);
	return false;
}

void right_button_pressed()
{
	if (is_bootloader_state()) enter_bootloader_mode();
	else
		gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_DOWN);
}

void handle_bBack()
{
	if (!present_menu->visible)
		gfx_action_menu_init(present_menu);
}

void select_button_pressed()
{
	delay();
	if (!ioport_get_value(FP_OK_BUTTON)) handle_bBack();
	else
		gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_ENTER);
}

void left_button_pressed()
{
	if (is_bootloader_state()) enter_bootloader_mode();
	else
		gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_UP);
}

uint8_t size;

void action_types_init()
{
	struct gfx_action_menu *menu;
	struct gfx_item_action *action;
	for (int i=0; i < size; i++){
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

void clear_counter()
{
	tc_counter = 0;
}

struct gfx_mono_bitmap splash_bitmap;

void clear_screen_framebuffer()
{
	for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
//	gfx_mono_ssd1306_put_framebuffer();
}

void show_splash()
{
	present_menu->visible = false;
	clear_screen_framebuffer();
	gfx_mono_generic_put_bitmap(&splash_bitmap, 0, 15);
}

void splash_init(struct cnf_blk config_block)
{
	splash_bitmap.width = config_block.splash_width;
	splash_bitmap.height = config_block.splash_height;
	splash_bitmap.data.progmem = config_block.splash;
	splash_bitmap.type = GFX_MONO_BITMAP_SECTION;
	show_splash();
}

void load_config_block()
{
	struct cnf_blk config_block;
	struct cnf_menu config_menu;
	struct gfx_mono_menu *mono_menu;
	memcpy_P(&config_block,(void *) CONFIG_SECTION_ADDRESS, sizeof(struct cnf_blk));
	size = config_block.size;
	splash_init(config_block);
	action_menus = malloc(sizeof (struct gfx_action_menu *) * size);
	struct cnf_menu_node *cnf_menu_node = config_block.menus_head;
	struct cnf_menu_node cnf_menu;
	for (int i=0; i < size; i++) 	action_menus[i] = malloc(sizeof(struct gfx_action_menu));
	for (int i=0; i < size; i++){
		if (cnf_menu_node != 0){
			memcpy_P(&cnf_menu, cnf_menu_node, sizeof(struct cnf_menu_node));
			memcpy_P(&config_menu, cnf_menu.menu, sizeof(struct cnf_menu));
			mono_menu = malloc(sizeof(struct gfx_mono_menu));
			MSG_dec(config_menu.id)
			delay_s(0.4);
			memcpy_P(mono_menu, config_menu.menu, sizeof(struct gfx_mono_menu));
			action_menus[config_menu.id]->menu= mono_menu;
			action_menus[config_menu.id]->actions = malloc (sizeof(struct gfx_item_action) * mono_menu->num_elements);
			struct cnf_action_node *cnf_action_node = config_menu.actions_head;
			struct cnf_action_node action_node;
			memcpy_P(&action_node, cnf_action_node, sizeof(struct cnf_action_node));
			uint8_t action_index = 0;
			while (cnf_action_node != 0){
				memcpy_P(&action_node, cnf_action_node, sizeof(struct cnf_action_node));
				load_action(&(action_menus[i]->actions[action_index]), action_node.action);
				action_index++;
				cnf_action_node = action_node.next;
			}
			cnf_menu_node = cnf_menu.next;
		} else {
			break;
		}
	}
	action_types_init();
}

void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index)
{
	if (index < size){
		*menu = action_menus[index];
		(*menu)->visible = false;
		#ifdef DEBUG_MODE
			MSG_T_N("setting menu number: ", index)
		#endif
	}
}

void update_button_state(bool pressed, uint8_t *time, enum button_state *state)
{
	if (!pressed && *time > 0){
		if (*time < 10)
			*state = BUTTON_CLICK;
		else
			*state = BUTTON_HOLD;
		*time = 0;
	} else {
		*state = BUTTON_NOT_PRESSED;
	}
}

void update_button_pressed(bool *pressed, uint8_t *time, port_pin_t pin)
{
	bool is_high = gpio_pin_is_high(pin);
	if (!(*pressed) && is_high){
		(*time) = tc_counter;
		(*pressed) = true;
	} else if (*pressed && !is_high){
		(*time) = tc_counter - (*time);
		(*pressed) = false;
	}
}

void handle_button_pressed()
{
	update_button_pressed(&left_pressed, &left_time, FP_LEFT_BUTTON);
	update_button_pressed(&right_pressed, &right_time,FP_RIGHT_BUTTON);
	update_button_pressed(&ok_pressed, &ok_time, FP_OK_BUTTON);

	update_button_state(left_pressed, &left_time, &left_button);
	update_button_state(right_pressed, &right_time, &right_button);
	update_button_state(ok_pressed, &ok_time, &ok_button);
	switch (ok_button){
	case BUTTON_HOLD:
		handle_bBack();
		clear_counter();
		return;
		break;
	case BUTTON_CLICK:
		clear_counter();
		if (present_menu->visible)
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_ENTER);
		else
			gfx_action_menu_init(present_menu);
		return;
		break;
	default:
		break;
	}
	switch (left_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
		clear_counter();
		if (present_menu->visible)
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_UP);
		else
			gfx_action_menu_init(present_menu);
		return;
		break;
	default:
		break;
	}
	switch (right_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
		clear_counter();
		if (present_menu->visible)
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_DOWN);
		else
			gfx_action_menu_init(present_menu);
		return;
		break;
	default:
		break;
	}
}
