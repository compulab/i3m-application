/*
 * layout.h
 *
 *  Created on: Sep 3, 2015
 *      Author: arkadi
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_
#include "asf.h"

#define SIG_FIRST_BYTE_ADDRESS 		0X00
#define SIG_SECOND_BYTE_ADDRESS 	0X01
#define SIG_THIRD_BYTE_ADDRESS 		0X02
#define SIG_FOURTH_BYTE_ADDRESS 	0X03
#define LAYOUT_VERSION_ADDRESS 		0x04
#define MAJOR_REVISION_LSB_ADDRESS 	0x05
#define MAJOR_REVISION_MSB_ADDRESS 	0x06
#define MINOR_REVISION_LSB_ADDRESS 	0x07
#define MINOR_REVISION_MSB_ADDRESS 	0x08

#define POST_CODE_LSB_ADDRESS 		0x10
#define POST_CODE_MSB_ADDRESS 		0x11
#define POWER_STATE_ADDRESS 		0x12

#define CPU0_TEMP_ADDRESS 			0x20
#define CPU1_TEMP_ADDRESS 			0x21
#define CPU2_TEMP_ADDRESS 			0x22
#define CPU3_TEMP_ADDRESS 			0x23
#define CPU4_TEMP_ADDRESS 			0x24
#define CPU5_TEMP_ADDRESS 			0x25
#define CPU6_TEMP_ADDRESS 			0x26
#define CPU7_TEMP_ADDRESS 			0x27
#define GPU_TEMP_ADDRESS 			0x28
#define AMBIENT_TEMP_ADDRESS 		0x29
#define HDD0_TEMP_ADDRESS 			0x30
#define HDD1_TEMP_ADDRESS 			0x31
#define HDD2_TEMP_ADDRESS 			0x32
#define HDD3_TEMP_ADDRESS 			0x33
#define HDD4_TEMP_ADDRESS 			0x34
#define HDD5_TEMP_ADDRESS 			0x35
#define HDD6_TEMP_ADDRESS 			0x36
#define HDD7_TEMP_ADDRESS 			0x37
#define CPU_STATUS_ADDRESS 			0x38
#define TEMPERTURE_CONTROL_ADDRESS	0x39
#define HDD_STATUS_ADDRESS 			0x3a

#define ADC_LSB_ADDRESS 			0x40
#define ADC_MSB_ADDRESS 			0x41

#define MEM_SLOT01_ADDRESS 			0x50
#define MEM_SLOT23_ADDRESS 			0x51

#define HDD0_LSB_SZ_ADDRESS 	0x60
#define HDD0_MSB_SZ_ADDRESS 	0x61
#define HDD1_LSB_SZ_ADDRESS 	0x62
#define HDD1_MSB_SZ_ADDRESS 	0x63
#define HDD2_LSB_SZ_ADDRESS 	0x64
#define HDD2_MSB_SZ_ADDRESS 	0x65
#define HDD3_LSB_SZ_ADDRESS 	0x66
#define HDD3_MSB_SZ_ADDRESS 	0x67
#define HDD4_LSB_SZ_ADDRESS 	0x68
#define HDD4_MSB_SZ_ADDRESS 	0x69
#define HDD5_LSB_SZ_ADDRESS 	0x6a
#define HDD5_MSB_SZ_ADDRESS 	0x6b
#define HDD6_LSB_SZ_ADDRESS 	0x6c
#define HDD6_MSB_SZ_ADDRESS 	0x6d
#define HDD7_LSB_SZ_ADDRESS 	0x6e
#define HDD7_MSB_SZ_ADDRESS 	0x6f

#define CPU0F_LSB_ADDRESS 			0x70
#define CPU0F_MSB_ADDRESS 			0x71
#define CPU1F_LSB_ADDRESS 			0x72
#define CPU1F_MSB_ADDRESS 			0x73
#define CPU2F_LSB_ADDRESS 			0x74
#define CPU2F_MSB_ADDRESS 			0x75
#define CPU3F_LSB_ADDRESS 			0x76
#define CPU3F_MSB_ADDRESS 			0x77
#define CPU4F_LSB_ADDRESS 			0x78
#define CPU4F_MSB_ADDRESS 			0x79
#define CPU5F_LSB_ADDRESS 			0x7a
#define CPU5F_MSB_ADDRESS 			0x7b
#define CPU6F_LSB_ADDRESS 			0x7c
#define CPU6F_MSB_ADDRESS 			0x7d
#define CPU7F_LSB_ADDRESS 			0x7e
#define CPU7F_MSB_ADDRESS 			0x7f

#define RESET_ADDRESS 				0x80
#define REQUEST_ADDRESS 			0x81

#define DIRECT_TYPE_ADDRESS 		0x90
#define DIRECT_CONTENT_ADDRESS		0x91

#define RTC_TIME_ADDRESS 			0x98
#define RTC_DATE_ADDRESS			0x99


#define LAYOUT_VERSION_EEPROM_ADDRESS 		0x2c
#define SIG_FIRST_BYTE_EEPROM_ADDRESS 		0X2d
#define SIG_SECOND_BYTE_EEPROM_ADDRESS 		0X2e
#define SIG_THIRD_BYTE_EEPROM_ADDRESS 		0X2f
#define SIG_FOURTH_BYTE_EEPROM_ADDRESS 		0X30
#define MAJOR_REVISION_LSB_EEPROM_ADDRESS 	0x00
#define MAJOR_REVISION_MSB_EEPROM_ADDRESS 	0x01
#define MINOR_REVISION_LSB_EEPROM_ADDRESS 	0x02
#define MINOR_REVISION_MSB_EEPROM_ADDRESS 	0x03


#define BIOS_STATE_ADDRESS 0x18
#define SERIAL_NUMBER_ADDRESS 0x0c
#define PRODUCTION_YEAR_ADDRESS 0x0a
#define PRODUCTION_MONTH_ADDRESS 0x09
#define PRODUCTION_DAY_ADDRESS 0x08
#define MAC_ADDRESS 0x02
#define REVISION_ADDRESS 0x00


#define MAX_HDD 		6
#define MAX_CPU				8
#define MAX_MEMORY_SLOT		4

#define HDD_TMP_VALID_BIT	0x80
#define CPU_TMP_VALID_BIT	0x80

struct direct_string_item {
	char *type;
	char *content;
	struct direct_string_item *next;
};

struct update_information {
	struct direct_string_item *direct_string;
	bool wen;
	bool valid_ambient_temp;
	bool valid_gpu_temp;
	bool valid_hdd_size[MAX_HDD];
	bool valid_hdd_temp[MAX_HDD];
	bool valid_cpu_temp[MAX_CPU];
	bool valid_mem[MAX_MEMORY_SLOT];
	bool valid_cpu_fq[MAX_CPU];
	bool req_cpu_temp;
	bool req_cpu_fq;
	bool req_gpu_temp;
	bool hdd_tera_units[MAX_HDD];
	uint8_t gpu_temp;
	uint8_t ambient_temp;
	uint8_t mem_slot_sz[MAX_MEMORY_SLOT];
	uint8_t cpu_temp[MAX_CPU];
	uint8_t hdd_temp[MAX_HDD];
	uint16_t hdd_size[MAX_HDD];
	uint16_t cpu_fq[MAX_CPU];
	uint16_t bios_post_code;
};

struct update_information computer_data;

#endif /* LAYOUT_H_ */
