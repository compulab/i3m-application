/*
 * gfx_utils.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_UTILS_H_
#define GFX_GFX_UTILS_H_

#include "config/cnf_blk_components.h"
#include "Fp-utils.h"
#include "gfx_components.h"

#define MAX_TEMPERATURE_LENGTH 10

struct gfx_frame *frame_present;

int gfx_frame_init(struct gfx_frame *frame, struct cnf_frame *cnf_frame_pgmem, bool is_dashboard);

void gfx_frame_draw(struct gfx_frame *frame, bool redraw);

void draw_graphic_signs(uint8_t selection, uint8_t min_index, uint8_t max_index, bool is_numeric);

#endif /* GFX_GFX_UTILS_H_ */
