/*
 * screen_saver.h
 *
 * Created: 15-Apr-17 9:25:55 PM
 *  Author: Nikita
 */


#ifndef SCREEN_SAVER_H_
#define SCREEN_SAVER_H_

#include <stdbool.h>

enum screen_saver_type {
	SCREEN_SAVER_SPLASH,
	SCREEN_SAVER_DASHBOARD,
	SCREEN_SAVER_CLOCK,
	SCREEN_SAVER_TYPE_SIZE,
};

extern bool screen_saver_mode_enabled;

void enable_screen_saver_mode(void);
void disable_screen_saver_mode(void);


#endif /* SCREEN_SAVER_H_ */