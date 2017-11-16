/*
 * HDD_Menu.h
 *
 *  Created on: Sep 27, 2015
 *      Author: arkadi
 */

#ifndef FP_CPU_MENU_H
#define FP_CPU_MENU_H
#include "menus_id.h"
#include "config/cnf_blk_components.h"
#include "config/fonts.h"

char __attribute__((section (".configData"))) airtop_cpu_menu_title[] = "CPU";
char __attribute__((section (".configData"))) cpu_temperature_title_progmem[] = "CPU temperature";
char __attribute__((section (".configData"))) cpu_frequency_title_progmem[] = "CPU frequency";
char __attribute__((section (".configData"))) airtop_cpu_go_back_to_main_progmem[] = "Back";

struct gfx_mono_menu  __attribute__((section (".configData"))) airtop_cpu_menu = {
	.title = airtop_cpu_menu_title,
	.strings[0] = cpu_temperature_title_progmem,
	.strings[1] = cpu_frequency_title_progmem,
	.strings[2] = airtop_cpu_go_back_to_main_progmem,
	.num_elements = 3,
	.current_selection = 0
};

struct cnf_info_node __attribute__((section (".configData"))) cpu3_frequency_info = {
	.info = {
		.info_type = SHOW_CPU_FREQUENCY,
		.information = 3,
		.x = 70,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) cpu2_frequency_info = {
	.info = {
		.info_type = SHOW_CPU_FREQUENCY,
		.information = 2,
		.x = 10,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &cpu3_frequency_info
};

struct cnf_info_node __attribute__((section (".configData"))) cpu1_frequency_info = {
	.info = {
		.info_type = SHOW_CPU_FREQUENCY,
		.information = 1,
		.x = 70,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &cpu2_frequency_info
};

struct cnf_info_node __attribute__((section (".configData"))) cpu0_frequency_info = {
	.info = {
		.info_type = SHOW_CPU_FREQUENCY,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &cpu1_frequency_info
};

struct cnf_info_node __attribute__((section (".configData"))) cpu3_temperature_info = {
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

struct cnf_info_node __attribute__((section (".configData"))) cpu2_temperature_info = {
	.info = {
		.info_type = SHOW_CPU_TEMPERTURE,
		.information = 2,
		.x = 10,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &cpu3_temperature_info,
};

struct cnf_info_node __attribute__((section (".configData"))) cpu1_temperature_info = {
	.info = {
		.info_type = SHOW_CPU_TEMPERTURE,
		.information = 1,
		.x = 80,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &cpu2_temperature_info,
};

struct cnf_info_node __attribute__((section (".configData"))) cpu0_temperature_info = {
	.info = {
		.info_type = SHOW_CPU_TEMPERTURE,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &cpu1_temperature_info,
};

struct cnf_label_node __attribute__((section (".configData"))) cpu_frequency_title = {
	.label = {
		.text = cpu_frequency_title_progmem,
		.x = 27,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_label_node __attribute__((section (".configData"))) cpu_temperature_title = {
	.label = {
		.text = cpu_temperature_title_progmem,
		.x = 17,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) cpu_frequency_frame = {
	.labels_head = &cpu_frequency_title,
	.infos_head = &cpu0_frequency_info,
	.images_head = 0
};

struct cnf_frame __attribute__((section (".configData"))) cpu_temperature_frame = {
	.labels_head = &cpu_temperature_title,
	.infos_head = &cpu0_temperature_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) cpu_menu_go_back_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = MAIN_MENU_ID
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) cpu_frequency_action = {
	.action = {
			.type = ACTION_TYPE_SHOW_FRAME,
			.frame = &cpu_frequency_frame
	},
	.next = &cpu_menu_go_back_action
};

struct cnf_action_node __attribute__((section (".configData"))) cpu_temperature_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &cpu_temperature_frame
	},
	.next = &cpu_frequency_action
};

struct cnf_image_node  __attribute__((section (".configData"))) cpu_back_to_main_menu_image = {
	.image = {
		.bitmap_progmem = back_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0
};

struct cnf_image_node  __attribute__((section (".configData"))) cpu_frequency_image = {
	.image = {
		.bitmap_progmem = frequency_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &cpu_back_to_main_menu_image
};

struct cnf_image_node  __attribute__((section (".configData"))) cpu_temperature_image = {
	.image = {
		.bitmap_progmem = temp_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &cpu_frequency_image
};

struct cnf_menu   __attribute__((section (".configData"))) airtop_cpu_menu_cnf = {
	.menu = &airtop_cpu_menu,
	.actions_head = &cpu_temperature_action,
	.id = AIRTOP_CPU_MENU_ID,
	.images_items_head = &cpu_temperature_image
};

#endif /* FP_CPU_MENU_H */
