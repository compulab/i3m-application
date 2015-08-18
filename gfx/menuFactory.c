/*
 * menuFactory.c
 *
 *  Created on: Aug 17, 2015
 *      Author: arkadi
 */
#include "menuFactory.h"

gfx_action_menu ** menus;
struct cnf_blk configBlock;

void menuFactoryInit(void * cnf_blk){
	memcpy_P(&configBlock,cnf_blk,sizeof(struct cnf_blk));
	menus = malloc(sizeof(gfx_action_menu) * configBlock.size);
	struct MENU_CONFIG_T * menuConfig;
	void * menuConfigP = configBlock.menuConfig;
	do {
		menuConfig = malloc(sizeof(struct MENU_CONFIG_T));
		memcpy_P(&menuConfig,menuConfigP,sizeof(struct MENU_CONFIG_T));
		menus[menuConfig->id] = malloc(sizeof(gfx_action_menu));
		memcpy_P(&menus[menuConfig->id],menuConfig->menu,sizeof(struct gfx_action_menu_t));
//		menuConfigP = pgm_read_byte(menuConfig->next);
		free(menuConfig);
	} while (menuConfig != NULL);
}

void setMenuById(struct gfx_action_menu_t * menu, uint8_t index){
	if (index < configBlock.size)
		menu = menus[index];
}




