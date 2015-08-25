/*
 * testTwi.c
 *
 *  Created on: Aug 10, 2015
 *      Author: arkadi
 */

#include "testTwi.h"

void sendWritePackage(){
	twi_package packageToWriteSend = {
			.slaveAddress = TWI_WRITE_ADDRESS,
			.buffer[0] =  0x43,
			.buffer[1] = 0x3f,
			.buffer[2] = 0xe2,
			.buffer[3] = 0x3e,
			.buffer[4] = 0x4f,
			.buffer[5] = 0x53,
			.length = 6,
			.writeRequest = true
	};
	SendPackage(&packageToWriteSend);
}

void sendReadPackage(handleDataFunc updateData){
	twi_package packageToRead = {
			.slaveAddress = TWI_READ_ADDRESS,
			.buffer[0] =  0x05,
			.length = 1,
			.writeRequest = false,
			.handleData = updateData
	};
	SendPackage(&packageToRead);
}
