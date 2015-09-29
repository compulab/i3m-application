/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"

enum power_state current_power_state = POWER_ON;

struct twi_package twi_ambient_temp = {
	.slave_address = AMBIENT_TWI_ADDRESS,
	.buffer[0] = TEMPERATURE_LT_ADDRESS,
	.length = 1,
	.is_write_request = false,
	.handle_data_received = update_ambient_value
};

void set_invalid_string(char **str){
	*str = "INVALID";
}

void update_power_state()
{
	if (gpio_pin_is_low(FP_S5))
		current_power_state = POWER_OFF;
	else if (gpio_pin_is_low(FP_S4))
		current_power_state = POWER_STD;
	else if (gpio_pin_is_low(FP_S3))
		current_power_state = POWER_STR;
	else
		current_power_state = POWER_ON;
}

void set_state(char **data)
{
	char *state = "";
	switch (current_power_state){
	case POWER_ON:
		state = "Computer is on";
		break;
	case POWER_STR:
		state = "Sleep mode";
		break;
	case POWER_STD:
		state = "Hibernate mode";
		break;
	case POWER_OFF:
		state = "Computer off";
		break;
	}
	*data = state;
}


void set_temp_string(char **str, uint8_t temp)
{
	uint8_t length = roundf(log(temp));
	if (temp < 0)
		length += 2;
	char *tempStr = malloc(sizeof(char) * length);
	itoa(temp, tempStr, 10);
	*str = malloc(sizeof(char) * (length + 3));
	int i=0;
	if (temp < 0){
		(*str[i++]) = '-';
		(*str[i++]) = ' ';
	}
	for (; i < length; i++)
		(*str)[i] = tempStr[i];
	(*str)[length] = ' ';
	(*str)[length + 1] = 'C';
	(*str)[length + 2] = '\0';
}

void set_gpu_updated_temp(char **data)
{
	uint8_t temp = computer_data.gpu_temp;
	if (computer_data.valid_gpu)
		set_temp_string(data,temp);
	else
		set_invalid_string(data);
}

void set_cpu_updated_temp(char **data,uint8_t cpu_id)
{
	uint8_t temp = computer_data.cpu_temp[cpu_id];
	if ((temp & CPU_TMP_VALID_BIT) != 0)
		set_temp_string(data,temp);
	else
		set_invalid_string(data);
}

void set_update_hdd_temp(char **data, uint8_t hdd_id){
	uint8_t temp = computer_data.hd_temp[hdd_id];
	if ((temp & HDD_TMP_VALID_BIT) != 0)
		set_temp_string(data,temp);
	else
		set_invalid_string(data);
}

void update_ambient_value(uint8_t high_bit)
{
	computer_data.ambient_temp = 0x00 | high_bit << 8;
	computer_data.valid_ambient = true;
}


void update_ambient_temp()
{
	send_package(&twi_ambient_temp);
}

void update_data_by_type(enum information_type type, char **output_str, uint8_t info)
{
	switch (type){
	case SHOW_HDD_TEMPERTURE:
		set_update_hdd_temp(output_str,info);
		break;
	case SHOW_VOLTAGE:
		set_voltage_data(output_str);
		break;
	case SHOW_POWER_STATE:
		set_state(output_str);
		break;
	case SHOW_CPU_TEMPERTURE:
		set_cpu_updated_temp(output_str,info);
		break;
	case SHOW_GPU_TEMPERTURE:
		set_gpu_updated_temp(output_str);
		break;
	default:
		break;
	}
}
