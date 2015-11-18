/*
 * rtc.h
 *
 *  Created on: Aug 31, 2015
 *      Author: arkadi
 */

#ifndef RTC_RTC_H_
#define RTC_RTC_H_

#define RTC_Busy()               ( RTC.STATUS & RTC_SYNCBUSY_bm )
#define RTC_CYCLES_1S     1024


extern long rtc_time;

void rtc_init();

#endif /* RTC_RTC_H_ */
