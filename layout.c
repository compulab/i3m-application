/*
 * layout.c
 *
 *  Created on: Dec 6, 2015
 *      Author: arkadi
 */

#include "layout.h"

union layout_t layout = {
		.l.sig = {'C', 'L', 'F', 'P'},
		.l.layout_ver = 0x01,
		.l.major_rev = 0x00,
		.l.minor_rev = 0x01,
};
