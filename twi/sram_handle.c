/*
 * sram_handle.c
 *
 *  Created on: Sep 17, 2015
 *      Author: arkadi
 */

#include "../debug.h"
#define SRAM_DEBUG
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
uint16_t dmi_eeprom_index;

bool is_length_set;

struct direct_string_item *direct_string_to_add;
enum dmi_state_t dmi_curr_state;

void clear_direct_help_vars()
{
//	uart_send_string("direct string clear");
	direct_write_index = 0;
	direct_write_length = 0;
	is_length_set = false;
}

void init_direct_write_vars()
{
//	uart_send_string("direct string init");
	direct_string_to_add = 0;
	dmi_curr_state = DMI_IDLE;
	clear_direct_help_vars();
}

void sram_handle_init()
{
	init_direct_write_vars();
}

bool is_valid_register(int8_t index, uint8_t max_index)
{
	return index >= 0 && index < max_index;
}

void clear_req()
{
	if (layout.direct.i2c[PENDR0] == 0)
		layout.l.req = 0;
}

void write_cpu_fq_msb(uint8_t cpu_addr)
{
	int8_t index = (cpu_addr - CPU0F_MSB)/2;
	if (!is_valid_register(index,MAX_CPU))
		return ;
	if (layout.direct.i2c[cpu_addr] & CPU_FQ_MSB_MSK) {
		computer_data.packed.cpuf[index] = (layout.direct.i2c[cpu_addr] & CPU_FQ_MSB_MSK) << 8 | layout.direct.i2c[cpu_addr - 1];
		if (layout.direct.i2c[cpu_addr] & CPU_FQ_VALID_BIT)
			computer_data.packed.cpufs |= (0x01) << index;
		else
			computer_data.packed.cpufs &= ~((0x01) << index);
		computer_data.packed.cpufq_update |= (0x01) << index;
	}
	layout.l.cpufr = 0;
	clear_req();
}

void read_temp_control(uint8_t *data)
{
	*data = 0x02 & computer_data.packed.other_temp_status;
}

void write_temp_control()
{
	computer_data.details.gput = layout.l.gpus;
	if (computer_data.details.gput && (layout.l.gput != computer_data.details.gput))
		computer_data.details.gput = layout.l.gput;
}

void write_hd_sz_msb(uint8_t hdd_addr)
{
	int8_t index = (hdd_addr - HDD0_SZ_MSB) / 2;
	if (!is_valid_register(index, MAX_HDD))
		return ;//////////
	if (layout.direct.i2c[hdd_addr] & HDD_SZ_STATUS_MSK) {
		computer_data.packed.hddsz[index] = (layout.direct.i2c[hdd_addr - 1] & ~ MSB_MSK) | ((layout.direct.i2c[hdd_addr] & HDD_SZ_MSK) << 8);
		computer_data.packed.hdds |= (0x01 << index);
		uint8_t factor = (layout.direct.i2c[hdd_addr] & HDD_SZ_UNIT_MSK) != 0 ? 1 : 0;
		computer_data.packed.hddf |= (0x01 << index) & factor;
	}
}

void software_reset()
{
  uint8_t oldInterruptState = SREG;  // no real need to store the interrupt context as the reset will pre-empt its restoration
  cli();

  CCP = 0xD8;                        // Configuration change protection: allow protected IO regiser write
  RST.CTRL = RST_SWRST_bm;           // Request software reset by writing to protected IO register

  SREG=oldInterruptState;            // Restore interrupts enabled/disabled state (out of common decency - this line will never be reached because the reset will pre-empt it)
}

void reset_to_bootloader()
{
	uint8_t magic = nvm_eeprom_read_byte(BOOTLOADER_MAGIC_EEPROM_ADDRESS);
	magic &= ~BOOTLOADER_APPLICATION_START;
	nvm_eeprom_write_byte(BOOTLOADER_MAGIC_EEPROM_ADDRESS, magic);
	delay_ms(4);
	software_reset();
}

void validate_temperate(bool *valid_bit, uint8_t *dest, uint8_t src)
{
	*valid_bit = true;
	*dest = src;
}

void write_cpu_status()
{
	layout.l.cputr = 0;
	clear_req();
	if (layout.direct.i2c[CPUTS] == 0){
		computer_data.packed.cputs = 0;
	} else {
		computer_data.packed.cputs |= layout.direct.i2c[CPUTS];
		uint8_t bit = 0x01;
		for (uint8_t i = 0 ; i < MAX_CPU; i++){
			if (layout.direct.i2c[CPUTS] & bit) {
				if (computer_data.packed.cput[i] != layout.direct.i2c[CPU0T + i]) {
					computer_data.packed.cput[i] = layout.direct.i2c[CPU0T + i];
					computer_data.packed.cput_update |= bit;
				}
			}
			bit = bit << 1;
		}
	}
}



void write_hdd_status()
{
	if (layout.direct.i2c[HDDTS]  == 0){
		computer_data.packed.hddts = 0;
	} else {
		uint8_t bit = 0x01;
		for (uint8_t i = 0 ; i < MAX_CPU; i++){
			if (layout.direct.i2c[CPUTS] & bit)
				computer_data.packed.hddt[i] = layout.direct.i2c[CPU0T + i];
			bit = bit << 1;
		}
	}
	layout.l.hddtr = 0;
	clear_req();
}

void write_reset()
{
	if (layout.l.rstusb)
		reset_to_bootloader();
	else if (layout.l.rst)
		software_reset();
}

void write_post_code_lsb()
{
	computer_data.packed.post_code = layout.l.bios_post_code;
}

void write_memory(uint8_t mem_addr) //Todo: change memory status set
{
	uint8_t index = (mem_addr - MEM_LSB ) * 2;
	uint8_t data = layout.direct.i2c[mem_addr];
	computer_data.packed.memsz[index] = data & MEM_SZ_LSB_MSK;
	if (data & MEM_SZ_STATUS_LSB_MSK)
		computer_data.packed.mems |= 0x01 << index;
	computer_data.packed.memsz[index + 1] = (data & MEM_SZ_MSB_MSK) >> 4;
	if (data & MEM_SZ_STATUS_MSB_MSK)
		computer_data.packed.mems |= 0x01 << (index + 1);
}

void read_bios_post_code(enum i2c_addr_space post_code_address, uint8_t *data)
{
	switch(post_code_address){
	case POST_CODE_LSB:
		*data = computer_data.details.post_code_lsb;
		break;
	case POST_CODE_MSB:
		*data = computer_data.details.post_code_msb;
		break;
	default:
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
	if (layout.l.ambs != 0)
		*data = layout.l.ambt;
	else
		*data = DEFAULT_DATA;
}

void read_adc(enum i2c_addr_space adc_address, uint8_t *data)
{
	switch (adc_address){
	case ADC_LSB:
		//TODO
		break;
	case ADC_MSB:
		//TODO
		break;
	default:
		break;
	}
}

void free_direct_string(void)
{
	if (direct_string_to_add != NULL){
		free(direct_string_to_add->content);
		free(direct_string_to_add->type);
	}
	free(direct_string_to_add);
	dmi_curr_state = DMI_IDLE;
	clear_direct_help_vars();
}

void read_iwren(uint8_t *data)
{
	*data = layout.l.iwren;
}

void read_hdd_status(uint8_t *data)
{
	*data = computer_data.packed.hddts;
}

void read_cpu_stauts(uint8_t *data)
{
	*data = computer_data.packed.cputs;
}

void read_pending_requests(uint8_t *data)
{
	*data = 0x0f & computer_data.packed.pending_req;
}

int set_direct_string()
{
	direct_string_to_add = malloc_locked(sizeof(struct direct_string_item));
	if (direct_string_to_add == NULL)
		return -1;
	direct_string_to_add->content = 0;
	direct_string_to_add->next = 0;
	direct_string_to_add->type = 0;
	return 0;
}

void get_dmi_string(char **str)
{
	uint8_t len = eeprom_read_byte(dmi_eeprom_index);
	(*str) = malloc_locked(sizeof(char) * (len + 1));
	for (int i = 0; i < len; i++, dmi_eeprom_index++)
		(*str)[i] = eeprom_read_byte(dmi_eeprom_index);
	(*str)[len] = '\0';
	dmi_eeprom_index++;
}

void dmi_init()
{
	dmi_eeprom_index = EEPROM_DMI_START;
	uint8_t dmi_count = eeprom_read_byte(EEPROM_DMI_COUNT);
	struct direct_string_item *direct_string;

	while (dmi_count) {
		while(eeprom_read_byte(dmi_eeprom_index) == '\0')
			dmi_eeprom_index++;
		direct_string = malloc_locked(sizeof(struct direct_string_item));
		if (direct_string == NULL)
			return;

		direct_string->backup_addr = dmi_eeprom_index + 1;
		get_dmi_string(&direct_string->type);
		get_dmi_string(&direct_string->content);
		if (computer_data.details.direct_string == NULL)
			computer_data.details.direct_string = direct_string;
		else
			computer_data.details.direct_string->next = direct_string;
		direct_string = NULL;
		dmi_count--;
	}
}

void add_dmi_backup(struct direct_string_item *dmi)
{
	uint8_t dmi_count = eeprom_read_byte(EEPROM_DMI_COUNT);
	dmi->backup_addr = dmi_eeprom_index;
	dmi_count++;
	uint16_t writing_index = dmi->backup_addr;
	eeprom_write_byte(writing_index, strlen(dmi->type) + 1);
	writing_index++;
	for (int i = 0; i < strlen(dmi->type); i++, writing_index++)
		eeprom_write_byte(writing_index, dmi->type[i]);
	eeprom_write_byte(writing_index, strlen(dmi->content) + 1);
	writing_index++;
	for (int i = 0; i < strlen(dmi->content); i++, writing_index++)
		eeprom_write_byte(writing_index, dmi->content[i]);
	eeprom_write_byte(EEPROM_DMI_COUNT, dmi_count);
}

void remove_dmi_backup(struct direct_string_item *dmi)
{
	uint8_t dmi_count = eeprom_read_byte(EEPROM_DMI_COUNT);
	if (dmi_count == 0)
		return;
	dmi_count--;
	uint8_t length = strlen(dmi->type) + strlen(dmi->content) + 1;
	for (uint8_t i = 0; i < length; i++)
		eeprom_write_byte(dmi->backup_addr + i, '\0');
	eeprom_write_byte(EEPROM_DMI_COUNT, dmi_count);
}

void update_dmi_backup(struct direct_string_item *dmi)
{
	uint16_t start_index = dmi->backup_addr + strlen(dmi->type) + 2;
	uint8_t new_length = strlen(dmi->content);
	uint8_t old_length = eeprom_read_byte(start_index);
	if (new_length < old_length) {
		eeprom_write_byte(start_index, new_length);
		for (uint16_t blank_index = start_index +  new_length + 1; blank_index < start_index + old_length + 1; blank_index++)
			eeprom_write_byte(blank_index, '\0');
	}
	start_index++;
	for(uint8_t i = 0; i < new_length; i++, start_index++)
		eeprom_write_byte(start_index, dmi->content[i]);
}

void add_direct_string()
{
	uart_send_string("direct string added");
	struct direct_string_item *curr = computer_data.details.direct_string;
	while (curr != NULL) {
		if (strcmp(direct_string_to_add->type, curr->type))
			break;
		else
			curr = curr->next;
	}

	if (curr != NULL) { /* change existing DMI string */
		if (strlen(direct_string_to_add->content) > strlen(curr->content)) {
			remove_dmi_backup(curr);
			strdup(direct_string_to_add->content);
			add_dmi_backup(curr);
		} else {
			strdup(direct_string_to_add->content);
			update_dmi_backup(curr);
		}
	} else {
		direct_string_to_add->next = computer_data.details.direct_string;
		computer_data.details.direct_string = direct_string_to_add;
		add_dmi_backup(direct_string_to_add);
		direct_string_to_add = 0;
	}

	free_direct_string();
}


void end_direct_string(bool is_name_byte)
{
	if(is_name_byte && dmi_curr_state == DMI_NAME_WRITE){
		direct_string_to_add->type[direct_write_length]='\0';
		dmi_curr_state = DMI_VALUE_WRITE;
		uart_send_string("now value turn\n\r");
		clear_direct_help_vars();
	} else if (dmi_curr_state == DMI_VALUE_WRITE){
		direct_string_to_add->content[direct_write_length]='\0';
		add_direct_string();
	} else{
		init_direct_write_vars();
		return;
	}
}

bool is_iwren_mode()
{
	bool res = layout.l.iwren != 0;
	layout.l.iwren = 0;
	return res;
}

void write_byte_direct_string(bool is_written_type, uint8_t data)
{
	if (is_written_type)
		direct_string_to_add->type[direct_write_index] = data;
	else
		direct_string_to_add->content[direct_write_index] = data;
	direct_write_index++;
	uart_send_string(" direct write index: ");
	uart_send_num(direct_write_index, 10);
	uart_send_string(" direct write length: ");
	uart_send_num(direct_write_length, 10);
	uart_send_string("\n\r");
}

void set_written_length(bool is_written_name, uint8_t data)
{
	direct_write_length = data;
	direct_write_index = 0;
	uart_send_string(" length set: ");
	uart_send_num(data, 16);
	uart_send_string("\t \n\r");
	is_length_set = true;
	if (is_written_name){
		if (direct_string_to_add == 0){
			if (set_direct_string() != 0) {
				uart_send_string("malloc failed\n\r");
				free_direct_string();
				return ;
			} else {
				direct_string_to_add->type = malloc_locked(sizeof(char *) * direct_write_length + 1);
				if (direct_string_to_add->type == NULL) {
					uart_send_string("malloc failed ");
					free_direct_string();
					return ;
				}
				uart_send_string(" num of chars in string: ");
				uart_send_num(data, 10);
			}
		} else {
			uart_send_string("written value not name!!\n\r");
			free_direct_string();
			return;
		}
	} else if (dmi_curr_state == DMI_VALUE_WRITE){
		direct_string_to_add->content = malloc_locked(sizeof(char *) * direct_write_length + 1);
		if (direct_string_to_add->content == NULL) {
			free(direct_string_to_add->type);
			free(direct_string_to_add);
			clear_direct_help_vars();
			return ;
		}
	} else {
		init_direct_write_vars();
		return;
	}
}

void write_direct_byte(bool is_written_type, uint8_t data)
{
	uart_send_num(data, 16);
	if (is_length_set){
		uart_send_string(" byte added ");
		write_byte_direct_string(is_written_type, data);
		if (direct_write_length == direct_write_index) {
			uart_send_string(" last byte in written ");
			end_direct_string(is_written_type);
		}
	} else {
		uart_send_string(" first byte is written ");
		set_written_length(is_written_type, data);
		if (is_written_type)
			dmi_curr_state = DMI_NAME_WRITE;
	}
}

void write_direct(enum i2c_addr_space write_address)
{
	switch (write_address){
	case DMIN:
		uart_send_string("direct write name byte ");
		if (dmi_curr_state == DMI_VALUE_WRITE) {
			uart_send_string("DMI reset- START NEW NAME\n\r");
			free_direct_string();
			break;
		}
		write_direct_byte(true, layout.l.dmi_name);
		break;
	case DMIV:
		if (dmi_curr_state != DMI_VALUE_WRITE) {
			uart_send_string("DMI reset - NOT NAME WRITE TIME\n\r");
			break;
		}
		uart_send_string("direct write value byte");
		write_direct_byte(false,layout.l.dmi_value);
		break;
	default:
		break;
	}
	uart_send_string("write direct end \n\r");
}

void handle_sram_read_request(enum i2c_addr_space addr, uint8_t *data)
{
	switch (addr) {
	case SIG0:
	case SIG1:
	case SIG2:
	case SIG3:
	case LAYOUT_VER:
	case MAJOR_LSB:
	case MAJOR_MSB:
	case MINOR_LSB:
	case MINOR_MSB:
	case RTCT:
	case RTCD:
	case ADC_LSB:
	case ADC_MSB:
	case AMBT:
	case SENSORT:
	case POST_CODE_LSB:
	case POST_CODE_MSB:
	case FPCTRL:
	case REQ:
	case PENDR0:
	case POWER_STATE:
		*data = layout.direct.i2c[addr];
		break;
	case HDD0T:
	case HDD1T:
	case HDD2T:
	case HDD3T:
	case HDD4T:
	case HDD5T:
	case HDD6T:
	case HDD7T:
	case HDDTS:
	case HDD0_SZ_LSB:
	case HDD1_SZ_LSB:
	case HDD2_SZ_LSB:
	case HDD3_SZ_LSB:
	case HDD4_SZ_LSB:
	case HDD5_SZ_LSB:
	case HDD6_SZ_LSB:
	case HDD7_SZ_LSB:
	case HDD0_SZ_MSB:
	case HDD1_SZ_MSB:
	case HDD2_SZ_MSB:
	case HDD3_SZ_MSB:
	case HDD4_SZ_MSB:
	case HDD5_SZ_MSB:
	case HDD6_SZ_MSB:
	case HDD7_SZ_MSB:
	case MEM_LSB :
	case MEM_MSB:
	case CPU0T:
	case CPU1T:
	case CPU2T:
	case CPU3T:
	case CPU4T:
	case CPU5T:
	case CPU6T:
	case CPU7T:
	case CPUTS:
	case CPU0F_MSB:
	case CPU1F_MSB:
	case CPU2F_MSB:
	case CPU3F_MSB:
	case CPU4F_MSB:
	case CPU5F_MSB:
	case CPU6F_MSB:
	case CPU7F_MSB:
	case CPU0F_LSB:
	case CPU1F_LSB:
	case CPU2F_LSB:
	case CPU3F_LSB:
	case CPU4F_LSB:
	case CPU5F_LSB:
	case CPU6F_LSB:
	case CPU7F_LSB:
	case GPUT:
	case DMIN:
	case DMIV:
	default:
		if (is_iwren_mode())
			*data = layout.direct.i2c[addr];
		break;
	}
}

void write_gpu_temp()
{
	computer_data.details.gpus = layout.l.gpus;
	if (computer_data.details.gpus == 1) {
		if (layout.l.gput != computer_data.details.gput) {
			computer_data.details.gput = layout.l.gput;
		}
	}
	layout.l.gputr = 0;
	clear_req();
}

void update_data(void *write_address)
{
	uint8_t addr = (uint16_t)write_address;
//	uart_send_num(addr, 16);
//	uart_send_string("update sram\n\r");
	switch (addr){
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
		write_post_code_lsb();
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
	is_twi_busy = false;
}

void write_data(enum i2c_addr_space addr, uint8_t data)
{
	switch (addr) {
	case RTCT:
	case RTCD:
	case GPUT:
	case CPU0T:
	case CPU1T:
	case CPU2T:
	case CPU3T:
	case CPU4T:
	case CPU5T:
	case CPU6T:
	case CPU7T:
	case CPUTS:
	case HDD0T:
	case HDD1T:
	case HDD2T:
	case HDD3T:
	case HDD4T:
	case HDD5T:
	case HDD6T:
	case HDD7T:
	case HDDTS:
	case CPU0F_MSB:
	case CPU1F_MSB:
	case CPU2F_MSB:
	case CPU3F_MSB:
	case CPU4F_MSB:
	case CPU5F_MSB:
	case CPU6F_MSB:
	case CPU7F_MSB:
	case CPU0F_LSB:
	case CPU1F_LSB:
	case CPU2F_LSB:
	case CPU3F_LSB:
	case CPU4F_LSB:
	case CPU5F_LSB:
	case CPU6F_LSB:
	case CPU7F_LSB:
	case MEM_LSB :
	case MEM_MSB:
	case SENSORT:
	case POST_CODE_LSB:
	case POST_CODE_MSB:
	case HDD0_SZ_LSB:
	case HDD1_SZ_LSB:
	case HDD2_SZ_LSB:
	case HDD3_SZ_LSB:
	case HDD4_SZ_LSB:
	case HDD5_SZ_LSB:
	case HDD6_SZ_LSB:
	case HDD7_SZ_LSB:
	case HDD0_SZ_MSB:
	case HDD1_SZ_MSB:
	case HDD2_SZ_MSB:
	case HDD3_SZ_MSB:
	case HDD4_SZ_MSB:
	case HDD5_SZ_MSB:
	case HDD6_SZ_MSB:
	case HDD7_SZ_MSB:
	case DMIN:
	case DMIV:
	case FPCTRL:
		layout.direct.i2c[addr] = data;
		break;
	case SIG0:
	case SIG1:
	case SIG2:
	case SIG3:
	case LAYOUT_VER:
	case MAJOR_LSB:
	case MAJOR_MSB:
	case MINOR_LSB:
	case MINOR_MSB:
	case AMBT:
	case ADC_LSB:
	case ADC_MSB:
	case REQ:
	case PENDR0:
	case POWER_STATE:
	default:
		if (is_iwren_mode())
			layout.direct.i2c[addr] = data;
		break;
	}
}

struct work update_data_work = {
		.do_work = update_data,
		.next = NULL,
};

int handle_sram_write_request(uint8_t write_address, uint8_t data)
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
