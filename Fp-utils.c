/*
 * utils.c
 *
 *  Created on: Aug 19, 2015
 *      Author: arkadi
 */
#include "Fp-utils.h"
#include "uart/uart.h"
#include "scheduler/scheduler.h"
#include "screen_saver/screen_saver.h"
#include "twi/i2c_buffer.h"
#include "lib/syntax.h"
#include "screens/brightness/brightness.h"
#include "screens/static_data/static_data.h"

#define MAX_DIGITS 5
#define UPDATE_FRAME_MIN_TICKS	0x08ff
#define UPDATE_MENU_MIN_TICKS	0xafff

enum power_state current_power_state = POWER_ON;
enum display_state display_state;
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

void update_requests(void *data)
{
	if (!i2c_buffer.layout.hddtr && need_to_update_req(SHOW_HDD_TEMPERTURE))
		i2c_buffer.layout.hddtr = 1;

	if (!i2c_buffer.layout.cpufr && need_to_update_req(SHOW_CPU_FREQUENCY))
		i2c_buffer.layout.cpufr = 1;

	if (!i2c_buffer.layout.cputr && need_to_update_req(SHOW_CPU_TEMPERTURE))
		i2c_buffer.layout.cputr = 1;

	if (!i2c_buffer.layout.gputr && need_to_update_req(SHOW_GPU_TEMPERTURE))
		i2c_buffer.layout.gputr = 1;

	if (!i2c_buffer.layout.req && i2c_buffer.raw[PENDR0])
		i2c_buffer.layout.req = 1;
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
	if (BIT_ON(computer_data.packed.cputs, cpu_id))
		set_temp_string(data, computer_data.details.cput[cpu_id]);
	else
		set_invalid_string(data);
}

static void set_updated_memory_size(char *output_str, uint8_t mem_id)
{
	if (BIT_ON(computer_data.packed.mems, mem_id))
		set_mem_size_str(output_str, computer_data.packed.memsz[mem_id]);
	else
		set_invalid_string(output_str);
}

static void set_updated_hdd_size(char *output_str, uint8_t hdd_id)
{
	if (BIT_ON(computer_data.packed.hdds, hdd_id))
		set_hdd_size_str(output_str, computer_data.packed.hddsz[hdd_id], computer_data.packed.hddf & (0x01 << hdd_id));
	else
		set_invalid_string(output_str);
}

static void set_updated_cpu_frequency(char *output_str, uint8_t cpu_id)
{
	if (BIT_ON(computer_data.packed.cpufs, cpu_id))
		set_fq_string(output_str, computer_data.packed.cpuf[cpu_id]);
	else
		set_invalid_string(output_str);
}

static void set_updated_ambient_temp(char *output_str)
{
	if (computer_data.details.ambs)
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
	if (computer_data.details.gpus)
		set_temp_string(output_str, computer_data.details.gput);
	else
		set_invalid_string(output_str);
}



static void set_update_hdd_temp(char *output_str, uint8_t hdd_id)
{
	if ((computer_data.packed.hddts & (0x01 << hdd_id)) != 0)
		set_temp_string(output_str, computer_data.packed.hddt[hdd_id]);
	else
		set_invalid_string(output_str);
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
		sprintf(str, "");
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
	present_menu->is_active_frame = false;
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

	if (type == SET_BRIGHTNESS)
		present_menu->is_active_frame = true;
}

static bool is_data_need_update_packed(uint8_t data, struct gfx_information *info, bool is_visible)
{
	if (!is_visible)
		return false;

	bool data_valid = BIT_ON(data, info->info_data);
	return present_menu->visible ? !data_valid : data_valid;
}

static bool is_hdd_size_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible)
		need_update = !BIT_ON(computer_data.packed.hdds, info->info_data);

	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		set_hdd_size_str(temp_str, computer_data.packed.hddsz[info->info_data], BIT_ON(computer_data.packed.hddf, info->info_data));
		set_curr_str(curr_str, SHOW_HDD_SIZE);
		need_update = strcmp(temp_str, curr_str);
	}
	return need_update;
}

static bool is_mem_size_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible)
		need_update = !BIT_ON(computer_data.packed.mems, info->info_data);

	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		set_mem_size_str(temp_str, info->info_data);
		set_curr_str(curr_str, SHOW_MEMORY_SIZE);
		need_update = strcmp(temp_str, curr_str);
	}
	return need_update;
}

static bool is_hdd_temp_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible)
		need_update = !BIT_ON(computer_data.packed.hddts, info->info_data);

	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		set_update_hdd_temp(temp_str, info->info_data);
		set_curr_str(curr_str, SHOW_HDD_SIZE);
		need_update = strcmp(temp_str, curr_str);
	}
	return need_update;
}

static bool is_gpu_temp_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = is_visible && !computer_data.details.gpus;
	if (!need_update){
		char temp_str[3];
		char curr_str[3];
		set_curr_str(curr_str, SHOW_GPU_TEMPERTURE);
		set_updated_gpu_temp(temp_str);
		need_update = strcmp(temp_str, curr_str);
	}
	return need_update;
}

static bool is_ambient_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = is_visible && !computer_data.details.ambs;
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
		return is_data_need_update_packed(computer_data.packed.cpufs, info, is_visible);
	case SHOW_HDD_SIZE:
		return is_hdd_size_need_update(info, is_visible);
	case SHOW_MEMORY_SIZE:
		return is_mem_size_need_update(info, is_visible);
	case SHOW_HDD_TEMPERTURE:
		return is_hdd_temp_need_update(info, is_visible);
	case SHOW_CPU_TEMPERTURE:
		return is_data_need_update_packed(computer_data.packed.cputs, info, is_visible);
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
