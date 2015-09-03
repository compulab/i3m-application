/*
 * twi-master.c
 *
 *  Created on: Aug 4, 2015
 *      Author: arkadi
 */

#include "twi_master.h"

#include "../debug.h"


struct twi_package current_package;
int send_index;
int retrying_count;
bool is_exacuting;

void twi_master_init()
{
	TWI_MASTER_BASE.CTRLA = TWI_MASTER_INTLVL_MED_gc | TWI_MASTER_WIEN_bm |TWI_MASTER_RIEN_bm | TWI_MASTER_ENABLE_bm;
	TWI_MASTER_BASE.CTRLB = TWI_MASTER_TIMEOUT_50US_gc;
	TWI_MASTER_BASE.BAUD =  TWI_BAUDSETTING;
	TWI_MASTER_BASE.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

void reset(){
	send_index = 0;
	retrying_count = 0;
	is_exacuting = false;
}

void send_address(){
	uint8_t address =  current_package.slave_address << 1;
	TWI_MASTER_BASE.ADDR = address;
	TWI_MASTER_BASE.CTRLC = TWI_MASTER_CMD_REPSTART_gc;
}

void send_read_request(){
	uint8_t address =  current_package.slave_address << 1;
	address = address | READ_REQUEST;
	TWI_MASTER_BASE.ADDR = address;
	TWI_MASTER_BASE.CTRLC = TWI_MASTER_CMD_REPSTART_gc;
}

void send_package(struct twi_package *package)
{
	reset();
	current_package = *package;
	send_address();
}

void send_ack()
{
	TWI_MASTER_BASE.CTRLC = TWI_MASTER_ACKACT_bm;
}

void send_stop()
{
	TWI_MASTER_BASE.CTRLC = TWI_MASTER_CMD_gm;
}

void send_next_data()
{
	uint8_t data = current_package.buffer[send_index++];
	TWI_MASTER_BASE.DATA = data;
	send_ack();
}


void handle_get_data()
{
	uint8_t data = TWI_MASTER_BASE.DATA;
	current_package.handle_data_received(data);
	send_stop();
}

void handle_send_data()
{
	if (current_package.length > send_index){
		send_next_data();
	} else {
		send_stop();
		if (!current_package.is_write_request) send_read_request();
	}
	is_exacuting = true;
}

void handle_error()
{
	if (retrying_count < MAX_RETRY){
		retrying_count++;
		send_address();
	}
}

void handle_bus_error()
{
	handle_error();
}

void handle_arbitration_error()
{
	handle_error();
}

void handle_address_not_acknowledged(){}

void interrupt_handler()
{
	uint8_t status = TWI_MASTER_BASE.STATUS;
	if ((status & (TWI_MASTER_WIF_bm | TWI_MASTER_ARBLOST_bm)) == (TWI_MASTER_WIF_bm | TWI_MASTER_ARBLOST_bm))
		if ((status & TWI_MASTER_BUSERR_bm) == TWI_MASTER_BUSERR_bm)
			handle_bus_error();
		else
			handle_arbitration_error();
	else if ((status & (TWI_MASTER_WIF_bm | TWI_MASTER_RXACK_bm))==(TWI_MASTER_WIF_bm | TWI_MASTER_RXACK_bm))
		handle_address_not_acknowledged();
	else if ((status & TWI_MASTER_RIF_bm) == TWI_MASTER_RIF_bm)
			handle_get_data();
	else if ((status & TWI_MASTER_WIF_bm) == TWI_MASTER_WIF_bm)
		handle_send_data();
}
