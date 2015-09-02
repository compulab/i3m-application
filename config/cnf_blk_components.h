/*
 * cnf_blk_header.h
 *
 *  Created on: Aug 16, 2015
 *      Author: arkadi
 */

#ifndef CONFIG_CNF_BLK_COMPONENTS_H_
#define CONFIG_CNF_BLK_COMPONENTS_H_

#include <stdbool.h>
#include <avr/pgmspace.h>
#include "../Fp-utils.h"
typedef uint8_t gfx_mono_color_t;

#define MAX_MENUS 15
#define MAX_INFORMATION 3
#define MAX_LABEL 7
#define MAX_IMAGE 4
#define MAX_ACTIONS 6
#define MAX_TITLES 6

struct cnf_menu_titles {
	char * titles[MAX_TITLES];
};

struct cnf_blk  {
	struct cnf_menu *menus[MAX_MENUS];
	uint8_t signature;
	uint8_t size;
	uint32_t reserved[24];
};


struct cnf_label{
	char *text;
	uint8_t x, y;
	bool border_visible;
};

struct cnf_image{
	gfx_mono_color_t *bitmapProgmem;
	uint8_t height, width;
	uint8_t x, y;
	bool border_visible;
};

struct cnf_info{
	enum information_type info_type;
	uint8_t information;
	uint8_t x, y;
	bool border_visible;
};

struct cnf_frame{
	struct cnf_info *informations[MAX_INFORMATION];
	struct cnf_label *labels[MAX_LABEL];
	struct cnf_image *images[MAX_IMAGE];
	uint8_t information_size,label_size,image_size;
};

struct cnf_action{
	struct cnf_frame *frame;
	enum action_type type;
	uint8_t menuId;
};

struct cnf_menu{
	struct gfx_mono_menu *menu;
	struct cnf_action *actions[MAX_ACTIONS];
};

#endif /* CONFIG_CNF_BLK_COMPONENTS_H_ */
