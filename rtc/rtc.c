/*
 * rtc.c
 *
 *  Created on: Aug 31, 2015
 *      Author: arkadi
 */

#include "rtc.h"
#include "work-queue/work.h"
#include "scheduler/scheduler.h"
#include "ASF/common/services/calendar/calendar.h"
#include "asf.h"
#include <avr/io.h>
#include <stdbool.h>

#define RTC_CYCLES_1S     (1024 - 1)

void rtc_init()
{
	RTC.PER = RTC_CYCLES_1S;
	RTC.CNT = 0;
	RTC.COMP = 0;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_OVFINTLVL_HI_gc | RTC_COMPINTLVL_OFF_gc;
}

struct calendar_date computer_date_time = {
    .second = 40,
    .minute = 02,
    .hour = 11,
    .date = 26,
    .month = 11,
    .year = 2015
};

static void rtc_time_task(void *data)
{
	calendar_add_second_to_date(&computer_date_time);
}

static struct work time_work = { .do_work = rtc_time_task };

static bool rtc_can_schedule = false;
void switch_rtc_interrupt_schedule(bool on)
{
	rtc_can_schedule = on;
}

ISR(RTC_OVF_vect)
{
	if (rtc_can_schedule)
		insert_work(&time_work);
}
