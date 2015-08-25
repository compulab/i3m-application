/*
 * sampleSubMenu.h
 *
 *  Created on: Aug 10, 2015
 *      Author: arkadi
 */

#ifndef GFX_SAMPLEMENUS_SAMPLESUBMENU_H_
#define GFX_SAMPLEMENUS_SAMPLESUBMENU_H_

#include "../action_menu/gfx_action_menu.h"
#include "logos.h"

#define SAMPLE_MAIN_MENU 0
#define SAMPLE_SUB_MENU 1


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
struct gfx_mono_menu  __attribute__((section (".configData"))) subTestMenu = {
	.title= sub_menu_title,
	.strings = sub_menu_strings,
	.num_elements = 2,
	.current_selection = 1
};


cnf_gfx_label __attribute__((section (".configData"))) titleLabel = {
		.text = "Testing...",
		.x=0,
		.y=0,
		.borderVisible = false
};

cnf_gfx_label __attribute__((section (".configData"))) dataLabel = {
		.text = "1.. 2.. 3..",
		.x=0,
		.y=40,
		.borderVisible = false
};

cnf_frame __attribute__((section (".configData"))) testDataFrame = {
		.image_size = 0,
		.information_size = 0,
		.label_size = 2,
		.label[0] = &titleLabel,
		.label[1] = &dataLabel
};

cnf_action __attribute__((section (".configData"))) testDataAction = {
		.isFrame = true,
		.frame = &testDataFrame
};

cnf_frame __attribute__((section (".configData"))) doNothingSubFrame = {
		.image_size = 0,
		.information_size = 0,
		.label_size = 0
};

cnf_action __attribute__((section (".configData")))  doNothingSubAction = {
		.isFrame = true,
		.frame = &doNothingSubFrame
};

cnf_menu __attribute__((section (".configData"))) subMenuCnf = {
		.menu = &subTestMenu,
		.actions[0] = &testDataAction,
		.actions[1] = &doNothingSubAction
};

#endif /* GFX_SAMPLEMENUS_SAMPLESUBMENU_H_ */
