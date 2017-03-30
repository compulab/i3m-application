/*
 * Fp-utils.h
 *
 *  Created on: Aug 26, 2015
 *      Author: arkadi
 */

#ifndef FP_UTILS_H_
#define FP_UTILS_H_

#include <stdio.h>
#include "work-queue/work.h"
#include "calendar/calendar.h"
#include "effects.h"
#include "layout.h"
#include "debug.h"
#include "def.h"

#define AMBIENT_TWI_ADDRESS  		0x4C
#define AMBIENT_TEMPERATURE_ADDRESS 0x00

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

#define SEPERATE_LINE_Y					50

extern enum display_state display_state;

extern enum information_type update_information_type;

extern struct calendar_date computer_date_time;

#endif /* FP_UTILS_H_ */
