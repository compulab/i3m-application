/*
 * hdd.c
 *
 * Created: 3/21/2017 1:59:51 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"
#include "lib/syntax.h"

void set_hdd_size_str(char *str, uint16_t size, bool is_tera)
{
	char * units;
	if (is_tera)
		units = " TB";
	else
		units = " GB";
	sprintf(str, "%d %s", size, units);
}

sprintf_hdd_temp(char *output_str, uint8_t hdd_id)
{
	if (BIT_ON(computer_data.packed.hddts, hdd_id))
		set_temp_string(output_str, computer_data.packed.hddt[hdd_id]);
	else
		sprintf_inval_data(output_str);
}

bool is_hdd_temp_need_update(struct gfx_information *info, bool is_visible)
{
	bool need_update = false;
	if (is_visible)
	need_update = !BIT_ON(computer_data.packed.hddts, info->info_data);

	if (!need_update) {
		char temp_str[3];
		char curr_str[3];
		sprintf_hdd_temp(temp_str, info->info_data);
		set_curr_str(curr_str, SHOW_HDD_SIZE);
		need_update = strcmp(temp_str, curr_str);
	}
	return need_update;
}

void sprintf_hdd_size(char *output_str, uint8_t hdd_id)
{
	if (BIT_ON(computer_data.packed.hdds, hdd_id))
		set_hdd_size_str(output_str, computer_data.packed.hddsz[hdd_id], BIT_ON(computer_data.packed.hddf, hdd_id));
	else
		sprintf_inval_data(output_str);
}

bool is_hdd_size_need_update(struct gfx_information *info, bool is_visible)
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
