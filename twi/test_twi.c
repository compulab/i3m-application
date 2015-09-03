/*
 * testTwi.c
 *
 *  Created on: Aug 10, 2015
 *      Author: arkadi
 */

#include "test_twi.h"

void send_write_package()
{
	struct twi_package package_to_write = {
			.slave_address = TWI_EEPROM_ADDRESS,
			.buffer[0] =  0x43,
			.buffer[1] = 0x3f,
			.buffer[2] = 0xe2,
			.buffer[3] = 0x3e,
			.buffer[4] = 0x4f,
			.buffer[5] = 0x53,
			.length = 6,
			.is_write_request = true
	};
	send_package(&package_to_write);
}

void send_read_package(handle_data_func update_data)
{
	struct twi_package package_to_read = {
			.slave_address = TWI_EEPROM_ADDRESS,
			.buffer[0] =  0x05,
			.length = 1,
			.is_write_request = false,
			.handle_data_received = update_data
	};
	send_package(&package_to_read);
}
