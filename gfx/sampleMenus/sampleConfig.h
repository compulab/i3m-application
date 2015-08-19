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

struct cnf_blk  __attribute__((section (".configSec"))) sampleConfig = {
//struct cnf_blk  sampleConfig = {
	.signature = 0x40,
	.size = 2,
	.menus[0] = &mainMenu,
	.menus[1] = &subMenu
	};

#endif /* GFX_SAMPLEMENUS_SAMPLECONFIG_H_ */
