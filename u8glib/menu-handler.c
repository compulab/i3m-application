#include "menu-handler.h"
#include "../timer/tc.h"
#include "../u8glib/menu.h"

#define is_key_selected(var,key) (var & key) == key
enum key_Select selected_Key;

enum button_state left_button, right_button, ok_button;

bool left_pressed = false,
		right_pressed = false,
		ok_pressed = false;

uint8_t left_time = 0,
		right_time = 0,
		ok_time = 0;

//TODO: enter bootloader threw SW
void enter_bootloader_mode()
{
//	udc_detach();
//	udc_stop();
//	asm ("ldi r30, 0xFE\n"  /* Low byte to ZL */
//		  "ldi r31, 0x40\n" /* mid byte to ZH */
//		  "ldi r24, 0x00\n" /* high byte to EIND which lives */
//		  "out 0x3c, r24\n" /* at addr 0x3c in I/O space Extended Indirect register */
//		  "eijmp":  :: "r24", "r30", "r31");
}

void clear_counter()
{
	tc_counter = 0;
}


void update_button_state(bool pressed, uint8_t *time, enum button_state *state)
{
	if (!pressed && *time > 0){
		if (*time < 10)
			*state = BUTTON_CLICK;
		else
			*state = BUTTON_HOLD;
		*time = 0;
	} else {
		*state = BUTTON_NOT_PRESSED;
	}
}

void update_button_pressed(bool *pressed, uint8_t *time, port_pin_t pin)
{
	bool is_high = gpio_pin_is_high(pin);
	if (!(*pressed) && is_high){
		(*time) = tc_counter;
		(*pressed) = true;
	} else if (*pressed && !is_high){
		(*time) = tc_counter - (*time);
		(*pressed) = false;
	}
}

void handle_button_pressed()
{
	update_button_pressed(&left_pressed, &left_time, FP_LEFT_BUTTON);
	update_button_pressed(&right_pressed, &right_time,FP_RIGHT_BUTTON);
	update_button_pressed(&ok_pressed, &ok_time, FP_OK_BUTTON);

	update_button_state(left_pressed, &left_time, &left_button);
	update_button_state(right_pressed, &right_time, &right_button);
	update_button_state(ok_pressed, &ok_time, &ok_button);
	if (ok_button != BUTTON_NOT_PRESSED || left_button != BUTTON_NOT_PRESSED || right_button != BUTTON_NOT_PRESSED)
		clear_counter();
	switch (ok_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
		handle_select_pressed();
		break;
	default:
		break;
	}
	switch (left_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
		handle_up_pressed();
		return;
		break;
	default:
		break;
	}
	switch (right_button){
	case BUTTON_HOLD:
	case BUTTON_CLICK:
		handle_down_pressed();
		return;
		break;
	default:
		break;
	}
}
