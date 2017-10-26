/*
 * gfx_gui.h
 *
 * Created: 17-Apr-17 10:21:58 PM
 *  Author: Nikita
 */


#ifndef GFX_GUI_H_
#define GFX_GUI_H_

enum display_state {
	DISPLAY_MENU,
	DISPLAY_FRAME,
	DISPLAY_ACTION_FRAME,
	DISPLAY_LOGO,
	DISPLAY_DASHBOARD,
	DISPLAY_WAIT_FOR_USER_ACK,
	DISPLAY_CLOCK,
};

void gfx_action_menu_goto_main(void);
void gfx_redraw_current_frame(void);
void gfx_switch_to_menu(struct gfx_action_menu *action_menu);

#endif /* GFX_GUI_H_ */