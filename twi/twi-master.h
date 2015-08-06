/*
 * twi-master.h
 *
 *  Created on: Aug 3, 2015
 *      Author: arkadi
 */

#ifndef TWI_TWI_MASTER_H_
#define TWI_TWI_MASTER_H_

#include "defines.h"

typedef void (*handleDataFunc)(uint8_t);

#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)

typedef struct TWI_PACKAGE_t{
	uint8_t slaveAddress;
	uint8_t buffer[PAGESIZE];
	uint8_t length;
	bool writeRequest;
	handleDataFunc handleData;
} twi_package;

void SendPackage(twi_package *package);
void InterruptHandler();
void twi_master_init();

void handleReadInterrupt(void);
void handleWriteInterrupt(void);

#endif /* TWI_TWI_MASTER_H_ */
