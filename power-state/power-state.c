/*
 * power-state.c
 *
 *  Created on: Aug 12, 2015
 *      Author: arkadi
 */

#include "power-state.h"
#include "../debug.h"

power_state currentPowerState = POWER_ON;

void changePowerState(){
	if (gpio_pin_is_low(FP_S5)){
		currentPowerState = POWER_OFF;
	} else if (gpio_pin_is_low(FP_S4)){
		currentPowerState = POWER_STD;
	} else if (gpio_pin_is_low(FP_S3)){
		currentPowerState = POWER_STR;
	}	else{
		currentPowerState = POWER_ON;
	}
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
