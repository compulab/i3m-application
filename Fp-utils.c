/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"
#include "uart/uart.h"
#include "scheduler/scheduler.h"
#include "twi/i2c_buffer.h"

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
    .minute = 02,
    .hour = 11,
    .date = 26,
    .month = 11,
    .year = 2015
};

void enable_screen_saver_mode(void)
{
	screen_saver_mode_enabled = true;
}

void disable_screen_saver_mode(void)
{
	screen_saver_mode_enabled = false;
}

void print_work_count(void *data)
{
	uart_send_string("works in queue: ");
	uart_send_num(works_count, 10);
	uart_send_string("\n\r");
}

#define BIT_ON(value, bitnum)	((value) & (1 << (bitnum))) != 0

bool is_valid_cpu_temp(uint8_t cpu_id)
{
	return BIT_ON(computer_data.packed.cputs, cpu_id);
}

bool is_valid_cpu_fq(uint8_t cpu_id)
{
	return BIT_ON(computer_data.packed.cpufs, cpu_id);
}

bool is_valid_ambient_temp(void)
{
	return computer_data.details.ambs == 1;
}

bool is_valid_gpu_temp(void)
{
	return computer_data.details.gpus == 1;
}

bool is_valid_hdd_size(uint8_t hdd_id)
{
	return BIT_ON(computer_data.packed.hdds, hdd_id);
}

bool is_valid_hdd_temp(uint8_t hdd_id)
{
	return BIT_ON(computer_data.packed.hddts, hdd_id);
}

bool is_valid_mem(uint8_t mem_id)
{
	return BIT_ON(computer_data.packed.mems, mem_id);
}

void fp_init(void)
{
	wait_time = 1;
	update_timestamp = 0;
}

static void set_invalid_string(char *str){
	sprintf(str, "-");
}

static bool is_type_in_frame(enum information_type info_type, struct gfx_information_node *info_node)
{
	while (info_node != NULL) {
		if (info_node->information.info_type == info_type)
						return true;
		info_node = info_node->next;
	}
	return false;
}

static bool need_to_update_req(enum information_type info_type)
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

static void check_update_hddtr_temp_req(void)
{
	if(i2c_buffer.layout.hddtr == 0 && need_to_update_req(SHOW_HDD_TEMPERTURE))
		i2c_buffer.layout.hddtr = 1;
}

static void check_update_cpu_fq_req(void)
{
	if (i2c_buffer.layout.cpufr == 0 && need_to_update_req(SHOW_CPU_FREQUENCY))
		i2c_buffer.layout.cpufr = 1;
}

static void check_update_cpu_temp_req(void)
{
	if (i2c_buffer.layout.cputr == 0 && need_to_update_req(SHOW_CPU_TEMPERTURE))
		i2c_buffer.layout.cputr = 1;
}

static void check_update_gpu_temp_req(void)
{
	if (i2c_buffer.layout.gputr == 0 && need_to_update_req(SHOW_GPU_TEMPERTURE))
		i2c_buffer.layout.gputr = 1;
}

static void check_update_pending_req(void)
{
	if (i2c_buffer.layout.req == 0 && i2c_buffer.raw[PENDR0] != 0)
		i2c_buffer.layout.req = 1;
}


void update_requests(void *data)
{
	check_update_hddtr_temp_req();
	check_update_cpu_fq_req();
	check_update_cpu_temp_req();
	check_update_gpu_temp_req();
	check_update_pending_req();
}


static void clear_regs(uint8_t *beg_addr, uint8_t *end_addr)
{
	for (uint8_t *addr = beg_addr; addr < end_addr; addr++)
			*addr = 0x00;
}

static void reset_temperatures(void)
{
	uint8_t *p_computer_data = (uint8_t *)&computer_data;
	for (int i = 0; i < ((uint16_t)&computer_data.details.direct_string - (uint16_t)&computer_data); i++)
		p_computer_data[i] = 0x00;
	clear_regs((uint8_t *)&i2c_buffer.raw[CPU0T], (uint8_t *)&i2c_buffer.raw[RESERVED42]);
	clear_regs((uint8_t *)&i2c_buffer.raw[CPU0F_LSB], (uint8_t *)&i2c_buffer.raw[RESERVED83]);
}

static void handle_power_off(void)
{
//	reset_ambient();
	reset_temperatures();
}

static void handle_power_on(void)
{
	update_adc(NULL);
	update_ambient_temp(NULL);
}

static void handle_power_state_changed(void *data)
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
	update_computer_state();
}

struct work power_state_work = { .do_work = handle_power_state_changed, .data = NULL, .next = NULL };

void update_power_state(void)
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
		i2c_buffer.layout.power_state = current_power_state;
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

static void set_mem_size_str(char *str, uint8_t mem)
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

static void set_hdd_size_str(char *str, uint16_t size, bool is_tera)
{
	char * units;
	if (is_tera)
			units = " TB";
		else
			units = " GB";
	sprintf(str, "%d %s", size, units);
}

static void set_fq_string(char *str, uint16_t fq)
{
	sprintf(str, "%d MHZ", fq);
}

static void set_temp_string(char *str, int8_t temperature)
{
	sprintf(str,"%d%c",temperature ,(uint8_t)128);
}

static void set_cpu_updated_temp(char *data, uint8_t cpu_id)
{
	if ((computer_data.packed.cputs & (0x01 << cpu_id)) != 0)
		set_temp_string(data, computer_data.details.cput[cpu_id]);
	else
		set_invalid_string(data);
}

static void set_updated_memory_size(char *output_str, uint8_t mem_id)
{
	if ((computer_data.packed.mems & (0x01 << mem_id)))
		set_mem_size_str(output_str, computer_data.packed.memsz[mem_id]);
	else
		set_invalid_string(output_str);
}

static void set_updated_hdd_size(char *output_str, uint8_t hdd_id)
{
	if (computer_data.packed.hdds & (0x01 << hdd_id))
		set_hdd_size_str(output_str, computer_data.packed.hddsz[hdd_id], computer_data.packed.hddf & (0x01 << hdd_id));
	else
		set_invalid_string(output_str);
}

static void set_updated_cpu_frequency(char *output_str, uint8_t cpu_id)
{
	if (computer_data.packed.cpufs & (0x01 << cpu_id))
		set_fq_string(output_str, computer_data.packed.cpuf[cpu_id]);
	else
		set_invalid_string(output_str);
}

static void set_updated_ambient_temp(char *output_str)
{
	if (computer_data.details.ambs == 1)
		set_temp_string(output_str, computer_data.details.ambt);
	else
		set_invalid_string(output_str);
}

void update_adc(void *data)
{
	set_power_data(power_value);
}

static void set_updated_gpu_temp(char *output_str)
{
	if (computer_data.details.gpus == 1)
		set_temp_string(output_str, computer_data.details.gput);
	else
		set_invalid_string(output_str);
}

static void set_serial_number(char *output_str)
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

static void set_app_version(char *output_str, uint8_t type)
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

static void set_part_number(char *output_str)
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


static void set_mac_address(char *output_str, uint8_t info)
{
	uint8_t eeprom_addr = info * MAC_ADDRESS_LENGTH + MAC_ADDRESS_EEPROM_ADDRESS;
	uint8_t mac_addr[MAC_ADDRESS_LENGTH];
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		mac_addr[i] = eeprom_read_byte(eeprom_addr + i);
	sprintf(output_str, "[%d] %02X:%02X:%02X:%02X:%02X:%02X", (info + 1),
			mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

static void set_update_hdd_temp(char *output_str, uint8_t hdd_id)
{
	if ((computer_data.packed.hddts & (0x01 << hdd_id)) != 0)
		set_temp_string(output_str, computer_data.packed.hddt[hdd_id]);
	else
		set_invalid_string(output_str);
}

void update_brightness(void)
{
	ssd1306_set_contrast(eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS));
}

uint8_t get_brightness_level(void)
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

static void increse_brightness_level(void)
{
	uint8_t brightness_level = get_brightness_level();
	if (brightness_level < MAX_BRIGHTNESS_LEVEL)
		brightness_level++;
	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

static void decrese_brightness_level(void)
{
	uint8_t brightness_level = get_brightness_level();
	if (brightness_level > 0)
		brightness_level--;
	uint8_t brightness = brightness_level * BRIGHTNESS_STEP;
	eeprom_write_byte(BRIGHTNESS_EEPROM_ADDRESS, brightness);
}

static void handle_screen_saver_time_units_buttons(uint8_t key)
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

static void handle_screen_saver_type_buttons(uint8_t key)
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

static int screen_saver_less_time(void)
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

static bool screen_saver_more_time(void)
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

static void handle_screen_saver_time_buttons(uint8_t key)
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

static void handle_screen_saver_enable_buttons(uint8_t key)
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

static void handle_brightness_buttons(uint8_t key)
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

const char *screen_saver_time_units_str[SCREEN_SAVER_TIME_UNITS_SIZE] = { "SEC", "MIN", "HOUR"};

const char *screen_saver_type_str[SCREEN_SAVER_TYPE_SIZE] = { "LOGO", "DASHBOARD", "CLOCK"};

static void set_disabled(char *str)
{
	sprintf(str, "DISABLED");
}

static void set_screen_saver_time_unit(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_time_units_buttons;
	if (computer_data.details.screen_saver_visible == 1)
		sprintf(str, screen_saver_time_units_str[computer_data.details.screen_saver_update_time_unit]);
	else
		set_disabled(str);
}
static void set_rtc_hour(char *str)
{
	if (calendar_is_date_valid(&computer_date_time))
		sprintf(str, "%d" ,computer_date_time.hour);
	else
		set_invalid_string(str);
}

static void set_rtc_min(char *str)
{
	if (calendar_is_date_valid(&computer_date_time))
		sprintf(str, "%02d" ,computer_date_time.minute);
	else
		set_invalid_string(str);
}

static void set_rtc_sec(char *str)
{
	if (calendar_is_date_valid(&computer_date_time) && computer_date_time.second % 2)
		sprintf(str, ":");
	else
		str = "";
}

static void set_screen_saver_type(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_type_buttons;
	if (computer_data.details.screen_saver_visible == 1)
		sprintf(str, screen_saver_type_str[computer_data.details.screen_saver_type]);
	else
		set_disabled(str);
}

static void set_screen_saver_time(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_time_buttons;
	if (computer_data.details.screen_saver_visible == 1)
		sprintf(str, "%d" , computer_data.details.screen_saver_update_time);
	else
		set_disabled(str);
}

static void set_screen_saver_enable(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_enable_buttons;
	sprintf(str, "DISABLE ENABLE");
}

static void set_brightness(char *str)
{
	frame_present->handle_buttons = handle_brightness_buttons;
	sprintf(str,"%d ", (eeprom_read_byte(BRIGHTNESS_EEPROM_ADDRESS) / BRIGHTNESS_STEP));
}

static void set_curr_str(char *str, enum information_type type)
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

static void set_update_post_code(char *str)
{
	sprintf(str, "%04X", i2c_buffer.layout.bios_post_code);
}

extern char buf[1];
static void set_usb_serial_string(char *str)
{
	strncpy(str, buf, 1);
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
		update_ambient_temp(NULL);
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
	case SHOW_POST_CODE:
		set_update_post_code(output_str);
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
	case SHOW_RTC_HOUR:
		set_rtc_hour(output_str);
		break;
	case SHOW_RTC_MIN:
		set_rtc_min(output_str);
		break;
	case SHOW_RTC_SEC:
		set_rtc_sec(output_str);
		break;
	case SHOW_USB_SERIAL_INPUT:
		set_usb_serial_string(output_str);
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

static bool is_cpu_fq_need_update(struct gfx_information *info, bool is_visible)
{
	if (is_visible) {
			return present_menu->visible ? ((computer_data.packed.cpufs & (0x01 << info->info_data)) == 0x00) :
					((computer_data.packed.cpufs & (0x01 << info->info_data)) != 0x00);
		}

	return false;
}

static bool is_hdd_size_need_update(struct gfx_information *info, bool is_visible)
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

static bool is_mem_size_need_update(struct gfx_information *info, bool is_visible)
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

static bool is_hdd_temp_need_update(struct gfx_information *info, bool is_visible)
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

static bool is_cpu_temp_need_update(struct gfx_information *info, bool is_visible)
{
	if (is_visible) {
		return present_menu->visible ? ((computer_data.packed.cputs & (0x01 << info->info_data)) == 0x00) :
				((computer_data.packed.cputs & (0x01 << info->info_data)) != 0x00);
	}
	return false;
}

static bool is_gpu_temp_need_update(struct gfx_information *info, bool is_visible)
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

static bool is_ambient_need_update(struct gfx_information *info, bool is_visible)
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

static bool is_information_need_to_change(struct gfx_information *info, bool is_visible)
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
	case SHOW_POST_CODE:
	case SHOW_RTC_DAY:
	case SHOW_RTC_MONTH:
	case SHOW_RTC_YEAR:
	case SHOW_RTC_HOUR:
	case SHOW_RTC_SEC:
	case SHOW_RTC_MIN:
	case SHOW_USB_SERIAL_INPUT:
		return true;
	default:
		return false;
	}
}

static bool is_menu_need_update(struct gfx_action_menu *menu)
{
	bool need_to_update = false;
	struct gfx_information_node *info_node;
	for (uint8_t i = 0; i < menu->menu->num_elements; i++) {
		if (menu->actions[i].type == ACTION_TYPE_SHOW_FRAME) {
			if (menu->actions[i].frame == dashboard)
				continue;
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

static bool is_info_need_update(struct gfx_information *info)
{
	return is_information_need_to_change(info, true);
}

static bool is_frame_need_update(struct gfx_frame *frame)
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

void update_computer_state(void)
{
	if (current_power_state == POWER_OFF)
		computer_state = COMPUTER_OFF;
	else if (current_power_state == POWER_ON && computer_state == COMPUTER_OFF)
		computer_state = COMPUTER_ON;
	else if (computer_data.packed.post_code == POST_CODE_BIOS_START)
		computer_state = COMPUTER_IN_BIOS;
	else if ((computer_state == COMPUTER_IN_BIOS) && (computer_data.packed.post_code == POST_CODE_BIOS_DONE))
		computer_state = COMPUTER_IN_OS;
}

void update_info(void *data)
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

	case DISPLAY_CLOCK:
		gfx_frame_draw(frame_present, true);
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

static struct work update_screen_work = { .do_work = update_info };

#define UPDATE_SCREEN_TIME		1
static double screen_get_recur_period(void)
{
    return UPDATE_SCREEN_TIME;
}

struct scheduler_task screen_sec_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};

#define UPDATE_REQ_SEC	1

static struct work requests_work = { .do_work = update_requests };

static double pending_req_get_recur_period(void)
{
	return UPDATE_REQ_SEC;
}

struct scheduler_task pending_req_tick_task = {
    .work = &requests_work,
    .get_recur_period = pending_req_get_recur_period,
};

#define PRINT_WORKS_COUNT_SEC	1

static struct work print_works_count_work = { .do_work = print_work_count };

static double print_works_get_recur_period(void)
{
	return PRINT_WORKS_COUNT_SEC;
}

struct scheduler_task print_works_count_sec_task = {
    .work = &print_works_count_work,
    .get_recur_period = print_works_get_recur_period,
};
