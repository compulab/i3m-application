/*
 * sram_handle.h
 *
 *  Created on: Sep 17, 2015
 *      Author: arkadi
 */

#ifndef TWI_SRAM_HANDLE_H_
#define TWI_SRAM_HANDLE_H_
#include "../layout.h"
#include "../asf.h"
#include "eeprom.h"
#include "../Fp-utils.h"

void handle_sram_read_request(uint8_t read_address, uint8_t *data);

void handle_sram_write_request(uint8_t write_address, uint8_t data);

#endif /* TWI_SRAM_HANDLE_H_ */
