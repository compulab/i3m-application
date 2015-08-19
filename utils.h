/*
 * utils.h
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "asf.h"
#include "adc/adc.h"

typedef void (*func_ptr)(char **);

typedef enum INFORMATION_t {
	SHOW_POWER_STATE,
	SHOW_VOLTAGE,
	SHOW_CPU_TEMPERTURE,
	SHOW_GPU_TEMPERTURE
} information_type;


typedef enum POWER_STATE_T {
	POWER_ON,
	POWER_STR,
	POWER_STD,
	POWER_OFF
} power_state;

extern power_state currentPowerState;

void changePowerState();

void showState(char ** data);


void updateDataByType(information_type type, char ** data);


#endif /* UTILS_H_ */
