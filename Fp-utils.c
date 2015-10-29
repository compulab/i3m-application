/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"

#define MAX_DIGITS 5

enum power_state current_power_state = POWER_ON;

char power_value[10];

void update_information_frame(enum information_type type, bool need_to_update)
{
	if (!present_menu->visible) {
		if (information_present->info_type == type && need_to_update) {
			update_information();
		}
	}
}

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
		strcpy(state, "Airtop on");
		break;
	case POWER_STR:
		strcpy(state, "Sleep");
		break;
	case POWER_STD:
		strcpy(state, "Hibernate");
		break;
	case POWER_OFF:
		strcpy(state, "Airtop off");
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

void set_hdd_size_str(char *str, uint16_t size, bool is_tera)
{
	char * units;
	if (is_tera)
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
	if ((computer_data.packed.cputs & (0x01 << cpu_id)) != 0){
	uint8_t temp = computer_data.details.cput[cpu_id];
		set_temp_string(data, temp);
	} else {
		set_invalid_string(data);
	}
}

//bool is_valid_mem(uint8_t id)
//{
//	bool res = false;
//	switch (id) {
//	case 0:
//		res = computer_data.details.mem0s == 1;
//		break;
//	case 1:
//		res = computer_data.details.mem1s == 1;
//		break;
//	case 2:
//		res = computer_data.details.mem2s == 1;
//		break;
//	case 3:
//		res = computer_data.details.mem3s == 1;
//		break;
//	default:
//		break;
//	}
//	return res;
//}
//
//bool is_valid_hdd(uint8_t id)
//{
//	bool res = false;
//	switch (id) {
//	case 0:
//		res = computer_data.details.hdd0s == 1;
//		break;
//	case 1:
//		res = computer_data.details.hdd1s == 1;
//		break;
//	case 2:
//		res = computer_data.details.hdd2s == 1;
//		break;
//	case 3:
//		res = computer_data.details.hdd3s == 1;
//		break;
//	case 4:
//		res = computer_data.details.hdd4s == 1;
//		break;
//	case 5:
//		res = computer_data.details.hdd5s == 1;
//		break;
//	case 6:
//		res = computer_data.details.hdd6s == 1;
//		break;
//	case 7:
//		res = computer_data.details.hdd7s == 1;
//		break;
//	default:
//		break;
//	}
//	return res;
//}
//
//bool is_valid_cpu_fq(uint8_t id)
//{
//	bool res = false;
//	switch (id) {
//	case 0:
//		res = computer_data.details.cpu0fs == 1;
//		break;
//	case 1:
//		res = computer_data.details.cpu1fs == 1;
//		break;
//	case 2:
//		res = computer_data.details.cpu2fs == 1;
//		break;
//	case 3:
//		res = computer_data.details.cpu3fs == 1;
//		break;
//	case 4:
//		res = computer_data.details.cpu4fs == 1;
//		break;
//	case 5:
//		res = computer_data.details.cpu5fs == 1;
//		break;
//	case 6:
//		res = computer_data.details.cpu6fs == 1;
//		break;
//	case 7:
//		res = computer_data.details.cpu7fs == 1;
//		break;
//	default:
//		break;
//	}
//	return res;
//}
//
//uint8_t get_mem_sz(uint8_t id)
//{
//	uint8_t size = 0;
//	switch (id) {
//	case 0:
//		size = computer_data.details.mem0sz;
//		break;
//	case 1:
//		size = computer_data.details.mem1sz;
//		break;
//	case 2:
//		size = computer_data.details.mem2sz;
//		break;
//	case 3:
//		size = computer_data.details.mem3sz;
//		break;
//	default:
//		break;
//	}
//	return size;
//}
//
//uint16_t get_hdd_sz_with_factor(uint8_t id)
//{
//	uint16_t size = 0;
//	switch (id) {
//	case 0:
//		size = computer_data.details.hdd0sz;
//		if (computer_data.details.hdd0f == 1)
//			size |= 0x0400;
//		break;
//	case 1:
//		size = computer_data.details.hdd1sz;
//		if (computer_data.details.hdd1f == 1)
//			size |= 0x0400;
//		break;
//	case 2:
//		size = computer_data.details.hdd2sz;
//			if (computer_data.details.hdd2f == 1)
//				size |= 0x0400;
//			break;
//	case 3:
//		size = computer_data.details.hdd3sz;
//			if (computer_data.details.hdd3f == 1)
//				size |= 0x0400;
//			break;
//	case 4:
//			size = computer_data.details.hdd4sz;
//			if (computer_data.details.hdd4f == 1)
//				size |= 0x0400;
//			break;
//	case 5:
//		size = computer_data.details.hdd5sz;
//		if (computer_data.details.hdd5f == 1)
//			size |= 0x0400;
//		break;
//	case 6:
//		size = computer_data.details.hdd6sz;
//		if (computer_data.details.hdd6f == 1)
//			size |= 0x0400;
//		break;
//	case 7:
//		size = computer_data.details.hdd7sz;
//		if (computer_data.details.hdd7f == 1)
//			size |= 0x0400;
//			break;
//	default:
//		break;
//	}
//	return size;
//}
//
//uint16_t get_cpu_fq(uint8_t id)
//{
//	uint16_t fq = 0;
//	switch (id) {
//	case 0:
//		fq = computer_data.details.cpu0f;
//		break;
//	case 1:
//		fq = computer_data.details.cpu1f;
//		break;
//	case 2:
//		fq = computer_data.details.cpu2f;
//		break;
//	case 3:
//		fq = computer_data.details.cpu3f;
//		break;
//	case 4:
//		fq = computer_data.details.cpu4f;
//		break;
//	case 5:
//		fq = computer_data.details.cpu5f;
//		break;
//	case 6:
//		fq = computer_data.details.cpu6f;
//		break;
//	case 7:
//		fq = computer_data.details.cpu7f;
//		break;
//	default:
//		break;
//	}
//	return fq;
//}

void set_updated_memory_size(char *output_str, uint8_t mem_id)
{
	if ((computer_data.packed.mems & (0x01 << mem_id)))
		set_mem_size_str(output_str, computer_data.packed.memsz[mem_id]);
	else
		set_invalid_string(output_str);
}

void set_updated_hdd_size(char *output_str, uint8_t hdd_id)
{
	if (computer_data.packed.hdds & (0x01 << hdd_id))
		set_hdd_size_str(output_str, computer_data.packed.hddsz[hdd_id], computer_data.packed.hddf & (0x01 << hdd_id));
	else
		set_invalid_string(output_str);
}

void set_updated_cpu_frequency(char *output_str, uint8_t cpu_id)
{
	if (computer_data.packed.cpufs & (0x01 << cpu_id))
		set_fq_string(output_str, computer_data.packed.cpuf[cpu_id]);
	else
		set_invalid_string(output_str);
}

void set_updated_ambient_temp(char *output_str)
{
	if (computer_data.details.ambs == 1)
		set_temp_string(output_str, computer_data.details.ambt);
	else
		set_invalid_string(output_str);
}

void update_adc()
{
	char last_value[10] = "0 W";
	strcpy(last_value, power_value);
	set_power_data(power_value);
	if (strcmp(power_value, last_value) != 0){
		update_information_frame(SHOW_POWER, true);
	}
}


void set_updated_gpu_temp(char *output_str)
{
	if (computer_data.details.gpus == 1)
		set_temp_string(output_str, computer_data.details.gput);
	else
		set_invalid_string(output_str);
}

void set_serial_number(char *output_str)
{
	char serial[SERIAL_NUMBER_LENGTH + 1];
	for (int i=0; i < SERIAL_NUMBER_LENGTH; i++)
		serial[i] = eeprom_read_byte(SERIAL_NUMBER_EEPROM_ADDRESS + i);
	serial[SERIAL_NUMBER_LENGTH] = '\0';
	strcpy(output_str, serial);
}

void set_product_name(char *output_str)
{
	char product_name[PRODUCT_NAME_LENGTH + 1];
	for (int i = 0; i < PRODUCT_NAME_LENGTH; i++)
		product_name[i] = eeprom_read_byte(PRODUCT_NAME_EEPROM_ADDRESS + i);
	product_name[PRODUCT_NAME_LENGTH] = '\0';
	strcpy(output_str, product_name);
}

void set_mac_address(char *output_str)
{
	uint8_t mac_address[MAC_ADDRESS_LENGTH];
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		mac_address[i] = eeprom_read_byte(MAC_ADDRESS_EEPROM_ADDRESS + i);
	sprintf(output_str, "%02x%02x%02x%02x%02x%02x", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
}

void set_update_hdd_temp(char *output_str, uint8_t hdd_id)
{
	if ((computer_data.packed.hddts & (0x01 << hdd_id)) != 0)
		set_temp_string(output_str, computer_data.packed.hddt[hdd_id]);
	else
		set_invalid_string(output_str);
}


void update_brightness()
{
	ssd1306_set_contrast(eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS));
	update_information_frame(SET_BRIGHTNESS, true);
}

void handle_brightness_buttons(uint8_t key)
{
	uint8_t brightness = eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS);
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (brightness > BRIHTNESS_STEP)
			brightness -= BRIHTNESS_STEP;
		else
			brightness = 0;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (brightness < (0xff - BRIHTNESS_STEP))
			brightness += BRIHTNESS_STEP;
		else
			brightness = 0xff;
		break;
	}
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
	update_brightness();
}

void set_dmi_content(char *output_str, uint8_t string_id)
{
//	uint8_t count = 0;
//	struct direct_string_item * string_item = computer_data.direct_string;
//	while (string_item != 0 && count < string_id){
//		string_item = string_item->next;
//		count++;
//	}
//	if (count == string_id)
//		output_str = strdup(string_item->content);
//	else
//		set_invalid_string(output_str);
}

void set_brightness(char *str)
{
	frame_present->handle_buttons = handle_brightness_buttons;
	sprintf(str,"%d ", (eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS) / BRIHTNESS_STEP));
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
		set_serial_number(output_str);
		break;
	case SHOW_PRODUCT_NAME:
		set_product_name(output_str);
		break;
	case SHOW_MAC_ADDRESS:
		set_mac_address(output_str);
		break;
	case SHOW_POWER_STATE:
		set_state(output_str);
		break;
	case SET_BRIGHTNESS:
		set_brightness(output_str);
		break;
	case SHOW_CPU_TEMPERTURE:
		set_cpu_updated_temp(output_str, info);
		break;
	default:
		break;
	}
	present_menu->is_active_frame = type == SET_BRIGHTNESS;
}
