#include "gfx_action_menu.h"
#include "../../debug.h"
void gfx_action_menu_init(struct gfx_action_menu_t *actionMenu){
	presentMenu = actionMenu;
	actionMenu->visible = true;
	gfx_mono_menu_init(&actionMenu->menu);
}

void showMenu(struct gfx_action_menu_t *menu,bool updateParent){
	if (updateParent)
		menu->parent = presentMenu;
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

void updateDataByType(information_type type, char ** data){
	func_ptr ptr= NULL;
	switch (type){
	case SHOW_VOLTAGE:
		ptr = setPowerString;
		break;
	case SHOW_POWER_STATE:
		ptr = showState;
		break;
	case SHOW_CPU_TEMPERTURE:
	case SHOW_GPU_TEMPERTURE:
		break;
	}
	if (ptr != NULL)
		ptr(data);
}

uint8_t gfx_action_menu_process_key(struct gfx_action_menu_t *actionMenu, uint8_t keycode){
	if (keycode == GFX_MONO_MENU_KEYCODE_ENTER){
		item_action selectedAction = actionMenu->actions[(actionMenu->menu)->current_selection];
		item_action_type type = selectedAction.type;
		if (type != ACTION_TYPE_NONE){
			ssd1306_clear();
			actionMenu->visible = false;
		}
		switch (type){
		case ACTION_TYPE_SHOW_DATA:
			showData(&selectedAction.data);
			break;
		case ACTION_TYPE_SHOW_SPLASH:
			showSplash(selectedAction.splashData);
			break;
		case ACTION_TYPE_SHOW_DATA_FROM_FUNC:
			updateDataByType(selectedAction.info_type,&(selectedAction.data).text);
			showData(&selectedAction.data);
			break;
		case ACTION_TYPE_SHOW_MENU:
			showMenu(selectedAction.subMenu,true);
			break;
		case ACTION_TYPE_NONE:
			break;
		}
	} else if (keycode == GFX_MONO_MENU_KEYCODE_BACK) {
		if (presentMenu->visible){
			if (actionMenu->parent != NULL)
				showMenu(actionMenu->parent,false);
		}else
				showThisMenu();
	}else {
		return gfx_mono_menu_process_key(&actionMenu->menu,keycode);
	}

	return 0;
}

