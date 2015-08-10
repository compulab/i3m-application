/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_
#include "../asf.h"

typedef void (*func_ptr)(char **);


struct gfx_action_menu_t * presentMenu;


typedef enum gfx_item_action_type_t {
	ACTION_TYPE_NONE,
	ACTION_TYPE_SHOW_MENU,
	ACTION_TYPE_SHOW_DATA_FROM_FUNC,
	ACTION_TYPE_SHOW_DATA,
	ACTION_TYPE_SHOW_SPLASH
} item_action_type;

typedef struct item_data_t {
	char * title;
	char * text;
} item_data;


typedef struct item_action_t {
	item_action_type type;
	struct gfx_action_menu_t * menu;
	struct item_data_t * data;
	func_ptr dataFunc;
	struct gfx_mono_bitmap *  splashData;
} item_action;


typedef struct gfx_action_menu_t {
	struct gfx_mono_menu * menu;
	bool visible;
	item_action *actions;
	struct gfx_action_menu_t * parent;
}gfx_action_menu;

void gfx_action_menu_init(struct gfx_action_menu_t *actionMenu);
uint8_t gfx_action_menu_process_key(struct gfx_action_menu_t *actionMenu, uint8_t keycode);

#endif /* GFX_GFX_ACTION_MENU_H_ */
