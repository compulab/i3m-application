#include "gfx_graphic_menu.h"
#include "gfx/gfx_gui_control.h"
#include "gfx_label.h"
#include "graphic_menu_theme/graphic_menu_theme.h"
#include "display/display_render.h"
#include "lib/syntax.h"

static void update_action_visibility(struct gfx_graphic_menu_action *action)
{
	if (action->type != ACTION_TYPE_SHOW_FRAME) {
		action->visible = true;
		return;
	}

	bool visible = true;
	list_foreach(struct gfx_information_node *, action->frame->information_head, info_node) {
		visible = info_node->information.is_valid(&info_node->information);
		if (!visible)
			break;
	}

	action->visible = visible;
}

static void gfx_menu_handle_button(struct gfx_graphic_menu *graphic_menu, uint8_t keycode)
{
	if (keycode != GFX_MONO_MENU_KEYCODE_ENTER) {
		gfx_mono_menu_process_key(graphic_menu->menu, keycode, graphic_menu->is_progmem);
		gfx_graphic_menu_move_cursor(current_menu);
		return;
	}

	struct gfx_graphic_menu_action *selected_action = &graphic_menu->actions[(graphic_menu->menu)->current_selection];
	if (selected_action->type == ACTION_TYPE_SHOW_FRAME)
		gfx_switch_to_frame(selected_action->frame);
	else if (selected_action->type == ACTION_TYPE_SHOW_MENU)
		gfx_switch_to_menu(selected_action->menu);
}

void gfx_graphic_menu_move_cursor(struct gfx_graphic_menu *graphic_menu)
{
	graphic_menu->draw(graphic_menu);
	graphic_menu_deselect_item(graphic_menu, graphic_menu->menu->last_selection);
	graphic_menu_select_item(graphic_menu, graphic_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
}

static void gfx_graphic_menu_display(struct gfx_graphic_menu *graphic_menu)
{
	for (int i = 0; i < graphic_menu->menu->num_elements; i++)
		update_action_visibility(&graphic_menu->actions[i]);
	clear_screen();
	graphic_menu_format(graphic_menu);
	graphic_menu_select_item(graphic_menu, graphic_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
}

void gfx_graphic_menu_init(struct gfx_graphic_menu *graphic_menu, uint8_t id, bool is_progmem,
						  struct gfx_mono_menu *menu, struct gfx_graphic_menu_action *actions,
						  struct gfx_image_node *graphic_items_head)
{
	graphic_menu->id = id;
	graphic_menu->is_progmem = is_progmem;
	graphic_menu->menu = menu;
	graphic_menu->actions = actions;
	graphic_menu->graphic_items_head = graphic_items_head;
	graphic_menu->draw = gfx_graphic_menu_display;
	graphic_menu->handle_button = gfx_menu_handle_button;
}