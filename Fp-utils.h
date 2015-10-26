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

#define AMBIENT_TWI_ADDRESS  		0x4C
#define AMBIENT_TEMPERATURE_ADDRESS 0x00
#define EMPTY_SLOT "Empty Slot"

void update_information_frame(enum information_type type, bool need_to_update);

void update_ambient_value(uint8_t high_bit);

void update_ambient_temp();

void update_adc();

extern enum power_state current_power_state;

void update_power_state();

void set_state(char *state);

void update_data_by_type(enum information_type type, char *output_str, uint8_t info);

#endif /* FP_UTILS_H_ */
