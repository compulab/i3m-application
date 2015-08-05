/*
 * twi-slave.h
 *
 *  Created on: Jul 21, 2015
 *      Author: arkadi
 */

#ifndef TWI_SLAVE_H
#define TWI_SLAVE_H

#include <stdint.h>
#include <stdbool.h>


/* Transaction status defines.*/
#define TWIS_STATUS_READY                0x00
#define TWIS_STATUS_BUSY                 0xff

void twi_slave_init(void);
void clear_addresses(void);
void TWI_EndTransmission(void);
void TWI_ACK(void);
void TWI_NACK(void);
void TWI_CLEAR_APIF(void);
void TWI_CLEAR_DIF(void);
void handleRead(uint8_t address);
void handleWrite(uint8_t data);
void TWI_SlaveAddressMatchHandler(void);
void TWI_SlaveReadDataHandler(void);
void TWI_SlaveWriteDataHandler(void);
void TWI_SaveAddress(void);
void TWI_SlaveStopHandler(void);
void TWI_interrupt_handler(void);

#endif /* TWI_SLAVE_H */
