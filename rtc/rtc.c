/*
 * rtc.c
 *
 *  Created on: Aug 31, 2015
 *      Author: arkadi
 */
#include <avr/io.h>
#include "rtc.h"

#define RTC_CYCLES_1S     (1024 - 1)

void rtc_init()
{
	RTC.PER = RTC_CYCLES_1S;
	RTC.CNT = 0;
	RTC.COMP = 0;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_OVFINTLVL_HI_gc | RTC_COMPINTLVL_OFF_gc;
}

