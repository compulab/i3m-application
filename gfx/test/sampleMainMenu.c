/*
 * sampleMenu.c
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */
#include "sampleMainMenu.h"
#include "sampleMenuStruct.h"
#include "../../power-state.h"

action_menu mainMenu = {.isInitialized = false};

/* Menu Strings  */
PROGMEM_DECLARE(char const, main_menu_title[]) = "Menu Test Title";
PROGMEM_DECLARE(char const, main_menu_1[]) = "Show Compulab Logo";
PROGMEM_DECLARE(char const, main_menu_2[]) = "Show Computer State";
PROGMEM_DECLARE(char const, main_menu_3[]) = "Show Data";
PROGMEM_DECLARE(char const, main_menu_4[]) = "Go To Sub Menu";
PROGMEM_DECLARE(char const, main_menu_5[]) = "Do Nothing";
PROGMEM_DECLARE(char const, main_menu_6[]) = "Show Voltage";

/* Organize Menu Strings */
PROGMEM_STRING_T main_menu_strings[] = {
	main_menu_1,
	main_menu_2,
	main_menu_3,
	main_menu_4,
	main_menu_5,
	main_menu_6,
};

/* Initialize Splash */
struct gfx_mono_bitmap compulab_logo = {
	.type = GFX_MONO_BITMAP_RAM,
	.width = 128,
	.height = 32,
	.data.pixmap = compulab_new
};

/* Initialize Menu */
struct gfx_mono_menu testMenu = {
	.title= main_menu_title,
	.strings = main_menu_strings,
	.num_elements = 6,
	.current_selection = 3
};

/* Test Indirect Data Action */
void sampleDataFunc(char ** data){
	*data = "day!";
}

void showState(char ** data){
	char * state = "";
	switch (currentPowerState){
	case POWER_ON:
		state = "Computer is on";
		break;
	case POWER_STR:
		state = "Sleep mode";
		break;
	case POWER_STD:
		state = "Hibernate mode";
		break;
	case POWER_OFF:
		state = "Computer off";
		break;
	}
	*data = state;
}

/* Initialize Item Action */
gfx_action_menu * getSampleMenu(){
	if (!mainMenu.isInitialized){
		gfx_action_menu * menu = malloc(sizeof(gfx_action_menu));
		item_action *actionArr = malloc(sizeof(item_action)*menu->menu->num_elements);
		item_splash_action_init(&actionArr[0],&compulab_logo);
		item_data_function_action_init(&actionArr[1],"",showState);
		item_data_function_action_init(&actionArr[2],"Good",sampleDataFunc);
		item_menu_action_init(&actionArr[3],getSampleSubMenu());
		item_none_action_init(&actionArr[4]);
		item_data_function_action_init(&actionArr[5],"Voltage: ",setPowerString);

		menu->menu = &testMenu;
		menu->parent = NULL;
		menu->actions = actionArr;
		mainMenu.isInitialized = true;
		mainMenu.menu = menu;
	}
	return mainMenu.menu;
}

