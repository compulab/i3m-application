/*
 * sampleSubMenu.h
 *
 *  Created on: Aug 10, 2015
 *      Author: arkadi
 */

#ifndef GFX_SAMPLEMENUS_SAMPLESUBMENU_H_
#define GFX_SAMPLEMENUS_SAMPLESUBMENU_H_

#include "../action_menu/gfx_action_menu.h"
#include "../action_menu/action_menu_factory.h"
#include "../logos.h"

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
PROGMEM_DECLARE(char const, sub_menu_title[]) = "Sub Menu Test Title";
PROGMEM_DECLARE(char const, sub_menu_1[]) = "Test data";
PROGMEM_DECLARE(char const, sub_menu_2[]) = "Test nothing";

/* Declare Objects Titles */
PROGMEM_DECLARE(char const, sub_menu_title1[]) = "Testing...";

/* Organize Menu Strings */
PROGMEM_STRING_T sub_menu_strings[] = {
	sub_menu_1,
	sub_menu_2,
};

/* Initialize Mono Menu */
struct gfx_mono_menu subTestMenu = {
	.title= sub_menu_title,
	.strings = sub_menu_strings,
	.num_elements = 2,
	.current_selection = 1
};

/* Initialize Action Menu */
struct gfx_action_menu_t subMenu = {
		.menu = &subTestMenu,
		.visible = false,
		.actions[0].type = ACTION_TYPE_SHOW_DATA,
		.actions[0].data.title = sub_menu_title1,
		.actions[0].data.text = "1.. 2.. 3..",
		.actions[1].type = ACTION_TYPE_NONE
};

#endif /* GFX_SAMPLEMENUS_SAMPLESUBMENU_H_ */
