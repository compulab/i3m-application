/*
 * gfx_frame.h
 *
 * Created: 3/27/2017 6:18:04 PM
 *  Author: Nikita
 *
 * FRAME SUMMARY:
 * A frame is a collection of images, labels, and information artifacts.
 * A frame has a default behavior when responding to side buttons, which is to
 * scroll to the next frame in the current menu. It will also draw the appropriate
 * controls to match this behavior.
 * The default behavior of the frame can be overridden by the information artifact.
 * The information artifact can also override draw_controls accordingly.
 */

#ifndef GFX_FRAME_H_
#define GFX_FRAME_H_

#include "gfx_components/gfx_text.h"
#include "gfx_components/gfx_image.h"
#include "gfx_components/gfx_label.h"
#include "gfx_components/gfx_information.h"

struct gfx_frame {
	void (*handle_buttons)(uint8_t);
	void (*draw_controls)(struct gfx_frame *info);
	void (*draw)(struct gfx_frame *frame);
	struct gfx_image_node *image_head;
	struct gfx_label_node *label_head;
	struct gfx_information_node *information_head;
};

int gfx_frame_init(struct gfx_frame *frame, struct cnf_frame *cnf_frame_pgmem, bool is_dashboard);

#endif /* GFX_FRAME_H_ */