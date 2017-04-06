/*
 * menu-handler.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_MENU_HANDLER_H_
#define GFX_MENU_HANDLER_H_

#ifndef TC_DRIVER_H
#include "timer/tc.h"
#endif

#ifndef GFX_GFX_UTILS_H_
#include "gfx_utils.h"
#endif

#include "display/glcd-0.5.2/glcd_font.h"

#include "work-queue/work.h"

#include "scheduler/scheduler.h"

#include "display/display_render.h"

#define CONFIG_SECTION_ADDRESS 0xA000

enum button_state {
	BUTTON_NOT_PRESSED,
	BUTTON_CLICK,
	BUTTON_HOLD
};

extern struct gfx_frame *dashboard;

extern struct gfx_frame *clock;

extern bool is_screen_saver_on;

struct gfx_action_menu **action_menus;

//struct gfx_font** fonts;

int load_config_block(void);

void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index);

void memcpy_config(void *dst, void *src_addr, size_t size);

void hadle_back_to_menu(void);

void show_logo(void);

void handle_button_pressed(void);

#endif /* GFX_MENU_HANDLER_H_ */
