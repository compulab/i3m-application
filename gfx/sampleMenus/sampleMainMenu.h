
#ifndef GFX_SAMPLEMENUS_SAMPLEMAINMENU_H_
#define GFX_SAMPLEMENUS_SAMPLEMAINMENU_H_

#include "../action_menu/gfx_action_menu.h"
#include "logos.h"
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
#define PAGE_SPACING 8
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


/* Initialize Mono Menu */
struct gfx_mono_menu  __attribute__((section (".configData"))) testMenu = {
//struct gfx_mono_menu  testMenu = {
	.title= main_menu_title,
	.strings = main_menu_strings,
	.num_elements = 6,
	.current_selection = 0
};

struct cnf_info __attribute__((section (".configData"))) voltageInfo = {
		.info_type = SHOW_VOLTAGE,
		.x = 44,
		.y = 46 + PAGE_SPACING,
		.border_visible = false
};

struct cnf_image __attribute__((section (".configData"))) voltageImage = {
	.bitmapProgmem = v_bits,
	.height = v_height,
	.width = v_width,
	.x = 30,
	.y = PAGE_SPACING * 2,
	.border_visible = false
};

struct cnf_label __attribute__((section (".configData"))) voltageTitle = {
	.text = "Computer Voltage",
	.x = 0,
	.y = 0,
	.border_visible = false
};

struct cnf_frame __attribute__((section (".configData"))) voltageFrame = {
		.label_size = 1,
		.information_size = 1,
		.image_size = 1,
		.labels[0] = &voltageTitle,
		.informations[0] = &voltageInfo,
		.images[0] = &voltageImage
};

struct cnf_action __attribute__((section (".configData"))) voltageAction = {
	.is_frame = true,
	.frame = &voltageFrame
};

struct cnf_action __attribute__((section (".configData"))) subMenuAction = {
	.is_frame = false,
	.menuId = SAMPLE_SUB_MENU
};

struct cnf_image __attribute__((section (".configData"))) copmulabLogo = {
		.bitmapProgmem = compulab_new,
		.height = 32,
		.width = 128,
		.x = 0,
		.y = PAGE_SPACING*2,
		.border_visible = false
};

struct cnf_frame __attribute__((section (".configData"))) compulabLogoFrame = {
		.label_size = 0,
		.information_size = 0,
		.image_size = 1,
		.images[0] = &copmulabLogo
};

struct cnf_action  __attribute__((section (".configData"))) compulabLogoAction = {
		.is_frame = true,
		.frame = &compulabLogoFrame
};

struct cnf_label __attribute__((section (".configData"))) testTitle = {
		.text = "Testing ...",
		.x = 0,
		.y = PAGE_SPACING*4,
		.border_visible = false
};

struct cnf_label __attribute__((section (".configData"))) testDataLabel = {
		.x = 0,
		.y = PAGE_SPACING*2,
		.border_visible = false,
		.text = "Test Data"
};

struct cnf_frame __attribute__((section (".configData"))) testLabelFrame = {
		.label_size = 2,
		.information_size = 0,
		.image_size = 0,
		.labels[0] = &testTitle,
		.labels[1] = &testDataLabel
};

struct cnf_action __attribute__((section (".configData"))) testLabelAction  = {
		.is_frame = true,
		.frame = &testLabelFrame
};

struct cnf_action __attribute__((section (".configData"))) showSubMenuAction = {
		.is_frame = false,
		.menuId = 1
};

struct cnf_frame __attribute__((section (".configData"))) doNothingFrame = {
		.label_size = 0,
		.information_size = 0,
		.image_size = 0
};
struct cnf_action __attribute__((section (".configData"))) doNothingAction = {
		.is_frame = true,
		.frame = &doNothingFrame
};

struct cnf_label __attribute__((section (".configData"))) powerStateTitle = {
		.text = "Computer Power State",
		.x = 0,
		.y = 0,
		.border_visible = false
};

struct cnf_info __attribute__((section (".configData"))) powerStateInfo = {
		.info_type = SHOW_POWER_STATE,
		.x = 0,
		.y = 45+PAGE_SPACING,
		.border_visible = false
};

struct cnf_frame __attribute__((section (".configData"))) powerStateFrame = {
		.label_size = 1,
		.information_size = 1,
		.image_size = 0,
		.labels[0] = & powerStateTitle,
		.informations[0] = &powerStateInfo
};

struct cnf_action __attribute__((section (".configData"))) powerStateAction = {
		.is_frame = true,
		.frame = &powerStateFrame
};

struct cnf_menu   __attribute__((section (".configData")))mainMenuCnf = {
		.menu = &testMenu,
		/* Splash screen settings */
		.actions[0] = &compulabLogoAction,
		/* Power State */
		.actions[1] = &powerStateAction,
		/* Test label action */
		.actions[2] = &testLabelAction,
//		/* Show Menu */
		.actions[3] = &showSubMenuAction,
//		/* Do Nothing */
		.actions[4] = &doNothingAction,
//		/* Computer Voltage Settings */
		.actions[5] = &voltageAction
};

#endif /* GFX_SAMPLEMENUS_SAMPLEMAINMENU_H_ */
