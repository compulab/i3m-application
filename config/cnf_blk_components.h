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
#define MAX_INFORMATION_LABEL 3
#define MAX_LABEL 7
#define MAX_IMAGE 4
#define MAX_ACTIONS 6

struct cnf_blk  {
	uint8_t signature;
	uint8_t size;
	struct CNF_MENU_T * menus[MAX_MENUS];
	uint32_t reserved[24];
};


typedef struct CNF_LABEL_T {
	char * text;
	uint8_t x,y;
	bool borderVisible;
} cnf_gfx_label;

typedef struct CNF_IMAGE_T {
	gfx_mono_color_t PROGMEM_T * bitmapProgmem;
	uint8_t height,width;
	uint8_t x,y;
	bool borderVisible;
}cnf_gfx_image ;

typedef struct CNF_INFORMATION_LABEL_T {
	information_type info_type;
	uint8_t x, y;
	bool borderVisible;
} cnf_gfx_information_label;

typedef struct CNF_FRAME_T {
	cnf_gfx_information_label * information_label[MAX_INFORMATION_LABEL];
	cnf_gfx_label * label[MAX_LABEL];
	cnf_gfx_image * image[MAX_IMAGE];
	uint8_t information_size,label_size,image_size;
}cnf_frame;

typedef struct CNF_ACTION_T {
	bool isFrame;
	cnf_frame frame;
	uint8_t menuId;
} cnf_action;

typedef struct CNF_MENU_T {
	struct gfx_mono_menu * menu;
	cnf_action * actions[MAX_ACTIONS];
} cnf_menu;

#endif /* CONFIG_CNF_BLK_COMPONENTS_H_ */
