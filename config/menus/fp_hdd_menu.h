/*
 * fp_hdd_menu.h
 *
 * Created: 12-Nov-17 7:04:59 PM
 *  Author: Nikita
 */


#ifndef FP_HDD_MENU_H_
#define FP_HDD_MENU_H_

#include "menus_id.h"
#include "config/fonts.h"

char __attribute__((section (".configData"))) airtop_hdd_menu_title[] = "Hard Drive";
char __attribute__((section (".configData"))) hdd_temperature_title_progmem[] = "HDD temperature";
char __attribute__((section (".configData"))) hdd_frequency_title_progmem[] = "HDD frequency";
char __attribute__((section (".configData"))) airtop_hdd_go_back_to_main_progmem[] = "Back";

struct gfx_mono_menu  __attribute__((section (".configData"))) airtop_hdd_menu = {
	.title = airtop_hdd_menu_title,
	.strings[0] = hdd_temperature_title_progmem,
	.strings[1] = hdd_frequency_title_progmem,
	.strings[2] = airtop_hdd_go_back_to_main_progmem,
	.num_elements = 3,
	.current_selection = 0
};

struct cnf_info_node __attribute__((section (".configData"))) hdd3_frequency_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 3,
		.x = 70,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) hdd2_frequency_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 2,
		.x = 10,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &hdd3_frequency_info
};

struct cnf_info_node __attribute__((section (".configData"))) hdd1_frequency_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 1,
		.x = 70,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &hdd2_frequency_info
};

struct cnf_info_node __attribute__((section (".configData"))) hdd0_frequency_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &hdd1_frequency_info
};

struct cnf_info_node __attribute__((section (".configData"))) hdd3_temperature_info = {
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

struct cnf_info_node __attribute__((section (".configData"))) hdd2_temperature_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 2,
		.x = 10,
		.y = 25,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &hdd3_temperature_info,
};

struct cnf_info_node __attribute__((section (".configData"))) hdd1_temperature_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 1,
		.x = 80,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &hdd2_temperature_info,
};

struct cnf_info_node __attribute__((section (".configData"))) hdd0_temperature_info = {
	.info = {
		.info_type = SHOW_HDD_TEMPERTURE,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &hdd1_temperature_info,
};

struct cnf_label_node __attribute__((section (".configData"))) hdd_frequency_title = {
	.label = {
		.text = hdd_frequency_title_progmem,
		.x = 27,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_label_node __attribute__((section (".configData"))) hdd_temperature_title = {
	.label = {
		.text = hdd_temperature_title_progmem,
		.x = 17,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) hdd_frequency_frame = {
	.labels_head = &hdd_frequency_title,
	.infos_head = &hdd0_frequency_info,
	.images_head = 0
};

struct cnf_frame __attribute__((section (".configData"))) hdd_temperature_frame = {
	.labels_head = &hdd_temperature_title,
	.infos_head = &hdd0_temperature_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) hdd_menu_go_back_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = MAIN_MENU_ID
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) hdd_frequency_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &hdd_frequency_frame
	},
	.next = &hdd_menu_go_back_action
};

struct cnf_action_node __attribute__((section (".configData"))) hdd_temperature_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &hdd_temperature_frame
	},
	.next = &hdd_frequency_action
};

struct cnf_image_node  __attribute__((section (".configData"))) hdd_back_to_main_menu_image = {
	.image = {
		.bitmap_progmem = back_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0
};

struct cnf_image_node  __attribute__((section (".configData"))) hdd_frequency_image = {
	.image = {
		.bitmap_progmem = frequency_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &hdd_back_to_main_menu_image
};

struct cnf_image_node  __attribute__((section (".configData"))) hdd_temperature_image = {
	.image = {
		.bitmap_progmem = temp_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &hdd_frequency_image
};

struct cnf_menu   __attribute__((section (".configData"))) airtop_hdd_menu_cnf = {
	.menu = &airtop_hdd_menu,
	.actions_head = &hdd_temperature_action,
	.id = AIRTOP_HDD_MENU_ID,
	.images_items_head = &hdd_temperature_image
};

#endif /* FP_HDD_MENU_H_ */