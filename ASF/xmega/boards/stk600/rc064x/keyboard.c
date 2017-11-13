/*
 * keyboard.c
 *
 * Created: 13-Nov-17 2:17:04 PM
 *  Author: Nikita
 */
#include "scheduler/scheduler.h"
#include "power/power.h"
#include "work-queue/work.h"
#include "gfx/gfx_gui_control.h"
#include "asf.h"

static bool ok_button = false;
static bool left_button = false;
static bool right_button = false;

static void handle_buttons(void *data)
{
	if (ok_button) {
		gfx_handle_button(GFX_MONO_MENU_KEYCODE_ENTER);
		return;
	}

	if (left_button) {
		gfx_handle_button(GFX_MONO_MENU_KEYCODE_DOWN);
		return;
	}

	if (right_button) {
		gfx_handle_button(GFX_MONO_MENU_KEYCODE_UP);
		return;
	}
}

struct work button_work = { .do_work = handle_buttons, .data = NULL, .next = NULL, };

/*
 * Buttons GPIO change
 */
ISR(PORTF_INT1_vect)
{
	left_button = gpio_pin_is_high(FP_LEFT_BUTTON);
	right_button = gpio_pin_is_high(FP_RIGHT_BUTTON);
	ok_button = gpio_pin_is_high(FP_OK_BUTTON);
	insert_work(&button_work);
}
