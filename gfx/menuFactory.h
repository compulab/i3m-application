/*
 * menuFactory.h
 *
 *  Created on: Aug 17, 2015
 *      Author: arkadi
 */

#ifndef GFX_MENUFACTORY_H_
#define GFX_MENUFACTORY_H_

#include "action_menu/gfx_action_menu.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "../config/cnf_blk_components.h"
#include "gfx_utils.h"

#define CONFIG_SECTION_ADDRESS 0x8000

gfx_action_menu ** menus;
struct cnf_blk configBlock;

void loadConfigBlock();

void setMenuById(struct gfx_action_menu_t ** menu, uint8_t index);

#endif /* GFX_MENUFACTORY_H_ */
