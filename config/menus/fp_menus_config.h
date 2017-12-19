/*
 * fp_menus_config.h
 *
 *  Created on: Aug 17, 2015
 *      Author: arkadi
 */

#ifndef FP_MENUS_CONFIG_H
#define FP_MENUS_CONFIG_H

#include "fp_main_menu.h"
#include "fp_info_menu.h"
#include "fp_cpu_menu.h"
#include "fp_gpu_menu.h"
#include "fp_hdd_menu.h"
#include "fp_debug_menu.h"
#include "fp_dashboard.h"
#include "config/fonts.h"
#include "config/logos.h"
#include "config/cnf_blk_components.h"
#include "menus_id.h"

struct cnf_font_node __attribute__((section (".configData"))) courier_new_7X11_font = {
	.font = {
		.font_table = Courier_New7X11,
		.width = 7,
		.height = 11,
		.start_char = 32,
		.end_char = 127,
		.table_type = MIKRO,
	},
	.id = GLCD_FONT_COURIER_NEW_7X11,
	.next = 0,
};

struct cnf_font_node __attribute__((section (".configData"))) consolas_18x28_num_font = {
	.font = {
		.font_table = Consolas18x28,
		.width = 18,
		.height = 28,
		.start_char = 45,
		.end_char = 57,
		.table_type = MIKRO,
	},
	.id = GLCD_FONT_CONSOLAS_18X28_NUM,
	.next = &courier_new_7X11_font,
};

struct cnf_font_node __attribute__((section (".configData"))) courier_new_10x16_font = {
	.font = {
		.font_table = Courier_New10X16,
		.width = 10,
		.height = 16,
		.start_char = 32,
		.end_char = 127,
		.table_type = MIKRO,
	},
//	.id = GLCD_FONT_COURIER_NEW_10X16,
	.next = &consolas_18x28_num_font,
};

struct cnf_font_node __attribute__((section (".configData"))) courier_new_13X21_font = {
	.font = {
		.font_table = Courier_New13x21,
		.width = 13,
		.height = 21,
		.start_char = 32,
		.end_char = 128,
		.table_type = MIKRO,
	},
	.id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &consolas_18x28_num_font,
};

struct cnf_menu_node __attribute__((section (".configData"))) airtop_dashboard_menu_node = {
	.menu = &airtop_dashboard_menu_cnf,
	.next = 0
};

struct cnf_menu_node __attribute__((section (".configData"))) airtop_hdd_menu_node = {
	.menu = &airtop_hdd_menu_cnf,
	.next = &airtop_dashboard_menu_node
};

struct cnf_menu_node __attribute__((section (".configData"))) airtop_gpu_menu_node = {
	.menu = &airtop_gpu_menu_cnf,
	.next = &airtop_hdd_menu_node
};

struct cnf_menu_node __attribute__((section (".configData"))) airtop_cpu_menu_node = {
	.menu = &airtop_cpu_menu_cnf,
	.next = &airtop_gpu_menu_node
};

struct cnf_menu_node __attribute__((section (".configData"))) airtop_info_menu_node = {
	.menu = &airtop_info_menu_cnf,
	.next = &airtop_cpu_menu_node
};

struct cnf_menu_node __attribute__((section (".configData"))) main_menu_node = {
	.menu = &fp_main_menu_cnf,
	.next = &airtop_info_menu_node
};

struct cnf_blk  __attribute__((section (".configSec"))) menus_config = {
	.signature = 0x40,
	.menu_size = NUMBER_OF_MENUS,
	.menus_head = &main_menu_node,
	.fonts_head = &courier_new_13X21_font,
	.font_size = 3,
};

#endif /* FP_MENUS_CONFIG_H */
