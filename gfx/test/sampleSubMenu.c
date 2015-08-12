/*
 * sampleSubMenu.c
 *
 *  Created on: Aug 10, 2015
 *      Author: arkadi
 */

#include "sampleSubMenu.h"

action_menu subMenu = {.isInitialized = false};

/* Menu Strings  */
PROGMEM_DECLARE(char const, sub_menu_title[]) = "Sub Menu Test Title";
PROGMEM_DECLARE(char const, sub_menu_1[]) = "Test data";
PROGMEM_DECLARE(char const, sub_menu_2[]) = "Test nothing";
PROGMEM_DECLARE(char const, sub_menu_3[]) = "Test indirect data";

/* Organize Menu Strings */
PROGMEM_STRING_T sub_menu_strings[] = {
	sub_menu_1,
	sub_menu_2,
	sub_menu_3,
};

/* Initialize Menu */
struct gfx_mono_menu subTestMenu = {
	.title= sub_menu_title,
	.strings = sub_menu_strings,
	.num_elements = 3,
	.current_selection = 2
};

/* Test Indirect Data Action */
void sampleDataFunc2(char ** data){
	*data = "day!";
}

/* Initialize Item Action */
gfx_action_menu * getSampleSubMenu(){
	if (!subMenu.isInitialized){
		gfx_action_menu * menu = malloc(sizeof(gfx_action_menu));
		item_action *actionArr = malloc(sizeof(item_action)*menu->menu->num_elements);
		item_data_action_init(&actionArr[0],"Testing...","1.. 2.. 3..");
		item_none_action_init(&actionArr[1]);
		item_data_function_action_init(&actionArr[2],"Good",sampleDataFunc2);

		menu->menu = &subTestMenu;
		menu->parent = NULL;
		menu->actions = actionArr;
		subMenu.menu = menu;
		subMenu.isInitialized = true;
	}
	return subMenu.menu;
}



