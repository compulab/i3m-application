/*
 * gfx_utils.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_UTILS_H_
#define GFX_GFX_UTILS_H_

#include <math.h>
#include <string.h>
#include "../debug.h"
#include "../asf.h"
#include "../config/cnf_blk_components.h"
#include "gfx_components.h"

void gfx_frame_init(gfx_frame * frame, cnf_frame * cnf_frame);

void gfx_frame_draw(gfx_frame * frame);

#endif /* GFX_GFX_UTILS_H_ */
