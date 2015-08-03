/*
 * oled-menu.h
 *
 *  Created on: Jul 29, 2015
 *      Author: arkadi
 */
#ifndef OLED_MENU_H_
#define OLED_MENU_H_

#include "ASF/common/services/gfx_mono/gfx_mono_menu.h"


/** Menu struct */
struct oled_menu {
	struct gfx_mono_menu* menu;
	void (** operations)();
	void (* handleBack)();
};

void oled_menu_init(struct oled_menu *oledMenu);
uint8_t oled_menu_process_key(struct oled_menu *oledMenu, uint8_t keycode);

#endif /* OLED_MENU_H_ */
