/*
 * cnf_blk_header.h
 *
 *  Created on: Aug 16, 2015
 *      Author: arkadi
 */

#ifndef CONFIG_CNF_BLK_COMPONENTS_H_
#define CONFIG_CNF_BLK_COMPONENTS_H_

#include <avr/pgmspace.h>
#include <stdbool.h>

#ifndef DEF_H_
#include "def.h"
#endif

#include "display/glcd-0.5.2/glcd_font.h"
#include "display/glcd-0.5.2/glcd.h"

# define PROGMEM_BYTE_ARRAY_T uint8_t*

typedef uint8_t gfx_mono_color_t;

struct cnf_label {
	char *text;
	uint8_t x, y;
};

struct cnf_image {
	gfx_mono_color_t *bitmap_progmem;
	uint8_t height, width;
	uint8_t x, y;
};

struct cnf_info {
	enum information_type info_type;
	uint8_t information;
	uint8_t max_length;
	uint8_t x, y;
};

struct cnf_label_node {
	struct cnf_label label;
	uint8_t font_id;
	struct cnf_label_node *next;
};

struct cnf_image_node {
	struct cnf_image image;
	struct cnf_image_node *next;
};

struct cnf_info_node {
	struct cnf_info info;
	uint8_t font_id;
	struct cnf_info_node *next;
};

struct cnf_frame {
	struct cnf_label_node *labels_head;
	struct cnf_info_node *infos_head;
	struct cnf_image_node *images_head;
};

struct cnf_action {
	struct cnf_frame *frame;
	enum action_type type;
	uint8_t menu_id;
};

struct cnf_action_node {
	struct cnf_action action;
	struct cnf_action_node *next;
};

struct cnf_menu {
	struct gfx_mono_menu *menu;
	struct cnf_action_node *actions_head;
	uint8_t id;
	struct cnf_image_node *images_items_head;
};

struct cnf_menu_node {
	struct cnf_menu *menu;
	struct cnf_menu_node *next;
};

//struct cnf_font {
//	PROGMEM_BYTE_ARRAY_T source;
//	uint8_t width;
//	uint8_t height;
//	bool is_numeric_only;
//};

struct cnf_font_node {
	struct glcd_FontConfig_t font;
	uint8_t id;
	struct cnf_font_node *next;
};


struct cnf_blk {
	struct cnf_menu_node *menus_head;
	struct cnf_font_node *fonts_head;
	struct cnf_frame *dashboard;
	uint8_t font_size;
	uint8_t signature;
	uint8_t menu_size;
	uint8_t *splash;
	uint8_t splash_height;
	uint8_t splash_width;
	struct cnf_frame *clock;
};

#endif /* CONFIG_CNF_BLK_COMPONENTS_H_ */
