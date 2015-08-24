/*
 * sampleConfig.h
 *
 *  Created on: Aug 17, 2015
 *      Author: arkadi
 */

#ifndef GFX_SAMPLEMENUS_SAMPLECONFIG_H_
#define GFX_SAMPLEMENUS_SAMPLECONFIG_H_
#include "../../config/cnf_blk_components.h"
#include "sampleMainMenu.h"




cnf_gfx_label  __attribute__((section (".configData"))) title = {
			.text = "Computer Voltage",
			.x = 10,
			.y = 0,
			.borderVisible = false
};

cnf_gfx_image  __attribute__((section (".configData"))) voltageImageTest = {
		.bitmapProgmem = v_bits,
		.height = v_height,
		.width = v_width,
		.x = 30,
		.y = PAGE_SPACING*2,
		.borderVisible = false

};
cnf_gfx_information_label  __attribute__((section (".configData"))) voltageInfoTest = {
		.info_type = SHOW_VOLTAGE,
		.x = 45,
		.y = v_height+PAGE_SPACING+7,
		.borderVisible = false
};
cnf_frame  __attribute__((section (".configData"))) testFrame = {
		.label_size = 1,
		.information_size = 1,
		.image_size = 1,
		.label[0] = &title,
		.image[0] = & voltageImageTest,
		.information_label[0] = &voltageInfoTest
};

PROGMEM_STRING_T test_main_menu_strings[] = {
	main_menu_1,
};


/* Initialize Mono Menu */
struct gfx_mono_menu  __attribute__((section (".configData"))) testMonoMenu = {
	.title= main_menu_title,
	.strings = test_main_menu_strings,
	.num_elements = 1,
	.current_selection = 0
};


cnf_action  __attribute__((section (".configData"))) testAction = {
		.isFrame = true,
		.frame = &testFrame
};

cnf_menu  __attribute__((section (".configData")))  mainMenuCnf = {
		.menu = &testMonoMenu,
		/* Splash screen settings */
		.actions[0] = &testAction,
		/* Computer Voltage Settings */
};


struct cnf_blk  __attribute__((section (".configSec"))) sampleConfig = {
	.signature = 0x40,
	.size = 1,
	.menus[0] = &mainMenuCnf,
//	.menus[1] = &subMenu
	};


#endif /* GFX_SAMPLEMENUS_SAMPLECONFIG_H_ */
