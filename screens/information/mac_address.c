/*
 * mac_address.c
 *
 * Created: 3/29/2017 8:58:20 PM
 *  Author: Nikita
 */

#include "gfx/gfx_components/gfx_information.h"
#include "eeprom/eeprom_layout.h"
#include "information.h"
#include <stdio.h>

static void sprintf_mac_address(struct gfx_information *info, char *output_str)
{
	uint8_t mac_address_index = info->metadata;
	uint16_t reg_addr = MAC_ADDRESS_EEPROM_ADDRESS + MAC_ADDRESS_LENGTH * mac_address_index;
	uint8_t mac_addr[MAC_ADDRESS_LENGTH] = { 0 };
	eeprom_read_byte_array(reg_addr, mac_addr, MAC_ADDRESS_LENGTH);
	sprintf(output_str, "[%d] %02X:%02X:%02X:%02X:%02X:%02X", (mac_address_index + 1),
			mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

int gfx_information_init_show_mac_address(struct gfx_information *info)
{
	info->to_string = sprintf_mac_address;
	return 0;
}
