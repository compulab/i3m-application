/*
 * screen_saver.h
 *
 * Created: 3/18/2017 12:21:13 AM
 *  Author: Nikita
 */


#ifndef SCREEN_SAVER_SCREEN_H_
#define SCREEN_SAVER_SCREEN_H_

#include "gfx/gfx_components/gfx_information.h"
#include <stdio.h>

#define SCREEN_SAVER_SECOND_MIN_VALUE	5
#define SCREEN_SAVER_SECOND_MAX_VALUE	55
#define SCREEN_SAVER_SECOND_JUMP		5

static inline void sprintf_disabled(char *output_string)
{
	sprintf(output_string, "DISABLED");
}

int gfx_information_init_set_screen_saver_type(struct gfx_information *info);
int gfx_information_init_set_screen_saver_time(struct gfx_information *info);
int gfx_information_init_set_screen_saver_enable(struct gfx_information *info);

#endif /* SCREEN_SAVER_SCREEN_H_ */