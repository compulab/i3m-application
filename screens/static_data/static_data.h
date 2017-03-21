/*
 * static_data.h
 *
 * Created: 3/19/2017 12:01:11 AM
 *  Author: Nikita
 */


#ifndef STATIC_DATA_H_
#define STATIC_DATA_H_

void sprintf_serial_number(char *output_str);
void sprintf_part_number(char *output_str);
void sprintf_app_version(char *output_str, uint8_t type);
void sprintf_mac_address(char *output_str, uint8_t mac_address_index);

#endif /* STATIC_DATA_H_ */