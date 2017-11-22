/*
 * fp_dashboard.h
 *
 *  Created on: Jan 25, 2016
 *      Author: arkadi
 */

#ifndef FP_MENU_FP_DASHBOARD_H_
#define FP_MENU_FP_DASHBOARD_H_

#include "config/cnf_blk_components.h"

char __attribute__((section (".configData"))) dashboard_cput_progmem[] = "CPU";
char __attribute__((section (".configData"))) dashboard_gpu_progmem[] = "GPU";

struct cnf_info_node __attribute__((section (".configData"))) gpu_info = {
	.info = {
		.info_type = SHOW_GPU_TEMPERTURE,
		.information = 0,
		.x = 72,
		.y = 30,
		.max_length = 5
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = 0,
};

struct cnf_label_node __attribute__((section (".configData"))) gpu_title = {
	.label = {
		.text = dashboard_gpu_progmem,
		.x = 72,
		.y = 5,
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = 0,
};

struct cnf_info_node __attribute__((section (".configData"))) cput_info = {
	.info = {
		.info_type = SHOW_CPU_TEMPERTURE,
		.information = 0,
		.x = 10,
		.y = 30,
		.max_length = 5
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &gpu_info,
};

struct cnf_label_node __attribute__((section (".configData"))) cput_title = {
	.label = {
		.text = dashboard_cput_progmem,
		.x = 10,
		.y = 5,
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &gpu_title,
};

struct cnf_frame __attribute__((section (".configData"))) airtop_dashboard = {
	.labels_head = &cput_title,
	.infos_head = &cput_info,
	.images_head = 0,
};

#endif /* FP_MENU_FP_DASHBOARD_H_ */
