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
#include "../gfx/gfx_utils.h"
#include "../gfx/action_menu/gfx_action_menu.h"
#include "../work-queue/work.h"
#include "../debug.h"
#include "twi_slave.h"

void handle_sram_read_request(uint8_t read_address, uint8_t *data);

int handle_sram_write_request(uint8_t write_address, uint8_t data);

void sram_handle_init();

void dmi_init();

enum dmi_state_t {
	DMI_IDLE,
	DMI_NAME_WRITE,
	DMI_VALUE_WRITE
};

#endif /* TWI_SRAM_HANDLE_H_ */
