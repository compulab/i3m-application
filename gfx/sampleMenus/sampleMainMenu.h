
#ifndef GFX_SAMPLEMENUS_SAMPLEMAINMENU_H_
#define GFX_SAMPLEMENUS_SAMPLEMAINMENU_H_

#include "../action_menu/gfx_action_menu.h"
#include "../logos.h"
#include "../sampleMenus/sampleSubMenu.h"

/*
 * 		Building Menu:
 *
 * 		1. Declare Menu Strings.
 * 		2. Declare Objects Titles.
 * 		3. Orgnize Menu Strings.
 * 		4. Initialize Splash.
 * 		5. Initialize Mono Menu.
 * 		6. Initialize Action Menu.
 */

/* Menu Strings  */
PROGMEM_DECLARE(char const, main_menu_title[]) = "Menu Test Title";
PROGMEM_DECLARE(char const, main_menu_1[]) = "Show Compulab Logo";
PROGMEM_DECLARE(char const, main_menu_2[]) = "Show Computer State";
PROGMEM_DECLARE(char const, main_menu_3[]) = "Show Data";
PROGMEM_DECLARE(char const, main_menu_4[]) = "Go To Sub Menu";
PROGMEM_DECLARE(char const, main_menu_5[]) = "Do Nothing";
PROGMEM_DECLARE(char const, main_menu_6[]) = "Show Voltage";

/* Objects Titles */
PROGMEM_DECLARE(char const, main_menu_title2[]) = "Computer Power State";
PROGMEM_DECLARE(char const, main_menu_title3[]) = "Test Title";
PROGMEM_DECLARE(char const, main_menu_title6[]) = "Voltage";

/* Organize Menu Strings */
PROGMEM_STRING_T main_menu_strings[] = {
	main_menu_1,
	main_menu_2,
	main_menu_3,
	main_menu_4,
	main_menu_5,
	main_menu_6,
};



struct gfx_mono_bitmap  v_logo = {

	.type = GFX_MONO_BITMAP_RAM,
	.width = v_width,
	.height = v_height,
	.data.pixmap = v_bits
};



/* Initialize Mono Menu */
struct gfx_mono_menu  __attribute__((section (".configData"))) testMenu = {
//struct gfx_mono_menu  testMenu = {
	.title= main_menu_title,
	.strings = main_menu_strings,
	.num_elements = 6,
	.current_selection = 0
};

/* Initialize Action Menu */
struct gfx_action_menu_t  __attribute__((section (".configData"))) mainMenu  = {
//struct gfx_action_menu_t   mainMenu  = {
		.menu = &testMenu,
		.visible = false,
		.actions[0].type = ACTION_TYPE_SHOW_SPLASH,
		.actions[0].splashData.height = 32,
		.actions[0].splashData.width = 128,
		.actions[0].splashData.type = GFX_MONO_BITMAP_PROGMEM,
		.actions[0].splashData.data.progmem = &compulab_new,
		.actions[1].type = ACTION_TYPE_SHOW_DATA_FROM_FUNC,
		.actions[1].data.title =main_menu_title2,
		.actions[1].info_type = SHOW_POWER_STATE,
		.actions[2].type = ACTION_TYPE_SHOW_DATA,
		.actions[2].data.title = main_menu_title3,
		.actions[2].data.text = "Test Data",
		.actions[3].type = ACTION_TYPE_SHOW_MENU,
		.actions[3].menuId = SAMPLE_SUB_MENU,
		.actions[4].type = ACTION_TYPE_NONE,
		.actions[5].type = ACTION_TYPE_SHOW_DATA_FROM_FUNC,
		.actions[5].data.title =main_menu_title6,
		.actions[5].info_type = SHOW_VOLTAGE
};



#endif /* GFX_SAMPLEMENUS_SAMPLEMAINMENU_H_ */
