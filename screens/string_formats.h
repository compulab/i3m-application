/*
 * string_formats.h
 *
 * Created: 3/23/2017 6:16:30 PM
 *  Author: Nikita
 */


#ifndef STRING_FORMATS_H_
#define STRING_FORMATS_H_

#include <stdio.h>

static inline void set_fq_string(char *str, uint16_t fq)
{
	sprintf(str, "%d MHZ", fq);
}

static inline void sprintf_temperature(char *str, int8_t temperature)
{
	sprintf(str, "%d%c", temperature, (uint8_t)128);
}

static inline void sprintf_inval_data(char *str) {
	sprintf(str, "-");
}

#endif /* STRING_FORMATS_H_ */