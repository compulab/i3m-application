/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  XMEGA Timer/Counter driver source file.
 *
 *      This file contains the function implementations the XMEGA Timer/Counter
 *      driver.
 *
 *      The driver is not intended for size and/or speed critical code, since
 *      most functions are just a few lines of code, and the function call
 *      overhead would decrease code performance. The driver is intended for
 *      rapid prototyping and documentation purposes for getting started with
 *      the XMEGA Timer/Counter module.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call.
 *
 *      Several functions use the following construct:
 *          "some_register = ... | (some_parameter ? SOME_BIT_bm : 0) | ..."
 *      Although the use of the ternary operator ( if ? then : else ) is discouraged,
 *      in some occasions the operator makes it possible to write pretty clean and
 *      neat code. In this driver, the construct is used to set or not set a
 *      configuration bit based on a boolean input parameter, such as
 *      the "some_parameter" in the example above.
 *
 * \par Application note:
 *      AVR1306: Using the XMEGA Timer/Counter
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Revision: 1569 $
 * $Date: 2008-04-22 13:03:43 +0200 (ti, 22 apr 2008) $  \n
 *
 * Copyright (c) 2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "tc.h"

int tc_counter;

int standby_counter;

bool cmp_enable;

void tc_init()
{
	tc_counter = 0;
	standby_counter = 0;
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;
	TCC0.CTRLB = (TCC0.CTRLB & ~TC0_WGMODE_gm) | TC_WGMODE_NORMAL_gc;
	TCC0.CTRLE = TC_BYTEM_NORMAL_gc;
	TCC0.INTCTRLA = TC_OVFINTLVL_HI_gc;
	TCC0.INTCTRLB = TC_CCAINTLVL_HI_gc;
	TCC0.PER = TIMER_MAX_VALUE;
	tc_cmp_disable();
}

void tc_cmp_enable()
{
	TCC0.CTRLB |= TC0_CCAEN_bm;
	cmp_enable = true;
}

bool is_tc_cmp_enable()
{
	return cmp_enable;
}

void tc_cmp_disable()
{
	TCC0.CTRLB &= ~TC0_CCAEN_bm;
	cmp_enable = false;
}

void tc_set_cmp_value(uint16_t value)
{
	TCC0.CCA = value;
}

uint16_t tc_get_div(void)
{
	switch (TCC0.CTRLA & TC_CLKSEL_DIV1024_gc) {
	case TC_CLKSEL_DIV1_gc:
		return 1;
	case TC_CLKSEL_DIV2_gc:
		return 2;
	case TC_CLKSEL_DIV4_gc:
		return 4;
	case TC_CLKSEL_DIV8_gc:
		return 8;
	case TC_CLKSEL_DIV64_gc:
		return 64;
	case TC_CLKSEL_DIV256_gc:
		return 256;
	case TC_CLKSEL_DIV1024_gc:
		return 1024;
	}

	return 1;
}
