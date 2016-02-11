/*
 * adc.c
 *
 *  Created on: Aug 12, 2015
 *      Author: arkadi
 */
#include "adc.h"
#include <stdio.h>


//***ADC configuration
#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0

#define POWER_COUNT		1000
#define POWER_GAIN		0
#define POWER_VCC		3.3

static long current_power;

void adc_init()
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;


	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_12, ADC_REFSEL_INTVCC2_gc);  //signed, 12-bit resolution, VREF = VCC/2
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SINGLE, 1, 0);
	adc_set_clock_rate(&adc_conf, 100000UL); // 100KHz. Up to 200000UL=200KHz

	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_PAD_GND, POWER_GAIN); // GAIN = 0.5
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	adc_enable(&MY_ADC);
}

long adc_avg()
{
	uint32_t i;
	uint16_t adca1_result = 0;
	uint32_t power_sum = 0;
	for (i=0; i < POWER_COUNT ;i++) {
		adc_start_conversion(&MY_ADC, MY_ADC_CH);  //one conversion begins
		adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
		adca1_result = adc_get_result(&MY_ADC, MY_ADC_CH) & 0x3FFF;
		power_sum = power_sum + adca1_result;
	}

	return power_sum / (i);
}

void set_power_data(char *str)
{
	long avg = adc_avg();
	long new_power = avg * 0.06378 * 2; //0.06378 /// (gain * gain);// * 158.34);//* 0.177; //*0.07731r P =79.17 * V_adc , P/GAIN = 158.34 //
	new_power = new_power * 0.778 + 15.8;
	if (new_power > 0)
		current_power = new_power;
	sprintf(str, "%ld W", current_power);
}
