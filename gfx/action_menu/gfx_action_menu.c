#include "gfx_action_menu.h"
#include "../../debug.h"

void clearMenu(){
	gfx_mono_generic_draw_filled_rect(0,0,128,64,GFX_PIXEL_CLR);
}

void gfx_action_menu_init(gfx_action_menu *actionMenu){
	presentMenu = actionMenu;
	actionMenu->visible = true;
	clearMenu();
	gfx_mono_menu_init(actionMenu->menu);
}

void showMenu(struct gfx_action_menu_t *menu,bool updateParent){
	if (updateParent) menu->parent = presentMenu;
	gfx_action_menu_init(menu);
}

void showThisMenu(){
	gfx_action_menu_init(presentMenu);
}

void writeText(const char* text,int pageAddr,int colAddr){
	ssd1306_set_page_address(pageAddr);
	ssd1306_set_column_address(colAddr);
	ssd1306_write_text(text);
}

void showData (item_data * data){
	gfx_mono_draw_progmem_string((char PROGMEM_PTR_T)data->title,0, 0, &sysfont);
	writeText(data->text,4,0);
}

void showSplash(struct gfx_mono_bitmap * splash){
	gfx_mono_put_bitmap(splash,0,8);
}

struct gfx_action_menu_t * menu;

uint8_t gfx_action_menu_process_key(struct gfx_action_menu_t *actionMenu, uint8_t keycode){
	if (keycode == GFX_MONO_MENU_KEYCODE_ENTER){
		item_action selectedAction = actionMenu->actions[(actionMenu->menu)->current_selection];
		item_action_type type = selectedAction.type;
		if (type != ACTION_TYPE_NONE){
			clearMenu();
			actionMenu->visible = false;
		}
		switch (type){
		case ACTION_TYPE_SHOW_FRAME:
			gfx_frame_draw(selectedAction.frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			menu = selectedAction.menu;
			showMenu(menu,true);
			break;
		default:
			break;
		}
	} else if (keycode == GFX_MONO_MENU_KEYCODE_BACK) {
		if (presentMenu->visible){
			if (actionMenu->parent != NULL) showMenu(actionMenu->parent,false);
		}else{
				showThisMenu();
		}
	}else {
		return gfx_mono_menu_process_key(actionMenu->menu,keycode);
	}
	return 0;
}

