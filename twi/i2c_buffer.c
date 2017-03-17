/*
 * i2c_buffer.c
 *
 *      Author: Nikita Kiryanov
 */

#include "i2c_buffer.h"

union i2c_buffer i2c_buffer = {
	.layout.sig = {'C', 'L', 'F', 'P'},
	.layout.layout_ver = 0x01,
	.layout.major_rev = 0x00,
	.layout.minor_rev = 0x01,
};
