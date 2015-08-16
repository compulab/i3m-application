

#include "menu-handler.h"
#include "../debug.h"
#include "../timer/tc.h"

#define isKeySelected(var,key) (var & key) == key
key_Select selectedKey;

#include "menu-handler.h"

uint32_t leftTimeStamp = 0,rightTimeStamp = 0,selectTimeStamp = 0;
bool isLeftSet = false, isRightSet = false, isSelectSet = false;
int returnToMenuCount = 0;
key_Select selectedKey;


//TODO: enter bootloader threw SW
void enterBootloaderMode(){
//	udc_detach();
//	udc_stop();
	asm ("ldi r30, 0xFE\n"  /* Low byte to ZL */
		  "ldi r31, 0x40\n" /* mid byte to ZH */
		  "ldi r24, 0x00\n" /* high byte to EIND which lives */
		  "out 0x3c, r24\n" /* at addr 0x3c in I/O space Extended Indirect register */
		  "eijmp":  :: "r24", "r30", "r31");

}



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

