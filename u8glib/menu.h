/*
 * menu.h
 *
 *  Created on: Oct 7, 2015
 *      Author: arkadi
 */
#include "u8g_components.h"
#include "csrc/u8g.h"
#include "gfx_utils.h"
#include "u8g.h"

extern struct menu *current_menu;

void set_menu_font();

void show_menu(struct menu *menu);

void handle_select_pressed();

void handle_up_pressed();

void handle_down_pressed();
