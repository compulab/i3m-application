/*
 * effects.c
 *
 *  Created on: Jan 30, 2016
 *      Author: arkadi
 */

#include "effects.h"
#define SLEEP_BRIGHTNESS 	100
#define SLEEP_MSG 			"SLEEP"
#define POWER_OFF_MSG		"OFF"
#define SLEEP_MSG_X			40
#define SLEEP_MSG_Y			20

void enter_dim_mode(char *msg)
{
	display_state = DISPLAY_SLEEP;
	ssd1306_set_contrast(SLEEP_BRIGHTNESS);
	for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
	draw_string_in_buffer(SLEEP_MSG, SLEEP_MSG_X, SLEEP_MSG_Y,fonts[1]);
	gfx_mono_ssd1306_put_framebuffer();
}

void enter_power_off_mode()
{
	enter_dim_mode(POWER_OFF_MSG);
}

void enter_sleep_mode()
{
	enter_dim_mode(SLEEP_MSG);
}

void exit_dim_mode()
{
	ssd1306_set_contrast(eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS));
	show_current_menu(true);
}

void exit_power_off_mode()
{
	exit_dim_mode();
}

void exit_sleep_mode()
{
	exit_dim_mode();
}
