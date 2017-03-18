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
#define SCREEN_SAVER_TIME_EEPROM_ADDRESS	0x120
#define SCREEN_SAVER_CONFIG_EEPROM_ADDRESS	0x121
#define BRIGHTNESS_EEPROM_ADDRESS 			0x122
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

#define HDD_TMP_VALID_BIT					0x80
#define CPU_TMP_VALID_BIT					0x80
#define CPU_FQ_VALID_BIT					0x80

struct direct_string_item {
	char *type;
	char *content;
	uint16_t backup_addr;
	struct direct_string_item *next;
};

#define MAX_LOG_SIZE 30

struct twi_log {
	char data[MAX_LOG_SIZE];
	uint8_t bottom;
	uint8_t top;
};

enum computer_state_t {
	COMPUTER_OFF,
	COMPUTER_ON,
	COMPUTER_IN_BIOS,
	COMPUTER_IN_OS,
	COMPUTER_DAEMON_WORK,
};

enum memory_error_codes {
	NO_ERROR = 0,
	NEW_WORK,
	INFO_LABEL_TEXT,
	IMAGE_BITMAP,
	IMAGE_NODE,
	LABEL_NODE,
	INFORMATION_NODE,
	FRAME_NODE,
	MENU_BITMAP,
	FONTS_ARRAY,
	FONT,
	GRAPHIC_HEAD,
	GRAPHIC_IMAGE_NEXT,
	MONO_MENU,
	ACTIONS_MENU_ARRAY,
	ACTION_MENU,
	DMI_STRING_ADD,
	DMI_STRING_TYPE,
	DMI_STRING_CONTENT,
	DMI_MENU,
	DMI_LABEL,
	DMI_ACTION,
	DMI_FRAME,
};

union updatable_info {
	struct {
		uint8_t post_code_lsb;
		uint8_t post_code_msb;
		uint8_t power_state:3;
		uint8_t power_state_reserved:5;
		uint8_t cput[MAX_CPU];
		uint8_t cpu0ts:1;
		uint8_t cpu1ts:1;
		uint8_t cpu2ts:1;
		uint8_t cpu3ts:1;
		uint8_t cpu4ts:1;
		uint8_t cpu5ts:1;
		uint8_t cpu6ts:1;
		uint8_t cpu7ts:1;
		uint8_t	gput;
		uint8_t ambt;
		uint8_t hddt[MAX_HDD];
		uint8_t hdd0ts:1;
		uint8_t hdd1ts:1;
		uint8_t hdd2ts:1;
		uint8_t hdd3ts:1;
		uint8_t hdd4ts:1;
		uint8_t hdd5ts:1;
		uint8_t hdd6ts:1;
		uint8_t hdd7ts:1;
		uint8_t gpus:1;
		uint8_t ambs:1;
		uint8_t sensor_reserved:6;
		uint16_t adc:12;
		uint8_t adc_reserved:3;
		uint8_t adcs:1;
		uint8_t mem0s:1;
		uint8_t mem1s:1;
		uint8_t mem2s:1;
		uint8_t mem3s:1;
		uint8_t mems_reserved:4;
		uint8_t mem0sz;
		uint8_t mem1sz;
		uint8_t mem2sz;
		uint8_t mem3sz;
		uint16_t hdd0sz;
		uint16_t hdd1sz;
		uint16_t hdd2sz;
		uint16_t hdd3sz;
		uint16_t hdd4sz;
		uint16_t hdd5sz;
		uint16_t hdd6sz;
		uint16_t hdd7sz;
		uint8_t hdd0s:1;
		uint8_t hdd1s:1;
		uint8_t hdd2s:1;
		uint8_t hdd3s:1;
		uint8_t hdd4s:1;
		uint8_t hdd5s:1;
		uint8_t hdd6s:1;
		uint8_t hdd7s:1;
		uint8_t hdd0f:1;
		uint8_t hdd1f:1;
		uint8_t hdd2f:1;
		uint8_t hdd3f:1;
		uint8_t hdd4f:1;
		uint8_t hdd5f:1;
		uint8_t hdd6f:1;
		uint8_t hdd7f:1;
		uint16_t cpu0f;
		uint16_t cpu1f;
		uint16_t cpu2f;
		uint16_t cpu3f;
		uint16_t cpu4f;
		uint16_t cpu5f;
		uint16_t cpu6f;
		uint16_t cpu7f;
		uint8_t cpu0fs:1;
		uint8_t cpu1fs:1;
		uint8_t cpu2fs:1;
		uint8_t cpu3fs:1;
		uint8_t cpu4fs:1;
		uint8_t cpu5fs:1;
		uint8_t cpu6fs:1;
		uint8_t cpu7fs:1;
		uint8_t hddtr:1;
		uint8_t cpufr:1;
		uint8_t cputr:1;
		uint8_t gputr:1;
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
		uint8_t cput[MAX_CPU];
		uint8_t cputs;
		uint8_t	gput;
		uint8_t ambt;
		uint8_t hddt[MAX_HDD];
		uint8_t hddts;
		uint8_t other_temp_status;
		uint16_t adc;
		uint8_t mems;
		uint8_t memsz[MAX_MEMORY_SLOT];
		uint16_t hddsz[MAX_HDD];
		uint8_t hdds;
		uint8_t hddf;
		uint16_t cpuf[MAX_CPU];
		uint8_t cpufs;
		uint8_t pending_req;
		struct direct_string_item *direct_string;
		uint8_t screen_saver_update_time;
		uint8_t screen_saver_config;
		uint32_t error_count;
	} packed;
};

struct twi_log log_twi;

union updatable_info computer_data;
enum computer_state_t computer_state;
#endif /* LAYOUT_H_ */
