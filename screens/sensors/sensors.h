/*
 * sensors.h
 *
 * Created: 3/29/2017 8:34:47 PM
 *  Author: Nikita
 */


#ifndef SENSORS_H_
#define SENSORS_H_

#include "gfx/gfx_information.h"

int gfx_information_init_show_cpu_temp(struct gfx_information *info);
int gfx_information_init_show_cpu_freq(struct gfx_information *info);
int gfx_information_init_show_gpu_temp(struct gfx_information *info);
int gfx_information_init_show_hdd_temp(struct gfx_information *info);
int gfx_information_init_show_hdd_size(struct gfx_information *info);
int gfx_information_init_show_memory_size(struct gfx_information *info);
int gfx_information_init_show_ambient_temp(struct gfx_information *info);

#endif /* SENSORS_H_ */