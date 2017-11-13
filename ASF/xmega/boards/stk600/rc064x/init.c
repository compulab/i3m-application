/**
 * \file
 *
 * \brief STK600 with RC064X routing board init.
 *
 * To use this board, define BOARD=STK600_RC064X.
 *
 * Copyright (c) 2010-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include "ASF/common/boards/board.h"
#include "ASF/xmega/utils/compiler.h"
#include "stk600_rc064x.h"
#include "config/conf_board.h"
#include "ASF/common/services/ioport/ioport.h"

void board_init(void)
{
	//Buttons:
	ioport_configure_pin(FP_RIGHT_BUTTON, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	ioport_configure_pin(FP_OK_BUTTON, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	ioport_configure_pin(FP_LEFT_BUTTON, IOPORT_DIR_INPUT | IOPORT_PULL_UP);

	ioport_configure_pin(FP_S3, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	ioport_configure_pin(FP_S4, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	ioport_configure_pin(FP_S5, IOPORT_DIR_INPUT | IOPORT_PULL_UP);

	//Display interface:
	ioport_configure_pin(FP_DC, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(FP_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(FP_RST, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(FP_MOSI, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(FP_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	//This pin needs to be an output in order for SPI Master mode to work:
	ioport_configure_pin(FP_SPI_SS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH); //NOT IN USE

	//Misc.:
	ioport_configure_pin(FP_VCC_EN, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);

	//Debug LEDs:
	ioport_configure_pin(LED3_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED5_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED6_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED7_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);

	//PORTF contains the UI buttons and the power states
	uint8_t sreg = SREG;
	uint8_t power_pin_cfg =(uint8_t)  PORT_ISC_BOTHEDGES_gc | PORT_OPC_PULLUP_gc;
	uint8_t buttons_pin_cfg = PIN4_bm | PIN5_bm | PIN6_bm;
	PORTF.PIN4CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN5CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN6CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN0CTRL = power_pin_cfg;
	PORTF.PIN1CTRL = power_pin_cfg;
	PORTF.PIN2CTRL = power_pin_cfg;
	/* Restore status register. */
	SREG = sreg;
	PORTF.INTCTRL =  PORT_INT0LVL_MED_gc | PORT_INT1LVL_MED_gc;
	PORTF.INT0MASK = PIN0_bm | PIN1_bm |PIN2_bm;
	PORTF.INT1MASK = buttons_pin_cfg;
}
