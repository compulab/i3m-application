/*
 * adc.h
 *
 *  Created on: Aug 12, 2015
 *      Author: arkadi
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#include "../asf.h"
#include "math.h"

extern char current_power_str[7];

void adc_init(void);

void set_power_data(char *str);

#endif /* ADC_ADC_H_ */
