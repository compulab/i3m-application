/*
 * sram_handle.h
 *
 *  Created on: Sep 17, 2015
 *      Author: arkadi
 */

#ifndef SRAM_HANDLE_H_
#define SRAM_HANDLE_H_
#include "layout.h"
#include "asf.h"
#include "Fp-utils.h"
#include "gfx/gfx_utils.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "work-queue/work.h"
#include "debug.h"

#define CPU_FQ_MSK 				0x1f00
#define CPU_FQ_MSB_MSK 			0x1f
#define CPU_FQ_LSB_MSK 			0xff
#define VALID_CPU_FQ_MSK		0x80
#define VALID_AMBIENT_MASK 		0x02
#define VALID_GPU_MASK 			0x01
#define LSB_MSK 				0x00ff
#define MSB_MSK 				0x0300
#define HDD_SZ_MSK				0x03
#define HDD_SZ_UNIT_MSK			0x04
#define HDD_SZ_STATUS_MSK		0x80
#define MEM_SZ_STATUS_LSB_MSK	0x80
#define MEM_SZ_STATUS_MSB_MSK	0x08
#define MEM_SZ_LSB_MSK			0x70
#define MEM_SZ_MSB_MSK			0x07
#define RSTUSB_MSK 				0x02
#define RST_MSK 				0x01
#define RST_MSK 				0x01
#define WEN_MSK 				0x80
#define DEFAULT_DATA			0x00
#define POWER_ON_BM				0x07
#define POWER_STR_BM			0x03
#define POWER_STD_BM			0x01
#define POWER_OFF_BM			0x00
#define REQUEST_ENABLE			0x01
#define REQUEST_CPUF			0x02
#define REQUEST_CPUT			0x04
#define REQUEST_GPUT			0x08

void handle_sram_read_request(uint8_t read_address, uint8_t *data);

int handle_sram_write_request(uint8_t write_address, uint8_t data);

void sram_handle_init();

void dmi_init();

enum dmi_state_t {
	DMI_IDLE,
	DMI_NAME_WRITE,
	DMI_VALUE_WRITE
};

#endif /* SRAM_HANDLE_H_ */
