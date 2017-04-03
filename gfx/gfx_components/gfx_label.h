/*
 * gfx_label.h
 *
 * Created: 4/2/2017 5:40:19 PM
 *  Author: Nikita
 */


#ifndef GFX_LABEL_H_
#define GFX_LABEL_H_

#include "gfx_item.h"
#include "gfx_text.h"

struct gfx_label {
	struct gfx_item postion;
	struct gfx_text text;
	void (*draw)(struct gfx_label *label);
};

struct gfx_label_node {
	struct gfx_label label;
	struct gfx_label_node *next;
};

void gfx_label_init(struct gfx_label *label, char *text, uint8_t x, uint8_t y, uint8_t font_id);

#endif /* GFX_LABEL_H_ */