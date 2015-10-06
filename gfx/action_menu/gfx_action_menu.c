#include "gfx_action_menu.h"
#include "../../debug.h"
#include "../../twi/eeprom.h"

void clear_menu()
{
	for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
	gfx_mono_ssd1306_put_framebuffer();

}

void gfx_action_menu_init(struct gfx_action_menu *actionMenu)
{
	present_menu->visible = false;
	present_menu = actionMenu;
	actionMenu->visible = true;
	clear_menu();
	gfx_mono_menu_init(actionMenu->menu);
}

void show_menu(struct gfx_action_menu *menu, bool updateParent)
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

uint8_t gfx_action_menu_process_key(struct gfx_action_menu *actionMenu, uint8_t keycode)
{
	if (keycode == GFX_MONO_MENU_KEYCODE_ENTER){
		struct gfx_item_action selectedAction = actionMenu->actions[(actionMenu->menu)->current_selection];
		enum action_type type = selectedAction.type;
		switch (type){
		case ACTION_TYPE_SHOW_FRAME:
			clear_screen(actionMenu);
			gfx_frame_draw(selectedAction.frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			clear_screen(actionMenu);
			menu = selectedAction.menu;
			show_menu(menu, true);
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
		return gfx_mono_menu_process_key(actionMenu->menu, keycode);
	}
	return 0;
}
