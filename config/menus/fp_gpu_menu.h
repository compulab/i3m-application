/*
 * fp_gpu_menu.h
 *
 * Created: 07-Nov-17 3:30:23 PM
 *  Author: Nikita
 */


#ifndef FP_GPU_MENU_H_
#define FP_GPU_MENU_H_

#include "menus_id.h"
#include "config/fonts.h"

char __attribute__((section (".configData"))) gpu_menu_title[] = "GPU";
char __attribute__((section (".configData"))) gpu_temperature_title_progmem[] = "GPU temperature";
char __attribute__((section (".configData"))) gpu_frequency_title_progmem[] = "GPU frequency";
char __attribute__((section (".configData"))) gpu_go_back_to_main_progmem[] = "Back";

struct gfx_mono_menu  __attribute__((section (".configData"))) airtop_gpu_menu = {
	.title = gpu_menu_title,
	.strings[0] = gpu_temperature_title_progmem,
	.strings[1] = gpu_frequency_title_progmem,
	.strings[2] = gpu_go_back_to_main_progmem,
	.num_elements = 3,
	.current_selection = 0
};

struct cnf_info_node __attribute__((section (".configData"))) gpu_frequency_info = {
	.info = {
		.info_type = SHOW_GPU_TEMPERTURE,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) gpu_temperature_info = {
	.info = {
		.info_type = SHOW_GPU_TEMPERTURE,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = 0,
};

struct cnf_label_node __attribute__((section (".configData"))) gpu_frequency_title = {
	.label = {
		.text = gpu_frequency_title_progmem,
		.x = 27,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_label_node __attribute__((section (".configData"))) gpu_temperature_title = {
	.label = {
		.text = gpu_temperature_title_progmem,
		.x = 17,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) gpu_frequency_frame = {
	.labels_head = &gpu_frequency_title,
	.infos_head = &gpu_frequency_info,
	.images_head = 0
};

struct cnf_frame __attribute__((section (".configData"))) gpu_temperature_frame = {
	.labels_head = &gpu_temperature_title,
	.infos_head = &gpu_temperature_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) gpu_menu_go_back_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = MAIN_MENU_ID
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) gpu_frequency_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &gpu_frequency_frame
	},
	.next = &gpu_menu_go_back_action
};

struct cnf_action_node __attribute__((section (".configData"))) gpu_temperature_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &gpu_temperature_frame
	},
	.next = &gpu_frequency_action
};

struct cnf_image_node  __attribute__((section (".configData"))) gpu_back_to_main_menu_image = {
	.image = {
		.bitmap_progmem = back_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0
};

struct cnf_image_node  __attribute__((section (".configData"))) gpu_frequency_image = {
	.image = {
		.bitmap_progmem = frequency_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &gpu_back_to_main_menu_image
};

struct cnf_image_node  __attribute__((section (".configData"))) gpu_temperature_image = {
	.image = {
		.bitmap_progmem = temp_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &gpu_frequency_image
};

struct cnf_menu   __attribute__((section (".configData"))) airtop_gpu_menu_cnf = {
	.menu = &airtop_gpu_menu,
	.actions_head = &gpu_temperature_action,
	.id = AIRTOP_GPU_MENU_ID,
	.images_items_head = &gpu_temperature_image
};

#endif /* FP_GPU_MENU_H_ */