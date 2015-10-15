/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"
#include "debug.h"


#define MAX_DIGITS 5
enum power_state current_power_state = POWER_ON;

char power_value[10];

void update_information_frame(enum information_type type, bool need_to_update)
{
	if (!present_menu->visible)
		if (information_present->info_type == type && need_to_update)
			update_information();
}

struct twi_package twi_ambient_temp = {
	.slave_address = AMBIENT_TWI_ADDRESS,
	.buffer[0] = TEMPERATURE_LT_ADDRESS,
	.length = 1,
	.is_write_request = false,
	.handle_data_received = update_ambient_value
};

void set_invalid_string(char *str){
	sprintf(str, "INVALID");
}

void update_power_state()
{
	enum power_state  last_power_state = current_power_state;
	if (gpio_pin_is_low(FP_S5))
		current_power_state = POWER_OFF;
	else if (gpio_pin_is_low(FP_S4))
		current_power_state = POWER_STD;
	else if (gpio_pin_is_low(FP_S3))
		current_power_state = POWER_STR;
	else
		current_power_state = POWER_ON;
	if (current_power_state != last_power_state)
		update_information_frame(SHOW_POWER_STATE,true);
}

void set_state(char *state)
{
	switch (current_power_state){
	case POWER_ON:
		strcpy(state, "Computer is on");
		break;
	case POWER_STR:
		strcpy(state, "Sleep mode");
		break;
	case POWER_STD:
		strcpy(state, "Hibernate mode");
		break;
	case POWER_OFF:
		strcpy(state, "Computer off");
		break;
	default:
		strcpy(state, "");
		break;
	}
}

int num_of_digits(int x) {
	if (x < 0)
		x *= -1;
	for (int i = 1; i < MAX_DIGITS; i++)
		if (x < pow(10, i))
		  return i;
  return 0;
}

void set_mem_size_str(char *str, uint8_t mem)
{
	uint8_t size;
	switch (mem){
	case 0x01:
	case 0x02:
		size = mem;
		break;
	case 0x03:
		size = 4;
		break;
	case 0x04:
		size = 8;
		break;
	case 0x05:
		size = 16;
		break;
	default:
		size = 0;
		break;
	}
	if (size == 0){
		str = EMPTY_SLOT;
	} else {
		sprintf(str,"%d GB",size);
	}
}

void set_hdd_size_str(char *str, uint16_t size, bool is_tera_units)
{
	char * units;
	if (is_tera_units)
			units = " TB";
		else
			units = " GB";
	sprintf(str, "%d %s", size, units);
}

void set_fq_string(char *str, uint16_t fq)
{
	sprintf(str, "%d MHZ", fq);
}

void set_temp_string(char *str, int8_t temperature)
{
	sprintf(str,"%d C",temperature);
}

void set_cpu_updated_temp(char *data, uint8_t cpu_id)
{
	if (computer_data.valid_cpu_temp[cpu_id]){
	uint8_t temp = computer_data.cpu_temp[cpu_id];
		set_temp_string(data, temp);
	} else {
		set_invalid_string(data);
	}
}

void set_updated_memory_size(char *output_str, uint8_t mem_id)
{
	if (computer_data.valid_mem[mem_id])
		set_mem_size_str(output_str, computer_data.mem_slot_sz[mem_id]);
	else
		set_invalid_string(output_str);
}

void set_updated_hdd_size(char *output_str, uint8_t hdd_id)
{
	if (computer_data.valid_hdd_size[hdd_id])
		set_hdd_size_str(output_str, computer_data.hdd_size[hdd_id], computer_data.hdd_tera_units[hdd_id]);
	else
		set_invalid_string(output_str);
}

void set_updated_cpu_frequency(char *output_str, uint8_t cpu_id)
{
	if (computer_data.valid_cpu_fq[cpu_id])
		set_fq_string(output_str, computer_data.cpu_fq[cpu_id]);
	else
		set_invalid_string(output_str);
}

void set_updated_ambient_temp(char *output_str)
{
	update_ambient_temp();
	if (computer_data.valid_ambient_temp)
		set_temp_string(output_str, computer_data.gpu_temp);
	else
		set_invalid_string(output_str);
}

void update_adc()
{
	char last_value[10];
	strcpy(last_value,power_value);
	set_power_data(power_value);
	if (strcmp(power_value,last_value) != 0)
		update_information_frame(SHOW_POWER, true);
}


void set_updated_gpu_temp(char *output_str)
{
	if (computer_data.valid_gpu_temp)
		set_temp_string(output_str, computer_data.gpu_temp);
	else
		set_invalid_string(output_str);
}

void set_serial_number(char *output_str)
{
	char serial[SERIAL_NUMBER_LENGTH];
	for (int i=0; i < SERIAL_NUMBER_LENGTH; i++)
		serial[i] = eeprom_read_byte(SERIAL_NUMBER_EEPROM_ADDRESS + i);
	strcpy(output_str, serial);
}

void set_product_name(char *output_str)
{
	char product_name[PRODUCT_NAME_LENGTH];
	for (int i = 0; i < PRODUCT_NAME_LENGTH; i++)
		product_name[i] = eeprom_read_byte(PRODUCT_NAME_EEPROM_ADDRESS + i);
	strcpy(output_str, product_name);
}

void set_mac_address(char *output_str)
{
	char mac_address[MAC_ADDRESS_LENGTH];
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		mac_address[i] = eeprom_read_byte(MAC_ADDRESS_EEPROM_ADDRESS + i);
	strcpy(output_str, mac_address);
}

void set_update_hdd_temp(char *output_str, uint8_t hdd_id)
{
	uint8_t temp = computer_data.hdd_temp[hdd_id];
	if (computer_data.valid_hdd_temp[hdd_id])
		set_temp_string(output_str,temp);
	else
		set_invalid_string(output_str);
}

void update_ambient_value(uint8_t ambient_temp)
{
	computer_data.ambient_temp = ambient_temp;
	computer_data.valid_ambient_temp = true;
}


void update_ambient_temp()
{
	computer_data.valid_ambient_temp = false;
	send_package(&twi_ambient_temp);
	delay_s(2);
}

void set_dmi_content(char *output_str, uint8_t string_id)
{
	uint8_t count = 0;
	struct direct_string_item * string_item = computer_data.direct_string;
	while (string_item != 0 && count < string_id){
		string_item = string_item->next;
		count++;
	}
	if (count == string_id)
		output_str = strdup(string_item->content);
	else
		set_invalid_string(output_str);
}

void update_data_by_type(enum information_type type, char *output_str, uint8_t info)
{
	switch (type){
	case SHOW_MEMORY_SIZE:
		set_updated_memory_size(output_str, info);
		break;
	case SHOW_HDD_SIZE:
		set_updated_hdd_size(output_str, info);
		break;
	case SHOW_AMBIENT_TEMPERATURE:
		set_updated_ambient_temp(output_str);
		break;
	case SHOW_GPU_TEMPERTURE:
		set_updated_gpu_temp(output_str);
		break;
	case SHOW_CPU_FREQUENCY:
		set_updated_cpu_frequency(output_str, info);
		break;
	case SHOW_HDD_TEMPERTURE:
		set_update_hdd_temp(output_str, info);
		break;
	case SHOW_POWER:
		set_power_data(output_str);
		break;
	case SHOW_SERIAL_NUMBER:
//		set_serial_number(output_str);
		strcpy(output_str, "TEST12TEST"); //TODO
		break;
	case SHOW_PRODUCT_NAME:
//		set_product_name(output_str);
		strcpy(output_str, "1.0"); //TODO
		break;
	case SHOW_MAC_ADDRESS:
//		set_mac_address(output_str);
		strcpy(output_str, "0t:3e:4s:5t"); //TODO
		break;
	case SHOW_POWER_STATE:
		set_state(output_str);
		break;
	case SET_BRIGHTNESS:
		strcpy(output_str, "test"); //TODO
		break;
	case SHOW_CPU_TEMPERTURE:
		set_cpu_updated_temp(output_str, info);
		break;
	default:
		break;
	}
}
