/*
 * gfx_utils.c
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#include "gfx_components.h"
#include "../asf.h"
#include <math.h>

#define PAGE_ADDRESS(y) floor (y/8)

void printHorizontalLine(uint8_t x, uint8_t y, uint8_t length){
	gfx_mono_draw_line(x,y,x+length,y,GFX_PIXEL_SET);
}

void printVerticalLine(uint8_t x, uint8_t y, uint8_t length){
	gfx_mono_draw_line(x,y,x,y+length,GFX_PIXEL_SET);
}

void gfx_window_draw(gfx_window * window){
	if (window->visible && window->borderVisible){
		printHorizontalLine(window->x,window->y,window->width);
		printVerticalLine(window->x,window->y,window->height);
		printHorizontalLine(window->x,window->y+window->height,window->width);
		printVerticalLine(window->x+window->width,window->y,window->height);
	}
}

void gfx_window_init(gfx_window * window, uint8_t x, uint8_t y, bool visible,bool borderVisible, uint8_t width,uint8_t height){
	window->x = x;
	window->y = y;
	window->width = width;
	window->height = height;
	window->visible = visible;
	window->borderVisible = borderVisible;
}



