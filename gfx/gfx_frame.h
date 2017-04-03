/*
 * gfx_frame.h
 *
 * Created: 3/27/2017 6:18:04 PM
 *  Author: Nikita
 */


#ifndef GFX_FRAME_H_
#define GFX_FRAME_H_

#include "gfx_text.h"
#include "gfx_information.h"

struct gfx_frame {
	void (*handle_buttons)(uint8_t);
	void (*draw)(struct gfx_frame *frame, bool redraw);
	struct gfx_image_node *image_head;
	struct gfx_label_node *label_head;
	struct gfx_information_node *information_head;
};

#endif /* GFX_FRAME_H_ */