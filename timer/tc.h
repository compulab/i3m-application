#ifndef TC_DRIVER_H
#define TC_DRIVER_H

#include <stdio.h>
#include <stdbool.h>

#define TIMER_MAX_VALUE		0xffff

void tc_init(void);

bool is_tc_cmp_enable(void);

void tc_cmp_enable(void);

void tc_cmp_disable(void);

void tc_set_cmp_value(uint16_t value);

uint16_t tc_get_div(void);

#endif
