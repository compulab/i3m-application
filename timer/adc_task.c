#include "scheduler.h"
#include "../Fp-utils.h"

static struct work adc_work = { .do_work = update_adc };

#define UPDATE_ADC_SEC			1
static double adc_get_recur_period(void)
{
	return UPDATE_ADC_SEC;
}

struct scheduler_task adc_tick_task = {
    .work = &adc_work,
    .get_recur_period = adc_get_recur_period,
};
