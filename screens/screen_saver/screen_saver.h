/*
 * screen_saver.h
 *
 * Created: 3/18/2017 12:21:13 AM
 *  Author: Nikita
 */


#ifndef SCREEN_SAVER_H_
#define SCREEN_SAVER_H_

#include <stdio.h>
#include "gfx/gfx_components/gfx_information.h"

#define SCREEN_SAVER_SECOND_MIN_VALUE	5
#define SCREEN_SAVER_SECOND_MAX_VALUE	55
#define SCREEN_SAVER_SECOND_JUMP		5

enum screen_saver_type {
	SCREEN_SAVER_SPLASH,
	SCREEN_SAVER_DASHBOARD,
	SCREEN_SAVER_CLOCK,
	SCREEN_SAVER_TYPE_SIZE,
};

static inline void sprintf_disabled(char *output_string)
{
	sprintf(output_string, "DISABLED");
}

extern bool screen_saver_mode_enabled;

int gfx_information_init_set_screen_saver_type(struct gfx_information *info);
int gfx_information_init_set_screen_saver_time(struct gfx_information *info);
int gfx_information_init_set_screen_saver_enable(struct gfx_information *info);
void enable_screen_saver_mode(void);
void disable_screen_saver_mode(void);

#endif /* SCREEN_SAVER_H_ */