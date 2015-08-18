/*
 * sampleConfig.h
 *
 *  Created on: Aug 17, 2015
 *      Author: arkadi
 */

#ifndef GFX_SAMPLEMENUS_SAMPLECONFIG_H_
#define GFX_SAMPLEMENUS_SAMPLECONFIG_H_
#include "../../config/cnf_blk_header.h"
#include "sampleMainMenu.h"
#include "menuConfig.h"




struct MENU_CONFIG_T __attribute__((section (".configSec"))) sampleSubMenuConfig = {
		.menu = &subMenu,
		.id = SAMPLE_SUB_MENU,
		.next = NULL
};


struct MENU_CONFIG_T __attribute__((section (".configSec"))) sampleMainMenuConfig = {
		.menu = &mainMenu,
		.id = SAMPLE_MAIN_MENU,
		.next = &sampleSubMenuConfig
};

struct cnf_blk  __attribute__((section (".configSec"))) sampleConfig = {
//struct cnf_blk  sampleConfig = {
	.signature = 0x40,
	.size = 1,
	.menuConfig = &sampleMainMenuConfig
};

#endif /* GFX_SAMPLEMENUS_SAMPLECONFIG_H_ */
