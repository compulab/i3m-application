/*
 * Fp-utils.h
 *
 *  Created on: Aug 26, 2015
 *      Author: arkadi
 */

#ifndef FP_UTILS_H_
#define FP_UTILS_H_

#ifndef LAYOUT_H_
#include "layout.h"
#endif

#ifndef GFX_GFX_ACTION_MENU_H_
#include "gfx/action_menu/gfx_action_menu.h"
#endif

#ifndef DEF_H
#include "def.h"
#endif

#ifndef EEPROM_H
#include "twi/eeprom.h"
#endif

#ifndef ADC_ADC_H_
#include "adc/adc.h"
#endif

#include <stdio.h>
#include <string.h>

#include "calendar/calendar.h"

#include "work-queue/work.h"
#include "debug.h"
#define AMBIENT_TWI_ADDRESS  		0x4C
#define AMBIENT_TEMPERATURE_ADDRESS 0x00
#define EMPTY_SLOT "Empty Slot"
#define BRIGHTNESS_STEP				25
#define MAX_BRIGHTNESS_LEVEL		10
#define HDDTR_BIT					0
#define CPUFR_BIT					1
#define CPUTR_BIT					2
#define GPUTR_BIT					3

extern enum information_type update_information_type;

extern struct calendar_date computer_date_time;

extern bool sleep_mode_enabled;

void enable_screen_saver_mode();

void disable_screen_saver_mode();

bool is_valid_cpu_temp(uint8_t cpu_id);

bool is_valid_cpu_fq(uint8_t cpu_id);

bool is_valid_ambient_temp();

uint8_t get_brightness_level();

bool is_valid_gpu_temp();

bool is_valid_hdd_size(uint8_t hdd_id);

bool is_valid_hdd_temp(uint8_t hdd_id);

bool is_valid_mem(uint8_t mem_id);

void update_information_frame(enum information_type type, bool need_to_update);

void update_ambient_value(uint8_t high_bit);

void update_requests();

void update_ambient_temp();

void update_adc();

extern enum power_state current_power_state;

void update_brightness();

void update_power_state();

void set_state(char *state);

void update_data_by_type(enum information_type type, char *output_str, uint8_t info);

#endif /* FP_UTILS_H_ */
