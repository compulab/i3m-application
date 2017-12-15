/*
 * i2c_buffer.c
 *
 *      Author: Nikita Kiryanov
 */

#include "i2c_buffer.h"
#include "lib/memory.h"
#include "work-queue/work.h"
#include "eeprom/eeprom_layout.h"
#include "sram/sram_direct_strings.h"
#include "power/power.h"
#include <string.h>

union i2c_buffer i2c_buffer = {
	.layout.sig = {'C', 'L', 'F', 'P'},
	.layout.layout_ver = 0x01,
	.layout.major_rev = 0x00,
	.layout.minor_rev = 0x01,
};

void i2c_buffer_init(void)
{
	init_direct_write_vars();
}

static bool is_valid_register(int8_t index, uint8_t max_index)
{
	return index >= 0 && index < max_index;
}

static void write_cpu_fq_msb(uint8_t cpu_addr)
{
	int8_t index = (cpu_addr - CPU0F_MSB) / 2;
	if (!is_valid_register(index, MAX_CPU))
		return;

	if (!(i2c_buffer.raw[cpu_addr] & CPU_FQ_MSB_MSK))
		return;

	computer_data.packed.cpu_freq[index] = (i2c_buffer.raw[cpu_addr] & CPU_FQ_MSB_MSK) << 8 |
											i2c_buffer.raw[cpu_addr - 1];
	if (i2c_buffer.raw[cpu_addr] & CPU_FQ_VALID_BIT)
		computer_data.packed.cpu_freq_set |= 1 << index;
	else
		computer_data.packed.cpu_freq_set &= ~(1 << index);
}

static void write_temp_control(void)
{
	computer_data.details.gpu_temp = i2c_buffer.layout.gpus;
	if (computer_data.details.gpu_temp && (i2c_buffer.layout.gput != computer_data.details.gpu_temp))
		computer_data.details.gpu_temp = i2c_buffer.layout.gput;
}

static void write_hd_sz_msb(uint8_t hdd_addr)
{
	int8_t index = (hdd_addr - HDD0_SZ_MSB) / 2;
	if (!is_valid_register(index, MAX_HDD))
		return;

	if (!(i2c_buffer.raw[hdd_addr] & HDD_SZ_STATUS_MSK)) {
		computer_data.packed.hdd_size_set &= ~(1 << index);
		return;
	}

	computer_data.packed.hdd_size[index] = (i2c_buffer.raw[hdd_addr - 1] & ~ MSB_MSK) |
											((i2c_buffer.raw[hdd_addr] & HDD_SZ_MSK) << 8);
	computer_data.packed.hdd_size_set |= (1 << index);
	uint8_t factor = (i2c_buffer.raw[hdd_addr] & HDD_SZ_UNIT_MSK) ? 1 : 0;
	computer_data.packed.hdd_units_tera |= (1 << index) & factor;
}

static void software_reset(void)
{
	uint8_t oldInterruptState = SREG;  // no real need to store the interrupt context as the reset will pre-empt its restoration
	cli();
	CCP = 0xD8;                        // Configuration change protection: allow protected IO regiser write
	RST.CTRL = RST_SWRST_bm;           // Request software reset by writing to protected IO register
	SREG = oldInterruptState;          // Restore interrupts enabled/disabled state (out of common decency - this line will never be reached because the reset will pre-empt it)
}

static void reset_to_bootloader(void)
{
	uint8_t magic = nvm_eeprom_read_byte(BOOTLOADER_MAGIC_EEPROM_ADDRESS);
	magic &= ~BOOTLOADER_APPLICATION_START;
	nvm_eeprom_write_byte(BOOTLOADER_MAGIC_EEPROM_ADDRESS, magic);
	delay_ms(200);
	software_reset();
}

static void write_cpu_status(void)
{
	if (i2c_buffer.raw[CPUTS] == 0) {
		computer_data.packed.cpu_temp_set = 0;
		return;
	}

	for (uint8_t i = 0; i < MAX_CPU; i++) {
		if (i2c_buffer.raw[CPUTS] & (1 << i))
			computer_data.packed.cpu_temp[i] = i2c_buffer.raw[CPU0T + i];
	}

	computer_data.packed.cpu_temp_set |= i2c_buffer.raw[CPUTS];
}

static void write_hdd_status(void)
{
	if (i2c_buffer.raw[HDDTS] == 0) {
		computer_data.packed.hdd_temp_set = 0;
		return;
	}

	for (uint8_t i = 0 ; i < MAX_CPU; i++) {
		if (i2c_buffer.raw[HDDTS] & (1 << i))
			computer_data.packed.hdd_temp[i] = i2c_buffer.raw[HDD0T + i];
	}

	computer_data.packed.hdd_temp_set |= i2c_buffer.raw[HDDTS];
}

static void write_reset(void)
{
	if (i2c_buffer.layout.rstusb)
		reset_to_bootloader();
	else if (i2c_buffer.layout.rst)
		software_reset();
}

static void write_memory(uint8_t mem_addr) //Todo: change memory status set
{
	uint8_t index = (mem_addr - MEM_LSB ) * 2;
	uint8_t data = i2c_buffer.raw[mem_addr];
	if (data & MEM_SZ_STATUS_LSB_MSK) {
		computer_data.packed.memsz[index] = (data & MEM_SZ_LSB_MSK) >> 4;
		computer_data.packed.mems |= 0x01 << index;
	} else {
		computer_data.packed.mems &= ~(0x01 << index);
		computer_data.packed.memsz[index] = 0;
	}
	if (data & MEM_SZ_STATUS_MSB_MSK) {
		computer_data.packed.mems |= 0x01 << (index + 1);
		computer_data.packed.memsz[index + 1] = data & MEM_SZ_MSB_MSK;
	} else {
		computer_data.packed.mems &= ~(0x01 << (index + 1));
		computer_data.packed.memsz[index + 1] = 0;
	}
}

static void read_power_state(uint8_t *data)
{
	switch (current_power_state) {
	case POWER_ON:
		*data = POWER_ON_BM;
		break;
	case POWER_STR:
		*data = POWER_STR_BM;
		break;
	case POWER_STD:
		*data = POWER_STD_BM;
		break;
	case POWER_OFF:
		*data = POWER_OFF_BM;
		break;
	}
}

void i2c_buffer_read_request(enum i2c_addr_space addr, uint8_t *data)
{
	switch (addr) {
	case POST_CODE_LSB:
		*data = computer_data.details.post_code_lsb;
		break;
	case POST_CODE_MSB:
		*data = computer_data.details.post_code_msb;
		break;
    case ADC_LSB:
		*data = computer_data.details.adc_set ? LSB(computer_data.packed.adc) : 0xff;
	case ADC_MSB:
		*data = computer_data.details.adc_set ? MSB(computer_data.packed.adc) : 0xff;
	case AMBT:
		*data = i2c_buffer.layout.ambs ? i2c_buffer.layout.ambt : DEFAULT_DATA;
		break;
	case SENSORT:
		*data = 0x02 & computer_data.packed.other_temp_status;
		break;
	case LAYOUT_VER:
		*data = eeprom_read_byte(LAYOUT_VERSION_EEPROM_ADDRESS);
		break;
	case POWER_STATE:
		read_power_state(data);
		break;
	case FPCTRL:
		*data = i2c_buffer.layout.iwren << 7;
		break;
	default:
		*data = i2c_buffer.raw[addr];
		break;
	}
}

static void write_gpu_temp(void)
{
	computer_data.details.gpu_temp_set = i2c_buffer.layout.gpus;
	if (computer_data.details.gpu_temp_set)
		computer_data.details.gpu_temp = i2c_buffer.layout.gput;
}

static void update_data(void *write_address)
{
	uint8_t addr = (uint16_t)write_address;
	switch (addr) {
		case GPUT:
			write_gpu_temp();
			break;
		case CPUTS:
			write_cpu_status();
			break;
		case HDDTS:
			write_hdd_status();
			break;
		case CPU0F_MSB:
		case CPU1F_MSB:
		case CPU2F_MSB:
		case CPU3F_MSB:
		case CPU4F_MSB:
		case CPU5F_MSB:
		case CPU6F_MSB:
		case CPU7F_MSB:
			write_cpu_fq_msb(addr);
			break;
		case MEM_LSB :
		case MEM_MSB:
			write_memory(addr);
			break;
		case SENSORT:
			write_temp_control();
			break;
		case POST_CODE_LSB:
			computer_data.packed.post_code = i2c_buffer.layout.bios_post_code;
			break;
		case HDD0_SZ_MSB:
		case HDD1_SZ_MSB:
		case HDD2_SZ_MSB:
		case HDD3_SZ_MSB:
		case HDD4_SZ_MSB:
		case HDD5_SZ_MSB:
		case HDD6_SZ_MSB:
		case HDD7_SZ_MSB:
			write_hd_sz_msb(addr);
			break;
		case DMIN:
		case DMIV:
			break;
		case FPCTRL:
			write_reset();
			break;
	}
}

static void write_data(enum i2c_addr_space addr, uint8_t data)
{
	i2c_buffer.raw[addr] = data;
}

struct work update_data_work = {
	.do_work = update_data,
	.next = NULL,
};

int i2c_buffer_write_request(uint8_t write_address, uint8_t data)
{
	write_data(write_address, data);
	switch(write_address) {
	case DMIV:
	case DMIN:
		write_direct(write_address);
		return 0;
	default:
		break;
	}
	uint16_t work_data = 0x00FF & write_address;
	update_data_work.data = (void *) work_data;
	return insert_work(&update_data_work);
}
