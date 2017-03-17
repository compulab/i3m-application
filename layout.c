/*
 * layout.c
 *
 *  Created on: Dec 6, 2015
 *      Author: arkadi
 */

#include "layout.h"

union i2c_buffer i2c_buffer = {
		.layout.sig = {'C', 'L', 'F', 'P'},
		.layout.layout_ver = 0x01,
		.layout.major_rev = 0x00,
		.layout.minor_rev = 0x01,
};
