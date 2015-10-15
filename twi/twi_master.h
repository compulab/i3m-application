/*
 * twi-master.h
 *
 *  Created on: Aug 3, 2015
 *      Author: arkadi
 */

#ifndef TWI_TWI_MASTER_H_
#define TWI_TWI_MASTER_H_

#include <stdint.h>
#include <stdbool.h>
#include "../config/conf_twi.h"
#include <avr/io.h>

#define MAX_RETRY 2
#define READ_REQUEST 0x01
#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)

typedef void (*handle_data_func)(uint8_t);

struct twi_package{
	uint8_t slave_address;
	uint8_t buffer[EEPROM_BYTES_IN_PAGE];
	uint8_t length;
	bool is_write_request;
	handle_data_func handle_data_received;
};

void send_package(struct twi_package *package);

void interrupt_handler();

void twi_master_init();

#endif /* TWI_TWI_MASTER_H_ */
