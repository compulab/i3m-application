/*
 * menu-handler.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_MENU_HANDLER_H_
#define GFX_MENU_HANDLER_H_

#include "gfx_utils.h"
#include "timer/tc.h"
#include "display/display_render.h"
#include "display/glcd-0.5.2/glcd_font.h"
#include "work-queue/work.h"
#include "scheduler/scheduler.h"

#define CONFIG_SECTION_ADDRESS 0xA000

extern struct gfx_frame *dashboard;

extern struct gfx_frame *clock;

extern struct gfx_frame *splash;

extern bool is_screen_saver_on;

struct gfx_action_menu **action_menus;

//struct gfx_font** fonts;

int load_config_block(void);

void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index);

void memcpy_config(void *dst, void *src_addr, size_t size);

void handle_back_to_menu(void);

void handle_button_pressed(void);

#endif /* GFX_MENU_HANDLER_H_ */
