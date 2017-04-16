/*
 * gfx_information.h
 *
 * Created: 3/27/2017 3:52:33 PM
 *  Author: Nikita
 */


#ifndef GFX_INFORMATION_H_
#define GFX_INFORMATION_H_

#include "gfx_text.h"
#include "gfx_item.h"
#include "config/cnf_blk_components.h"

struct gfx_information {
	struct gfx_item postion;
	struct gfx_text text;
	enum information_type info_type;
	uint8_t metadata;	//Can be used for indexing, flagging, etc.

	//to_string: sprintf the formatted data value into output_str
	void (*to_string)(struct gfx_information *info, char *output_str);
	//draw_data: display formatted data on the screen
	void (*draw_data)(struct gfx_information *info);
	//draw_controls: display control symbols for left/right buttons
	void (*draw_controls)(struct gfx_information *info);
	void (*handle_buttons)(uint8_t key);
	bool (*is_valid)(struct gfx_information *info);
};

struct gfx_information_node {
	struct gfx_information information;
	struct gfx_information_node *next;
};

int gfx_information_init(struct gfx_information *info, struct cnf_info *cnf_info, uint8_t font_id);

#endif /* GFX_INFORMATION_H_ */
