/*
 * graphic_menu.c
 *
 *  Created on: Oct 18, 2015
 *      Author: arkadi
 */

#include "graphic_menu.h"

uint8_t x_pos[] = {5, 45, 85, 5, 45, 85},
		y_pos[] = {1, 1, 1, 25, 25, 25};

void set_item_position(struct gfx_image *bitmap, uint8_t i)
{
	bitmap->postion.x = lrint(i % 3) * 40 + 5;
	bitmap->postion.y = lrint(i / 3) * 25 + 1;
}

void invert_item(uint8_t index)
{
	uint8_t frame_x = lrint(index % 3) * 40 + 5;
	uint8_t frame_y = lrint(index / 3) * 25 + 1;
	for (uint8_t x = frame_x; x < frame_x + FRAME_WIDTH; x++)
		for (uint8_t y = frame_y; y < frame_y + FRAME_HEIGHT; y++)
			gfx_mono_ssd1306_draw_pixel(x, y, GFX_PIXEL_XOR);
}

void draw_selected_item(char *title)
{
	uint8_t start_title = (GFX_MONO_LCD_WIDTH - strlen_P(title)) / 4;
	gfx_mono_draw_progmem_string(title, start_title, 54, &sysfont);
}


void graphic_menu_init(struct gfx_action_menu *action_menu, bool redraw)
{
	struct gfx_image_node *curr_node = action_menu->graphic_items_head;
	struct gfx_mono_menu *menu = action_menu->menu;
	if (redraw){
		for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
			framebuffer[i] = 0x00;
		for (int i = 0; i < action_menu->menu->num_elements; i++){
			if (curr_node == 0)
				break;
			set_item_position(&curr_node->image, i);
			gfx_mono_generic_put_bitmap(curr_node->image.bitmap, curr_node->image.postion.x, curr_node->image.postion.y);
			curr_node = curr_node->next;
		}
	} else {
		invert_item(menu->last_selection);
		gfx_mono_draw_filled_rect(0, 54, 128, 10, GFX_PIXEL_CLR);
	}
	invert_item(menu->current_selection);
	draw_selected_item(menu->strings[menu->current_selection]);
	gfx_mono_ssd1306_put_framebuffer();
}
