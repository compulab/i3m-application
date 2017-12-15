/*
 * sram_direct_strings.c
 *
 * Created: 14-Dec-17 6:18:38 PM
 *  Author: Nikita
 */

#include "power/power.h"
#include "twi/i2c_buffer.h"
#include "lib/memory.h"
#include "eeprom/eeprom_layout.h"
#include "twi/direct_strings.h"
#include "asf.h"
#include <string.h>

uint8_t direct_write_length;
uint8_t direct_write_index;
uint16_t dmi_eeprom_index;

bool is_length_set;

struct direct_string_item *direct_string_to_add;
enum dmi_state_t dmi_curr_state;

static void clear_direct_help_vars(void)
{
	direct_write_index = 0;
	direct_write_length = 0;
	is_length_set = false;
}

void init_direct_write_vars(void)
{
	direct_string_to_add = 0;
	dmi_curr_state = DMI_IDLE;
	clear_direct_help_vars();
}

static void free_direct_string(void)
{
	if (direct_string_to_add != NULL) {
		free(direct_string_to_add->content);
		free(direct_string_to_add->type);
	}
	free(direct_string_to_add);
	dmi_curr_state = DMI_IDLE;
	clear_direct_help_vars();
}

static int set_direct_string(void)
{
	direct_string_to_add = malloc_locked(sizeof(struct direct_string_item));
	if (direct_string_to_add == NULL)
		return -1;
	direct_string_to_add->content = 0;
	direct_string_to_add->next = 0;
	direct_string_to_add->type = 0;
	return 0;
}

static void get_dmi_string(char **str)
{
	uint8_t len = eeprom_read_byte(dmi_eeprom_index);
	(*str) = malloc_locked(sizeof(char) * (len + 1));
	for (int i = 0; i < len; i++, dmi_eeprom_index++)
		(*str)[i] = eeprom_read_byte(dmi_eeprom_index);
	(*str)[len] = '\0';
	dmi_eeprom_index++;
}

void dmi_init(void)
{
	dmi_eeprom_index = EEPROM_DMI_START;
	uint8_t dmi_count = eeprom_read_byte(EEPROM_DMI_COUNT);
	struct direct_string_item *direct_string;

	while (dmi_count) {
		while(eeprom_read_byte(dmi_eeprom_index) == '\0')
			dmi_eeprom_index++;
		direct_string = malloc_locked(sizeof(struct direct_string_item));
		if (direct_string == NULL)
			return;

		direct_string->backup_addr = dmi_eeprom_index + 1;
		get_dmi_string(&direct_string->type);
		get_dmi_string(&direct_string->content);
		if (computer_data.details.direct_string == NULL)
			computer_data.details.direct_string = direct_string;
		else
			computer_data.details.direct_string->next = direct_string;
		direct_string = NULL;
		dmi_count--;
	}
}

/*
static void add_dmi_backup(struct direct_string_item *dmi)
{
	uint8_t dmi_count = eeprom_read_byte(EEPROM_DMI_COUNT);
	dmi->backup_addr = max(dmi_eeprom_index, EEPROM_DMI_START);
	dmi_count++;
	uint16_t writing_index = dmi->backup_addr;
	eeprom_write_byte(writing_index, strlen(dmi->type) + 1);
	writing_index++;
	for (int i = 0; i < strlen(dmi->type); i++, writing_index++)
		eeprom_write_byte(writing_index, dmi->type[i]);
	eeprom_write_byte(writing_index, strlen(dmi->content) + 1);
	writing_index++;
	for (int i = 0; i < strlen(dmi->content); i++, writing_index++)
		eeprom_write_byte(writing_index, dmi->content[i]);
	eeprom_write_byte(EEPROM_DMI_COUNT, dmi_count);
}
*/

static void remove_dmi_backup(struct direct_string_item *dmi)
{
	uint8_t dmi_count = eeprom_read_byte(EEPROM_DMI_COUNT);
	if (dmi_count == 0)
		return;
	dmi_count--;
	uint8_t length = strlen(dmi->type) + strlen(dmi->content) + 1;
	for (uint8_t i = 0; i < length; i++)
		eeprom_write_byte(dmi->backup_addr + i, '\0');
	eeprom_write_byte(EEPROM_DMI_COUNT, dmi_count);
}

static void update_dmi_backup(struct direct_string_item *dmi)
{
	uint16_t start_index = dmi->backup_addr + strlen(dmi->type) + 2;
	uint8_t new_length = strlen(dmi->content);
	uint8_t old_length = eeprom_read_byte(start_index);
	if (new_length < old_length) {
		eeprom_write_byte(start_index, new_length);
		for (uint16_t blank_index = start_index +  new_length + 1; blank_index < start_index + old_length + 1; blank_index++)
			eeprom_write_byte(blank_index, '\0');
	}
	start_index++;
	for(uint8_t i = 0; i < new_length; i++, start_index++)
		eeprom_write_byte(start_index, dmi->content[i]);
}

static void add_direct_string(void)
{
	struct direct_string_item *curr = computer_data.details.direct_string;
	while (curr != NULL) {
		if (strcmp(direct_string_to_add->type, curr->type))
			break;
		else
			curr = curr->next;
	}

	if (curr != NULL) { /* change existing DMI string */
		if (strlen(direct_string_to_add->content) > strlen(curr->content)) {
			remove_dmi_backup(curr);
			strdup(direct_string_to_add->content);
//			add_dmi_backup(curr);
		} else {
			strdup(direct_string_to_add->content);
			update_dmi_backup(curr);
		}
	} else {
		direct_string_to_add->next = computer_data.details.direct_string;
		computer_data.details.direct_string = direct_string_to_add;
//		add_dmi_backup(direct_string_to_add);
		direct_string_to_add = 0;
	}

	free_direct_string();
}


static void end_direct_string(bool is_name_byte)
{
	if (is_name_byte && dmi_curr_state == DMI_NAME_WRITE) {
		direct_string_to_add->type[direct_write_length] = '\0';
		dmi_curr_state = DMI_VALUE_WRITE;
		clear_direct_help_vars();
	} else if (dmi_curr_state == DMI_VALUE_WRITE) {
		direct_string_to_add->content[direct_write_length] = '\0';
		add_direct_string();
	} else {
		init_direct_write_vars();
	}
}

static void write_byte_direct_string(bool is_written_type, uint8_t data)
{
	if (is_written_type)
		direct_string_to_add->type[direct_write_index] = data;
	else
		direct_string_to_add->content[direct_write_index] = data;
	direct_write_index++;
}

static void set_written_length(bool is_written_name, uint8_t data)
{
	direct_write_length = data;
	direct_write_index = 0;
	is_length_set = true;

	if (is_written_name) {
		if (direct_string_to_add) {
			free_direct_string();
			return;
		}

		if (set_direct_string()) {
			free_direct_string();
			return;
		}

		direct_string_to_add->type = malloc_locked(sizeof(char *) * direct_write_length + 1);
		if (direct_string_to_add->type == NULL)
			free_direct_string();

		return;
	}

	if (dmi_curr_state == DMI_VALUE_WRITE) {
		direct_string_to_add->content = malloc_locked(sizeof(char *) * direct_write_length + 1);
		if (direct_string_to_add->content == NULL) {
			free(direct_string_to_add->type);
			free(direct_string_to_add);
			clear_direct_help_vars();
		}
		return;
	}

	init_direct_write_vars();
}

static void write_direct_byte(bool is_written_type, uint8_t data)
{
	if (is_length_set) {
		write_byte_direct_string(is_written_type, data);
		if (direct_write_length == direct_write_index) {
			end_direct_string(is_written_type);
		}
	} else {
		set_written_length(is_written_type, data);
		if (is_written_type)
			dmi_curr_state = DMI_NAME_WRITE;
	}
}

void write_direct(enum i2c_addr_space write_address)
{
	switch (write_address) {
	case DMIN:
		if (dmi_curr_state == DMI_VALUE_WRITE) {
			free_direct_string();
			break;
		}
		write_direct_byte(true, i2c_buffer.layout.dmi_name);
		break;
	case DMIV:
		if (dmi_curr_state != DMI_VALUE_WRITE)
			break;

		write_direct_byte(false,i2c_buffer.layout.dmi_value);
		break;
	default:
		break;
	}
}

