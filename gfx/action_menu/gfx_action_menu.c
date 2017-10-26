#include "gfx_action_menu.h"
#include "gfx_item_action.h"
#include "gfx/gfx_gui_control.h"
#include "gfx/gfx_components/gfx_label.h"
#include "gfx/action_menu/graphic_menu_theme/graphic_menu_theme.h"
#include "lib/syntax.h"

static void update_action_visibility(struct gfx_item_action *action)
{
	if (action->type == ACTION_TYPE_SHOW_DMI_MENU) {
		action->visible = computer_data.details.direct_string != 0;
		return;
	} else if (action->type != ACTION_TYPE_SHOW_FRAME) {
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

static void gfx_menu_handle_button(struct gfx_action_menu *action_menu, uint8_t keycode)
{
	if (keycode != GFX_MONO_MENU_KEYCODE_ENTER) {
		gfx_mono_menu_process_key(action_menu->menu, keycode, action_menu->is_progmem);
		gfx_action_menu_move_cursor(current_menu);
		return;
	}

	struct gfx_item_action *selected_action = &action_menu->actions[(action_menu->menu)->current_selection];
	if (selected_action->type == ACTION_TYPE_SHOW_FRAME)
		gfx_switch_to_frame(selected_action->frame);
	else if (selected_action->type == ACTION_TYPE_SHOW_MENU)
		gfx_switch_to_menu(selected_action->menu);
}

void gfx_action_menu_move_cursor(struct gfx_action_menu *action_menu)
{
	action_menu->draw(action_menu);
	graphic_menu_deselect_item(action_menu, action_menu->menu->last_selection);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
}

static void gfx_action_menu_display(struct gfx_action_menu *action_menu)
{
	for (int i = 0; i < action_menu->menu->num_elements; i++)
		update_action_visibility(&action_menu->actions[i]);
	clear_screen();
	graphic_menu_format(action_menu);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
}

void gfx_action_menu_init(struct gfx_action_menu *action_menu, uint8_t id, bool is_progmem,
						  struct gfx_mono_menu *menu, struct gfx_item_action *actions,
						  struct gfx_image_node *graphic_items_head)
{
	action_menu->id = id;
	action_menu->is_progmem = is_progmem;
	action_menu->menu = menu;
	action_menu->actions = actions;
	action_menu->graphic_items_head = graphic_items_head;
	action_menu->draw = gfx_action_menu_display;
	action_menu->handle_button = gfx_menu_handle_button;
}