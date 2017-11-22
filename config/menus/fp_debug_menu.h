/*
 * HDD_Menu.h
 *
 *  Created on: 3/8/2017
 *      Author: Nikita Kiryanov
 */

#ifndef FP_DEBUG_MENU_H
#define FP_DEBUG_MENU_H
#include "config/cnf_blk_components.h"
#include "config/fonts.h"
#include "menus_id.h"

char __attribute__((section (".configData"))) debug_menu_title[] = "Debug";
char __attribute__((section (".configData"))) debug_usb_serial_input_title_text[] = "USB Serial Input";
char __attribute__((section (".configData"))) debug_go_back_to_main[] = "Back";

struct gfx_mono_menu  __attribute__((section (".configData"))) airtop_debug_menu = {
	.title = debug_menu_title,
	.strings[0] = debug_usb_serial_input_title_text,
	.strings[1] = debug_go_back_to_main,
	.num_elements = 2,
	.current_selection = 0
};

struct cnf_info_node __attribute__((section (".configData"))) debug_usb_serial_input_info = {
	.info = {
		.info_type = SHOW_USB_SERIAL_INPUT,
		.information = 100,
		.x = 0,
		.y = 0,
		.max_length = 100
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_label_node __attribute__((section (".configData"))) debug_usb_serial_input_title = {
	.label = {
		.text = debug_usb_serial_input_title_text,
		.x = (92 - 5 * 5) / 2,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) debug_usb_serial_output_frame = {
	.labels_head = &debug_usb_serial_input_title,
	.infos_head = &debug_usb_serial_input_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) debug_menu_go_back_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = MAIN_MENU_ID
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) debug_usb_serial_output_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &debug_usb_serial_output_frame
	},
	.next = &debug_menu_go_back_action
};

struct cnf_image_node  __attribute__((section (".configData"))) debug_back_to_main_menu_image = {
	.image = {
		.bitmap_progmem = back_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0
};

struct cnf_image_node  __attribute__((section (".configData")))debug_usb_serial_output_image = {
	.image = {
		.bitmap_progmem = size_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &debug_back_to_main_menu_image
};

struct cnf_menu   __attribute__((section (".configData"))) airtop_debug_menu_cnf = {
	.menu = &airtop_debug_menu,
	.actions_head = &debug_usb_serial_output_action,
	.id = AIRTOP_DEBUG_MENU_ID,
	.images_items_head = &debug_usb_serial_output_image
};

#endif /* FP_DEBUG_MENU_H */
