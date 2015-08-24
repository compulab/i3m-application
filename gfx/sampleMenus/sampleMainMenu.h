
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


//
//struct gfx_mono_bitmap  v_logo = {
//	.type = GFX_MONO_BITMAP_PROGMEM,
//	.width = v_width,
//	.height = v_height,
//	.data.progmem = v_bits
//};



/* Initialize Mono Menu */
struct gfx_mono_menu  __attribute__((section (".configData"))) testMenu = {
//struct gfx_mono_menu  testMenu = {
	.title= main_menu_title,
	.strings = main_menu_strings,
	.num_elements = 6,
	.current_selection = 0
};

cnf_gfx_information_label __attribute__((section (".configData"))) voltageInfo = {
		.info_type = SHOW_VOLTAGE,
		.x = 44,
		.y = 46 + PAGE_SPACING,
		.borderVisible = false
};

cnf_gfx_image __attribute__((section (".configData"))) voltageImage = {
	.bitmapProgmem = v_bits,
	.height = v_height,
	.width = v_width,
	.x = 30,
	.y = PAGE_SPACING * 2,
	.borderVisible = false
};

cnf_gfx_label __attribute__((section (".configData"))) voltageTitle = {
	.text = "Computer Voltage",
	.x = 0,
	.y = 0,
	.borderVisible = false
};

cnf_frame __attribute__((section (".configData"))) voltageFrame = {
		.label_size = 1,
		.information_size = 1,
		.image_size = 1,
		.label[0] = &voltageTitle,
		.information_label[0] = &voltageInfo,
		.image[0] = &voltageImage
};

cnf_action __attribute__((section (".configData"))) voltageAction = {
	.isFrame = true,
	.frame = &voltageFrame
};

cnf_action __attribute__((section (".configData"))) subMenuAction = {
	.isFrame = false,
	.menuId = SAMPLE_SUB_MENU
};

cnf_gfx_image __attribute__((section (".configData"))) copmulabLogo = {
		.bitmapProgmem = compulab_new,
		.height = 32,
		.width = 128,
		.x = 30,
		.y = PAGE_SPACING*2,
		.borderVisible = false
};

cnf_frame __attribute__((section (".configData"))) compulabLogoFrame = {
		.label_size = 0,
		.information_size = 0,
		.image_size = 1,
		.image[0] = &copmulabLogo
};

cnf_action  __attribute__((section (".configData"))) compulabLogoAction = {
		.isFrame = true,
		.frame = &compulabLogoFrame
};

cnf_gfx_label __attribute__((section (".configData"))) testTitle = {
		.text = "Testing ...",
		.x = 0,
		.y = PAGE_SPACING*4,
		.borderVisible = false
};

cnf_gfx_label __attribute__((section (".configData"))) testDataLabel = {
		.x = 0,
		.y = PAGE_SPACING*2,
		.borderVisible = false,
		.text = "Test Data"
};

cnf_frame __attribute__((section (".configData"))) testLabelFrame = {
		.label_size = 2,
		.information_size = 0,
		.image_size = 0,
		.label[0] = &testTitle,
		.label[1] = &testDataLabel
};

cnf_action __attribute__((section (".configData"))) testLabelAction  = {
		.isFrame = true,
		.frame = &testLabelFrame
};

cnf_action __attribute__((section (".configData"))) showSubMenuAction = {
		.isFrame = false,
		.menuId = SAMPLE_SUB_MENU,
};

cnf_frame __attribute__((section (".configData"))) doNothingFrame = {
		.label_size = 0,
		.information_size = 0,
		.image_size = 0,
};
cnf_action __attribute__((section (".configData"))) doNothingAction = {
		.isFrame = true,
		.frame = &doNothingFrame
};

cnf_gfx_label __attribute__((section (".configData"))) powerStateTitle = {
		.text = "Computer Power State",
		.x = 0,
		.y = 0,
		.borderVisible = false
};

cnf_gfx_information_label __attribute__((section (".configData"))) powerStateInfo = {
		.info_type = SHOW_POWER_STATE,
		.x = 0,
		.y = 45+PAGE_SPACING,
		.borderVisible = false
};

cnf_frame __attribute__((section (".configData"))) powerStateFrame = {
		.label_size = 1,
		.information_size = 1,
		.image_size = 0,
		.label[0] = & powerStateTitle,
		.information_label[0] = &powerStateInfo
};

cnf_action __attribute__((section (".configData"))) powerStateAction = {
		.isFrame = true,
		.frame = &powerStateFrame
};

cnf_menu   __attribute__((section (".configData")))mainMenuCnf = {
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
