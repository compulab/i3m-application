/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"
#include "debug.h"
#include <string.h>

#define MAX_DIGITS 5
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

int num_of_digits(int x) {
	if (x < 0)
		x *= -1;
	for (int i = 1; i < MAX_DIGITS; i++)
		if (x < pow(10, i))
		  return i;
  return 0;
}

void set_fq_string(char **str, uint16_t fq)
{
	*str = malloc(sizeof(char) * 10);
	char fq_str[5];
	itoa(fq,fq_str,10);
	strcpy((*str), fq_str);
	strcat((*str), " MHZ");
}

void set_temp_string(char **str, int8_t temperature)
{
	uint8_t length = num_of_digits(temperature);
	bool is_neg = temperature < 0;
	if (is_neg)
		length += 2;
	char *tempStr = malloc(sizeof(char) * length);
	itoa(temperature, tempStr, 10);
	*str = malloc(sizeof(char) * (length + 3));
	int i = 0;
	if (is_neg){
		strcpy((*str), "- ");
		i = 2;
	}
	for (; i < length; i++)
		(*str)[i] = tempStr[i];
	strcat((*str), " C");
}

void set_cpu_updated_temp(char **data,uint8_t cpu_id)
{
	uint8_t temp = computer_data.cpu_temp[cpu_id];
	if (computer_data.valid_cpu_temp[cpu_id])
		set_temp_string(data,temp);
	else
		set_invalid_string(data);
}

void set_updated_cpu_frequency(char **output_str, uint8_t cpu_id)
{
	if (computer_data.valid_cpu_fq[cpu_id])
		set_fq_string(output_str, computer_data.cpu_fq[cpu_id]);
	else
		set_invalid_string(output_str);
}

void set_updated_ambient_temp(char **output_str)
{
	update_ambient_temp();
	if (computer_data.valid_ambient_temp)
		set_temp_string(output_str, computer_data.gpu_temp);
	else
		set_invalid_string(output_str);
}
void set_updated_gpu_temp(char **output_str)
{
	if (computer_data.valid_gpu_temp)
		set_temp_string(output_str, computer_data.gpu_temp);
	else
		set_invalid_string(output_str);
}

void set_update_hdd_temp(char **output_str, uint8_t hdd_id)
{
	uint8_t temp = computer_data.hdd_temp[hdd_id];
	if (computer_data.valid_hdd_temp[hdd_id])
		set_temp_string(output_str,temp);
	else
		set_invalid_string(output_str);
}

void update_ambient_value(uint8_t ambient_temp)
{
	MSG("AMBIENT UPDATED")
	computer_data.ambient_temp = ambient_temp;
	computer_data.valid_ambient_temp = true;
}


void update_ambient_temp()
{
	computer_data.valid_ambient_temp = false;
	send_package(&twi_ambient_temp);
	delay_s(2);
}

void update_data_by_type(enum information_type type, char **output_str, uint8_t info)
{
	switch (type){
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
	case SHOW_VOLTAGE:
		set_voltage_data(output_str);
		break;
	case SHOW_POWER_STATE:
		set_state(output_str);
		break;
	case SHOW_CPU_TEMPERTURE:
		set_cpu_updated_temp(output_str,info);
		break;
	default:
		break;
	}
}
