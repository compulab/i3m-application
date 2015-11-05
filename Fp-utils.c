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

bool is_valid_cpu_temp(uint8_t cpu_id)
{
	return (computer_data.packed.cputs & 0x01 << (cpu_id)) != 0;
}

bool is_valid_cpu_fq(uint8_t cpu_id)
{
	return (computer_data.packed.cpufs & 0x01 << (cpu_id)) != 0;
}

bool is_valid_ambient_temp()
{
	return computer_data.details.ambs == 1;
}

bool is_valid_gpu_temp()
{
	return computer_data.details.gpus == 1;
}

bool is_valid_hdd_size(uint8_t hdd_id)
{
	return (computer_data.packed.hdds & 0x01 << (hdd_id)) != 0;
}

bool is_valid_hdd_temp(uint8_t hdd_id)
{
	return (computer_data.packed.hddts & 0x01 << (hdd_id)) != 0;
}

bool is_valid_mem(uint8_t mem_id)
{
	return (computer_data.packed.mems & 0x01 << (mem_id)) != 0;
}

bool is_action_in_menu(struct gfx_action_menu *menu, enum information_type type)
{
	struct gfx_information_node *info_node;
	for (uint8_t i = 0; i < menu->menu->num_elements; i++){
		if (menu->actions[i].type == ACTION_TYPE_SHOW_FRAME){
			info_node = menu->actions[i].frame->information_head;
			while (info_node != 0){
				if (info_node->information.info_type == type)
					return true;
				info_node = info_node->next;
			}
		}
	}
	return false;
}

void update_information_frame(enum information_type type, bool need_to_update)
{
	if (need_to_update){
		if (!present_menu->visible) {
			if (information_present != 0 && information_present->info_type == type) {
				update_information();
			}
		} else if (is_action_in_menu(present_menu, type)){
			gfx_action_menu_init(present_menu, true);
		}
	}

}

void set_invalid_string(char *str){
	sprintf(str, "INVALID");
}



bool need_to_update_req(struct gfx_information_node * curr_info, enum information_type info_type)
{
	bool need_to_update = false;
	while (curr_info != 0){
		if (curr_info->information.info_type == info_type){
			need_to_update = true;
			break;
		}
		curr_info = curr_info->next;
	}
	return need_to_update;
}


void check_update_hddtr_temp_req()
{
	if(need_to_update_req(frame_present->information_head, SHOW_HDD_TEMPERTURE))
		layout.l.hddtr = 1;
}

void check_update_cpu_fq_req()
{
	if (need_to_update_req(frame_present->information_head, SHOW_CPU_FREQUENCY))
		layout.l.cpufr = 1;
}

void check_update_cpu_temp_req()
{
	if (need_to_update_req(frame_present->information_head, SHOW_CPU_TEMPERTURE))
		layout.l.cputr = 1;
}

void check_update_gpu_temp_req()
{
	if (need_to_update_req(frame_present->information_head, SHOW_GPU_TEMPERTURE))
		layout.l.gputr = 1;
}

void check_update_pending_req()
{
	if (layout.direct.i2c[PENDR0] != 0)
		layout.l.req = 1;
}


void update_requests()
{
	check_update_hddtr_temp_req();
	check_update_cpu_fq_req();
	check_update_cpu_temp_req();
	check_update_gpu_temp_req();
	check_update_pending_req();
}


void clear_regs(uint8_t *beg_addr, uint8_t *end_addr)
{
	for (uint8_t *addr = beg_addr; addr < end_addr; addr++)
			*addr = 0x00;
}
void reset_temperatures()
{
	uint8_t *p_computer_data = (uint8_t *)&computer_data;
	for (int i = 0; i < sizeof(computer_data); i++)
		p_computer_data[i] = 0x00;
	clear_regs((uint8_t *)&layout.direct.i2c[CPU0T], (uint8_t *)&layout.direct.i2c[RESERVED42]);
	clear_regs((uint8_t *)&layout.direct.i2c[CPU0F_LSB], (uint8_t *)&layout.direct.i2c[RESERVED83]);
}

void handle_power_off()
{
	reset_ambient();
	reset_temperatures();
}

void handle_power_on()
{
	update_adc();
	update_ambient_temp();
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
	if (current_power_state != last_power_state) {
		if (current_power_state != POWER_ON)
			handle_power_off();
		else
			handle_power_on();
		update_information_frame(SHOW_POWER_STATE,true);
	}
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
	uint8_t count = 0;
	struct direct_string_item * string_item = computer_data.details.direct_string;
	while (string_item != 0 && count < string_id){
		string_item = string_item->next;
		count++;
	}
	if (count == string_id)
		output_str = strdup(string_item->content);
	else
		set_invalid_string(output_str);
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
		update_ambient_temp();
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
