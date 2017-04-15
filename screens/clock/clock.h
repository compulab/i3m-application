/*
 * clock.h
 *
 * Created: 15-Apr-17 8:19:11 PM
 *  Author: Nikita
 */


#ifndef CLOCK_H_
#define CLOCK_H_

#include "gfx/gfx_components/gfx_information.h"

int gfx_information_init_show_rtc_hour(struct gfx_information *info);
int gfx_information_init_show_rtc_min(struct gfx_information *info);
int gfx_information_init_show_rtc_sec(struct gfx_information *info);


#endif /* CLOCK_H_ */