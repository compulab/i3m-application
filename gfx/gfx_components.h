/*
 * gfx_components.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_COMPONENTS_H_
#define GFX_GFX_COMPONENTS_H_
#include <stdbool.h>
#include <stdint.h>

typedef struct GFX_WINDOW {
	uint8_t x,y;
	uint8_t width,height;
	bool visible;
	bool borderVisible;
	void (*draw)(struct GFX_WINDOW *);
} gfx_window ;

typedef struct GFX_ITEM {
	gfx_window window;
	void (*draw)(struct GFX_ITEM *);
} gfx_item;

typedef struct GFX_TEXT {
	char * text;
	struct font * font;
} gfx_text;

typedef struct GFX_LABEL {
	gfx_item labelPos;
	gfx_text labelText;
} gfx_label;




#endif /* GFX_GFX_COMPONENTS_H_ */
