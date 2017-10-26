/*
 * graphic_menu.c
 *
 *  Created on: Oct 18, 2015
 *      Author: arkadi
 */

#include "graphic_menu_theme.h"
#include "gfx/action_menu/gfx_item_action.h"
#include "gfx/gfx_components/gfx_image.h"
#include "gfx/gfx_utils.h"
#include "display/display_render.h"
#include "lib/syntax.h"
#include <math.h>

#define FRAME_WIDTH	38
#define FRAME_HEIGHT 22
#define MAX_ITEM_INDEX	5

static void set_icon_position(struct gfx_image *bitmap, uint8_t icon_index)
{
	bitmap->postion.x = lrint(icon_index % 3) * 40 + 5;
	bitmap->postion.y = lrint(icon_index / 3) * 25 + 1;
	bitmap->postion.height = FRAME_HEIGHT;
	bitmap->postion.width = FRAME_WIDTH;
}

static void set_clr_cursor_horizontal_lines(uint8_t frame_x, uint8_t frame_y, enum gfx_mono_color color)
{
	gfx_coord_t cursor_width = 5;
	gfx_mono_generic_draw_horizontal_line(frame_x + 1, frame_y, cursor_width - 1, color);
	gfx_mono_generic_draw_horizontal_line(frame_x, frame_y + FRAME_HEIGHT, cursor_width, color);
	gfx_mono_generic_draw_horizontal_line(frame_x + FRAME_WIDTH - cursor_width, frame_y, cursor_width, color);
	gfx_mono_generic_draw_horizontal_line(frame_x + FRAME_WIDTH - cursor_width, frame_y + FRAME_HEIGHT, cursor_width, color);
}

static void set_clr_cursor_vertical_lines(uint8_t frame_x, uint8_t frame_y, enum gfx_mono_color color)
{
	gfx_coord_t cursor_height = 5;
	gfx_mono_generic_draw_vertical_line(frame_x, frame_y, cursor_height, color);
	gfx_mono_generic_draw_vertical_line(frame_x + FRAME_WIDTH, frame_y, cursor_height, color);
	gfx_mono_generic_draw_vertical_line(frame_x, frame_y + FRAME_HEIGHT - cursor_height, cursor_height, color);
	gfx_mono_generic_draw_vertical_line(frame_x + FRAME_WIDTH, frame_y + FRAME_HEIGHT - cursor_height + 1, cursor_height, color);
}

static void draw_selection_title(char *title)
{
	uint8_t title_pos_x = (GFX_MONO_LCD_WIDTH - strlen_PF((uint16_t)title + 0x10000)) / 4;
	gfx_mono_draw_progmem_string(title, title_pos_x, 54, &sysfont);
}

static void select_deselect_item(uint8_t index, enum gfx_mono_color color)
{
	uint8_t frame_x = lrint(index % 3) * 40 + 5;
	uint8_t frame_y = lrint(index / 3) * 25 + 1;
	set_clr_cursor_horizontal_lines(frame_x, frame_y, color);
	set_clr_cursor_vertical_lines(frame_x, frame_y, color);
}

static void draw_disabled_item(struct gfx_item *pos)
{
	for (int y = pos->y + 1; y < pos->y + pos->height; y += 2)
		gfx_mono_draw_horizontal_line(pos->x, y, pos->width, GFX_PIXEL_CLR);
}

void graphic_menu_format(struct gfx_graphic_menu *graphic_menu)
{
	struct gfx_mono_menu *menu = graphic_menu->menu;
	uint8_t i = 0;
	clear_screen();
	list_foreach(struct gfx_image_node *, graphic_menu->graphic_items_head, curr_image) {
		uint8_t item_index = (graphic_menu->id && i == menu->num_elements - 1) ? MAX_ITEM_INDEX : i;
		set_icon_position(&curr_image->image, item_index);
		gfx_mono_generic_put_bitmap(curr_image->image.bitmap, curr_image->image.postion.x, curr_image->image.postion.y);
		if (!graphic_menu->actions[i].visible)
			draw_disabled_item(&curr_image->image.postion);
		i++;
	}
	draw_standard_separator_line();
}

void graphic_menu_deselect_item(struct gfx_graphic_menu *graphic_menu, uint8_t selection)
{
	struct gfx_mono_menu *menu = graphic_menu->menu;
	uint8_t item_index = (graphic_menu->id && selection == menu->num_elements - 1) ? MAX_ITEM_INDEX : selection;
	select_deselect_item(item_index, GFX_PIXEL_CLR);
	gfx_mono_draw_filled_rect(0, 54, 128, 10, GFX_PIXEL_CLR);
}

void graphic_menu_select_item(struct gfx_graphic_menu *graphic_menu, uint8_t selection)
{
	struct gfx_mono_menu *menu = graphic_menu->menu;
	uint8_t item_index = (graphic_menu->id && selection == menu->num_elements - 1) ? MAX_ITEM_INDEX : selection;
	select_deselect_item(item_index, GFX_PIXEL_SET);
	draw_selection_title(menu->strings[selection]);
}