/*
 * effects.c
 *
 *  Created on: Jan 30, 2016
 *      Author: arkadi
 */

#include "effects.h"
#define SLEEP_BRIGHTNESS 	100
#define SLEEP_MSG 			"SLEEP"
#define HIBERNATE_MSG		"HIBERNATE"
#define POWER_OFF_MSG		"OFF"
//#define DIM_FONT_ID			GLCD_FONT_SYSFONT_5X7
#define MSG_X				GFX_MONO_LCD_WIDTH / 2
#define MSG_Y				20

static void enter_dim_mode(char *msg)
{
	uint16_t font_id = fonts_size > 1 ? 2 : GLCD_FONT_SYSFONT_5X7;
	display_state = DISPLAY_DIM;
	ssd1306_set_contrast(SLEEP_BRIGHTNESS);
	for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
	uint8_t msg_x = MSG_X - ((strlen(msg) * (get_font_by_type(font_id))->width) / 2);
	draw_string_in_buffer(msg, msg_x, MSG_Y, get_font_by_type(font_id), 0);
	gfx_mono_ssd1306_put_framebuffer();
}

void enter_power_off_mode(void)
{
	enter_dim_mode(POWER_OFF_MSG);
}

void enter_sleep_mode(void)
{
	enter_dim_mode(SLEEP_MSG);
}

void enter_hibernate_mode(void)
{
	enter_dim_mode(HIBERNATE_MSG);
}

void enter_power_on_mode(void)
{
	ssd1306_set_contrast(eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS));
	show_logo();
}

static void exit_dim_mode(void)
{
	show_current_menu(true);
}

void exit_power_off_mode(void)
{
	exit_dim_mode();
}

void exit_sleep_mode(void)
{
	exit_dim_mode();
}
