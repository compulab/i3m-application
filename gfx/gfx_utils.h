/*
 * gfx_utils.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_UTILS_H_
#define GFX_GFX_UTILS_H_

#ifndef CONFIG_CNF_BLK_COMPONENTS_H_
#include "config/cnf_blk_components.h"
#endif

#ifndef FP_UTILS_H_
#include "Fp-utils.h"
#endif

#ifndef GFX_GFX_COMPONENTS_H_
#include "gfx_components.h"
#endif

#define MAX_TEMPERATURE_LENGTH 10

struct gfx_frame *frame_present;

int gfx_frame_init(struct gfx_frame *frame, struct cnf_frame *cnf_frame_pgmem, bool is_dashboard);

void gfx_frame_draw(struct gfx_frame *frame, bool redraw);

#endif /* GFX_GFX_UTILS_H_ */
