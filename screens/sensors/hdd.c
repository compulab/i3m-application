/*
 * hdd.c
 *
 * Created: 3/21/2017 1:59:51 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"
#include "lib/syntax.h"

void sprintf_hdd_temp(char *output_str, uint8_t hdd_id)
{
	if (BIT_ON(computer_data.packed.hddts, hdd_id))
		sprintf_temperature(output_str, computer_data.packed.hddt[hdd_id]);
	else
		sprintf_inval_data(output_str);
}

bool is_hdd_temp_need_update(struct gfx_information *info, bool is_visible)
{
	if (is_visible)
		return !BIT_ON(computer_data.packed.hddts, info->info_data);

	return true;
}

void sprintf_hdd_size(char *output_str, uint8_t hdd_id)
{
	uint16_t size = computer_data.packed.hddsz[hdd_id];
	bool is_tera = BIT_ON(computer_data.packed.hddf, hdd_id);
	char *units = is_tera ? " TB" : " GB";

	if (BIT_ON(computer_data.packed.hdds, hdd_id))
		sprintf(output_str, "%d %s", size, units);
	else
		sprintf_inval_data(output_str);
}

bool is_hdd_size_need_update(struct gfx_information *info, bool is_visible)
{
	if (is_visible)
		return !BIT_ON(computer_data.packed.hdds, info->info_data);

	return true;
}
