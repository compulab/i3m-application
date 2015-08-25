/*
 * twi-master.c
 *
 *  Created on: Aug 4, 2015
 *      Author: arkadi
 */

#include "twi-master.h"

#include "../debug.h"


#define MAX_RETRY 2

twi_package currentPackage;
int sendIndex;
int retryingCount;
bool isExacuting;

void twi_master_init()
{
	TWI_MASTER_BASE.CTRLA = TWI_MASTER_INTLVL_MED_gc | TWI_MASTER_WIEN_bm |TWI_MASTER_RIEN_bm | TWI_MASTER_ENABLE_bm;
	TWI_MASTER_BASE.CTRLB = TWI_MASTER_TIMEOUT_50US_gc;
	TWI_MASTER_BASE.BAUD =  TWI_BAUDSETTING;
	TWI_MASTER_BASE.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

void reset(){
	sendIndex = 0 ;
	retryingCount = 0;
	isExacuting = false;
}

void sendAddress(){
	uint8_t address =  currentPackage.slaveAddress << 1;
	TWI_MASTER_BASE.ADDR = address;
	TWI_MASTER_BASE.CTRLC = TWI_MASTER_CMD_REPSTART_gc;
}

void SendReadRequest(){
	uint8_t address =  currentPackage.slaveAddress << 1;
	address = address | 0x01;
	TWI_MASTER_BASE.ADDR = address;
	TWI_MASTER_BASE.CTRLC = TWI_MASTER_CMD_REPSTART_gc;
}

void SendPackage(twi_package *package){
	reset();
	currentPackage = *package;
	sendAddress();
}

void SendAck(){
	TWI_MASTER_BASE.CTRLC = TWI_MASTER_ACKACT_bm;
}

void SendStop(){
	TWI_MASTER_BASE.CTRLC = TWI_MASTER_CMD_gm;
}

void SendNextData(){
	uint8_t data = currentPackage.buffer[sendIndex++];
	TWI_MASTER_BASE.DATA = data;
	SendAck();
}


void handleGetData(){
	uint8_t data = TWI_MASTER_BASE.DATA;
	currentPackage.handleData(data);
	SendStop();
}


void handleSendData(){
	if (currentPackage.length > sendIndex){
		SendNextData();
	} else {
		SendStop();
		if (!currentPackage.writeRequest) SendReadRequest();
	}
	isExacuting = true;
}


void handleError(){
	if (retryingCount < MAX_RETRY){
		retryingCount++;
		sendAddress();
	}
}

void handleBusError(){
	handleError();
}

void handleArbitrationError(){
	handleError();
}

void handleAddressNotAcknowledged(){}

void InterruptHandler(){
	uint8_t status = TWI_MASTER_BASE.STATUS;
	if ((status & (TWI_MASTER_WIF_bm | TWI_MASTER_ARBLOST_bm)) == (TWI_MASTER_WIF_bm | TWI_MASTER_ARBLOST_bm))
		if ((status & TWI_MASTER_BUSERR_bm) == TWI_MASTER_BUSERR_bm)
			handleBusError();
		else
			handleArbitrationError();
	else if ((status & (TWI_MASTER_WIF_bm | TWI_MASTER_RXACK_bm))==(TWI_MASTER_WIF_bm | TWI_MASTER_RXACK_bm))
		handleAddressNotAcknowledged();
	else if ((status & TWI_MASTER_RIF_bm) == TWI_MASTER_RIF_bm)
			handleGetData();
	else if ((status & TWI_MASTER_WIF_bm) == TWI_MASTER_WIF_bm)
		handleSendData();

}
