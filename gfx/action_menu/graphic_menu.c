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
	bitmap->postion.height = FRAME_HEIGHT;
	bitmap->postion.width = FRAME_WIDTH;
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

void draw_disable_item(struct gfx_item *pos)
{
//	gfx_mono_draw_horizontal_line(pos->x, pos->y + (pos->height / 2), pos->width, GFX_PIXEL_SET);
	gfx_mono_generic_draw_line(pos->x, pos->y, pos->x + pos->width, pos->y + pos->height, GFX_PIXEL_SET);
	gfx_mono_generic_draw_line(pos->x, pos->y + pos->height, pos->x + pos->width, pos->y, GFX_PIXEL_SET);
}

void graphic_menu_init(struct gfx_action_menu *action_menu, bool redraw)
{
	struct gfx_image_node *curr_image = action_menu->graphic_items_head;
	struct gfx_mono_menu *menu = action_menu->menu;
	if (redraw){
		uint8_t i = 0;
		clear_screen();
		while (curr_image != NULL) {
			set_item_position(&curr_image->image, i, action_menu->id != 0 && i == menu->num_elements - 1);
			gfx_mono_generic_put_bitmap(curr_image->image.bitmap, curr_image->image.postion.x, curr_image->image.postion.y);
			if (!action_menu->actions[i].visible)
				draw_disable_item(&curr_image->image.postion);
			curr_image = curr_image->next;
			i++;
		}
	} else {
		invert_item(menu->last_selection, action_menu->id != 0 && menu->last_selection == menu->num_elements - 1);
		gfx_mono_draw_filled_rect(0, 54, 128, 10, GFX_PIXEL_CLR);
	}
	invert_item(menu->current_selection, action_menu->id != 0 && menu->current_selection == menu->num_elements - 1);
	draw_selected_item(menu->strings[menu->current_selection]);
}
