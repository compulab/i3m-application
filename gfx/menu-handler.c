

#include "menu-handler.h"

#define isKeySelected(var,key) (var & key) == key
key_Select selectedKey;

#include "menu-handler.h"

//TODO: enter bootloader threw SW
void enterBootloaderMode(){
//	udc_detach();
//	udc_stop();
//	asm ("ldi r30, 0xFE\n"  /* Low byte to ZL */
//		  "ldi r31, 0x40\n" /* mid byte to ZH */
//		  "ldi r24, 0x00\n" /* high byte to EIND which lives */
//		  "out 0x3c, r24\n" /* at addr 0x3c in I/O space Extended Indirect register */
//		  "eijmp":  :: "r24", "r30", "r31");
}


void menu_handler(){
	getPressedKey(&selectedKey);
	if (presentMenu->visible){
		if (selectedKey == KEY_NONE) return;
		else if (selectedKey == KEY_HOLD)
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_BACK);
		else if (isKeySelected(selectedKey,KEY_SELECT))
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_ENTER);
		else if (isKeySelected(selectedKey,KEY_LEFT))
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_UP);
		else
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_DOWN);
	} else {
		if (isKeySelected(selectedKey,KEY_HOLD))
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_BACK);
	}
}

void delay(){
	delay_ms(100);
}

bool isBootloaderState(){
	delay();
//	return ioport_get_value(FP_RIGHT_BUTTON) && ioport_get_value(FP_LEFT_BUTTON);
	return false;
}

void rightButtonPressed(){
	if (isBootloaderState()) enterBootloaderMode();
	else
		gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_DOWN);
}

void handleBack(){
	if (presentMenu->visible){
		if (presentMenu->parent != NULL) gfx_action_menu_init(presentMenu->parent);
	} else {
		gfx_action_menu_init(presentMenu);
	}
}

void selectButtonPressed(){
	delay();
	if (!ioport_get_value(FP_OK_BUTTON)) handleBack();
	else
		gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_ENTER);
}

void leftButtonPressed(){
	if (isBootloaderState()) enterBootloaderMode();
	else
		gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_UP);
}

uint8_t size;

void action_types_init(){
	gfx_action_menu * menu;
	item_action * action;
	for (int i=0; i < size; i++){
		menu = action_menus[i];
#ifdef DEBUG_MODE
		MSG_T_N("menu init in menu:",i)
#endif
		for (int j=0; j < menu->menu->num_elements; j++){
			action = &menu->actions[j];
			switch (action->type){
			case ACTION_TYPE_SHOW_MENU:
				setMenuById(&(action->menu),action->menuId);
#ifdef DEBUG_MODE
				MSG_T_N("Menu id",action->menuId)
#endif
				break;
			default:
				break;
			}
		}
	}
}

void loadAction(item_action * action, cnf_action * cnfAction){
	cnf_action configAction;
	memcpy_P(&configAction,cnfAction,sizeof(cnf_action));
	if (configAction.isFrame){
#ifdef DEBUG_MODE
		MSG("action is frame")
#endif
		action->type = ACTION_TYPE_SHOW_FRAME;
		action->frame = malloc (sizeof(gfx_frame));
		gfx_frame_init(action->frame,configAction.frame);
	} else {
#ifdef DEBUG_MODE
		MSG("action is menu")
#endif
		action->type = ACTION_TYPE_SHOW_MENU;
		action->menuId = configAction.menuId;
	}
}

void loadConfigBlock(){
	struct cnf_blk configBlock;
	cnf_menu configMenu;
	struct gfx_mono_menu * monoMenu;
	memcpy_P(&configBlock,(void *) CONFIG_SECTION_ADDRESS,sizeof(struct cnf_blk));
	size = configBlock.size;
	action_menus = malloc(sizeof (gfx_action_menu *) * configBlock.size);
	for (int i=0; i < configBlock.size; i++){
#ifdef DEBUG_MODE
		MSG_T_N("Config menu: " ,i)
#endif
		action_menus[i] = malloc(sizeof(gfx_action_menu));
		memcpy_P(&configMenu, configBlock.menus[i],sizeof(cnf_menu));
		monoMenu = malloc(sizeof(struct gfx_mono_menu));
		memcpy_P(monoMenu,configMenu.menu,sizeof(struct gfx_mono_menu));
		action_menus[i]->menu= monoMenu;
		action_menus[i]->actions = malloc (sizeof(item_action) * monoMenu->num_elements);
		for (uint8_t j=0; j < monoMenu->num_elements; j++){
			loadAction(&(action_menus[i]->actions[j]),configMenu.actions[j]);
		}
	}
	action_types_init();
}

void setMenuById(struct gfx_action_menu_t ** menu, uint8_t index){
	if (index < size){
		*menu = action_menus[index];
		#ifdef DEBUG_MODE
			MSG_T_N("setting menu number: ", index)
		#endif
	}
}
