/*
 * gfx_utils.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_UTILS_H_
#define GFX_GFX_UTILS_H_

#include "config/cnf_blk_components.h"
#include "gfx_frame.h"

struct gfx_frame *frame_present;

void draw_graphic_signs(uint8_t selection, uint8_t min_index, uint8_t max_index, bool is_numeric);
void draw_control_signs_numeric(uint8_t selection, uint8_t min_index, uint8_t max_index);
void draw_control_signs_arrows(uint8_t selection, uint8_t min_index, uint8_t max_index);

#endif /* GFX_GFX_UTILS_H_ */
