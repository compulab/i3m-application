#include "gfx_action_menu.h"
#include "../../debug.h"
#include "../../twi/eeprom.h"
#include "../menu-handler.h"

void clear_menu()
{
	for (int i=0 ; i < GFX_MONO_LCD_FRAMEBUFFER_SIZE; i++)
		framebuffer[i] = 0x00;
	gfx_mono_ssd1306_put_framebuffer();

}

void gfx_action_menu_init(struct gfx_action_menu *action_menu)
{
	present_menu->visible = false;
	present_menu = action_menu;
	action_menu->visible = true;
	clear_menu();
	gfx_mono_menu_init(action_menu->menu, action_menu->is_progmem);
}

void show_menu(struct gfx_action_menu *menu)
{
	gfx_action_menu_init(menu);
}

void show_current_menu()
{
	gfx_action_menu_init(present_menu);
}

struct gfx_action_menu *menu;
struct gfx_action_menu dmi_menu = {.is_progmem = false };


void clear_screen(struct gfx_action_menu *actionMenu)
{
	clear_menu();
	actionMenu->visible = false;
}

void set_dmi_mono_menu()
{
	uint8_t count = 0;
	struct direct_string_item * direct_item = computer_data.direct_string;
	while (direct_item != 0){
		count++;
		direct_item = direct_item->next;
	}
	if (count > 0){
		dmi_menu.menu = malloc(sizeof(struct gfx_mono_menu));
		dmi_menu.menu->num_elements = count;
		dmi_menu.menu->title = "DMI STRINGS";
		dmi_menu.menu->current_selection = 0;
		direct_item = computer_data.direct_string;
		for (uint8_t i = 0; i < count  && i < 5; i++){
			if (direct_item == 0)
				break;
			dmi_menu.menu->strings[i] = strdup(direct_item->type);
			direct_item = direct_item->next;
		}
		if (dmi_menu.menu->num_elements > 4){
			dmi_menu.menu->strings[5] = "Back To Main Menu";
			dmi_menu.menu->num_elements = 6;
		} else {
			dmi_menu.menu->strings[count] = "Back To Main Menu";
			dmi_menu.menu->num_elements++;
		}
	} else {
		free(dmi_menu.menu);
		dmi_menu.menu = 0;
	}
}

struct gfx_item default_position = { .x =0, .y = 32, .height = 8, .visible = true };

void set_dmi_information(struct gfx_information_node *info_node, uint8_t i)
{
	info_node->information.info_data = i;
	info_node->information.info_type = SHOW_DMI_CONTENT;
	info_node->information.postion = default_position;
	info_node->information.text.font = &sysfont;
	info_node->information.text.text = malloc(sizeof(char *) * 8);
}

void set_dmi_frame(struct gfx_frame *frame, uint8_t i)
{
	frame->information_head =  malloc(sizeof(struct gfx_information_node));
	frame->image_head = 0;
	frame->label_head = 0;
	set_dmi_information(frame->information_head, i);
}


void set_dmi_actions()
{
	dmi_menu.actions = malloc(sizeof(struct gfx_item_action) * dmi_menu.menu->num_elements);
	for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
		dmi_menu.actions[i].type =  ACTION_TYPE_SHOW_FRAME;
		dmi_menu.actions[i].frame = malloc(sizeof(struct gfx_frame));
		set_dmi_frame(dmi_menu.actions[i].frame, i);
	}
	dmi_menu.actions[dmi_menu.menu->num_elements -1].type = ACTION_TYPE_SHOW_MENU;
	dmi_menu.actions[dmi_menu.menu->num_elements -1].menu = action_menus[0];
}

void set_dmi_menu()
{
	set_dmi_mono_menu();
	set_dmi_actions();
}

uint8_t gfx_action_menu_process_key(struct gfx_action_menu *action_menu, uint8_t keycode)
{
	if (keycode == GFX_MONO_MENU_KEYCODE_ENTER){
		struct gfx_item_action selected_action = action_menu->actions[(action_menu->menu)->current_selection];
		enum action_type type = selected_action.type;
		clear_screen(action_menu);
		switch (type){
		case ACTION_TYPE_SHOW_FRAME:
			gfx_frame_draw(selected_action.frame);
			break;
		case ACTION_TYPE_SHOW_MENU:
			menu = selected_action.menu;
			show_menu(menu);
			break;
		case ACTION_TYPE_SHOW_DMI_MENU:
			set_dmi_menu();
			if (dmi_menu.menu != 0)
				show_menu(&dmi_menu);
			break;
//		case ACTION_TYPE_SET_BIOS_STATE:
//			eeprom_write_byte(BIOS_STATE_ADDRESS,selectedAction.frame->informations[0]->info_data);
//			break;
		default:
			break;
		}
	} else if (keycode == GFX_MONO_MENU_KEYCODE_BACK) {
		if (!present_menu->visible)
			show_current_menu();
	}else {
		return gfx_mono_menu_process_key(action_menu->menu, keycode, action_menu->is_progmem);
	}
	return 0;
}
