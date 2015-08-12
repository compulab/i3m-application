

#include "menu-handler.h"
#include "../debug.h"
#include "../timer/tc.h"

#define isKeySelected(var,key) (var & key) == key
key_Select selectedKey;

#include "menu-handler.h"
#include "../action_menu/gfx_action_menu.h"
uint32_t leftTimeStamp = 0,rightTimeStamp = 0,selectTimeStamp = 0;
bool isLeftSet = false, isRightSet = false, isSelectSet = false;

int returnToMenuCount = 0;
key_Select selectedKey;
void menu_handler(){
	getPressedKey(&selectedKey);

	if (presentMenu->visible){
		if (selectedKey == KEY_NONE)
			return;
		if (selectedKey == KEY_HOLD){
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_BACK);
		} else if (isKeySelected(selectedKey,KEY_SELECT)){
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_ENTER);
		} else if (isKeySelected(selectedKey,KEY_LEFT)){
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_UP);
		} else {
			gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_DOWN);
		}
	}else if (isKeySelected(selectedKey,KEY_HOLD)){
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

//TODO: enter bootloader threw SW
void enterBootloaderMode(){
	void (*bootloaderFunc)(void) =(void *)  (BOOT_SECTION_START + 0x1FC );
	bootloaderFunc();
}

void rightButtonPressed(){
	if (isBootloaderState())
		enterBootloaderMode();
	else
		gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_DOWN);

}

void handleBack(){
	if (presentMenu->visible){
		if (presentMenu->parent != NULL)
			gfx_action_menu_init(presentMenu->parent);
	} else
		gfx_action_menu_init(presentMenu);
}


void selectButtonPressed(){
	delay();
	if (!ioport_get_value(FP_OK_BUTTON))
		handleBack();
	else
		gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_ENTER);
}

void leftButtonPressed(){
	if (isBootloaderState())
		enterBootloaderMode();
	else
		gfx_action_menu_process_key(presentMenu,GFX_MONO_MENU_KEYCODE_UP);
}

