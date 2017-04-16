/*
 * gfx_action_menu_dmi.h
 *
 * Created: 11-Apr-17 8:53:04 PM
 *  Author: Nikita
 */


#ifndef GFX_ACTION_MENU_DMI_H_
#define GFX_ACTION_MENU_DMI_H_

#include "gfx_action_menu.h"
#include <stdbool.h>

extern struct gfx_action_menu dmi_menu;
extern bool is_dmi_set;

void set_dmi_menu(void);

#endif /* GFX_ACTION_MENU_DMI_H_ */