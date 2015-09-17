/*
 * sram_handle.c
 *
 *  Created on: Sep 17, 2015
 *      Author: arkadi
 */

#include "sram_handle.h"
#define CPU_FQ_MSB_MSK 		0x9f
#define CPU_FQ_LSB_MSK 		0xff
#define VALID_AMBIENT_MASK 	0x02
#define VALID_GPU_MASK 		0x01
#define LSB_MSK 			0x00ff
#define MSB_MSK 			0xff00
#define RSTUSB_MSK 			0x02
#define RST_MSK 			0x01
#define RST_MSK 			0x01
#define WEN_MSK 			0x80
#define DEFAULT_DATA		0x00
#define POWER_ON_BM			0x07
#define POWER_STR_BM		0x03
#define POWER_STD_BM		0x01
#define POWER_OFF_BM		0x00
#define REQUEST_ENABLE		0x01
#define REQUEST_CPUF		0x02
#define REQUEST_CPUT		0x04
#define REQUEST_GPUT		0x08

bool is_valid_register(int8_t index, uint8_t max_index)
{
	return index >= 0 && index < max_index;
}

void write_temp(uint8_t max_index, uint8_t base_addr, uint8_t reg_addr, uint8_t *write_base, uint8_t data)
{
	int8_t index = reg_addr - base_addr;
	if (!is_valid_register(index, max_index))
			return;
	write_base[index] = data;
}

void write_cpu_temp(uint8_t cpu_addr, uint8_t data)
{
	write_temp(MAX_CPU, CPU0_TEMP_ADDRESS, cpu_addr, computer_data.cpu_temp, data);
}

void write_hd_temp(uint8_t hd_addr, uint8_t data)
{
	write_temp(MAX_HARD_DISKS, HARD_DISK0_TEMP_ADDRESS, hd_addr, computer_data.hd_temp, data);
}

void write_cpu_fq_msb(uint8_t cpu_addr, uint8_t data)
{
	int8_t index = cpu_addr - CPU0F_LSB_ADDRESS;
	if (!is_valid_register(index,MAX_CPU))
		return ;
	uint8_t msb = (data & CPU_FQ_MSB_MSK);
	computer_data.cpu_fq[index] =  (computer_data.cpu_fq[index] & ~CPU_FQ_MSB_MSK) | (msb << 8);
}

void write_cpu_fq_lsb(uint8_t cpu_addr, uint8_t data)
{
	int8_t index = cpu_addr - CPU0F_LSB_ADDRESS;
	if (!is_valid_register(index,MAX_CPU))
		return ;
	computer_data.cpu_fq[index] =  (computer_data.cpu_fq[index] & ~CPU_FQ_LSB_MSK) | (data & CPU_FQ_LSB_MSK);
}

void write_temp_control(uint8_t data)
{
	computer_data.valid_gpu = (data & VALID_GPU_MASK) != 0;
	computer_data.valid_ambient = (data & VALID_AMBIENT_MASK) != 0;

}

void write_hd_sz_lsb(uint8_t hd_addr, uint8_t data)
{
	int8_t index = hd_addr - HARD_DISK0_LSB_SZ_ADDRESS;
	if (!is_valid_register(index, MAX_HARD_DISKS))
		return ;
	computer_data.hd_size[index] = (computer_data.hd_size[index] & ~ LSB_MSK) | data;
}

void write_hd_sz_msb(uint8_t hd_addr, uint8_t data)
{
	int8_t index = hd_addr - HARD_DISK0_LSB_SZ_ADDRESS;
	if (!is_valid_register(index, MAX_HARD_DISKS))
		return ;
	computer_data.hd_size[index] = (computer_data.hd_size[index] & ~ MSB_MSK) | (data << 8);
}

void reset_to_usb()
{
//TODO
}

void software_reset()
{
//TODO
}

void write_reset(uint8_t data)
{
	computer_data.wen = (data & WEN_MSK) != 0;
	if (data & RSTUSB_MSK)
		reset_to_usb();
	else if (data & RST_MSK)
		software_reset();
}

void write_post_code_lsb(uint8_t data)
{
	computer_data.bios_post_code = (computer_data.bios_post_code & ~LSB_MSK) | data;
}

void write_post_code_msb(uint8_t data)
{
	computer_data.bios_post_code = (computer_data.bios_post_code & ~MSB_MSK) | (data << 8);
}

void write_memory(uint8_t mem_addr, uint8_t data)
{
	uint8_t index = mem_addr - MEM_SLOT01_ADDRESS;
	computer_data.mem_slot_sz[index] = data;
}

void read_sig(uint8_t sig_address, uint8_t *data)
{
	switch(sig_address){
	case SIG_FIRST_BYTE_ADDRESS:
		*data = eeprom_read_byte(SIG_FIRST_BYTE_EEPROM_ADDRESS);
		break;
	case SIG_SECOND_BYTE_ADDRESS:
		*data = eeprom_read_byte(SIG_SECOND_BYTE_EEPROM_ADDRESS);
		break;
	case SIG_THIRD_BYTE_ADDRESS:
		*data = eeprom_read_byte(SIG_THIRD_BYTE_EEPROM_ADDRESS);
		break;
	case SIG_FOURTH_BYTE_ADDRESS:
		*data = eeprom_read_byte(SIG_FOURTH_BYTE_EEPROM_ADDRESS);
		break;
	}
}

void read_revision(uint8_t rev_address, uint8_t *data)
{
	switch (rev_address){
	case MAJOR_REVISION_LSB_ADDRESS:
		*data = eeprom_read_byte(MAJOR_REVISION_LSB_EEPROM_ADDRESS);
		break;
	case MAJOR_REVISION_MSB_ADDRESS:
		*data = eeprom_read_byte(MAJOR_REVISION_MSB_EEPROM_ADDRESS);
		break;
	case MINOR_REVISION_LSB_ADDRESS:
		*data = eeprom_read_byte(MINOR_REVISION_LSB_EEPROM_ADDRESS);
		break;
	case MINOR_REVISION_MSB_ADDRESS:
		*data = eeprom_read_byte(MINOR_REVISION_MSB_EEPROM_ADDRESS);
		break;
	}
}

void read_bios_post_code(uint8_t post_code_address, uint8_t *data)
{
	switch(post_code_address){
	case POST_CODE_LSB_ADDRESS:
		*data = computer_data.bios_post_code & LSB_MSK;
		break;
	case POST_CODE_MSB_ADDRESS:
		*data = computer_data.bios_post_code >> 8;
		break;
	}
}

void read_layout(uint8_t *data)
{
	*data = eeprom_read_byte(LAYOUT_VERSION_EEPROM_ADDRESS);
}

void read_power_state(uint8_t *data)
{
	switch (current_power_state){
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

void read_ambient(uint8_t *data)
{
	if (!computer_data.valid_ambient){
		update_ambient_temp();
		delay_us(4000);
	}
	if (computer_data.valid_ambient)
		*data = computer_data.ambient_temp;
	else
		*data = DEFAULT_DATA;
}

void read_adc(uint8_t adc_address, uint8_t *data)
{
	switch (adc_address){
	case ADC_LSB_ADDRESS:
		//TODO
		break;
	case ADC_MSB_ADDRESS:
		//TODO
		break;
	}
}

void read_wen(uint8_t *data)
{
	*data = computer_data.wen;
}

void read_pending_requests(uint8_t *data)
{
	uint8_t req = 0x00;
	if (computer_data.req_cpu_temp || computer_data.req_cpu_fq || computer_data.req_gpu_temp){
		req &= REQUEST_ENABLE;
		if (computer_data.req_cpu_temp)
			req &= REQUEST_CPUT;
		if (computer_data.req_cpu_fq)
			req &= REQUEST_CPUF;
		if (computer_data.req_gpu_temp)
			req &= REQUEST_GPUT;
	}
	*data = req;
}

void handle_sram_read_request(uint8_t read_address, uint8_t *data)
{
	*data = DEFAULT_DATA;
	switch(read_address){
	case SIG_FIRST_BYTE_ADDRESS:
	case SIG_SECOND_BYTE_ADDRESS:
	case SIG_THIRD_BYTE_ADDRESS:
	case SIG_FOURTH_BYTE_ADDRESS:
		read_sig(read_address, data);
		break;
	case MAJOR_REVISION_LSB_ADDRESS:
	case MAJOR_REVISION_MSB_ADDRESS:
	case MINOR_REVISION_LSB_ADDRESS:
	case MINOR_REVISION_MSB_ADDRESS:
		read_revision(read_address, data);
		break;
	case LAYOUT_VERSION_ADDRESS:
		read_layout(data);
		break;
	case POST_CODE_LSB_ADDRESS:
	case POST_CODE_MSB_ADDRESS:
		read_bios_post_code(read_address, data);
		break;
	case POWER_STATE_ADDRESS:
		read_power_state(data);
		break;
	case AMBIENT_TEMP_ADDRESS:
		read_ambient(data);
		break;
	case ADC_LSB_ADDRESS:
	case ADC_MSB_ADDRESS:
		read_adc(read_address, data);
		break;
	case RESET_ADDRESS:
		read_wen(data);
		break;
	case REQUEST_ADDRESS:
		read_pending_requests(data);
		break;
	case RTC_TIME_ADDRESS:
	case RTC_DATE_ADDRESS:
//		read_rtc(read_address, data); TODO
		break;
	}
}

void handle_sram_write_request(uint8_t write_address, uint8_t data)
{
	switch (write_address){
		case GPU_TEMP_ADDRESS:
			computer_data.gpu_temp = data;
			break;
		case CPU0_TEMP_ADDRESS:
		case CPU1_TEMP_ADDRESS:
		case CPU2_TEMP_ADDRESS:
		case CPU3_TEMP_ADDRESS:
		case CPU4_TEMP_ADDRESS:
		case CPU5_TEMP_ADDRESS:
		case CPU6_TEMP_ADDRESS:
		case CPU7_TEMP_ADDRESS:
			write_cpu_temp(write_address, data);
			break;
		case HARD_DISK0_TEMP_ADDRESS:
		case HARD_DISK1_TEMP_ADDRESS:
		case HARD_DISK2_TEMP_ADDRESS:
		case HARD_DISK3_TEMP_ADDRESS:
		case HARD_DISK4_TEMP_ADDRESS:
		case HARD_DISK5_TEMP_ADDRESS:
		case HARD_DISK6_TEMP_ADDRESS:
		case HARD_DISK7_TEMP_ADDRESS:
			write_hd_temp(write_address, data);
			break;
		case CPU0F_MSB_ADDRESS:
		case CPU1F_MSB_ADDRESS:
		case CPU2F_MSB_ADDRESS:
		case CPU3F_MSB_ADDRESS:
		case CPU4F_MSB_ADDRESS:
		case CPU5F_MSB_ADDRESS:
		case CPU6F_MSB_ADDRESS:
		case CPU7F_MSB_ADDRESS:
			write_cpu_fq_msb(write_address, data);
			break;
		case CPU0F_LSB_ADDRESS:
		case CPU1F_LSB_ADDRESS:
		case CPU2F_LSB_ADDRESS:
		case CPU3F_LSB_ADDRESS:
		case CPU4F_LSB_ADDRESS:
		case CPU5F_LSB_ADDRESS:
		case CPU6F_LSB_ADDRESS:
		case CPU7F_LSB_ADDRESS:
			write_cpu_fq_lsb(write_address, data);
			break;
		case MEM_SLOT01_ADDRESS:
		case MEM_SLOT23_ADDRESS:
			write_memory(write_address, data);
			break;
		case TEMPERTURE_CONTROL_ADDRESS:
			write_temp_control(data);
			break;
		case POST_CODE_LSB_ADDRESS:
			write_post_code_lsb(data);
			break;
		case POST_CODE_MSB_ADDRESS:
			write_post_code_msb(data);
			break;
		case HARD_DISK0_LSB_SZ_ADDRESS:
		case HARD_DISK1_LSB_SZ_ADDRESS:
		case HARD_DISK2_LSB_SZ_ADDRESS:
		case HARD_DISK3_LSB_SZ_ADDRESS:
		case HARD_DISK4_LSB_SZ_ADDRESS:
		case HARD_DISK5_LSB_SZ_ADDRESS:
		case HARD_DISK6_LSB_SZ_ADDRESS:
		case HARD_DISK7_LSB_SZ_ADDRESS:
			write_hd_sz_lsb(write_address, data);
			break;
		case HARD_DISK0_MSB_SZ_ADDRESS:
		case HARD_DISK1_MSB_SZ_ADDRESS:
		case HARD_DISK2_MSB_SZ_ADDRESS:
		case HARD_DISK3_MSB_SZ_ADDRESS:
		case HARD_DISK4_MSB_SZ_ADDRESS:
		case HARD_DISK5_MSB_SZ_ADDRESS:
		case HARD_DISK6_MSB_SZ_ADDRESS:
		case HARD_DISK7_MSB_SZ_ADDRESS:
			write_hd_sz_msb(write_address, data);
			break;
		case DIRECT_CONTENT_ADDRESS:
		case DIRECT_TYPE_ADDRESS:
//			write_direct(write_address, data);  TODO
			break;
		case RESET_ADDRESS:
			write_reset(data);
			break;
	}
}
