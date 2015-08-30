/*
 * menu-handler.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_MENU_HANDLER_H_
#define GFX_MENU_HANDLER_H_
#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "../asf.h"
#include "../timer/tc.h"
#include "action_menu/gfx_action_menu.h"
#include "../config/cnf_blk_components.h"
#include "gfx_utils.h"
#include "key-handle.h"

#define CONFIG_SECTION_ADDRESS 0x8000

struct gfx_action_menu **action_menus;

void load_config_block();

void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index);

void menu_handler();


#endif /* GFX_MENU_HANDLER_H_ */
