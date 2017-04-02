/*
 * layout.h
 *
 *  Created on: Sep 3, 2015
 *      Author: arkadi
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_

#include <stdbool.h>
#include <stdint.h>

#define APPLICATION_VER_MSB_EEPROM_ADDRESS	0x102
#define APPLICATION_VER_LSB_EEPROM_ADDRESS	0x103
#define BOOTLOADER_MAGIC_EEPROM_ADDRESS 	0x110

#define BRIGHTNESS_DEFAULT					0xee

#define SCREEN_SAVER_DEFAULT_MODE			0
#define	SCREEN_SAVER_DEFAULT_ENABLE			1
#define SCREEN_SAVER_DEFAULT_UNIT			0
#define SCREEN_SAVER_DEFAULT_TIME_PERIOD	10
#define	SCREEN_SAVER_TIME_DEFAULT			(SCREEN_SAVER_DEFAULT_UNIT) << 6 | SCREEN_SAVER_DEFAULT_TIME_PERIOD
#define SCREEN_SAVER_CONFIGURATION_DEFAULT 	(SCREEN_SAVER_DEFAULT_MODE) << 2 | SCREEN_SAVER_DEFAULT_ENABLE


#define EEPROM_DMI_COUNT 					0x180
#define EEPROM_DMI_START 					0x181

#define BOOTLOADER_APPLICATION_START		0x10

#define DEFAULT_SCREEN_SAVER_TIME			10

#define MAX_HDD 							8
#define MAX_CPU								8
#define MAX_MEMORY_SLOT						4

#define CPU_FQ_VALID_BIT					0x80

struct direct_string_item {
	char *type;
	char *content;
	uint16_t backup_addr;
	struct direct_string_item *next;
};

enum computer_state_t {
	COMPUTER_OFF,
	COMPUTER_ON,
	COMPUTER_IN_BIOS,
	COMPUTER_IN_OS,
	COMPUTER_DAEMON_WORK,
};

union updatable_info {
	struct {
		uint8_t post_code_lsb;
		uint8_t post_code_msb;
		uint8_t power_state:3;
		uint8_t power_state_reserved:5;
		uint8_t cpu_temp[MAX_CPU];
		uint8_t cpu0_temp_set:1;
		uint8_t cpu1_temp_set:1;
		uint8_t cpu2_temp_set:1;
		uint8_t cpu3_temp_set:1;
		uint8_t cpu4_temp_set:1;
		uint8_t cpu5_temp_set:1;
		uint8_t cpu6_temp_set:1;
		uint8_t cpu7_temp_set:1;
		uint8_t	gpu_temp;
		uint8_t ambient_temp;
		uint8_t hdd_temp[MAX_HDD];
		uint8_t hdd0_temp_set:1;
		uint8_t hdd1_temp_set:1;
		uint8_t hdd2_temp_set:1;
		uint8_t hdd3_temp_set:1;
		uint8_t hdd4_temp_set:1;
		uint8_t hdd5_temp_set:1;
		uint8_t hdd6_temp_set:1;
		uint8_t hdd7_temp_set:1;
		uint8_t gpu_temp_set:1;
		uint8_t ambient_temp_set:1;
		uint8_t sensor_reserved:6;
		uint16_t adc:12;
		uint8_t adc_reserved:3;
		uint8_t adc_set:1;
		uint8_t mem0_size_set:1;
		uint8_t mem1_size_set:1;
		uint8_t mem2_size_set:1;
		uint8_t mem3_size_set:1;
		uint8_t mem_size_set_reserved:4;
		uint8_t mem0_size;
		uint8_t mem1_size;
		uint8_t mem2_size;
		uint8_t mem3_size;
		uint16_t hdd0_size;
		uint16_t hdd1_size;
		uint16_t hdd2_size;
		uint16_t hdd3_size;
		uint16_t hdd4_size;
		uint16_t hdd5_size;
		uint16_t hdd6_size;
		uint16_t hdd7_size;
		uint8_t hdd0_size_set:1;
		uint8_t hdd1_size_set:1;
		uint8_t hdd2_size_set:1;
		uint8_t hdd3_size_set:1;
		uint8_t hdd4_size_set:1;
		uint8_t hdd5_size_set:1;
		uint8_t hdd6_size_set:1;
		uint8_t hdd7_size_set:1;
		uint8_t hdd0_units_tera:1;
		uint8_t hdd1_units_tera:1;
		uint8_t hdd2_units_tera:1;
		uint8_t hdd3_units_tera:1;
		uint8_t hdd4_units_tera:1;
		uint8_t hdd5_units_tera:1;
		uint8_t hdd6_units_tera:1;
		uint8_t hdd7_units_tera:1;
		uint16_t cpu_freq[MAX_CPU];
		uint8_t cpu0_freq_set:1;
		uint8_t cpu1_freq_set:1;
		uint8_t cpu2_freq_set:1;
		uint8_t cpu3_freq_set:1;
		uint8_t cpu4_freq_set:1;
		uint8_t cpu5_freq_set:1;
		uint8_t cpu6_freq_set:1;
		uint8_t cpu7_freq_set:1;
		uint8_t hddtr:1;
		uint8_t cpu_freq_request:1;
		uint8_t cput_temp_request:1;
		uint8_t gpu_temp_request:1;
		uint8_t req_reserved:4;
		struct direct_string_item *direct_string;
		uint8_t screen_saver_update_time;
		uint8_t screen_saver_visible:1;
		uint8_t screen_saver_type:3;
		uint8_t screen_saver_reserved:4;
		uint32_t error_count;
	} details;
	struct {
		uint16_t post_code;
		uint8_t power_state;
		uint8_t cpu_temp[MAX_CPU];
		uint8_t cpu_temp_set;
		uint8_t	gpu_temp;
		uint8_t ambient_temp;
		uint8_t hdd_temp[MAX_HDD];
		uint8_t hdd_temp_set;
		uint8_t other_temp_status;
		uint16_t adc;
		uint8_t mems;
		uint8_t memsz[MAX_MEMORY_SLOT];
		uint16_t hdd_size[MAX_HDD];
		uint8_t hdd_size_set;
		uint8_t hdd_units_tera;
		uint16_t cpu_freq[MAX_CPU];
		uint8_t cpu_freq_set;
		uint8_t pending_req;
		struct direct_string_item *direct_string;
		uint8_t screen_saver_update_time;
		uint8_t screen_saver_config;
		uint32_t error_count;
	} packed;
};

union updatable_info computer_data;
enum computer_state_t computer_state;
#endif /* LAYOUT_H_ */
