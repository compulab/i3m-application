/*
 * action_menu_factory.c
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#include "action_menu_factory.h"


item_data * item_data_init(char * title, char * text){
	item_data * item = malloc(sizeof(item_data));
	item->text = text;
	item->title = title;
	return item;
}

void reset_item_action(item_action* item){

	item->menu = NULL;
	item->data = item_data_init("","");
	item->dataFunc = NULL;
	item->splashData = NULL;
}

void item_none_action_init(item_action * item){
//	reset_item_action(item);
	item->type = ACTION_TYPE_NONE;
}

void item_menu_action_init(item_action * item, struct gfx_action_menu_t * menu){
//	reset_item_action(item);
	item->type = ACTION_TYPE_SHOW_MENU;
	item->menu = menu;
}

void item_data_action_init(item_action * item, char * title, char * text){
//	reset_item_action(item);
	item->type = ACTION_TYPE_SHOW_DATA;
	item->data = item_data_init(title,text);
}

void item_data_function_action_init(item_action * item,char * title, func_ptr func){
//	reset_item_action(item);
	item->type = ACTION_TYPE_SHOW_DATA_FROM_FUNC;
	item->data = item_data_init(title,"");
	item->dataFunc = func;
}

void item_splash_action_init(item_action * item, struct gfx_mono_bitmap * splash){
//	reset_item_action(item);
	item->type = ACTION_TYPE_SHOW_SPLASH;
	item->splashData = splash;
}
