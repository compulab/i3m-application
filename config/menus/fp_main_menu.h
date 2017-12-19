
#ifndef FP_MAIN_MENU_H_
#define FP_MAIN_MENU_H_

#include "config/cnf_blk_components.h"
#include "config/logos.h"
#include "config/fonts.h"
#include "config/menus/fp_dashboard.h"
#include "menus_id.h"

char __attribute__((section (".configData"))) main_menu_title[] = "Main Menu";
char __attribute__((section (".configData"))) main_menu_0[] = "Information";
char __attribute__((section (".configData"))) main_menu_1[] = "    CPU";
char __attribute__((section (".configData"))) main_menu_2[] = "    GPU";
char __attribute__((section (".configData"))) main_menu_3[] = " Hard Drive";
char __attribute__((section (".configData"))) main_menu_4[] = "Dashboard";

struct gfx_mono_menu  __attribute__((section (".configData"))) fp_main_mono_menu = {
	.title= main_menu_title,
	.strings[0] = main_menu_0,
	.strings[1] = main_menu_1,
	.strings[2] = main_menu_2,
	.strings[3] = main_menu_3,
	.strings[4] = main_menu_4,
	.num_elements = 5,
	.current_selection = 0
};

struct cnf_action_node __attribute__((section (".configData"))) show_airtop_dashboard_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = AIRTOP_DASHBOARD_MENU_ID
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) show_airtop_hdd_menu_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = AIRTOP_HDD_MENU_ID
	},
	.next = &show_airtop_dashboard_action
};

struct cnf_action_node __attribute__((section (".configData"))) show_airtop_gpu_menu_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = AIRTOP_GPU_MENU_ID
	},
	.next = &show_airtop_hdd_menu_action
};

struct cnf_action_node __attribute__((section (".configData"))) show_airtop_cpu_menu_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = AIRTOP_CPU_MENU_ID
	},
	.next = &show_airtop_gpu_menu_action
};

struct cnf_action_node __attribute__((section (".configData"))) show_airtop_info_menu_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = AIRTOP_INFORMATION_MENU_ID
	},
	.next = &show_airtop_cpu_menu_action
};

struct cnf_image_node  __attribute__((section (".configData"))) screen_saver_image = {
	.image = {
		.bitmap_progmem = screensaver_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0,
};

struct cnf_image_node  __attribute__((section (".configData"))) main_hdd_image = {
	.image = {
		.bitmap_progmem = hdd_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &screen_saver_image,
};

struct cnf_image_node  __attribute__((section (".configData"))) main_gpu_image = {
	.image = {
		.bitmap_progmem = gpu_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &main_hdd_image,
};

struct cnf_image_node  __attribute__((section (".configData"))) main_cpu_image = {
	.image = {
		.bitmap_progmem = cpu_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &main_gpu_image
};

struct cnf_image_node  __attribute__((section (".configData"))) main_info_image = {
	.image = {
		.bitmap_progmem = info_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &main_cpu_image
};

struct cnf_menu   __attribute__((section (".configData"))) fp_main_menu_cnf = {
	.menu = &fp_main_mono_menu,
	.actions_head = &show_airtop_info_menu_action,
	.id = MAIN_MENU_ID,
	.images_items_head = &main_info_image
};

#endif /* FP_MAIN_MENU_H_ */
