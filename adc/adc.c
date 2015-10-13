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

long current_power;
char resString[7];
float gain = 0.5;
float vcc = 3.3;

void adc_init()
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;


	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_12, ADC_REFSEL_INTVCC2_gc);  //signed, 12-bit resolution, VREF = VCC/2
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SINGLE, 1, 0);
	adc_set_clock_rate(&adc_conf, 100000UL); // 100KHz. Up to 200000UL=200KHz

	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_INTERNAL_GND, gain); // GAIN = 1
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	adc_enable(&MY_ADC);
}

long adc_avg()
{
	int i;
	long power_avg =0;
	for (i=0; i < 100 ;i++){
		adc_start_conversion(&MY_ADC, MY_ADC_CH);  //one conversion begins
	//	LED_On(LED3_GPIO);
//		adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
	//	LED_Off(LED3_GPIO);
		int adca1_result = adc_get_result(&MY_ADC, MY_ADC_CH);
		power_avg = power_avg+adca1_result;
	}
	return power_avg / (i);
}

void set_power_data(char *str)
{

    current_power= adc_avg() * 0.127 ; /// (gain * gain);// * 158.34);//* 0.177; //*0.07731r P =79.17 * V_adc , P/GAIN = 158.34
    sprintf(str, "%ld W" ,current_power);
}
