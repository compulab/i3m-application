/*
 * Fp-utils.h
 *
 *  Created on: Aug 26, 2015
 *      Author: arkadi
 */

#ifndef FP_UTILS_H_
#define FP_UTILS_H_

#include <stdio.h>
#include <string.h>
#include "gfx/action_menu/gfx_action_menu.h"
#include "work-queue/work.h"
#include "calendar/calendar.h"
#include "eeprom/eeprom.h"
#include "adc/adc.h"
#include "effects.h"
#include "layout.h"
#include "debug.h"
#include "def.h"

#define AMBIENT_TWI_ADDRESS  		0x4C
#define AMBIENT_TEMPERATURE_ADDRESS 0x00
#define EMPTY_SLOT "Empty"

#define POST_CODE_BIOS_START	0xE1		// BIOS post code that send when BIOS is end and the computer continue boot.
#define	POST_CODE_BIOS_DONE		0xA0 		// BIOS post code that send when BIOS is end and the computer continue boot.


#ifndef APPLICATION_VER_MSB
#define APPLICATION_VER_MSB 0
#endif

#ifndef APPLICATION_VER_LSB
#define APPLICATION_VER_LSB 1
#endif

#define SCREEN_SAVER_SECOND_MIN_VALUE	5
#define SCREEN_SAVER_SECOND_MAX_VALUE	55
#define SCREEN_SAVER_SECOND_JUMP		5

#define SCREEN_SAVER_MINUTE_MIN_VALUE	1
#define SCREEN_SAVER_MINUTE_MAX_VALUE	59
#define SCREEN_SAVER_MINUTE_JUMP		2

#define SCREEN_SAVER_HOUR_MIN_VALUE		1
#define SCREEN_SAVER_HOUR_MAX_VALUE		5
#define SCREEN_SAVER_HOUR_JUMP			1

#define SEPERATE_LINE_Y					50

extern enum display_state display_state;

extern enum information_type update_information_type;

extern struct calendar_date computer_date_time;

extern bool screen_saver_mode_enabled;

void print_work_count(void *);

uint8_t get_brightness_level(void);

void update_screen(void *);

void update_requests(void *);

void update_ambient_temp(void *);

void update_computer_state(void);

void update_power_state(void);

#endif /* FP_UTILS_H_ */
