/*
 * menuConfig.h
 *
 *  Created on: Aug 17, 2015
 *      Author: arkadi
 */

#ifndef GFX_SAMPLEMENUS_MENUCONFIG_H_
#define GFX_SAMPLEMENUS_MENUCONFIG_H_
#include "../action_menu/gfx_action_menu.h"
typedef struct MENU_CONFIG_T{
	gfx_action_menu * menu;
	uint8_t id;
	struct MENU_CONFIG_T * next;
} menu_config;

#endif /* GFX_SAMPLEMENUS_MENUCONFIG_H_ */
