/*
 * HDD_Menu.h
 *
 *  Created on: Sep 27, 2015
 *      Author: arkadi
 */

#ifndef FP_INFO_MENU_H
#define FP_INFO_MENU_H
#include "menus_id.h"
#include "config/fonts.h"

char __attribute__((section (".configData"))) airtop_info_menu_title[] = "Information";
char __attribute__((section (".configData"))) app_version_title_progmem[] = "Application Ver";
char __attribute__((section (".configData"))) serial_number_title_progmem[] = "Serial Number";
char __attribute__((section (".configData"))) part_number_title_progmem[] = "Part Number";
char __attribute__((section (".configData"))) mac_address_title_progmem[] = "MAC Addresses";
char __attribute__((section (".configData"))) memory_title_progmem[] = "Memory";
char __attribute__((section (".configData"))) airtop_info_go_back_to_main_progmem[] = "Back";

struct gfx_mono_menu  __attribute__((section (".configData"))) airtop_info_menu = {
	.title = airtop_info_menu_title,
	.strings[0] = app_version_title_progmem,
	.strings[1] = serial_number_title_progmem,
	.strings[2] = part_number_title_progmem,
	.strings[3] = mac_address_title_progmem,
	.strings[4] = memory_title_progmem,
	.strings[5] = airtop_info_go_back_to_main_progmem,
	.num_elements = 6,
	.current_selection = 0
};

struct cnf_info_node __attribute__((section (".configData"))) memory_info = {
	.info = {
		.info_type = SHOW_MEMORY_SIZE,
		.information = 20,
		.x = 0,
		.y = 15,
		.max_length = 25
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) serial_number_info = {
	.info = {
		.info_type = SHOW_SERIAL_NUMBER,
		.information = 20,
		.x = 0,
		.y = 15,
		.max_length = 25
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) part_number_info = {
	.info = {
		.info_type = SHOW_PART_NUMBER,
		.x = 1,
		.y = 5,
		.max_length = 80
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};


struct cnf_info_node __attribute__((section (".configData"))) app_version2_info = {
	.info = {
		.info_type = SHOW_APP_VERSION,
		.information = 2,
		.x = 1,
		.y = 25,
		.max_length = 30
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_info_node __attribute__((section (".configData"))) app_version1_info = {
	.info = {
		.info_type = SHOW_APP_VERSION,
		.information = 1,
		.x = 1,
		.y = 15,
		.max_length = 15
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &app_version2_info,
};

struct cnf_info_node __attribute__((section (".configData"))) app_version0_info = {
	.info = {
		.info_type = SHOW_APP_VERSION,
		.information = 0,
		.x = 1,
		.y = 5,
		.max_length = 15
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &app_version1_info,
};


struct cnf_info_node __attribute__((section (".configData"))) mac2_address_info = {
	.info = {
			.info_type = SHOW_MAC_ADDRESS,
			.information = 1,
			.x = 0,
			.y = 25,
			.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};


struct cnf_label_node __attribute__((section (".configData"))) app_version_title = {
	.label = {
		.text = app_version_title_progmem,
		.x = (92 - 10 * 5) / 2,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) app_version_frame = {
	.labels_head = &app_version_title,
	.infos_head = &app_version0_info,
	.images_head = 0
};

struct cnf_info_node __attribute__((section (".configData"))) mac_address_info = {
	.info = {
		.info_type = SHOW_MAC_ADDRESS,
		.information = 0,
		.x = 0,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = &mac2_address_info,
};

struct cnf_label_node __attribute__((section (".configData"))) memory_title = {
	.label = {
		.text = memory_title_progmem,
		.x = (92 - 5 * 5) / 2,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_label_node __attribute__((section (".configData"))) serial_number_title = {
	.label = {
		.text = serial_number_title_progmem,
		.x = (92 - 5 * 5) / 2,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_label_node __attribute__((section (".configData"))) part_number_title = {
	.label = {
		.text = part_number_title_progmem,
		.x = (92 - 5 * 5) / 2,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_label_node __attribute__((section (".configData"))) mac_address_title = {
	.label = {
		.text = mac_address_title_progmem,
		.x = (92 - 5 * 5) / 2,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) memory_frame = {
	.labels_head = &memory_title,
	.infos_head = &memory_info,
	.images_head = 0
};

struct cnf_frame __attribute__((section (".configData"))) serial_number_frame = {
	.labels_head = &serial_number_title,
	.infos_head = &serial_number_info,
	.images_head = 0
};

struct cnf_frame __attribute__((section (".configData"))) part_number_frame = {
	.labels_head = &part_number_title,
	.infos_head = &part_number_info,
	.images_head = 0
};

struct cnf_frame __attribute__((section (".configData"))) mac_address_frame = {
	.labels_head = &mac_address_title,
	.infos_head = &mac_address_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) info_menu_go_back_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = MAIN_MENU_ID
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) memory_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &memory_frame
	},
	.next = &info_menu_go_back_action
};

struct cnf_action_node __attribute__((section (".configData"))) mac_address_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &mac_address_frame
	},
	.next = &memory_action
};

struct cnf_action_node __attribute__((section (".configData"))) part_number_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &part_number_frame
	},
	.next = &mac_address_action
};

struct cnf_action_node __attribute__((section (".configData"))) serial_number_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &serial_number_frame
	},
	.next = &part_number_action
};

struct cnf_action_node __attribute__((section (".configData"))) app_version_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &app_version_frame
	},
	.next = &serial_number_action
};

struct cnf_image_node  __attribute__((section (".configData"))) info_back_to_main_menu_image = {
	.image = {
		.bitmap_progmem = back_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0
};

struct cnf_image_node  __attribute__((section (".configData"))) memory_image = {
	.image = {
		.bitmap_progmem = mac_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &info_back_to_main_menu_image
};

struct cnf_image_node  __attribute__((section (".configData"))) info_mac_address_image = {
	.image = {
		.bitmap_progmem = mac_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &memory_image
};

struct cnf_image_node  __attribute__((section (".configData"))) info_part_number_image = {
	.image = {
		.bitmap_progmem = part_number_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &info_mac_address_image
};

struct cnf_image_node  __attribute__((section (".configData"))) info_serial_number_image = {
	.image = {
		.bitmap_progmem = serial_number_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &info_part_number_image
};

struct cnf_image_node  __attribute__((section (".configData"))) app_version_image = {
	.image = {
		.bitmap_progmem = info_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &info_serial_number_image
};

struct cnf_menu   __attribute__((section (".configData"))) airtop_info_menu_cnf = {
	.menu = &airtop_info_menu,
	.actions_head = &app_version_action,
	.id = AIRTOP_INFORMATION_MENU_ID,
	.images_items_head = &app_version_image
};

#endif /* FP_INFO_MENU_H */
