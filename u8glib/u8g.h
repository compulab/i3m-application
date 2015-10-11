/*
 * u8g.h
 *
 *  Created on: Oct 7, 2015
 *      Author: arkadi
 */

#ifndef U8GLIB_U8G_H_
#define U8GLIB_U8G_H_
#include "csrc/u8g.h"
#include "gfx_components.h"
#include "menu.h"

extern u8g_t u8g;

void u8g_init();

void u8g_clear_screen();

void u8g_draw_menu(struct menu *menu);

void u8g_draw_frame(struct gfx_frame *frame);

void u8g_draw_string(uint8_t y, char *text);

void u8g_draw_stringP(uint8_t y, const u8g_pgm_uint8_t *text);

#endif /* U8GLIB_U8G_H_ */
