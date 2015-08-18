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
#include "../config/cnf_blk_header.h"

void menuFactoryInit(void * cnf_blk);

void setMenuById(struct gfx_action_menu_t * menu, uint8_t index);

#endif /* GFX_MENUFACTORY_H_ */
