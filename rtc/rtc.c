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
	OSC.CTRL |= OSC_RC32KEN_bm;
	do {} while ( ( OSC.STATUS & OSC_RC32KRDY_bm ) == 0);
	CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;
	do {} while ( RTC_Busy() );
	RTC.PER = RTC_CYCLES_1S - 1;
	RTC.CNT = 0;
	RTC.COMP = 0;
	RTC.CTRL = ( RTC.CTRL & ~RTC_PRESCALER_gm ) | RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = ( RTC.INTCTRL & ~( RTC_COMPINTLVL_gm | RTC_OVFINTLVL_gm ) ) |
						  RTC_OVFINTLVL_LO_gc | RTC_COMPINTLVL_OFF_gc;
}

