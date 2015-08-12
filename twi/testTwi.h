/*
 * testTwi.h
 *
 *  Created on: Aug 10, 2015
 *      Author: arkadi
 */

#ifndef TWI_TESTTWI_H_
#define TWI_TESTTWI_H_
#include "twi-master.h"

void sendWritePackage();

void sendReadPackage(handleDataFunc updateData);

#endif /* TWI_TESTTWI_H_ */
