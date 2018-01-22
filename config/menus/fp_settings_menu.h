/*
 * fp_settings.h
 *
 *  Created on: Nov 8, 2015
 *      Author: arkadi
 */

#ifndef FP_MENU_FP_SETTINGS_H_
#define FP_MENU_FP_SETTINGS_H_

#include "../fonts.h"

char __attribute__((section (".configData"))) fp_settings_menu_title[] = "Settings";
char __attribute__((section (".configData"))) brightness_title_progmem[] = "Adjust Brightness";
char __attribute__((section (".configData"))) logo_title_progmem[] = "Display Logo";
char __attribute__((section (".configData"))) screen_off_title_progmem[] = "Turn-off Screen";
char __attribute__((section (".configData"))) settings_back_to_main_title_progmem[] = "Back";

struct gfx_mono_menu  __attribute__((section (".configData"))) settings_menu = {
	.title = fp_settings_menu_title,
	.strings[0] = brightness_title_progmem,
	.strings[1] = logo_title_progmem,
	.strings[2] = screen_off_title_progmem,
	.strings[3] = settings_back_to_main_title_progmem,
	.num_elements = 4,
	.current_selection = 0
};

struct cnf_info_node __attribute__((section (".configData"))) brightness_info = {
	.info = {
		.info_type = SET_BRIGHTNESS,
		.information = 0,
		.x = 50,
		.y = 15,
		.max_length = 8
	},
	.font_id = GLCD_FONT_CONSOLAS_18X28_NUM,
	.next = 0
};

struct cnf_label_node __attribute__((section (".configData"))) brightness_title = {
	.label = {
		.text = brightness_title_progmem,
		.x = (128 - 18 * 5)  / 2 - 3,
		.y = 56,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) brightness_frame = {
	.labels_head = &brightness_title,
	.infos_head = &brightness_info,
	.images_head = 0
};

struct cnf_info_node __attribute__((section (".configData"))) show_logo_info = {
	.info = {
		.info_type = SHOW_LOGO,
		.information = 0,
		.x = 50,
		.y = 15,
		.max_length = 8
	},
	.font_id = GLCD_FONT_CONSOLAS_18X28_NUM,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) show_logo_frame = {
	.labels_head = 0,
	.infos_head = &show_logo_info,
	.images_head = 0
};

struct cnf_info_node __attribute__((section (".configData"))) screen_off_info = {
	.info = {
		.info_type = SCREEN_OFF,
		.information = 0,
		.x = 50,
		.y = 15,
		.max_length = 8
	},
	.font_id = GLCD_FONT_CONSOLAS_18X28_NUM,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) screen_off_frame = {
	.labels_head = 0,
	.infos_head = &screen_off_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) settings_menu_go_back_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = MAIN_MENU_ID
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) screen_off_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &screen_off_frame
	},
	.next = &settings_menu_go_back_action
};

struct cnf_action_node __attribute__((section (".configData"))) show_logo_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &show_logo_frame
	},
	.next = &screen_off_action
};

struct cnf_action_node __attribute__((section (".configData"))) show_brightness_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &brightness_frame
	},
	.next = &show_logo_action
};

struct cnf_image_node  __attribute__((section (".configData"))) settings_back_to_main_image = {
	.image = {
		.bitmap_progmem = back_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0,
};

struct cnf_image_node  __attribute__((section (".configData"))) screen_off_image = {
	.image = {
		.bitmap_progmem = screen_off_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &settings_back_to_main_image
};

struct cnf_image_node  __attribute__((section (".configData"))) show_logo_image = {
	.image = {
		.bitmap_progmem = show_logo_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &screen_off_image
};

struct cnf_image_node  __attribute__((section (".configData"))) settings_brightness_image = {
	.image = {
		.bitmap_progmem = brightness_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &show_logo_image
};

struct cnf_menu   __attribute__((section (".configData"))) airtop_settings_menu_cnf = {
	.menu = &settings_menu,
	.actions_head = &show_brightness_action,
	.id = AIRTOP_SETTINGS_MENU_ID,
	.images_items_head = &settings_brightness_image
};

#endif /* FP_MENU_FP_SETTINGS_H_ */