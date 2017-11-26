
#ifndef FP_MAIN_MENU_H_
#define FP_MAIN_MENU_H_

#include "config/cnf_blk_components.h"
#include "config/logos.h"
#include "config/fonts.h"
#include "menus_id.h"

char __attribute__((section (".configData"))) main_menu_title[] = "Main Menu";
char __attribute__((section (".configData"))) main_menu_0[] = "Information";
char __attribute__((section (".configData"))) main_menu_1[] = "    CPU";
char __attribute__((section (".configData"))) main_menu_2[] = "    GPU";
char __attribute__((section (".configData"))) main_menu_3[] = " Hard Drive";
char __attribute__((section (".configData"))) main_menu_4[] = "Screen Saver";
char __attribute__((section (".configData"))) main_menu_5[] = " Power State";

struct gfx_mono_menu  __attribute__((section (".configData"))) fp_main_mono_menu = {
	.title= main_menu_title,
	.strings[0] = main_menu_0,
	.strings[1] = main_menu_1,
	.strings[2] = main_menu_2,
	.strings[3] = main_menu_3,
	.strings[4] = main_menu_4,
	.strings[5] = main_menu_5,
	.num_elements = 6,
	.current_selection = 0
};

struct cnf_info_node __attribute__((section (".configData"))) screen_saver_info = {
	.info = {
		.info_type = SHOW_SCREEN_SAVER,
		.information = 0,
		.x = 10,
		.y = 5,
		.max_length = 22
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

char __attribute__((section (".configData"))) screen_saver_title_progmem[] = "Screen Saver";
struct cnf_label_node __attribute__((section (".configData"))) screen_saver_title = {
	.label = {
		.text = screen_saver_title_progmem,
		.x = (92 - 10 * 5) / 2,
		.y = 55,
	},
	.font_id = GLCD_FONT_SYSFONT_5X7,
	.next = 0
};

struct cnf_frame __attribute__((section (".configData"))) screen_saver_frame = {
	.labels_head = &screen_saver_title,
	.infos_head = &screen_saver_info,
	.images_head = 0
};

char __attribute__((section (".configData"))) power_state_title_text[] = "Power State";
char __attribute__((section (".configData"))) power_consumption_title_text[] = "Power Consumption";
struct cnf_info_node __attribute__((section (".configData"))) power_consumption_info = {
	.info = {
		.info_type = SHOW_COMPUTER_POWER,
		.information = 100,
		.x = 45,
		.y = 35,
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
		.y = 5,
		.max_length = 20
	},
	.font_id = GLCD_FONT_COURIER_NEW_13X21,
	.next = &power_consumption_info
};

struct cnf_frame __attribute__((section (".configData"))) power_state_frame = {
	.labels_head = 0,
	.infos_head = &power_state_info,
	.images_head = 0
};

struct cnf_action_node __attribute__((section (".configData"))) show_airtop_power_menu_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &power_state_frame
	},
	.next = 0
};

struct cnf_action_node __attribute__((section (".configData"))) show_airtop_screen_saver_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_FRAME,
		.frame = &screen_saver_frame
	},
	.next = &show_airtop_power_menu_action
};

struct cnf_action_node __attribute__((section (".configData"))) show_airtop_hdd_menu_action = {
	.action = {
		.type = ACTION_TYPE_SHOW_MENU,
		.menu_id = AIRTOP_HDD_MENU_ID
	},
	.next = &show_airtop_screen_saver_action
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

struct cnf_image_node  __attribute__((section (".configData"))) main_power_image = {
	.image = {
		.bitmap_progmem = power_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = 0
};

struct cnf_image_node  __attribute__((section (".configData"))) screen_saver_image = {
	.image = {
		.bitmap_progmem = screensaver_bits,
		.width = logo_width,
		.height = logo_height
	},
	.next = &main_power_image,
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
