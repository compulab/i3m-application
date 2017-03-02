/*
 * rtc.c
 *
 *  Created on: Aug 31, 2015
 *      Author: arkadi
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "rtc.h"


void rtc_init()
{
	RTC.PER = RTC_CYCLES_1S - 1;
	RTC.CNT = 0;
	RTC.COMP = 0;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_OVFINTLVL_HI_gc | RTC_COMPINTLVL_OFF_gc;
}

