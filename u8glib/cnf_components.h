/*
 * cnf_components.h
 *
 *  Created on: Oct 7, 2015
 *      Author: arkadi
 */

#ifndef U8GLIB_CNF_COMPONENTS_H_
#define U8GLIB_CNF_COMPONENTS_H_

#include <avr/pgmspace.h>
#include "../Fp-utils.h"
#include "u8g.h"


struct cnf_label{
	u8g_pgm_uint8_t *text;
	uint8_t x, y;
};

struct cnf_image{
	u8g_pgm_uint8_t *bitmapProgmem;
	uint8_t height, width;
	uint8_t x, y;
};

struct cnf_info{
	enum information_type info_type;
	uint8_t information;
	uint8_t max_length;
	uint8_t x, y;
};

struct cnf_label_node{
	struct cnf_label label;
	struct cnf_label_node *next;
};

struct cnf_image_node{
	struct cnf_image image;
	struct cnf_image_node *next;
};

struct cnf_info_node{
	struct cnf_info info;
	struct cnf_info_node *next;
};

struct cnf_frame{
	struct cnf_label_node *labels_head;
	struct cnf_info_node *infos_head;
	struct cnf_image_node *images_head;
};

struct cnf_action{
	struct cnf_frame *frame;
	enum action_type type;
	uint8_t menu_id;
};

struct cnf_menu_item_node {
	u8g_pgm_uint8_t *text;
	struct cnf_action *action;
	struct cnf_menu_item_node *next;
};

struct cnf_menu {
	u8g_pgm_uint8_t *title;
	struct cnf_menu_item_node *first_item;
	uint8_t id;
};

struct cnf_menu_node{
	struct cnf_menu *menu;
	struct cnf_menu_node *next;
};

struct cnf_blk  {
	struct cnf_menu_node *menus_head;
	uint8_t signature;
	uint8_t size;
	u8g_pgm_uint8_t *splash;
	uint8_t splash_height;
	uint8_t splash_width;
};


#endif /* U8GLIB_CNF_COMPONENTS_H_ */
