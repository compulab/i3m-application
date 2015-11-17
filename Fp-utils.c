/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"

#define MAX_DIGITS 5

enum power_state current_power_state = POWER_ON;
bool sleep_mode_enabled;
char power_value[10];

void enable_screen_saver_mode()
{
	sleep_mode_enabled = true;
}

void disable_screen_saver_mode()
{
	sleep_mode_enabled = false;
}

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

void set_invalid_string(char *str){
	sprintf(str, "UNAVAILABLE");
}

bool is_type_in_frame(enum information_type info_type, struct gfx_information_node *info_node)
{
	while (info_node != NULL) {
		if (info_node->information.info_type == info_type)
						return true;
		info_node = info_node->next;
	}
	return false;
}

bool need_to_update_req(enum information_type info_type)
{
	if (!present_menu->visible) {
		return is_type_in_frame(info_type, frame_present->information_head);
	} else {
		struct gfx_item_action *action;
		for (int i = 0; i < present_menu->menu->num_elements; i++){
			action = &present_menu->actions[i];
			if (action->type == ACTION_TYPE_SHOW_FRAME && is_type_in_frame(info_type, action->frame->information_head))
				return true;
		}
	}
	return false;
}

void check_update_hddtr_temp_req()
{
	if(layout.l.hddtr == 0 && need_to_update_req(SHOW_HDD_TEMPERTURE))
		layout.l.hddtr = 1;
}

void check_update_cpu_fq_req()
{
	if (layout.l.cpufr == 0 && need_to_update_req(SHOW_CPU_FREQUENCY))
		layout.l.cpufr = 1;
}

void check_update_cpu_temp_req()
{
	if (layout.l.cputr == 0 && need_to_update_req(SHOW_CPU_TEMPERTURE))
		layout.l.cputr = 1;
}

void check_update_gpu_temp_req()
{
	if (layout.l.gputr == 0 && need_to_update_req(SHOW_GPU_TEMPERTURE))
		layout.l.gputr = 1;
}

void check_update_pending_req()
{
	if (layout.l.req == 0 && layout.direct.i2c[PENDR0] != 0)
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
//	reset_ambient();
	reset_temperatures();
}

void handle_power_on()
{
	update_adc();
	update_ambient_temp();
}

void handle_power_state_changed()
{
	if (current_power_state != POWER_ON)
			handle_power_off();
		else
			handle_power_on();
	update_information_frame(SHOW_POWER_STATE, true);
}

struct work power_state_work = { .do_work = handle_power_state_changed, .data = NULL, .next = NULL };

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
		layout.l.power_state = current_power_state;
		insert_work(&power_state_work);
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

int num_of_digits(int x)
{
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
	sprintf(output_str, "%d", atoi(serial));
}

void set_part_number(char *output_str)
{
	char part_number[PRODUCT_NAME_LENGTH + 1];
	for (int i = 0; i < PRODUCT_NAME_LENGTH; i++)
		part_number[i] = eeprom_read_byte(PRODUCT_NAME_EEPROM_ADDRESS + i);
	part_number[PRODUCT_NAME_LENGTH] = '\0';
	strcpy(output_str, part_number);
}

void set_mac_address(char *output_str)
{
	uint8_t mac_address[MAC_ADDRESS_LENGTH];
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		mac_address[i] = eeprom_read_byte(MAC_ADDRESS_EEPROM_ADDRESS + i);
	sprintf(output_str, "%02X.%02X.%02X.%02X.%02X.%02X", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
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

uint8_t get_brightness_level()
{
	uint8_t brightness = eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS);
	uint8_t brightness_level;
	if (brightness > (MAX_BRIGHTNESS_LEVEL -1) * BRIGHTNESS_STEP)
		brightness_level = MAX_BRIGHTNESS_LEVEL;
	else if (brightness < BRIGHTNESS_STEP)
		brightness_level = 0;
	else
		brightness_level = brightness / BRIGHTNESS_STEP;
	return brightness_level;
}

void increse_brightness_level()
{
	uint8_t brightness_level = get_brightness_level();
	if (brightness_level < MAX_BRIGHTNESS_LEVEL)
		brightness_level++;
	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

void decrese_brightness_level()
{
	uint8_t brightness_level = get_brightness_level();
	if (brightness_level > 0)
		brightness_level--;
	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

void handle_brightness_buttons(uint8_t key)
{
	uint8_t brightness_level = get_brightness_level();
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (brightness_level > 0)
			decrese_brightness_level();
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (brightness_level < MAX_BRIGHTNESS_LEVEL)
			increse_brightness_level();
		break;
	}
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
	sprintf(str,"%d ", (eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS) / BRIGHTNESS_STEP));
}

void set_curr_str(char *str, enum information_type type)
{
	str = "";
	struct gfx_information_node *info_node = frame_present->information_head;
	while (info_node != NULL) {
		if (info_node->information.info_type == type){
			strcpy(str, info_node->information.text.text);
			break;
		}
		info_node = info_node->next;
	}
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
	case SHOW_PART_NUMBER:
		set_part_number(output_str);
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


bool is_cpu_fq_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible) {
	switch (info->info_data){
		case 0:
			need_update = computer_data.details.cpu0fs == 0;
			break;
		case 1:
			need_update = computer_data.details.cpu1fs == 0;
			break;
		case 2:
			need_update = computer_data.details.cpu2fs == 0;
			break;
		case 3:
			need_update = computer_data.details.cpu3fs == 0;
			break;
		case 4:
			need_update = computer_data.details.cpu4fs == 0;
			break;
		case 5:
			need_update = computer_data.details.cpu5fs == 0;
			break;
		case 6:
			need_update = computer_data.details.cpu6fs == 0;
			break;
		case 7:
			need_update = computer_data.details.cpu7fs == 0;
			break;
		default:
			need_update = false;
			break;
		}
	}
	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		set_updated_cpu_frequency(temp_str, info->info_data);
		set_curr_str(curr_str, SHOW_CPU_FREQUENCY);
		need_update = strcmp(temp_str, curr_str) != 0;
	}
	return need_update;
}

bool is_hdd_size_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible) {
		switch (info->info_data){
		case 0:
			need_update = computer_data.details.hdd0s == 0;
			break;
		case 1:
			need_update = computer_data.details.hdd1s == 0;
			break;
		case 2:
			need_update = computer_data.details.hdd2s == 0;
			break;
		case 3:
			need_update = computer_data.details.hdd3s == 0;
			break;
		case 4:
			need_update = computer_data.details.hdd4s == 0;
			break;
		case 5:
			need_update = computer_data.details.hdd5s == 0;
			break;
		case 6:
			need_update = computer_data.details.hdd6s == 0;
			break;
		case 7:
			need_update = computer_data.details.hdd7s == 0;
			break;
		default:
			need_update = false;
			break;
		}
	}
	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		set_hdd_size_str(temp_str, computer_data.packed.hddsz[info->info_data], computer_data.packed.hddf & (0x01 << info->info_data));
		set_curr_str(curr_str, SHOW_HDD_SIZE);
		need_update = strcmp(temp_str, curr_str) != 0;
	}
	return need_update;
}

bool is_mem_size_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible) {
		switch (info->info_data){
		case 0:
			need_update = computer_data.details.mem0s == 0;
			break;
		case 1:
			need_update = computer_data.details.mem1s == 0;
			break;
		case 2:
			need_update = computer_data.details.mem2s == 0;
			break;
		case 3:
			need_update = computer_data.details.mem3s == 0;
			break;
		default:
			need_update = false;
			break;
		}
	}
	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		set_mem_size_str(temp_str, info->info_data);
		set_curr_str(curr_str, SHOW_MEMORY_SIZE);
		need_update = strcmp(temp_str, curr_str) != 0;
	}
	return need_update;
}

bool is_hdd_temp_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible) {
		switch (info->info_data){
		case 0:
			need_update = computer_data.details.hdd0ts== 0;
			break;
		case 1:
			need_update = computer_data.details.hdd1ts == 0;
			break;
		case 2:
			need_update = computer_data.details.hdd2ts == 0;
			break;
		case 3:
			need_update = computer_data.details.hdd3ts == 0;
			break;
		case 4:
			need_update = computer_data.details.hdd4ts == 0;
			break;
		case 5:
			need_update = computer_data.details.hdd5ts == 0;
			break;
		case 6:
			need_update = computer_data.details.hdd6ts == 0;
			break;
		case 7:
			need_update = computer_data.details.hdd7ts == 0;
			break;
		default:
			need_update = false;
			break;
		}
	}
	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		set_update_hdd_temp(temp_str, info->info_data);
		set_curr_str(curr_str, SHOW_HDD_SIZE);
		need_update = strcmp(temp_str, curr_str) != 0;
	}
	return need_update;
}

bool is_cpu_temp_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible) {
		switch (info->info_data){
		case 0:
			need_update = computer_data.details.cpu0ts== 0;
			break;
		case 1:
			need_update = computer_data.details.cpu1ts == 0;
			break;
		case 2:
			need_update = computer_data.details.cpu2ts == 0;
			break;
		case 3:
			need_update = computer_data.details.cpu3ts == 0;
			break;
		case 4:
			need_update = computer_data.details.cpu4ts == 0;
			break;
		case 5:
			need_update = computer_data.details.cpu5ts == 0;
			break;
		case 6:
			need_update = computer_data.details.cpu6ts == 0;
			break;
		case 7:
			need_update = computer_data.details.cpu7ts == 0;
			break;
		default:
			need_update = false;
			break;
		}
	}
	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		set_cpu_updated_temp(temp_str, info->info_data);
		set_curr_str(curr_str, SHOW_CPU_TEMPERTURE);
		need_update = strcmp(temp_str, curr_str) != 0;
	}
		return need_update;
}

bool is_gpu_temp_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = is_visible && computer_data.details.gpus == 0;
	if (!need_update){
		char temp_str[3];
		char curr_str[3];
		set_curr_str(curr_str, SHOW_GPU_TEMPERTURE);
		set_updated_gpu_temp(temp_str);
		need_update = strcmp(temp_str, curr_str) != 0;
	}
	return need_update;
}

bool is_ambient_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = is_visible && computer_data.details.ambs == 0;
	if (!need_update){
		char temp_str[3];
		char curr_str[3];
		set_updated_ambient_temp(temp_str);
		set_curr_str(curr_str, SHOW_AMBIENT_TEMPERATURE);
		need_update = strcmp(temp_str, curr_str) != 0;
	}
	return need_update;
}

bool is_information_need_to_change(struct gfx_information *info, bool is_visible)
{
	switch (info->info_type){
	case SHOW_POWER:
		return !is_visible && computer_data.details.adcs == 1;
	case SHOW_CPU_FREQUENCY:
		return is_cpu_fq_need_update(info, is_visible);
	case SHOW_HDD_SIZE:
		return is_hdd_size_need_update(info, is_visible);
	case SHOW_MEMORY_SIZE:
		return is_mem_size_need_update(info, is_visible);
	case SHOW_HDD_TEMPERTURE:
		return is_hdd_temp_need_update(info, is_visible);
	case SHOW_CPU_TEMPERTURE:
		return is_cpu_temp_need_update(info, is_visible);
	case SHOW_GPU_TEMPERTURE:
		return is_gpu_temp_need_update(info, is_visible);
	case SHOW_AMBIENT_TEMPERATURE:
		return is_ambient_need_update(info, is_visible);
	case SET_BRIGHTNESS:
		return true;
	default:
		return false;
	}
}

bool is_data_need_update()
{
	return is_information_need_to_change(information_present, true);
}

void update_information()
{
	gfx_frame_draw(frame_present, true);
}

bool is_status_in_menu_changed(struct gfx_action_menu *menu, enum information_type type)
{
	bool need_to_update = false;
	struct gfx_information_node *info_node;
	for (uint8_t i = 0; i < menu->menu->num_elements; i++){
		if (menu->actions[i].type == ACTION_TYPE_SHOW_FRAME){
			info_node = menu->actions[i].frame->information_head;
			while (info_node != 0){
				if (info_node->information.info_type == type) {
					need_to_update = is_information_need_to_change(&info_node->information, menu->actions[i].visible);
				}
				if (need_to_update)
					return true;
				info_node = info_node->next;
			}
		}
	}
	return false;
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
	if (need_to_update && !is_screen_saver_on){
		if (!present_menu->visible) {
			if (information_present != 0 && information_present->info_type == type) {
				update_information();
			}
		} else if (is_status_in_menu_changed(present_menu, type)) {
			gfx_action_menu_init(present_menu, true);
		}
	}
}
