/*
 * gfx_gui.h
 *
 * Created: 17-Apr-17 10:21:58 PM
 *  Author: Nikita
 */


#ifndef GFX_GUI_H_
#define GFX_GUI_H_

#include "gfx/action_menu/gfx_graphic_menu.h"

enum display_state {
	DISPLAY_MENU,
	DISPLAY_FRAME,
	DISPLAY_ACTION_FRAME,
	DISPLAY_LOGO,
	DISPLAY_DASHBOARD,
	DISPLAY_WAIT_FOR_USER_ACK,
	DISPLAY_CLOCK,
};

void gfx_gui_init(void);
void gfx_redraw_current_frame(void);
void gfx_switch_to_current_menu(void);
void gfx_switch_to_frame(struct gfx_frame *frame);
void gfx_switch_to_menu(struct gfx_graphic_menu *graphic_menu);
void gfx_handle_button(uint8_t keycode);

#endif /* GFX_GUI_H_ */