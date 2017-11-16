/*
 * sleep.c
 *
 * Created: 16-Nov-17 5:35:13 PM
 *  Author: Nikita
 */

#include "work-queue/work.h"
#include "asf.h"
#include <stdbool.h>

/*
 * Sleep function makes the system Idle for timeout_us microseconds or until wakeup flag will be set
 */
bool sleep_interuptable(uint32_t timeout_us)
{
	for (uint32_t us = 0; us < timeout_us && !wakeup; us++)
		delay_us(1);

	return wakeup;
}