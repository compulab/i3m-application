/*
 * i2c_buffer.h
 *
 *      Author: Nikita Kiryanov
 */

#ifndef I2C_BUFFER_H_
#define I2C_BUFFER_H_

#include <stdbool.h>
#include <stdint.h>

union i2c_buffer {
	struct {
		const uint8_t sig[4];
		const uint8_t layout_ver;
		const uint16_t major_rev;
		const uint16_t minor_rev;
		uint8_t reserved0[7];
		uint16_t bios_post_code;
		uint8_t power_state:3;
		uint8_t reserved1:5;
		uint8_t reserved2[13];
		uint8_t cpu0t;
		uint8_t cpu1t;
		uint8_t cpu2t;
		uint8_t cpu3t;
		uint8_t cpu4t;
		uint8_t cpu5t;
		uint8_t cpu6t;
		uint8_t cpu7t;
		uint8_t gput;
		uint8_t ambt;
		uint8_t reserved3[6];
		uint8_t hdd0t;
		uint8_t hdd1t;
		uint8_t hdd2t;
		uint8_t hdd3t;
		uint8_t hdd4t;
		uint8_t hdd5t;
		uint8_t hdd6t;
		uint8_t hdd7t;
		uint8_t cpu0ts:1;
		uint8_t cpu1ts:1;
		uint8_t cpu2ts:1;
		uint8_t cpu3ts:1;
		uint8_t cpu4ts:1;
		uint8_t cpu5ts:1;
		uint8_t cpu6ts:1;
		uint8_t cpu7ts:1;
		uint8_t gpus:1;
		uint8_t ambs:1;
		uint8_t reserved4:6;
		uint8_t hdd0ts:1;
		uint8_t hdd1ts:1;
		uint8_t hdd2ts:1;
		uint8_t hdd3ts:1;
		uint8_t hdd4ts:1;
		uint8_t hdd5ts:1;
		uint8_t hdd6ts:1;
		uint8_t hdd7ts:1;
		uint8_t reserved5[5];
		uint16_t adc:12;
		uint8_t reserved6:3;
		uint8_t adcs:1;
		uint8_t reserved7[14];
		uint8_t mem0s:1;
		uint8_t mem0sz:3;
		uint8_t mem1s:1;
		uint8_t mem1sz:3;
		uint8_t mem2s:1;
		uint8_t mem2sz:3;
		uint8_t mem3s:1;
		uint8_t mem3sz:3;
		uint8_t reserved8[14];
		uint16_t hdd0sz:10;
		uint8_t hdd0f:1;
		uint8_t reserved9:4;
		uint8_t hdd0s:1;
		uint16_t hdd1sz:10;
		uint8_t hdd1f:1;
		uint8_t reserved10:4;
		uint8_t hdd1s:1;
		uint16_t hdd2sz:10;
		uint8_t hdd2f:1;
		uint8_t reserved11:4;
		uint8_t hdd2s:1;
		uint16_t hdd3sz:10;
		uint8_t hdd3f:1;
		uint8_t reserved12:4;
		uint8_t hdd3s:1;
		uint16_t hdd4sz:10;
		uint8_t hdd4f:1;
		uint8_t reserved13:4;
		uint8_t hdd4s:1;
		uint16_t hdd5sz:10;
		uint8_t hdd5f:1;
		uint8_t reserved14:4;
		uint8_t hdd5s:1;
		uint16_t hdd6sz:10;
		uint8_t hdd6f:1;
		uint8_t reserved15:4;
		uint8_t hdd6s:1;
		uint16_t hdd7sz:10;
		uint8_t hdd7f:1;
		uint8_t reserved16:4;
		uint8_t hdd7s:1;
		uint16_t cpu0f:13;
		uint8_t reserved17:2;
		uint8_t cpu0fs:1;
		uint16_t cpu1f:13;
		uint8_t reserved18:2;
		uint8_t cpu1fs:1;
		uint16_t cpu2f:13;
		uint8_t reserved19:2;
		uint8_t cpu2fs:1;
		uint16_t cpu3f:13;
		uint8_t reserved20:2;
		uint8_t cpu3fs:1;
		uint16_t cpu4f:13;
		uint8_t reserved21:2;
		uint8_t cpu4fs:1;
		uint16_t cpu5f:13;
		uint8_t reserved22:2;
		uint8_t cpu5fs:1;
		uint16_t cpu6f:13;
		uint8_t reserved23:2;
		uint8_t cpu6fs:1;
		uint16_t cpu7f:13;
		uint8_t reserved24:2;
		uint8_t cpu7fs:1;
		uint8_t rst:1;
		uint8_t rstusb:1;
		uint8_t reserved25:5;
		uint8_t iwren:1;
		uint8_t req:1;
		uint8_t reserved26:7;
		uint8_t hddtr:1;
		uint8_t cpufr:1;
		uint8_t cputr:1;
		uint8_t gpu_temp_request:1;
		uint8_t reserved27:4;
		uint8_t reserved28[13];
		uint8_t dmi_name;
		uint8_t dmi_value;
		uint8_t reserved29[6];
		uint8_t rtc_time:6;
		uint8_t rtctc:2;
		uint8_t rtc_date:6;
		uint8_t rtcdc:2;
		uint8_t reserved30[103];
	} layout;
	char raw[256];
};

extern union i2c_buffer i2c_buffer;

enum i2c_addr_space  {
		SIG0,
		SIG1,
		SIG2,
		SIG3,
		LAYOUT_VER,
		MAJOR_LSB,
		MAJOR_MSB,
		MINOR_LSB,
		MINOR_MSB,
		RESERVED09,
		RESERVED0A,
		RESERVED0B,
		RESERVED0C,
		RESERVED0D,
		RESERVED0E,
		RESERVED0F,
		POST_CODE_LSB,
		POST_CODE_MSB,
		POWER_STATE,
		RESERVED13,
		RESERVED14,
		RESERVED15,
		RESERVED16,
		RESERVED17,
		RESERVED18,
		RESERVED19,
		RESERVED1A,
		RESERVED1B,
		RESERVED1C,
		RESERVED1D,
		RESERVED1E,
		RESERVED1F,
		CPU0T,
		CPU1T,
		CPU2T,
		CPU3T,
		CPU4T,
		CPU5T,
		CPU6T,
		CPU7T,
		GPUT,
		AMBT,
		RESERVED2A,
		RESERVED2B,
		RESERVED2C,
		RESERVED2D,
		RESERVED2E,
		RESERVED2F,
		HDD0T,
		HDD1T,
		HDD2T,
		HDD3T,
		HDD4T,
		HDD5T,
		HDD6T,
		HDD7T,
		CPUTS,
		SENSORT,
		HDDTS,
		RESERVED3B,
		RESERVED3C,
		RESERVED3D,
		RESERVED3E,
		RESERVED3F,
		ADC_LSB,
		ADC_MSB,
		RESERVED42,
		RESERVED43,
		RESERVED44,
		RESERVED45,
		RESERVED46,
		RESERVED47,
		RESERVED48,
		RESERVED49,
		RESERVED4A,
		RESERVED4B,
		RESERVED4C,
		RESERVED4D,
		RESERVED4E,
		RESERVED4F,
		MEM_LSB,
		MEM_MSB,
		RESERVED52,
		RESERVED53,
		RESERVED54,
		RESERVED55,
		RESERVED56,
		RESERVED57,
		RESERVED58,
		RESERVED59,
		RESERVED5A,
		RESERVED5B,
		RESERVED5C,
		RESERVED5D,
		RESERVED5E,
		RESERVED5F,
		HDD0_SZ_LSB,
		HDD0_SZ_MSB,
		HDD1_SZ_LSB,
		HDD1_SZ_MSB,
		HDD2_SZ_LSB,
		HDD2_SZ_MSB,
		HDD3_SZ_LSB,
		HDD3_SZ_MSB,
		HDD4_SZ_LSB,
		HDD4_SZ_MSB,
		HDD5_SZ_LSB,
		HDD5_SZ_MSB,
		HDD6_SZ_LSB,
		HDD6_SZ_MSB,
		HDD7_SZ_LSB,
		HDD7_SZ_MSB,
		CPU0F_LSB,
		CPU0F_MSB,
		CPU1F_LSB,
		CPU1F_MSB,
		CPU2F_LSB,
		CPU2F_MSB,
		CPU3F_LSB,
		CPU3F_MSB,
		CPU4F_LSB,
		CPU4F_MSB,
		CPU5F_LSB,
		CPU5F_MSB,
		CPU6F_LSB,
		CPU6F_MSB,
		CPU7F_LSB,
		CPU7F_MSB,
		FPCTRL,
		REQ,
		PENDR0,
		RESERVED83,
		RESERVED84,
		RESERVED85,
		RESERVED86,
		RESERVED87,
		RESERVED88,
		RESERVED89,
		RESERVED8A,
		RESERVED8B,
		RESERVED8C,
		RESERVED8D,
		RESERVED8E,
		RESERVED8F,
		DMIN,
		DMIV,
		RESERVED92,
		RESERVED93,
		RESERVED94,
		RESERVED95,
		RESERVED96,
		RESERVED97,
		RTCT,
		RTCD,
		RESERVED9A,
		RESERVED9B,
		RESERVED9C,
		RESERVED9D,
		RESERVED9E,
		RESERVED9F,
		RESERVEDA0,
		RESERVEDA1,
		RESERVEDA2,
		RESERVEDA3,
		RESERVEDA4,
		RESERVEDA5,
		RESERVEDA6,
		RESERVEDA7,
		RESERVEDA8,
		RESERVEDA9,
		RESERVEDAA,
		RESERVEDAB,
		RESERVEDAC,
		RESERVEDAD,
		RESERVEDAE,
		RESERVEDAF,
		RESERVEDB0,
		RESERVEDB1,
		RESERVEDB2,
		RESERVEDB3,
		RESERVEDB4,
		RESERVEDB5,
		RESERVEDB6,
		RESERVEDB7,
		RESERVEDB8,
		RESERVEDB9,
		RESERVEDBA,
		RESERVEDBB,
		RESERVEDBC,
		RESERVEDBD,
		RESERVEDBE,
		RESERVEDBF,
		RESERVEDC0,
		RESERVEDC1,
		RESERVEDC2,
		RESERVEDC3,
		RESERVEDC4,
		RESERVEDC5,
		RESERVEDC6,
		RESERVEDC7,
		RESERVEDC8,
		RESERVEDC9,
		RESERVEDCA,
		RESERVEDCB,
		RESERVEDCC,
		RESERVEDCD,
		RESERVEDCE,
		RESERVEDCF,
		RESERVEDD0,
		RESERVEDD1,
		RESERVEDD2,
		RESERVEDD3,
		RESERVEDD4,
		RESERVEDD5,
		RESERVEDD6,
		RESERVEDD7,
		RESERVEDD8,
		RESERVEDD9,
		RESERVEDDA,
		RESERVEDDB,
		RESERVEDDC,
		RESERVEDDD,
		RESERVEDDE,
		RESERVEDDF,
		RESERVEDE0,
		RESERVEDE1,
		RESERVEDE2,
		RESERVEDE3,
		RESERVEDE4,
		RESERVEDE5,
		RESERVEDE6,
		RESERVEDE7,
		RESERVEDE8,
		RESERVEDE9,
		RESERVEDEA,
		RESERVEDEB,
		RESERVEDEC,
		RESERVEDED,
		RESERVEDEE,
		RESERVEDEF,
		RESERVEDF0,
		RESERVEDF1,
		RESERVEDF2,
		RESERVEDF3,
		RESERVEDF4,
		RESERVEDF5,
		RESERVEDF6,
		RESERVEDF7,
		RESERVEDF8,
		RESERVEDF9,
		RESERVEDFA,
		RESERVEDFB,
		RESERVEDFC,
		RESERVEDFD,
		RESERVEDFE,
		RESERVEDFF
};

#endif
