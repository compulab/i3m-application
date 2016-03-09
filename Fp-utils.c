/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"

#define MAX_DIGITS 5
#define UPDATE_FRAME_MIN_TICKS	0x08ff
#define UPDATE_MENU_MIN_TICKS	0xafff

enum power_state current_power_state = POWER_ON;
enum display_state display_state;
bool screen_saver_mode_enabled;
char power_value[10];
uint16_t update_timestamp;
uint16_t wait_time;

struct calendar_date computer_date_time = {
    .second = 40,
    .minute = 03,
    .hour = 17,
    .date = 26,
    .month = 11,
    .year = 2015
};

void enable_screen_saver_mode()
{
	screen_saver_mode_enabled = true;
}

void disable_screen_saver_mode()
{
	screen_saver_mode_enabled = false;
}

void print_work_count()
{
	uart_send_string("works in queue: ");
	uart_send_num(works_count, 10);
	uart_send_string("\n\r");
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

void fp_init()
{
	wait_time = 1;
	update_timestamp = 0;
}

void set_invalid_string(char *str){
	sprintf(str, "-");
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
	for (int i = 0; i < ((uint16_t)&computer_data.details.direct_string - (uint16_t)&computer_data); i++)
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
	switch(current_power_state) {
	case POWER_ON:
		handle_power_on();
		enter_power_on_mode();
		break;
	case POWER_STD:
		enter_hibernate_mode();
		break;
	case POWER_STR:
		enter_sleep_mode();
		break;
	case POWER_OFF:
		enter_power_off_mode();
		handle_power_off();
		break;
	}
}

struct work power_state_work = { .do_work = handle_power_state_changed, .data = NULL, .next = NULL };

void update_power_state()
{
	enum power_state  last_power_state = current_power_state;
	if (gpio_pin_is_low(FP_S5)) {
		current_power_state = POWER_OFF;
	} else if (gpio_pin_is_low(FP_S4)) {
		if (last_power_state != POWER_ON) /* Hibernate mode can be only after power on*/
			return;
		current_power_state = POWER_STD;
	} else if (gpio_pin_is_low(FP_S3)) {
		if (last_power_state != POWER_ON) /* Sleep mode can be only after power on*/
			return;
		current_power_state = POWER_STR;
	} else {
		current_power_state = POWER_ON;
	}

	if (current_power_state != last_power_state) {
		layout.l.power_state = current_power_state;
		if (!insert_work(&power_state_work))
			insert_to_log('P');
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
	case 0x00:
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
		size = -1;
		break;
	}
	if (size < 0)
		sprintf(str, "-");
	else if (size == 0)
		sprintf(str, EMPTY_SLOT);
	else
		sprintf(str,"%d GB",size);
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
	sprintf(str,"%d%c",temperature ,(uint8_t)128);
}

void set_cpu_updated_temp(char *data, uint8_t cpu_id)
{
	if ((computer_data.packed.cputs & (0x01 << cpu_id)) != 0)
		set_temp_string(data, computer_data.details.cput[cpu_id]);
	else
		set_invalid_string(data);
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
	char serial[SERIAL_NUMBER_LENGTH * 2  + 1];
	bool start_parsing = false;
	uint8_t index, serial_index = 0;;
	uint8_t serial_byte;

	for (index = SERIAL_NUMBER_LENGTH; index > 0 ; index--) {
		serial_byte = eeprom_read_byte(SERIAL_NUMBER_EEPROM_ADDRESS + index - 1);
		if (!start_parsing) {
			if (serial_byte != 0x00)
				start_parsing = true;
			 else
				continue;
		}
		sprintf(&serial[serial_index], "%02x", serial_byte);
		serial_index += 2;
	}

	for (index = serial_index; index > serial_index - 6; index--)
		serial[index] = serial[index - 1];
	serial[serial_index - 5] = '-';
	serial[serial_index + 1] = '\0';

	sprintf(output_str, serial);
}

void set_app_version(char *output_str, uint8_t type)
{
	switch(type) {
	case 0:
		strcpy(output_str, BUILD_TIME);
		break;

	case 1:
		strcpy(output_str, BUILD_DATE);
		break;

	case 2:
		strcpy(output_str,"VER: " VERSION);
		break;

	default:
		strcpy(output_str, "-");
		break;
	}
}

void set_part_number(char *output_str)
{
	char part_number[PART_NUMBER_LENGTH + 1];
	uint8_t index = 0;
	uint8_t info;
	for (int i = 0; i < PART_NUMBER_OPT_LENGTH; i++) {
		info = eeprom_read_byte(PART_NUMBER_EEPROM_ADDRESS + i);
		if (info == '\0')
			break;
		part_number[index] = info;
		index++;
	}

	part_number[index] = '\n';
	index++;


	for (int j = 1; j < 4; j++) {
		for (int i = 0; i < PART_NUMBER_OPT_LENGTH; i++) {
			info = eeprom_read_byte(PART_NUMBER_EEPROM_ADDRESS + i + j * PART_NUMBER_OPT_LENGTH);
//				if (info == '-')
//					continue;

			if (info == '\0')
				break;
			part_number[index] = info;
			index++;
		}
	}

	part_number[index] = '\0';
	strcpy(output_str, part_number);
}


void set_mac_address(char *output_str, uint8_t info)
{
	uint8_t eeprom_addr = info * MAC_ADDRESS_LENGTH + MAC_ADDRESS_EEPROM_ADDRESS;
	uint8_t mac_address[MAC_ADDRESS_LENGTH];
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		mac_address[i] = eeprom_read_byte(eeprom_addr + i);
	sprintf(output_str, "[%d] %02X:%02X:%02X:%02X:%02X:%02X", (info + 1),  mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
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

void handle_screen_saver_time_units_buttons(uint8_t key)
{
	if (computer_data.details.screen_saver_visible == 1) {
		switch (key) {
		case GFX_MONO_MENU_KEYCODE_DOWN:
			if (computer_data.details.screen_saver_update_time_unit == 0)
				return ;
			computer_data.details.screen_saver_update_time_unit--;
			break;
		case GFX_MONO_MENU_KEYCODE_UP:
			if (computer_data.details.screen_saver_update_time_unit == SCREEN_SAVER_TIME_UNITS_SIZE - 1)
				return ;
			computer_data.details.screen_saver_update_time_unit++;
			break;
		}
		eeprom_write_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS, computer_data.packed.screen_saver_update_time);
		gfx_frame_draw(frame_present, true);
	}
}

void handle_screen_saver_type_buttons(uint8_t key)
{
	if (computer_data.details.screen_saver_visible == 1) {
		switch (key) {
		case GFX_MONO_MENU_KEYCODE_DOWN:
			if (computer_data.details.screen_saver_type == 0)
				return ;
			computer_data.details.screen_saver_type--;
			break;
		case GFX_MONO_MENU_KEYCODE_UP:
			if (computer_data.details.screen_saver_type == SCREEN_SAVER_TYPE_SIZE - 1)
				return ;
			computer_data.details.screen_saver_type++;
			break;
		}
		eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, computer_data.packed.screen_saver_config);
		gfx_frame_draw(frame_present, true);
	}
}

int screen_saver_less_time()
{
	uint8_t min_value, jump;
	switch(computer_data.details.screen_saver_update_time_unit) {
	case SCREEN_SAVER_SECOND_UNIT:
		min_value = SCREEN_SAVER_SECOND_MIN_VALUE;
		jump = SCREEN_SAVER_SECOND_JUMP;
		break;

	case SCREEN_SAVER_MINUTE_UNIT:
		min_value = SCREEN_SAVER_MINUTE_MIN_VALUE;
		jump = SCREEN_SAVER_MINUTE_JUMP;
		break;

	case SCREEN_SAVER_HOUR_UNIT:
		min_value = SCREEN_SAVER_HOUR_MIN_VALUE;
		jump = SCREEN_SAVER_HOUR_JUMP;
		break;
	default:
		return false;
	}

	if (computer_data.details.screen_saver_update_time <= min_value)
		return false;
	computer_data.details.screen_saver_update_time -= jump;
	return true;
}

bool screen_saver_more_time()
{
	uint8_t max_value, jump;
	switch(computer_data.details.screen_saver_update_time_unit) {
	case SCREEN_SAVER_SECOND_UNIT:
		max_value = SCREEN_SAVER_SECOND_MAX_VALUE;
		jump = SCREEN_SAVER_SECOND_JUMP;
		break;

	case SCREEN_SAVER_MINUTE_UNIT:
		max_value = SCREEN_SAVER_MINUTE_MAX_VALUE;
		jump = SCREEN_SAVER_MINUTE_JUMP;
		break;

	case SCREEN_SAVER_HOUR_UNIT:
		max_value = SCREEN_SAVER_HOUR_MAX_VALUE;
		jump = SCREEN_SAVER_HOUR_JUMP;
		break;
	default:
		return false;
	}

	if (computer_data.details.screen_saver_update_time >= max_value)
		return false;
	computer_data.details.screen_saver_update_time += jump;
	return true;
}

void handle_screen_saver_time_buttons(uint8_t key)
{
	if (computer_data.details.screen_saver_visible == 1) {
		switch (key) {
		case GFX_MONO_MENU_KEYCODE_DOWN:
			if (!screen_saver_less_time())
				return ;
			break;
		case GFX_MONO_MENU_KEYCODE_UP:
			if (!screen_saver_more_time())
				return ;
			break;
		default:
			return ;
		}
		eeprom_write_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS, computer_data.packed.screen_saver_update_time);
		gfx_frame_draw(frame_present, true);
	}
}

void handle_screen_saver_enable_buttons(uint8_t key)
{
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (computer_data.details.screen_saver_visible == 0)
			return ;
		computer_data.details.screen_saver_visible = 0;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (computer_data.details.screen_saver_visible == 1)
			return ;
		computer_data.details.screen_saver_visible = 1;
		break;
	}
	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, computer_data.packed.screen_saver_config);
	gfx_frame_draw(frame_present, true);
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
	gfx_frame_draw(frame_present, true);
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

const char *screen_saver_time_units_str[SCREEN_SAVER_TIME_UNITS_SIZE] = { "SEC", "MIN", "HOUR"};

const char *screen_saver_type_str[SCREEN_SAVER_TYPE_SIZE] = { "LOGO", "DASHBOARD"};

void set_disabled(char *str)
{
	sprintf(str, "DISABLED");
}

void set_screen_saver_time_unit(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_time_units_buttons;
	if (computer_data.details.screen_saver_visible == 1)
		sprintf(str, screen_saver_time_units_str[computer_data.details.screen_saver_update_time_unit]);
	else
		set_disabled(str);
}

void set_screen_saver_type(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_type_buttons;
	if (computer_data.details.screen_saver_visible == 1)
		sprintf(str, screen_saver_type_str[computer_data.details.screen_saver_type]);
	else
		set_disabled(str);
}

void set_screen_saver_time(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_time_buttons;
	if (computer_data.details.screen_saver_visible == 1)
		sprintf(str, "%d" , computer_data.details.screen_saver_update_time);
	else
		set_disabled(str);
}

void set_screen_saver_enable(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_enable_buttons;
	sprintf(str, "DISABLE ENABLE");
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
	bool is_active_frame;

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
	case SHOW_COMPUTER_POWER:
		set_power_data(output_str);
		break;
	case SHOW_SERIAL_NUMBER:
		set_serial_number(output_str);
		break;
	case SHOW_APP_VERSION:
		set_app_version(output_str, info);
		break;
	case SHOW_PART_NUMBER:
		set_part_number(output_str);
		break;
	case SHOW_MAC_ADDRESS:
		set_mac_address(output_str, info);
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
	case SET_SCREEN_SAVER_ENABLE:
		set_screen_saver_enable(output_str);
		break;
	case SET_SCREEN_SAVER_TIME:
		set_screen_saver_time(output_str);
		break;
	case SET_SCREEN_SAVER_TIME_UNIT:
		set_screen_saver_time_unit(output_str);
		break;
	case SET_SCREEN_SAVER_TYPE:
		set_screen_saver_type(output_str);
		break;
	default:
		break;
	}

	switch (type) {
	case SET_BRIGHTNESS:
	case SET_SCREEN_SAVER_ENABLE:
	case SET_SCREEN_SAVER_TIME:
	case SET_SCREEN_SAVER_TIME_UNIT:
	case SET_SCREEN_SAVER_TYPE:
		is_active_frame = true;
		break;
	default:
		is_active_frame = false;
		break;
	}
	present_menu->is_active_frame = is_active_frame;
}

bool is_cpu_fq_need_update(struct gfx_information *info, bool is_visible)
{
	if (is_visible) {
			return present_menu->visible ? ((computer_data.packed.cpufs & (0x01 << info->info_data)) == 0x00) :
					((computer_data.packed.cpufs & (0x01 << info->info_data)) != 0x00);
		}

	if ((computer_data.packed.cpufq_update & (1 << info->info_data))  != 0x00) {
			computer_data.packed.cpufq_update = computer_data.packed.cpufq_update & ~(1 << info->info_data);
			return true;
		}
	return false;
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
	if (is_visible) {
		return present_menu->visible ? ((computer_data.packed.cputs & (0x01 << info->info_data)) == 0x00) :
				((computer_data.packed.cputs & (0x01 << info->info_data)) != 0x00);
	}

	if ((computer_data.packed.cput_update & (1 << info->info_data))  != 0x00) {
		computer_data.packed.cput_update = computer_data.packed.cput_update & ~(1 << info->info_data);
		return true;
	}
	return false;
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
	case SHOW_COMPUTER_POWER:
		return !present_menu->visible;
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
	case SET_SCREEN_SAVER_ENABLE:
	case SET_SCREEN_SAVER_TYPE:
	case SET_SCREEN_SAVER_TIME:
	case SET_BRIGHTNESS:
		return false;
	default:
		return false;
	}
}

bool is_status_in_menu_changed(struct gfx_action_menu *menu, enum information_type type)
{
	bool need_to_update = false;
	struct gfx_information_node *info_node;
	for (uint8_t i = 0; i < menu->menu->num_elements; i++){
		if (menu->actions[i].type == ACTION_TYPE_SHOW_FRAME) {
			info_node = menu->actions[i].frame->information_head;
			while (info_node != 0){
				if (info_node->information.info_type == type) {
					need_to_update = is_information_need_to_change(&info_node->information, menu->actions[i].visible);
				}
				if (need_to_update)
					return true;
				info_node = info_node->next;
			}
		} else if (menu->actions[i].type == ACTION_TYPE_SHOW_DMI_MENU) {
			if (!menu->actions[i].visible && computer_data.details.direct_string != NULL)
				return true;
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

bool is_enable_to_draw(uint16_t wait_time)
{
	return (TCC0.CNT >= update_timestamp && ((TCC0.CNT - update_timestamp) >= wait_time)) || (TCC0.CNT < update_timestamp && (TCC0.CNT >= wait_time));
}

bool is_enable_update_frame()
{
	return is_enable_to_draw(UPDATE_FRAME_MIN_TICKS);
}

bool is_enable_update_menu()
{
	return is_enable_to_draw(UPDATE_MENU_MIN_TICKS);
}

void update_draw()
{
	wait_time = UPDATE_FRAME_MIN_TICKS;
	update_timestamp = TCC0.CNT;
}

bool is_menu_need_update(struct gfx_action_menu *menu)
{
	bool need_to_update = false;
	struct gfx_information_node *info_node;
	for (uint8_t i = 0; i < menu->menu->num_elements; i++) {
		if (menu->actions[i].type == ACTION_TYPE_SHOW_FRAME) {
			info_node = menu->actions[i].frame->information_head;
			while (info_node != 0){
				need_to_update = is_information_need_to_change(&info_node->information, menu->actions[i].visible);
				if (need_to_update)
					return true;

				info_node = info_node->next;
			}
		}
	}
	return false;
}

bool is_info_need_update(struct gfx_information *info)
{
	return is_information_need_to_change(info, true);
}

bool is_frame_need_update(struct gfx_frame *frame)
{
	struct gfx_information_node *info_node= frame->information_head;
	while (info_node != NULL) {
		uart_send_string("check update info\n\r");
		if (is_info_need_update(&info_node->information))
			return true;
		info_node = info_node->next;
	}
	return false;
}

void update_info()
{
	uart_send_string("update_info start\n\r");
	switch (display_state) {
	case DISPLAY_LOGO:
	case DISPLAY_ACTION_FRAME:
	case DISPLAY_DIM:
		return;

	case DISPLAY_MENU:
		uart_send_string("check update menu\n\r");
		if (is_menu_need_update(present_menu))
			gfx_action_menu_init(present_menu, true);
		break;

	case DISPLAY_FRAME:
		uart_send_string("check update frame\n\r");
		if (is_frame_need_update(frame_present))
			gfx_frame_draw(frame_present, true);
	break;

	case DISPLAY_DASHBOARD:
		uart_send_string("check update dashboard**********\n\r");
		if (is_frame_need_update(dashboard))
			gfx_frame_draw(dashboard, true);
		break;
	default:
		break;
	}
}

