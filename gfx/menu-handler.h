/*
 * menu-handler.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_MENU_HANDLER_H_
#define GFX_MENU_HANDLER_H_

#ifndef TC_DRIVER_H
#include "../timer/tc.h"
#endif

#ifndef GFX_GFX_UTILS_H_
#include "gfx_utils.h"
#endif

#ifndef GFX_KEYPAD_H_
#include "key-handle.h"
#endif

#include "../work-queue/work.h"

#include "../timer/timer.h"

#include "../display/display_render.h"

#define CONFIG_SECTION_ADDRESS 0x00010000

extern bool is_screen_saver_on;

struct gfx_action_menu **action_menus;

struct gfx_font** fonts;

void load_config_block();

void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index);

void show_splash();

void handle_button_pressed();

#endif /* GFX_MENU_HANDLER_H_ */
