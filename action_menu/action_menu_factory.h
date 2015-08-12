/*
 * action_menu_factory.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#include "gfx_action_menu.h"
#include <string.h>

void reset_item_action(item_action* item);

item_data * item_data_init(char * title, char * text);

void item_none_action_init(item_action * item);

void item_menu_action_init(item_action * item, struct gfx_action_menu_t * menu);

void item_data_action_init(item_action * item, char * title, char * text);

void item_data_function_action_init(item_action * item,char * title, func_ptr func);

void item_splash_action_init(item_action * item, struct gfx_mono_bitmap * splash);
