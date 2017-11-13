/*
 * power.h
 *
 * Created: 3/23/2017 5:10:28 PM
 *  Author: Nikita
 */


#ifndef POWER_H_
#define POWER_H_

enum power_state {
	POWER_OFF = 0,
	POWER_STD = 1,
	POWER_STR = 3,
	POWER_ON  = 7,
};

extern enum power_state current_power_state;

void power_state_init(void);

#endif /* POWER_H_ */