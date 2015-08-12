/*
 * keypad.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */
#include "keypad.h"
#include "../asf.h"

bool leftButtonStatus = true,
	rightButtonStatus = true,
	selectButtonStatus = true,
	debugButtonStatus = true;

Button_State getButtonStatus(bool * lastStatus, uint8_t buttonPos){
		bool currentStatus = ioport_get_value(buttonPos);
		Button_State buttonState;
//		delay_ms(300);
//		bool longStatus = ioport_get_value(buttonPos);

		if (currentStatus && !(*lastStatus))
//			if (longStatus == currentStatus)
//				return BUTTON_HOLD;
//			else
				buttonState = BUTTON_CLICK;
		else
			buttonState = BUTTON_NOT_PRESSED;

		*lastStatus = currentStatus;
		return buttonState;
	}


Button_State getLeftButtonStatus(){
	return getButtonStatus(&leftButtonStatus,FP_LEFT_BUTTON);
}

Button_State getRightButtonStatus(){
	return getButtonStatus(&rightButtonStatus,FP_RIGHT_BUTTON);
}

bool getHoldStatus(){
	return getButtonStatus(&debugButtonStatus,FP_DEBUG_BACK_BUTTON) == BUTTON_CLICK;
}

Button_State getSelectButtonStatus(){
	if (getHoldStatus()) /* for debug only */
		return BUTTON_HOLD;
	return getButtonStatus(&selectButtonStatus,FP_OK_BUTTON);
}

void getPressedKey(key_Select* selectedKey){
	Button_State leftButton = getLeftButtonStatus(),
			rightButton = getRightButtonStatus(),
			selectButton = getSelectButtonStatus();
	*selectedKey = KEY_NONE;
	if (leftButton == BUTTON_CLICK)
		*selectedKey = *selectedKey | KEY_LEFT;
	if (rightButton == BUTTON_CLICK)
		*selectedKey = *selectedKey | KEY_RIGHT;
	if (selectButton == BUTTON_CLICK)
		*selectedKey = *selectedKey | KEY_SELECT;
	if (selectButton == BUTTON_HOLD)
		*selectedKey = *selectedKey | KEY_HOLD;
}

