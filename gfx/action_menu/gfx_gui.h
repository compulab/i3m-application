/*
 * gfx_gui.h
 *
 * Created: 17-Apr-17 10:21:58 PM
 *  Author: Nikita
 */


#ifndef GFX_GUI_H_
#define GFX_GUI_H_

void gfx_action_menu_goto_main(void);
void gfx_show_screen_saver(enum display_state state);
void gfx_redraw_current_frame(void);
void gfx_display_msg(char *msg);

#endif /* GFX_GUI_H_ */