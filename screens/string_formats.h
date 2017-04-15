/*
 * string_formats.h
 *
 * Created: 3/23/2017 6:16:30 PM
 *  Author: Nikita
 */


#ifndef STRING_FORMATS_H_
#define STRING_FORMATS_H_

#include <stdio.h>

static inline void sprintf_freq(char *output_str, uint16_t fq)
{
	sprintf(output_str, "%d MHZ", fq);
}

static inline void sprintf_temperature(char *output_str, int8_t temperature)
{
	sprintf(output_str, "%d%c", temperature, (uint8_t)128);
}

static inline void sprintf_inval_data(char *output_str) {
	sprintf(output_str, "-");
}


#endif /* STRING_FORMATS_H_ */