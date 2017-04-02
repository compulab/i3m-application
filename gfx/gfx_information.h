/*
 * gfx_information.h
 *
 * Created: 3/27/2017 3:52:33 PM
 *  Author: Nikita
 */


#ifndef GFX_INFORMATION_H_
#define GFX_INFORMATION_H_

#include "gfx_components.h"
#include "config/cnf_blk_components.h"

struct gfx_information {
	struct gfx_item postion;
	enum information_type info_type;
	uint8_t last_length;
	//TODO: The following is a really bad variable that at different times
	//holds anything from hardware enumeration index, to mac address index, to
	//other types of identifiers. This should be refactored the hell out of
	//here.
	uint8_t info_data;
	void (*to_string)(struct gfx_information *info, char *output_str);
	void (*draw_data)(struct gfx_information *info);
	struct gfx_text text;
};

struct gfx_information_node {
	struct gfx_information information;
	struct gfx_information_node *next;
};

int gfx_information_init(struct gfx_information *info, struct cnf_info *cnf_info, uint8_t font_id);
void gfx_information_draw_string(struct gfx_information *info);

#endif /* GFX_INFORMATION_H_ */
