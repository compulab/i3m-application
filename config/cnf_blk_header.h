/*
 * cnf_blk_header.h
 *
 *  Created on: Aug 16, 2015
 *      Author: arkadi
 */

#ifndef CONFIG_CNF_BLK_HEADER_H_
#define CONFIG_CNF_BLK_HEADER_H_
#include "../asf.h"
#include "../gfx/action_menu/gfx_action_menu.h"


#define MAX_MENUS 15

struct cnf_blk  {
	uint8_t signature;
	uint8_t size;
	struct gfx_action_menu_t * menus[MAX_MENUS];
	uint32_t reserved[24];
};


#endif /* CONFIG_CNF_BLK_HEADER_H_ */
