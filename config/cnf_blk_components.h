/*
 * cnf_blk_header.h
 *
 *  Created on: Aug 16, 2015
 *      Author: arkadi
 */

#ifndef CONFIG_CNF_BLK_COMPONENTS_H_
#define CONFIG_CNF_BLK_COMPONENTS_H_
#include "../asf.h"
#include "../gfx/action_menu/gfx_action_menu.h"
#include "../gfx/gfx_components.h"
#include "../config/cnf_blk_components.h"

#define MAX_MENUS 15
#define MAX_INFORMATION 3
#define MAX_LABEL 7
#define MAX_IMAGE 4
#define MAX_ACTIONS 6

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
	gfx_mono_color_t PROGMEM_T *bitmapProgmem;
	uint8_t height, width;
	uint8_t x, y;
	bool border_visible;
};

struct cnf_info{
	enum information_type info_type;
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
	bool is_frame;
	uint8_t menuId;
};

struct cnf_menu{
	struct gfx_mono_menu *menu;
	struct cnf_action *actions[MAX_ACTIONS];
};

#endif /* CONFIG_CNF_BLK_COMPONENTS_H_ */
