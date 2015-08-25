/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"

power_state currentPowerState = POWER_ON;

void changePowerState(){
	if (gpio_pin_is_low(FP_S5))
		currentPowerState = POWER_OFF;
	else if (gpio_pin_is_low(FP_S4))
		currentPowerState = POWER_STD;
	else if (gpio_pin_is_low(FP_S3))
		currentPowerState = POWER_STR;
	else
		currentPowerState = POWER_ON;
}

void showState(char ** data){
	char * state = "";
	switch (currentPowerState){
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

void updateDataByType(information_type type, char ** data){
	func_ptr ptr= NULL;
	switch (type){
	case SHOW_VOLTAGE:
		ptr = setPowerString;
		break;
	case SHOW_POWER_STATE:
		ptr = showState;
		break;
	case SHOW_CPU_TEMPERTURE:
	case SHOW_GPU_TEMPERTURE:
		break;
	}
	if (ptr != NULL)
		ptr(data);
}
