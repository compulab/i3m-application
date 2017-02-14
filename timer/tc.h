#ifndef TC_DRIVER_H
#define TC_DRIVER_H

#ifndef ASF_H
#include "asf.h"
#endif

#define TIMER_MAX_VALUE		0xffff
extern int tc_counter;

extern int standby_counter;


void tc_init();

bool is_tc_cmp_enable();

void tc_cmp_enable();

void tc_cmp_disable();

void tc_set_cmp_value(uint16_t value);

uint16_t tc_get_div(void);

#endif
