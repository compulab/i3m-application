/*
 * keypad.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */
#include "stdbool.h"


#ifndef GFX_KEYPAD_H_
#define GFX_KEYPAD_H_


typedef enum ButtonState_t {
	BUTTON_NOT_PRESSED,
	BUTTON_CLICK,
	BUTTON_HOLD
}Button_State;


typedef enum KEY {
	KEY_NONE = 0x00,
	KEY_LEFT = 0x01,
	KEY_RIGHT = 0x02,
	KEY_SELECT = 0x04,
	KEY_HOLD = 0x08
} key_Select;

void getPressedKey(key_Select* selectedKey);

#endif /* GFX_KEYPAD_H_ */
