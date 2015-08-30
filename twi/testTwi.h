/*
 * testTwi.h
 *
 *  Created on: Aug 10, 2015
 *      Author: arkadi
 */

#ifndef TWI_TESTTWI_H_
#define TWI_TESTTWI_H_
#include "twi-master.h"

void send_write_package();

void send_read_package(handle_data_func update_data);

#endif /* TWI_TESTTWI_H_ */
