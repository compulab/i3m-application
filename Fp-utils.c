/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"

enum power_state current_power_state = POWER_ON;

void update_power_state(){
	if (gpio_pin_is_low(FP_S5))
		current_power_state = POWER_OFF;
	else if (gpio_pin_is_low(FP_S4))
		current_power_state = POWER_STD;
	else if (gpio_pin_is_low(FP_S3))
		current_power_state = POWER_STR;
	else
		current_power_state = POWER_ON;
}

void show_state(char **data){
	char *state = "";
	switch (current_power_state){
	case POWER_ON:
		state = "Computer is on";
		break;
	case POWER_STR:
		state = "Sleep mode";
		break;
	case POWER_STD:
		state = "Hibernate mode";
		break;
	case POWER_OFF:
		state = "Computer off";
		break;
	}
	*data = state;
}

void update_data_by_type(enum information_type type, char **data){
	func_ptr ptr= NULL;
	switch (type){
	case SHOW_VOLTAGE:
		ptr = setPowerString;
		break;
	case SHOW_POWER_STATE:
		ptr = show_state;
		break;
	case SHOW_CPU_TEMPERTURE:
	case SHOW_GPU_TEMPERTURE:
		break;
	}
	if (ptr != NULL)
		ptr(data);
}
