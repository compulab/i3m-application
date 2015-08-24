
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


//
///* Initialize Mono Menu */
//struct gfx_mono_menu  __attribute__((section (".configData"))) testMenu = {
////struct gfx_mono_menu  testMenu = {
//	.title= main_menu_title,
//	.strings = main_menu_strings,
//	.num_elements = 6,
//	.current_selection = 0
//};
//
//cnf_gfx_information_label voltageInfo = {
//		.info_type = SHOW_VOLTAGE,
//		.x = 45,
//		.y = 45+PAGE_SPACING,
//		.borderVisible = false
//};
//
//cnf_gfx_image voltageImage = {
//	.bitmapProgmem = v_bits,
//	.height = v_height,
//	.width = v_width,
//	.x = 30,
//	.y = PAGE_SPACING*2,
//	.borderVisible = false
//};
//
//cnf_gfx_label voltageTitle = {
//	.text = "Computer Voltage",
//	.x = 10,
//	.y = 0,
//	.borderVisible = false
//};
//
//cnf_action voltageAction = {
//	.isFrame = true,
//	.frame.label_size = 1,
//	.frame.information_size = 1,
//	.frame.image_size = 1,
//	.frame.label[0] = &voltageTitle,
//	.frame.information_label[0] = &voltageInfo,
//	.frame.image[0] = &voltageImage
//};
//
//cnf_action subMenuAction = {
//	.isFrame = false,
//	.menuId = SAMPLE_SUB_MENU
//};
//
//cnf_gfx_image copmulabLogo = {
//		.bitmapProgmem = compulab_new,
//		.height = 32,
//		.width = 128,
//		.x = 30,
//		.y = PAGE_SPACING*2,
//		.borderVisible = false
//};
//
//cnf_action  compulabLogoAction = {
//		.isFrame = true,
//		.frame.label_size = 0,
//		.frame.information_size = 0,
//		.frame.image_size = 1,
//		.frame.image[0] = &copmulabLogo
//
//};
//
//cnf_gfx_label testTitle = {
//		.text = "Testing ...",
//		.x = 10,
//		.y = PAGE_SPACING*4,
//		.borderVisible = false
//};
//
//cnf_gfx_label testDataLabel = {
//		.x = 10,
//		.y = PAGE_SPACING*2,
//		.borderVisible = false,
//		.text = "Test Data"
//};
//
//cnf_action testLabelAction  = {
//		.isFrame = true,
//		.frame.label_size = 2,
//		.frame.information_size = 0,
//		.frame.image_size = 0,
//		.frame.label[0] = &testTitle,
//		.frame.label[1] = &testDataLabel
//};
//
//cnf_action showSubMenuAction = {
//		.isFrame = false,
//		.menuId = SAMPLE_SUB_MENU,
//
//};
//
//cnf_action doNothingAction = {
//		.isFrame = true,
//		.frame.label_size = 0,
//		.frame.information_size = 0,
//		.frame.image_size = 0,
//
//};
//
//cnf_gfx_label powerStateTitle = {
//		.text = "Computer Power State",
//		.x = 10,
//		.y = 0,
//		.borderVisible = false
//};
//
//cnf_gfx_information_label powerStateInfo = {
//		.info_type = SHOW_POWER_STATE,
//		.x = 45,
//		.y = 45+PAGE_SPACING,
//		.borderVisible = false
//};
//cnf_action powerStateAction = {
//		.isFrame = true,
//		.frame.label_size = 1,
//		.frame.information_size = 1,
//		.frame.image_size = 0,
//		.frame.label[0] = & powerStateTitle,
//		.frame.information_label[0] = &powerStateInfo
//
//};
//
//cnf_menu   mainMenuCnf = {
//		.menu = &testMenu,
//		/* Splash screen settings */
//		.actions[0] = &compulabLogoAction,
//		/* Computer Voltage Settings */
//		.actions[1] = &voltageAction,
//		/* Test label action */
//		.actions[2] = &testLabelAction,
////		/* Show Menu */
//		.actions[3] = &showSubMenuAction,
////		/* Do Nothing */
//		.actions[4] = &doNothingAction,
////		/* Power State */
//		.actions[5] = &powerStateAction
//};
////
/////* Initialize Action Menu */
////struct gfx_action_menu_t  __attribute__((section (".configData"))) mainMenu  = {
//////struct gfx_action_menu_t   mainMenu  = {
////		.menu = &testMenu,
////		.visible = false,
//////		.actions[0].type = ACTION_TYPE_SHOW_SPLASH,
//////		.actions[0].splashData.height = 32,
//////		.actions[0].splashData.width = 128,
//////		.actions[0].splashData.type = GFX_MONO_BITMAP_PROGMEM,
//////		.actions[0].splashData.data.progmem = compulab_new,
////		.actions[1].type = ACTION_TYPE_SHOW_DATA_FROM_FUNC,
////		.actions[1].data.title =main_menu_title2,
////		.actions[1].info_type = SHOW_POWER_STATE,
////		.actions[2].type = ACTION_TYPE_SHOW_DATA,
////		.actions[2].data.title = main_menu_title3,
////		.actions[2].data.text = "Test Data",
////		.actions[3].type = ACTION_TYPE_SHOW_MENU,
////		.actions[3].menuId = SAMPLE_SUB_MENU,
////		.actions[4].type = ACTION_TYPE_NONE,
////		.actions[5].type = ACTION_TYPE_SHOW_DATA_FROM_FUNC,
////		.actions[5].data.title =main_menu_title6,
////		.actions[5].info_type = SHOW_VOLTAGE
////};



#endif /* GFX_SAMPLEMENUS_SAMPLEMAINMENU_H_ */
