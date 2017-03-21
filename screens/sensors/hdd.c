/*
 * hdd.c
 *
 * Created: 3/21/2017 1:59:51 PM
 *  Author: Nikita
 */

#include <string.h>
#include "Fp-utils.h"
#include "lib/syntax.h"

sprintf_hdd_temp(char *output_str, uint8_t hdd_id)
{
	if (BIT_ON(computer_data.packed.hddts, hdd_id))
		set_temp_string(output_str, computer_data.packed.hddt[hdd_id]);
	else
		sprintf_inval_data(output_str);
}

void sprintf_hdd_size(char *output_str, uint8_t hdd_id)
{
	if (BIT_ON(computer_data.packed.hdds, hdd_id))
		set_hdd_size_str(output_str, computer_data.packed.hddsz[hdd_id], BIT_ON(computer_data.packed.hddf, hdd_id));
	else
		sprintf_inval_data(output_str);
}

