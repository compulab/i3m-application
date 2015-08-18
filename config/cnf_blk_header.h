/*
 * cnf_blk_header.h
 *
 *  Created on: Aug 16, 2015
 *      Author: arkadi
 */

#ifndef CONFIG_CNF_BLK_HEADER_H_
#define CONFIG_CNF_BLK_HEADER_H_
#include "../asf.h"
#include "../gfx/sampleMenus/menuConfig.h"

struct cnf_blk  {
	uint8_t signature;
	uint8_t size;
	struct MENU_CONFIG_T * menuConfig;
	uint32_t reserved[24];
};


#endif /* CONFIG_CNF_BLK_HEADER_H_ */
