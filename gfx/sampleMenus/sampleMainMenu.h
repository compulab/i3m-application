
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


/* Initialize Splash */
struct gfx_mono_bitmap compulab_logo = {
	.type = GFX_MONO_BITMAP_RAM,
	.width = 128,
	.height = 32,
	.data.pixmap = compulab_new
};


struct gfx_mono_bitmap voltage = {
	.type = GFX_MONO_BITMAP_RAM,
	.width = 28,
	.height = 35,
	.data.pixmap = voltage_icon
};


struct gfx_mono_bitmap avr_microcontroller = {
	.type = GFX_MONO_BITMAP_RAM,
	.width = avr_icon_width,
	.height = avr_icon_height,
	.data.pixmap = avr_icon
};

/* Initialize Mono Menu */
struct gfx_mono_menu testMenu = {
	.title= main_menu_title,
	.strings = main_menu_strings,
	.num_elements = 6,
	.current_selection = 3
};

/* Initialize Action Menu */
struct gfx_action_menu_t __attribute__((section ("configSec"))) mainMenu  = {
		.menu = &testMenu,
		.visible = false,
		.actions[0].type = ACTION_TYPE_SHOW_SPLASH,
		.actions[0].splashData = &compulab_logo,
		.actions[1].type = ACTION_TYPE_SHOW_DATA_FROM_FUNC,
		.actions[1].data.title =main_menu_title2,
		.actions[1].info_type = SHOW_POWER_STATE,
		.actions[2].type = ACTION_TYPE_SHOW_DATA,
		.actions[2].data.title = main_menu_title3,
		.actions[2].data.text = "Test Data",
		.actions[3].type = ACTION_TYPE_SHOW_MENU,
		.actions[3].subMenu = &subMenu,
		.actions[4].type = ACTION_TYPE_NONE,
		.actions[5].type = ACTION_TYPE_SHOW_DATA_FROM_FUNC,
		.actions[5].data.title =main_menu_title6,
		.actions[5].info_type = SHOW_VOLTAGE
};


#endif /* GFX_SAMPLEMENUS_SAMPLEMAINMENU_H_ */
