/*
 * eeprom_layout.c
 *
 * Created: 3/29/2017 5:20:27 PM
 *  Author: Nikita
 */

#include "eeprom.h"
#include "eeprom_layout.h"
#include <stdio.h>

void eeprom_get_mac_address(uint8_t output_mac_addr[MAC_ADDRESS_LENGTH], uint8_t mac_address_index)
{
	uint8_t eeprom_addr = MAC_ADDRESS_EEPROM_ADDRESS + MAC_ADDRESS_LENGTH * mac_address_index;
	for (int i = 0; i < MAC_ADDRESS_LENGTH; i++)
		output_mac_addr[i] = eeprom_read_byte(eeprom_addr + i);
}
