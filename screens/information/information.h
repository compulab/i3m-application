/*
 * information.h
 *
 * Created: 3/19/2017 12:01:11 AM
 *  Author: Nikita
 */


#ifndef INFORMATION_H_
#define INFORMATION_H_

#include "gfx/gfx_components/gfx_information.h"

int gfx_information_init_show_serial_number(struct gfx_information *info);
int gfx_information_init_show_part_number(struct gfx_information *info);
int gfx_information_init_show_app_version(struct gfx_information *info);
int gfx_information_init_show_mac_address(struct gfx_information *info);

#endif /* INFORMATION_H_ */