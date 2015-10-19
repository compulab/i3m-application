/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_


#include "../gfx_components.h"
#include "../gfx_utils.h"
#include "../menu-handler.h"
#include "../../asf.h"
#include "../../Fp-utils.h"
#include "../../def.h"
#include "../../adc/adc.h"
#include "../../ASF/common/services/gfx_mono/sysfont.h"
#include "../../config/cnf_blk_components.h"

struct gfx_action_menu *present_menu;


void gfx_action_menu_init(struct gfx_action_menu *action_menu, bool redraw);

void gfx_action_menu_process_key(struct gfx_action_menu *action_menu, uint8_t keycode);

#endif /* GFX_GFX_ACTION_MENU_H_ */
