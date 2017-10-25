/*
 * menu-handler.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_MENU_HANDLER_H_
#define GFX_MENU_HANDLER_H_

#include "gfx/gfx_utils.h"
#include "timer/tc.h"
#include "display/display_render.h"
#include "display/glcd-0.5.2/glcd_font.h"
#include "work-queue/work.h"
#include "scheduler/scheduler.h"

enum display_state {
	DISPLAY_MENU,
	DISPLAY_FRAME,
	DISPLAY_ACTION_FRAME,
	DISPLAY_LOGO,
	DISPLAY_DASHBOARD,
	DISPLAY_WAIT_FOR_USER_ACK,
	DISPLAY_CLOCK,
};

extern struct gfx_mono_bitmap splash_bitmap;

struct gfx_action_menu **action_menus;

int load_config_block(void);
void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index);
void handle_back_to_menu(void);
void handle_button_pressed(void);

#endif /* GFX_MENU_HANDLER_H_ */
