/*
 * fp_dashboard.h
 *
 *  Created on: Jan 25, 2016
 *      Author: arkadi
 */

#ifndef FP_MENU_FP_DASHBOARD_H_
#define FP_MENU_FP_DASHBOARD_H_

#include "config/cnf_blk_components.h"
#include "menus_id.h"

char __attribute__((section (".configData"))) dashboard_menu_title[] = "Dashboard";
char __attribute__((section (".configData"))) dashboard_cpu_gpu_temp_progmem[] = "CPU/GPU temp.";
char __attribute__((section (".configData"))) dashboard_cput_progmem[] = "CPU";
char __attribute__((section (".configData"))) dashboard_gpu_progmem[] = "GPU";
char __attribute__((section (".configData"))) dashboard_cpu_temperature_title_progmem[] = "Cores temp.";
char __attribute__((section (".configData"))) dashboard_gpu_temperature_title_progmem[] = "GPU Temperature";
char __attribute__((section (".configData"))) dashboard_hdd_temperature_title_progmem[] = "Disks temp.";
char __attribute__((section (".configData"))) power_state_title_text[] = "System Power";
char __attribute__((section (".configData"))) power_consumption_title_text[] = "System Power";
char __attribute__((section (".configData"))) airtop_dashboard_go_back_to_main_progmem[] = "Back";

struct gfx_mono_menu  __attribute__((section (".configData"))) airtop_dashboard_menu = {
	.title = dashboard_menu_title,
	.strings[0] = dashboard_cpu_gpu_temp_progmem,
	.strings[1] = dashboard_cpu_temperature_title_progmem,
	.strings[2] = dashboard_gpu_temperature_title_progmem,
	.strings[3] = dashboard_hdd_temperature_title_progmem,
	.strings[4] = power_state_title_text,
	.strings[5] = airtop_dashboard_go_back_to_main_progmem,
	.num_elements = 6,
	.current_selection = 0
};

struct cnf_info_node __attribute__((section (".configData"))) power_consumption_info = {
	.info = {
		.info_type = SHOW_COMPUTER_POWER,
		.information = 100,
		.x = 45,
		.y = 8,
		.max_length = 20
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) power_state_info = {
	.info = {
		.info_type = SHOW_POWER_STATE,
		.information = 0,
		.x = 0,
		.y = 38,
		.max_length = 20
	},
	.font_id = GLCD_FONT_COURIER_NEW_7X11,
	.next = &power_consumption_info
};

struct cnf_label_node __attribute__((section (".configData"))) power_state_title = {
	.label = {
		.text = power_state_title_text,
		.x = (128 - 5 * 12) / 2,
		.y = 54,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) power_state_frame = {
	.labels_head = &power_state_title,
	.infos_head = &power_state_info,
	.images_head = 0
};

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

struct cnf_label_node __attribute__((section (".configData"))) dashboard_title = {
	.label = {
		.text = dashboard_cpu_gpu_temp_progmem,
		.x = (128 - 5 * 12) / 2,
		.y = 54,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0,
};

struct cnf_label_node __attribute__((section (".configData"))) gpu_title = {
	.label = {
		.text = dashboard_gpu_progmem,
		.x = 72,
		.y = 5,
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &dashboard_title,
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

struct cnf_frame __attribute__((section (".configData"))) airtop_dashboard_frame = {
	.labels_head = &cput_title,
	.infos_head = &cput_info,
	.images_head = 0,
};

struct cnf_action_node __attribute__((section (".configData"))) dashboard_menu_go_back_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = MAIN_MENU_ID
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) power_state_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &power_state_frame
	},
	.next = &dashboard_menu_go_back_action
};

struct cnf_label_node __attribute__((section (".configData"))) dashboard_hdd_temperature_title = {
	.label = {
		.text = dashboard_hdd_temperature_title_progmem,
		.x = (128 - 5 * 11) / 2,
		.y = 54,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) dashboard_hdd3_temperature_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 3,
		.x = 80,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = 0,
};

struct cnf_info_node __attribute__((section (".configData"))) dashboard_hdd2_temperature_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 2,
		.x = 10,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &dashboard_hdd3_temperature_info,
};

struct cnf_info_node __attribute__((section (".configData"))) dashboard_hdd1_temperature_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 1,
		.x = 80,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &dashboard_hdd2_temperature_info,
};

struct cnf_info_node __attribute__((section (".configData"))) dashboard_hdd0_temperature_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &dashboard_hdd1_temperature_info,
};

struct cnf_frame __attribute__((section (".configData"))) dashboard_hdd_temperature_frame = {
	.labels_head = &dashboard_hdd_temperature_title,
	.infos_head = &dashboard_hdd0_temperature_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) dashboard_hdd_temperature_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &dashboard_hdd_temperature_frame
	},
	.next = &power_state_action
};


struct cnf_info_node __attribute__((section (".configData"))) dashboard_gpu_temperature_info = {
	.info = {
		.info_type = SHOW_GPU_TEMPERTURE,
		.information = 0,
		.x = 45,
		.y = 15,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = 0,
};

struct cnf_label_node __attribute__((section (".configData"))) dashboard_gpu_temperature_title = {
	.label = {
		.text = dashboard_gpu_temperature_title_progmem,
		.x = (128 - 5 * 15) / 2,
		.y = 54,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) dashboard_gpu_temperature_frame = {
	.labels_head = &dashboard_gpu_temperature_title,
	.infos_head = &dashboard_gpu_temperature_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) dashboard_gpu_temperature_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &dashboard_gpu_temperature_frame
	},
	.next = &dashboard_hdd_temperature_action
};

struct cnf_label_node __attribute__((section (".configData"))) dashboard_cpu_temperature_title = {
	.label = {
		.text = dashboard_cpu_temperature_title_progmem,
		.x = (128 - 5 * 11) / 2,
		.y = 54,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) dashboard_cpu3_temperature_info = {
	.info = {
		.info_type = SHOW_CPU_TEMPERTURE,
		.information = 3,
		.x = 80,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = 0,
};

struct cnf_info_node __attribute__((section (".configData"))) dashboard_cpu2_temperature_info = {
	.info = {
		.info_type = SHOW_CPU_TEMPERTURE,
		.information = 2,
		.x = 10,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &dashboard_cpu3_temperature_info,
};

struct cnf_info_node __attribute__((section (".configData"))) dashboard_cpu1_temperature_info = {
	.info = {
		.info_type = SHOW_CPU_TEMPERTURE,
		.information = 1,
		.x = 80,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &dashboard_cpu2_temperature_info,
};

struct cnf_info_node __attribute__((section (".configData"))) dashboard_cpu0_temperature_info = {
	.info = {
		.info_type = SHOW_CPU_TEMPERTURE,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &dashboard_cpu1_temperature_info,
};

struct cnf_frame __attribute__((section (".configData"))) dashboard_cpu_temperature_frame = {
	.labels_head = &dashboard_cpu_temperature_title,
	.infos_head = &dashboard_cpu0_temperature_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) dashboard_cpu_temperature_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &dashboard_cpu_temperature_frame
	},
	.next = &dashboard_gpu_temperature_action
};

struct cnf_action_node __attribute__((section (".configData"))) cpu_gpu_temp_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &airtop_dashboard_frame
	},
	.next = &dashboard_cpu_temperature_action
};

struct cnf_image_node  __attribute__((section (".configData"))) dasboard_back_to_main_menu_image = {
	.image = {
		.bitmap_progmem = back_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0
};

struct cnf_image_node  __attribute__((section (".configData"))) power_state_image = {
	.image = {
		.bitmap_progmem = power_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &dasboard_back_to_main_menu_image
};

struct cnf_image_node  __attribute__((section (".configData"))) dashboard_hdd_temperature_image = {
	.image = {
		.bitmap_progmem = hdd_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &power_state_image
};

struct cnf_image_node  __attribute__((section (".configData"))) dashboard_gpu_temperature_image = {
	.image = {
		.bitmap_progmem = gpu_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &dashboard_hdd_temperature_image
};

struct cnf_image_node  __attribute__((section (".configData"))) dashboard_cpu_temperature_image = {
	.image = {
		.bitmap_progmem = cpu_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &dashboard_gpu_temperature_image
};

struct cnf_image_node  __attribute__((section (".configData"))) cpu_gpu_temp_image = {
	.image = {
		.bitmap_progmem = temp_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &dashboard_cpu_temperature_image
};

struct cnf_menu   __attribute__((section (".configData"))) airtop_dashboard_menu_cnf = {
	.menu = &airtop_dashboard_menu,
	.actions_head = &cpu_gpu_temp_action,
	.id = AIRTOP_DASHBOARD_MENU_ID,
	.images_items_head = &cpu_gpu_temp_image
};

#endif /* FP_MENU_FP_DASHBOARD_H_ */
