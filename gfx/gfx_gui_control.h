/*
 * gfx_gui.h
 *
 * Created: 17-Apr-17 10:21:58 PM
 *  Author: Nikita
 */


#ifndef GFX_GUI_H_
#define GFX_GUI_H_

#include "gfx/gfx_components/gfx_graphic_menu.h"

void gfx_gui_init(struct gfx_graphic_menu **graphic_menus, uint8_t num_elements);
void gfx_redraw_current_frame(void);
void gfx_switch_to_current_menu(void);
void gfx_display_frame(struct gfx_frame *frame);
void gfx_switch_to_frame(struct gfx_frame *frame);
void gfx_switch_to_menu(struct gfx_graphic_menu *graphic_menu);
void gfx_handle_button(uint8_t keycode);
void gfx_display_msg(char *msg);

#endif /* GFX_GUI_H_ */