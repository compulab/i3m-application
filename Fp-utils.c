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

}

void set_gpu_updated_temp(char **data)
{
//	uint8_t temp = computer_data.gpu_temp;
//	set_temp_string(data,temp);
}

void set_cpu_updated_temp(char **data)
{
//	uint8_t temp = computer_data.cpu_temp;
//	set_temp_string(data,temp);
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

void update_data_by_type(enum information_type type, char **data)
{
	func_ptr ptr= NULL;
	switch (type){
	case SHOW_VOLTAGE:
		ptr = set_voltage_data;
		break;
	case SHOW_POWER_STATE:
		ptr = set_state;
		break;
	case SHOW_CPU_TEMPERTURE:
		ptr = set_cpu_updated_temp;
		break;
	case SHOW_GPU_TEMPERTURE:
		ptr = set_gpu_updated_temp;
		break;
	default:
		break;
	}
	if (ptr != NULL)
		ptr(data);
}
