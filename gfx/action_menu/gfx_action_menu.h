/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_

#include "../gfx_components.h"
#include "../gfx_utils.h"
#include "../../asf.h"
#include "../../Fp-utils.h"
#include "../../adc/adc.h"
#include "../../ASF/common/services/gfx_mono/sysfont.h"
#include "../../config/cnf_blk_components.h"

struct gfx_action_menu *present_menu;


enum gfx_item_action_type {
	ACTION_TYPE_NONE,
	ACTION_TYPE_SHOW_MENU,
	ACTION_TYPE_SHOW_FRAME
};

struct gfx_item_data {
	PROGMEM_STRING_T title;
	char *text;
};


struct gfx_item_action {
	enum gfx_item_action_type type;
	uint8_t menuId;
	struct gfx_action_menu *menu;
	struct gfx_frame *frame;
};


struct gfx_action_menu {
	struct gfx_mono_menu *menu;
	struct gfx_item_action *actions;
	struct gfx_action_menu *parent;
	uint8_t id;
	bool visible;
};

void gfx_action_menu_init(struct gfx_action_menu *actionMenu);
uint8_t gfx_action_menu_process_key(struct gfx_action_menu *actionMenu, uint8_t keycode);

#endif /* GFX_GFX_ACTION_MENU_H_ */
