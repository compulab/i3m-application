/*
 * gfx_utils.h
 *
 *  Created on: Aug 6, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_UTILS_H_
#define GFX_GFX_UTILS_H_

void gfx_item_init(gfx_item * item, uint8_t x, uint8_t y, bool borderVisible, uint8_t width,uint8_t height);
void gfx_item_draw(gfx_item * item);
void gfx_label_with_font_init(gfx_label * label,char * text, struct font * font, uint8_t x, uint8_t y, bool borderVisible);
void gfx_information_label_draw(gfx_information_label * info_label);
void gfx_information_label_init(gfx_information_label * label,information_type info_type, uint8_t x, uint8_t y, bool borderVisible);
void gfx_label_init(gfx_label * label,char * text, uint8_t x, uint8_t y, bool borderVisible);
void gfx_label_draw(gfx_label * label);
void gfx_image_init(gfx_image * image, struct gfx_mono_bitmap * bitmap, uint8_t x, uint8_t y, bool borderVisible);
void gfx_image_draw(gfx_image * image);
#endif /* GFX_GFX_UTILS_H_ */
