/*
 * screens.h
 *
 * Created: 3/31/2017 12:34:13 AM
 *  Author: Nikita
 */


#ifndef SCREENS_H_
#define SCREENS_H_

#include "screens/information/information.h"
#include "screens/sensors/sensors.h"
#include "screens/bios/bios_data.h"
#include "screens/debug/debug.h"

int gfx_information_init_custom_message(struct gfx_information *info);

#endif /* SCREENS_H_ */