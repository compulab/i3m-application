#include "gfx_action_menu.h"
#include "../../debug.h"
#include "../../twi/eeprom.h"

void clear_menu()
{
	for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
//	gfx_mono_ssd1306_put_framebuffer();

}

void gfx_action_menu_init(struct gfx_action_menu *action_menu)
{
	present_menu->visible = false;
	present_menu = action_menu;
	action_menu->visible = true;
	clear_menu();
	gfx_mono_menu_init(action_menu->menu);
}

void show_menu(struct gfx_action_menu *menu)
{
	gfx_action_menu_init(menu);
}

void show_current_menu()
{
	gfx_action_menu_init(present_menu);
}

struct gfx_action_menu *menu;

void clear_screen(struct gfx_action_menu *actionMenu)
{
	clear_menu();
	actionMenu->visible = false;
}

uint8_t gfx_action_menu_process_key(struct gfx_action_menu *action_menu, uint8_t keycode)
{
	if (keycode == GFX_MONO_MENU_KEYCODE_ENTER){
		struct gfx_item_action selected_action = action_menu->actions[(action_menu->menu)->current_selection];
		enum action_type type = selected_action.type;
		clear_screen(action_menu);
		switch (type){
		case ACTION_TYPE_SHOW_FRAME:
			gfx_frame_draw(selected_action.frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			menu = selected_action.menu;
			show_menu(menu);
			break;
//		case ACTION_TYPE_SET_BIOS_STATE:
//			eeprom_write_byte(BIOS_STATE_ADDRESS,selectedAction.frame->informations[0]->info_data);
//			break;
		default:
			break;
		}
	} else if (keycode == GFX_MONO_MENU_KEYCODE_BACK) {
		if (!present_menu->visible)
			show_current_menu();
	}else {
		return gfx_mono_menu_process_key(action_menu->menu, keycode);
	}
	return 0;
}
