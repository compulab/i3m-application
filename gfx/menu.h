/*
 * menu.h
 *
 *  Created on: Aug 4, 2015
 *      Author: arkadi
 */

#ifndef GFX_MENU_H_
#define GFX_MENU_H_

#include "../asf.h"

PROGMEM_DECLARE(char const, main_menu_title[]) = "Menu Test Title";
PROGMEM_DECLARE(char const, main_menu_1[]) = "Show Compulab logo";
PROGMEM_DECLARE(char const, main_menu_2[]) = "Do Nothing";
PROGMEM_DECLARE(char const, main_menu_3[]) = "Do Nothing";
PROGMEM_DECLARE(char const, main_menu_4[]) = "Do Nothing";
PROGMEM_DECLARE(char const, main_menu_5[]) = "Do Nothing";
PROGMEM_DECLARE(char const, main_menu_6[]) = "Show voltage";
//@}

PROGMEM_STRING_T main_menu_strings[] = {
	main_menu_1,
	main_menu_2,
	main_menu_3,
	main_menu_4,
	main_menu_5,
	main_menu_6,
};


struct gfx_mono_bitmap compulab_logo = {
	.type = GFX_MONO_BITMAP_RAM,
	.width = 128,
	.height = 32,
	.data.pixmap = compulab_new
};

struct gfx_mono_menu testMenu = {
	.title= main_menu_title,
	.strings = main_menu_strings,
	.num_elements = 6,
	.current_selection = 3
};


#endif /* GFX_MENU_H_ */
