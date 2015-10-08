/*
 * menu_utils.c
 *
 *  Created on: Oct 8, 2015
 *      Author: arkadi
 */
#include "menu_utils.h"
#include <stdio.h>
struct u8g_xbmp splash;

#define CONFIG_SECTION_ADDRESS 0xd000

void splash_init_u8g(struct cnf_blk config_block)
{
	splash.width = config_block.splash_width;
	splash.height = config_block.splash_height;
	splash.bitmap = config_block.splash;
}

void set_item_action(struct menu_item_node * item, struct cnf_action *cnf_action)
{
	struct cnf_action temp_cnf_action;
	struct item_action *action = malloc(sizeof(struct item_action));
	memcpy_P(&temp_cnf_action, cnf_action, sizeof(struct cnf_action));
	action->type = temp_cnf_action.type;
	switch (temp_cnf_action.type){
	case ACTION_TYPE_SHOW_MENU:
		action->menu_id = temp_cnf_action.menu_id;
		break;
	case ACTION_TYPE_SHOW_FRAME:
		action->frame = malloc(sizeof(struct gfx_frame));
		gfx_frame_init(action->frame, temp_cnf_action.frame);
		break;
	default:
		break;
	}
	item->action = action;
}

void set_menu_items(struct menu *menu, struct cnf_menu_item_node *item_node)
{
	bool is_first_item = true;
	struct cnf_menu_item_node temp_cnf_node;
	struct menu_item_node *new_item;
	while (item_node != 0){
		new_item = malloc(sizeof(struct menu_item_node));
		memcpy_P(&temp_cnf_node, item_node, sizeof(struct cnf_menu_item_node));
		new_item->text = temp_cnf_node.text;
		set_item_action(new_item, temp_cnf_node.action);
		new_item->visible = true;
		new_item->next = 0;
		if (is_first_item){
			menu->first_item = menu->curr_selected_item = menu->last_item = new_item;
			is_first_item = false;
		} else {
			menu->last_item->next = new_item;
			new_item->prev = menu->last_item;
			menu->last_item = new_item;
		}
		item_node = temp_cnf_node.next;
	}
}

uint8_t size;

void set_menu_by_id_u8g(struct menu **menu, uint8_t index)
{
	if (index < size){
		*menu = u8g_menus[index];
		(*menu)->visible = false;
		#ifdef DEBUG_MODE
			MSG_T_N("setting menu number: ", index)
		#endif
	}
}

void action_types_init_u8g()
{
	struct item_action *action;
	struct menu_item_node *item;
	for (int i=0; i < size; i++){
#ifdef DEBUG_MODE
		MSG_T_N("menu init in menu:",i)
#endif
		item = u8g_menus[i]->first_item;
		while (item != 0) {
			action = item->action;
			switch (action->type){
			case ACTION_TYPE_SHOW_MENU:
				set_menu_by_id_u8g(&(action->menu), action->menu_id);
#ifdef DEBUG_MODE
				MSG_T_N("Menu id",action->menu_id)
#endif
				break;
			default:
				break;
			}
			item = item->next;
		}
	}
}

void load_config_block_u8g()
{
	struct cnf_blk config_block;
	struct cnf_menu config_menu;
	uint8_t index;
	memcpy_P(&config_block,(void *) CONFIG_SECTION_ADDRESS, sizeof(struct cnf_blk));
	size = config_block.size;
	splash_init_u8g(config_block);
	u8g_menus = malloc(sizeof (struct u8g_menus *) * size);
	struct cnf_menu_node *cnf_menu_node = config_block.menus_head;
	struct cnf_menu_node cnf_menu;
	for (int i=0; i < size; i++) u8g_menus[i] = malloc(sizeof(struct menu));
	for (int count=0; count < size; count++){
		if (cnf_menu_node != 0){
			memcpy_P(&cnf_menu, cnf_menu_node, sizeof(struct cnf_menu_node));
			memcpy_P(&config_menu, cnf_menu.menu, sizeof(struct cnf_menu));
			index = config_menu.id;
			u8g_menus[index]->title = config_menu.title;
			u8g_menus[index]->first_item = 0;
			u8g_menus[index]->last_item = 0;
			u8g_menus[index]->curr_selected_item = 0;
			set_menu_items(u8g_menus[index], config_menu.first_item);
			cnf_menu_node = cnf_menu.next;
		} else {
			break;
		}
	}
	action_types_init_u8g();
}
