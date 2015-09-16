/*
 * layout.h
 *
 *  Created on: Sep 3, 2015
 *      Author: arkadi
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_

#define HARD_DISK4_TEMP_ADDRESS 0x25
#define HARD_DISK3_TEMP_ADDRESS 0x24
#define HARD_DISK2_TEMP_ADDRESS 0x23
#define HARD_DISK1_TEMP_ADDRESS 0x22
#define GPU_TEMP_ADDRESS 0x21
#define CPU_TEMP_ADDRESS 0x20
#define EEPROM_ID_ADDRESS 0x1b
#define LAYOUT_VERSION_ADDRESS 0x19
#define BIOS_STATE_ADDRESS 0x18
#define SERIAL_NUMBER_ADDRESS 0x0c
#define PRODUCTION_YEAR_ADDRESS 0x0a
#define PRODUCTION_MONTH_ADDRESS 0x09
#define PRODUCTION_DAY_ADDRESS 0x08
#define MAC_ADDRESS 0x02
#define REVISION_ADDRESS 0x00


#define MAX_HARD_DISKS 4
struct update_information {
	uint8_t gpu_temp;
	uint8_t cpu_temp;
	uint8_t hd_temp[MAX_HARD_DISKS];
	uint16_t ambient_temp;
};

struct update_information computer_data;

#endif /* LAYOUT_H_ */
