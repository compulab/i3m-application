/*
 * graphic_menu.c
 *
 *  Created on: Oct 18, 2015
 *      Author: arkadi
 */

#include "graphic_menu.h"

void set_item_position(struct gfx_image *bitmap, uint8_t i, bool is_back_item)
{
	bitmap->postion.x = is_back_item ? 85 : lrint(i % 3) * 40 + 5;
	bitmap->postion.y = is_back_item ? 26 : lrint(i / 3) * 25 + 1;
}

#define CURSOR_WIDTH   4
#define CURSOR_HEIGHT   4


void invert_cursor_horizontal_lines(uint8_t frame_x, uint8_t frame_y)
{
	gfx_mono_generic_draw_horizontal_line(frame_x, frame_y, CURSOR_WIDTH, GFX_PIXEL_XOR);
	gfx_mono_generic_draw_horizontal_line(frame_x, frame_y + FRAME_HEIGHT, CURSOR_WIDTH, GFX_PIXEL_XOR);
	gfx_mono_generic_draw_horizontal_line(frame_x + FRAME_WIDTH - CURSOR_WIDTH, frame_y, CURSOR_WIDTH, GFX_PIXEL_XOR);
	gfx_mono_generic_draw_horizontal_line(frame_x + FRAME_WIDTH - CURSOR_WIDTH, frame_y + FRAME_HEIGHT, CURSOR_WIDTH, GFX_PIXEL_XOR);
}

void invert_cursor_vertical_lines(uint8_t frame_x, uint8_t frame_y)
{
	gfx_mono_generic_draw_vertical_line(frame_x, frame_y, CURSOR_HEIGHT, GFX_PIXEL_XOR);
	gfx_mono_generic_draw_vertical_line(frame_x + FRAME_WIDTH, frame_y, CURSOR_HEIGHT, GFX_PIXEL_XOR);
	gfx_mono_generic_draw_vertical_line(frame_x, frame_y + FRAME_HEIGHT - CURSOR_HEIGHT, CURSOR_HEIGHT, GFX_PIXEL_XOR);
	gfx_mono_generic_draw_vertical_line(frame_x + FRAME_WIDTH, frame_y + FRAME_HEIGHT - CURSOR_HEIGHT, CURSOR_HEIGHT, GFX_PIXEL_XOR);
}

void invert_item(uint8_t index, bool is_back_item)
{
	uint8_t frame_x = is_back_item ? 85 : lrint(index % 3) * 40 + 5;
	uint8_t frame_y = is_back_item ? 26 : lrint(index / 3) * 25 + 1;
	invert_cursor_horizontal_lines(frame_x, frame_y);
	invert_cursor_vertical_lines(frame_x, frame_y);
}

void draw_selected_item(char *title)
{
	uint8_t start_title = (GFX_MONO_LCD_WIDTH - strlen_P(title)) / 4;
	gfx_mono_draw_progmem_string(title, start_title, 54, &sysfont);
}

void graphic_menu_init(struct gfx_action_menu *action_menu, bool redraw)
{
	struct gfx_image *curr_image;
	struct gfx_mono_menu *menu = action_menu->visible_items.visible_menu;
	if (redraw){
		clear_screen();
		for (int i = 0; i < menu->num_elements; i++){
			curr_image = action_menu->visible_items.visible_images[i];
			if (curr_image == 0)
				break;
			set_item_position(curr_image, i, action_menu->id != 0 && i == menu->num_elements - 1);
			gfx_mono_generic_put_bitmap(curr_image->bitmap, curr_image->postion.x, curr_image->postion.y);
		}
	} else {
		invert_item(menu->last_selection, action_menu->id != 0 && menu->last_selection == menu->num_elements - 1);
		gfx_mono_draw_filled_rect(0, 54, 128, 10, GFX_PIXEL_CLR);
	}
	invert_item(menu->current_selection, action_menu->id != 0 && menu->current_selection == menu->num_elements - 1);
	draw_selected_item(menu->strings[menu->current_selection]);
}
