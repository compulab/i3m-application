/*
 * gfx_utils.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_UTILS_H_
#define GFX_GFX_UTILS_H_

void gfx_window_init(gfx_window * window, uint8_t x, uint8_t y, bool visible,bool borderVisible, uint8_t width,uint8_t height);
void gfx_window_draw(gfx_window * window);

#endif /* GFX_GFX_UTILS_H_ */
