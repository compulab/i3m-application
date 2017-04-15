/*
 * twi-slave.h
 *
 *  Created on: Jul 21, 2015
 *      Author: arkadi
 */

#ifndef TWI_SLAVE_H
#define TWI_SLAVE_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include "config/conf_twi.h"

/* Transaction status defines.*/
#define TWIS_STATUS_READY                0x00
#define TWIS_STATUS_BUSY                 0xff

extern bool is_twi_busy;
void twi_slave_init(void);

void twi_disable(void);

void twi_enable(void);

void twi_slave_interrupt_handler(void);

#endif /* TWI_SLAVE_H */
