/*
 * sram_handle.c
 *
 *  Created on: Sep 17, 2015
 *      Author: arkadi
 */

#include "../debug.h"
//#define SRAM_DEBUG
#include "sram_handle.h"

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
#define MEM_SZ_STATUS_LSB_MSK	0x08
#define MEM_SZ_STATUS_MSB_MSK	0x80
#define MEM_SZ_LSB_MSK			0x07
#define MEM_SZ_MSB_MSK			0x70
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



uint8_t direct_write_length;
uint8_t direct_write_index;
bool is_length_set;
bool is_last_type;
struct direct_string_item *direct_string_to_add;


void clear_direct_help_vars()
{
	direct_write_index = 0;
	direct_write_length = 0;
	is_length_set = false;
}

void init_direct_write_vars()
{
	direct_string_to_add = 0;
	clear_direct_help_vars();
}

void sram_handle_init()
{
	is_last_type = false;
	init_direct_write_vars();
}
bool is_valid_register(int8_t index, uint8_t max_index)
{
	return index >= 0 && index < max_index;
}

void write_cpu_temp(uint8_t cpu_addr, uint8_t data)
{
	int8_t index = cpu_addr - CPU0_TEMP_ADDRESS;
	if (is_valid_register(index,MAX_CPU))
		computer_data.cpu_temp[index] = data;
}

void write_hdd_temp(uint8_t hdd_addr, uint8_t data)
{
	int8_t index = hdd_addr - HDD0_TEMP_ADDRESS;
	if (is_valid_register(index,MAX_HDD)){
		computer_data.hdd_temp[index] = data;
		computer_data.valid_hdd_temp[index] = false;
	}
}

void write_cpu_fq_msb(uint8_t cpu_addr, uint8_t data)
{
	int8_t index = (cpu_addr - CPU0F_MSB_ADDRESS)/2;
	if (!is_valid_register(index,MAX_CPU))
		return ;
	uint8_t msb = (data & CPU_FQ_MSB_MSK);
	computer_data.cpu_fq[index] =  (computer_data.cpu_fq[index] & ~CPU_FQ_MSK) | (msb << 8);
	computer_data.valid_cpu_fq[index] = data & VALID_CPU_FQ_MSK;
	update_information_frame(SHOW_CPU_FREQUENCY, information_present->info_data == index && computer_data.valid_cpu_fq[index]);
}

void write_cpu_fq_lsb(uint8_t cpu_addr, uint8_t data)
{
	int8_t index = (cpu_addr - CPU0F_LSB_ADDRESS) / 2;
	if (!is_valid_register(index,MAX_CPU))
		return ;
	computer_data.cpu_fq[index] = 0x0000 | data;
}


void write_temp_control(uint8_t data)
{
	computer_data.valid_gpu_temp = (data & VALID_GPU_MASK) != 0;
}

void write_hd_sz_lsb(uint8_t hd_addr, uint8_t data)
{
	int8_t index = (hd_addr - HDD0_LSB_SZ_ADDRESS) / 2;
	if (!is_valid_register(index, MAX_HDD))
		return ;
	computer_data.hdd_size[index] = (computer_data.hdd_size[index] & ~ LSB_MSK) | data;
}

void write_hd_sz_msb(uint8_t hd_addr, uint8_t data)
{
	int8_t index = (hd_addr - HDD0_LSB_SZ_ADDRESS) / 2;
	if (!is_valid_register(index, MAX_HDD))
		return ;
	computer_data.valid_hdd_size[index] = (data & HDD_SZ_STATUS_MSK);
	computer_data.hdd_tera_units[index] = (data & HDD_SZ_UNIT_MSK);
	computer_data.hdd_size[index] = (computer_data.hdd_size[index] & ~ MSB_MSK) | ((data & HDD_SZ_MSK) << 8);
	update_information_frame(SHOW_HDD_SIZE,information_present->info_data == index && computer_data.valid_hdd_size[information_present->info_data]);
}

void reset_to_usb()
{
//TODO
}

void software_reset()
{
//TODO
}

void write_cpu_status(uint8_t status)
{
	if (status == 0){
		for (int i = 0; i < MAX_CPU; i++)
			computer_data.valid_cpu_temp[i] = false;
	} else {
		if (status & 0x01)
			computer_data.valid_cpu_temp[0] = true;
		if (status & 0x02)
			computer_data.valid_cpu_temp[1] = true;
		if (status & 0x04)
			computer_data.valid_cpu_temp[2] = true;
		if (status & 0x08)
			computer_data.valid_cpu_temp[3] = true;
		if (status & 0x10)
			computer_data.valid_cpu_temp[4] = true;
		if (status & 0x20)
			computer_data.valid_cpu_temp[5] = true;
		if (status & 0x40)
			computer_data.valid_cpu_temp[6] = true;
		if (status & 0x80)
			computer_data.valid_cpu_temp[7] = true;
		update_information_frame(SHOW_CPU_TEMPERTURE, information_present->info_data < MAX_CPU && computer_data.valid_cpu_temp[information_present->info_data]);
	}
}

void write_hdd_status(uint8_t status)
{
	if (status == 0){
		for (int i = 0; i < MAX_HDD; i++)
			computer_data.valid_hdd_temp[i] = false;
	} else {
		if (status & 0x01)
			computer_data.valid_hdd_temp[0] = true;
		if (status & 0x02)
			computer_data.valid_hdd_temp[1] = true;
		if (status & 0x04)
			computer_data.valid_hdd_temp[2] = true;
		if (status & 0x08)
			computer_data.valid_hdd_temp[3] = true;
		if (status & 0x10)
			computer_data.valid_hdd_temp[4] = true;
		if (status & 0x20)
			computer_data.valid_hdd_temp[5] = true;
//		if (status & 0x40)
//			computer_data.valid_hdd_temp[6] = true;
//		if (status & 0x80)
//			computer_data.valid_hdd_temp[7] = true;
		update_information_frame(SHOW_HDD_TEMPERTURE, information_present->info_data < MAX_HDD && computer_data.valid_hdd_temp[information_present->info_data]);
		}
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
	uint8_t index = (mem_addr - MEM_SLOT01_ADDRESS) * 2;
	computer_data.mem_slot_sz[index] = data & MEM_SZ_LSB_MSK;
	computer_data.valid_mem[index] = data & MEM_SZ_STATUS_LSB_MSK;
	computer_data.mem_slot_sz[index + 1] = (data & MEM_SZ_MSB_MSK) >> 4;
	computer_data.valid_mem[index + 1] = data & MEM_SZ_STATUS_MSB_MSK;
	update_information_frame(SHOW_MEMORY_SIZE, information_present->info_data < MAX_MEMORY_SLOT && computer_data.valid_mem[information_present->info_data]);
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
//	if (!computer_data.valid_ambient_temp){
//		update_ambient_temp();
//		delay_us(4);
//	}
	if (computer_data.valid_ambient_temp)
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

void read_cpu_temp(uint8_t cpu_address, uint8_t *data)
{
	uint8_t temp = 0x00;
	uint8_t index = cpu_address - CPU0_TEMP_ADDRESS;
	if (is_valid_register(index, MAX_CPU) && computer_data.valid_cpu_temp[index])
		temp = computer_data.cpu_temp[index];
	*data = temp;
}

void read_hdd_temp(uint8_t hdd_address, uint8_t *data)
{
	uint8_t temp = 0x00;
	uint8_t index = hdd_address - HDD0_TEMP_ADDRESS;
	if (is_valid_register(index, MAX_HDD) && computer_data.valid_hdd_temp[index])
		temp = computer_data.hdd_temp[index];
	*data = temp;
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

void set_direct_string()
{
	direct_string_to_add = malloc(sizeof(struct direct_string_item));
	direct_string_to_add->content = 0;
	direct_string_to_add->next = 0;
	direct_string_to_add->type = 0;
}

void add_direct_string()
{
	direct_string_to_add->next = computer_data.direct_string;
	computer_data.direct_string = direct_string_to_add;
	direct_string_to_add = 0;
	init_direct_write_vars();
}


void end_direct_string(bool is_written_type)
{
	if(is_written_type && !is_last_type){
		direct_string_to_add->type[direct_write_length]='\0';
		clear_direct_help_vars();
	} else if (is_last_type){
		direct_string_to_add->content[direct_write_length]='\0';
		add_direct_string();
	} else{
		init_direct_write_vars();
		is_last_type = false;
		return;
	}
	is_last_type = !is_last_type;
}


void write_byte_direct_string(bool is_written_type, uint8_t data)
{
	if (is_written_type)
		direct_string_to_add->type[direct_write_index] = data;
	else
		direct_string_to_add->content[direct_write_index] = data;
	direct_write_index++;
}

void set_written_length(bool is_written_type, uint8_t data)
{
	direct_write_length = data;
	is_length_set = true;
	if (is_written_type){
		if (direct_string_to_add == 0){
			set_direct_string();
			direct_string_to_add->type = malloc (sizeof(char *) * direct_write_length + 1);
		} else {
			clear_direct_help_vars();
			return;
		}
	} else if (is_last_type){
		direct_string_to_add->content = malloc (sizeof(char *) * direct_write_length + 1);
	} else {
		is_last_type = false;
		init_direct_write_vars();
		return;
	}
}

void write_direct_byte(bool is_written_type, uint8_t data)
{
	if (is_length_set){
		write_byte_direct_string(is_written_type, data);
		if (direct_write_length == direct_write_index)
			end_direct_string(is_written_type);
	} else {
		set_written_length(is_written_type, data);
	}
}

void write_direct(uint8_t write_address, uint8_t data)
{
	switch (write_address){
	case DIRECT_TYPE_ADDRESS:
		write_direct_byte(true, data);
		break;
	case DIRECT_CONTENT_ADDRESS:
		write_direct_byte(false,data);
		break;
	}

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
#ifdef SRAM_DEBUG
	case CPU0_TEMP_ADDRESS:
	case CPU1_TEMP_ADDRESS:
	case CPU2_TEMP_ADDRESS:
	case CPU3_TEMP_ADDRESS:
	case CPU4_TEMP_ADDRESS:
	case CPU5_TEMP_ADDRESS:
	case CPU6_TEMP_ADDRESS:
	case CPU7_TEMP_ADDRESS:
		read_cpu_temp(read_address, data);
		break;
	case HDD0_TEMP_ADDRESS:
	case HDD1_TEMP_ADDRESS:
	case HDD2_TEMP_ADDRESS:
	case HDD3_TEMP_ADDRESS:
	case HDD4_TEMP_ADDRESS:
	case HDD5_TEMP_ADDRESS:
	case HDD6_TEMP_ADDRESS:
	case HDD7_TEMP_ADDRESS:
		read_hdd_temp(read_address, data);
		break;
#endif
	}
}

void write_gpu_temp(uint8_t temp)
{
	uint8_t old_temp = computer_data.gpu_temp;
	if (old_temp != temp){
		computer_data.gpu_temp = temp;
		update_information_frame(SHOW_GPU_TEMPERTURE,true);
	}
}

void handle_sram_write_request(uint8_t write_address, uint8_t data)
{
	switch (write_address){
		case GPU_TEMP_ADDRESS:
			write_gpu_temp(data);
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
		case CPU_STATUS_ADDRESS:
			write_cpu_status(data);
			break;
		case HDD0_TEMP_ADDRESS:
		case HDD1_TEMP_ADDRESS:
		case HDD2_TEMP_ADDRESS:
		case HDD3_TEMP_ADDRESS:
		case HDD4_TEMP_ADDRESS:
		case HDD5_TEMP_ADDRESS:
		case HDD6_TEMP_ADDRESS:
		case HDD7_TEMP_ADDRESS:
			write_hdd_temp(write_address, data);
			break;
		case HDD_STATUS_ADDRESS:
			write_hdd_status(data);
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
		case HDD0_LSB_SZ_ADDRESS:
		case HDD1_LSB_SZ_ADDRESS:
		case HDD2_LSB_SZ_ADDRESS:
		case HDD3_LSB_SZ_ADDRESS:
		case HDD4_LSB_SZ_ADDRESS:
		case HDD5_LSB_SZ_ADDRESS:
		case HDD6_LSB_SZ_ADDRESS:
		case HDD7_LSB_SZ_ADDRESS:
			write_hd_sz_lsb(write_address, data);
			break;
		case HDD0_MSB_SZ_ADDRESS:
		case HDD1_MSB_SZ_ADDRESS:
		case HDD2_MSB_SZ_ADDRESS:
		case HDD3_MSB_SZ_ADDRESS:
		case HDD4_MSB_SZ_ADDRESS:
		case HDD5_MSB_SZ_ADDRESS:
		case HDD6_MSB_SZ_ADDRESS:
		case HDD7_MSB_SZ_ADDRESS:
			write_hd_sz_msb(write_address, data);
			break;
		case DIRECT_CONTENT_ADDRESS:
		case DIRECT_TYPE_ADDRESS:
			write_direct(write_address, data);
			break;
		case RESET_ADDRESS:
			write_reset(data);
			break;
	}
}
