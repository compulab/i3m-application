/*
 * testTwi.c
 *
 *  Created on: Aug 10, 2015
 *      Author: arkadi
 */

#include "testTwi.h"

void sendWritePackage(){
	twi_package packageToWriteSend = {
			.slaveAddress = 0x32,
			.buffer[0] =  0x03,
			.buffer[1] = 0x3f,
			.buffer[2] = 0xe2,
			.buffer[3] = 0x2d,
			.length = 4,
			.writeRequest = true
	};
	SendPackage(&packageToWriteSend);
}

void sendReadPackage(handleDataFunc updateData){
	twi_package packageToRead = {
			.slaveAddress = 0x31,
			.buffer[0] =  0x03,
			.length = 1,
			.writeRequest = false,
			.handleData = updateData
	};
	SendPackage(&packageToRead);
}
