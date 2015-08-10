///*
// * keypad.c
// *
// *  Created on: Aug 6, 2015
// *      Author: arkadi
// */
//
//#include "keypad.h"
//#include "../asf.h"
//
//static bool leftButtonStatus = false,
//			rightButtonStatus = false,
//			selectButtonStatus = false;
//
//
//bool isButtonPressed(bool * lastStatus, uint8_t buttonPos){
//		bool currentStatus = ioport_get_value(buttonPos);
//		bool result = currentStatus && !(*lastStatus);
//		*lastStatus = currentStatus;
//		return result;
//	}
//
//
//	bool isLeftButtonPressed(){
//		return isButtonPressed(&leftButtonStatus,FP_LEFT_BUTTON);
//	}
//
//	bool isRightButtonPressed(){
//		return isButtonPressed(&rightButtonStatus,FP_RIGHT_BUTTON);
//	}
//
//	bool isSelectButtonPressed(){
//		return isButtonPressed(&selectButtonStatus,FP_OK_BUTTON);
//	}
//
//	void getPressedKey(key_Select* selectedKey){
//		*selectedKey = KEY_NONE;
//		if (isLeftButtonPressed())
//			*selectedKey = *selectedKey | KEY_LEFT;
//		if (isRightButtonPressed())
//			*selectedKey = *selectedKey | KEY_RIGHT;
//		if (isSelectButtonPressed())
//			*selectedKey = *selectedKey | KEY_SELECT;
//	}
