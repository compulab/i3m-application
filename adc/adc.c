/*
 * adc.c
 *
 *  Created on: Aug 12, 2015
 *      Author: arkadi
 */

#include "adc.h"
#include "scheduler/scheduler.h"
#include "work-queue/work.h"
#include "asf.h"
#include <stdio.h>
#include <string.h>

//***ADC configuration
#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0

#define POWER_COUNT		1000

long current_power;
int32_t power_sum;

void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;

	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_12, ADC_REFSEL_INTVCC2_gc);  //signed, 12-bit resolution, VREF = VCC/2
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SINGLE, 1, 0);
	adc_set_clock_rate(&adc_conf, 100000UL); // 100KHz. Up to 200000UL=200KHz

	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_INTERNAL_GND, 0);
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	adc_enable(&MY_ADC);
}

static double adc_avg(void)
{
	int16_t i;
	int16_t adca1_result = 0;
	power_sum = 0;
	for (i = 0; i < POWER_COUNT; i++) {
		adc_start_conversion(&MY_ADC, MY_ADC_CH);
		adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
		adca1_result = adc_get_result(&MY_ADC, MY_ADC_CH);
		power_sum = power_sum + adca1_result;
	}

	return power_sum / i;
}

static void update_adc(void *data)
{
	double avg = adc_avg();
	long power = avg * 0.10137 + 2.9;
	if (power >= 6 && power <= 300) {
		computer_data.details.adc = avg;
		computer_data.details.adc_set = 1;
		current_power = power;
	} else if (power > 0) {
		computer_data.details.adc_set = 1;
		computer_data.details.adc = 0;
	} else {
		computer_data.details.adc_set = 0;
	}
}

static struct work adc_work = { .do_work = update_adc };

static double adc_get_recur_period(void)
{
	return 1;
}

struct scheduler_task adc_tick_task = {
	.work = &adc_work,
	.get_recur_period = adc_get_recur_period,
};
