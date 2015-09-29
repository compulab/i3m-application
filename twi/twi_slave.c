/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file ********************************************************************
*
* Atmel Corporation
*
* File              : twi-xmega.c
* Compiler          : IAR Embedded Workbench EWAVR C Compiler
* Revision          : $5.51
* Date              : $Date: 23 Aug 11
* Updated by        : $Author: Marv Kausch
*
* Support mail      : avr@atmel.com
*
* Supported devices : The example is written for virtually all xmega devices that have one or more TWI peripherals
*
* AppNote           :avr1327-twi_bootloader-xmega
*
* Description       : TWI driver code C file for AVR1327
*
*
*
****************************************************************************/

/*! \page License
Copyright (c) 2011 Atmel Corporation. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. The name of Atmel may not be used to endorse or promote products derived
from this software without specific prior written permission.

4. This software may only be redistributed and used in connection with an Atmel
AVR product.

THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */


/* Doxygen documentation mainpage ********************************************/
/*! \mainpage
 * \section intro Introduction
 * This documents the software for application note AVR1327.
 *
 * \section compinfo Compilation Info
 * This software was compiled with IAR C/C++ Compiler for AVR
 * the source have to be modified.\n
 *
 * \section deviceinfo Device Info
 * Virtually all XMEGA AVRs that contain one or more TWI peripherals
 *
 * \section codesize Code Size
 * 1382 Bytes if optimized for size High Optimization level, 352 Bytes SRAM bytes, one EEPROM byte
 *
 *
 * \section contactinfo Contact Info
 * For more info about Atmel AVR visit http://www.atmel.com/products/AVR/ \n
 * For application notes visit
 * http://www.atmel.com/dyn/products \n
 * Support mail: avr@atmel.com
 */
#include "twi_slave.h"


#define UNSET_ADDRESS 0x00

static int index =0;
static uint8_t slave_address = UNSET_ADDRESS;
static uint8_t reg_address = UNSET_ADDRESS;
static bool data_sent;
static bool is_read_request;

void twi_slave_init()
{
		TWI_SLAVE_BASE.ADDR = TWI_SLAVE_ADDRESS << 1;
		TWI_SLAVE_BASE.ADDRMASK = TWI_SLAVE_MSK << 1;
        TWI_SLAVE_BASE.CTRLA = TWI_SLAVE_INTLVL_MED_gc |
                TWI_SLAVE_DIEN_bm |
                TWI_SLAVE_APIEN_bm |
                TWI_SLAVE_ENABLE_bm |
				TWI_SLAVE_PIEN_bm;
}

void clear()
{
	index = 0;
}

void clear_addresses()
{
	 slave_address = UNSET_ADDRESS;
	 reg_address = UNSET_ADDRESS;
}

void twi_ack()
{
	TWI_SLAVE_BASE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
}

void twi_nack()
{
	TWI_SLAVE_BASE.CTRLB = TWI_SLAVE_CMD_COMPTRANS_gc;
}

void twi_clear_status_bit (uint8_t bit)
{
	TWI_SLAVE_BASE.STATUS |= bit;
}
void twi_clear_apif()
{
	twi_clear_status_bit(TWI_SLAVE_APIF_bm);
}

void twi_clear_dif()
{
	twi_clear_status_bit(TWI_SLAVE_DIF_bm);
}

void twi_end_transmission()
{
	clear();
}

void twi_handle_read(uint8_t address)
{
	uint8_t data = 0xff;
	if (slave_address == TWI_EEPROM_ADDRESS)
		 data = eeprom_read_byte(address);
	else if (slave_address == TWI_REAL_TIME_ADDRESS)
		handle_sram_read_request(address, &data);
	TWI_SLAVE_BASE.DATA = data;
	++reg_address;
}

void twi_handle_write(uint8_t data)
{
	if (slave_address == TWI_EEPROM_ADDRESS)
		eeprom_write_byte(reg_address,data);
	else if (slave_address == TWI_REAL_TIME_ADDRESS)
		handle_sram_write_request(reg_address,data);

	if (reg_address != DIRECT_TYPE_ADDRESS && reg_address != DIRECT_CONTENT_ADDRESS)
		++reg_address;
}

void twi_slave_address_match_handler()
{
	is_read_request = (TWI_SLAVE_BASE.STATUS & TWI_SLAVE_DIR_bm) == TWI_SLAVE_DIR_bm;
	data_sent = false;
	uint8_t address = (TWI_SLAVE_BASE.DATA >>1);
	if (address != slave_address || !is_read_request){
		clear_addresses();
		slave_address= address;
	}
	twi_clear_apif();
	twi_ack();

}

void twi_slave_stop_handler()
{
	clear();
	twi_clear_apif();
}

void twi_slave_read_data_handler()
{
	uint8_t status = TWI_SLAVE_BASE.STATUS;
	if (((status & TWI_SLAVE_RXACK_bm) == TWI_SLAVE_RXACK_bm) &&
		(data_sent == true)) {
		twi_nack();
	} else {
		twi_handle_read(reg_address);
		data_sent = true;
		twi_clear_dif();
		twi_ack();
	}

}

void twi_slave_write_data_handler()
{
	twi_handle_write(TWI_SLAVE_BASE.DATA);
	twi_clear_dif();
	twi_ack();
}

void twi_save_address()
{
	reg_address = TWI_SLAVE_BASE.DATA;
	if (is_read_request)
		twi_slave_read_data_handler();
	 else
		twi_ack();
	twi_clear_dif();
}

void twi_slave_interrupt_handler()
{
	uint8_t current_status = TWI_SLAVE_BASE.STATUS;
	if (current_status & TWI_SLAVE_BUSERR_bm)     		/* If bus error. */
		twi_end_transmission();
	else if (current_status & TWI_SLAVE_COLL_bm) 		/* If transmit collision. */
		twi_end_transmission();
	else if ((current_status & TWI_SLAVE_APIF_bm) && 	/* If address match. */
			(current_status & TWI_SLAVE_AP_bm))
		twi_slave_address_match_handler();
	else if (current_status & TWI_SLAVE_APIF_bm) 		/* If stop (only enabled through slave read transaction). */
		twi_slave_stop_handler();
	else if (current_status & TWI_SLAVE_DIF_bm) 		/* If data interrupt. */
		if (current_status & TWI_SLAVE_DIR_bm)
			twi_slave_read_data_handler();
		else if (reg_address == UNSET_ADDRESS)
			twi_save_address();
		else
			twi_slave_write_data_handler();
	else 												/* If unexpected state. */
		twi_end_transmission();
}
