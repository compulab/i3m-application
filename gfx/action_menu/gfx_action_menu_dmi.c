/*
 * gfx_action_menu_dmi.c
 *
 * Created: 11-Apr-17 8:51:32 PM
 *  Author: Nikita
 */

#include "gfx_action_menu.h"
#include "gfx/action_menu/gfx_item_action.h"
#include "gfx/gfx_components/gfx_label.h"
#include "lib/syntax.h"

struct gfx_action_menu dmi_menu = { .is_progmem = false };

bool is_dmi_set;

static void free_dmi_frame(struct gfx_frame *frame)
{
	struct gfx_label_node *curr_label = frame->label_head;
	struct gfx_label_node *next_label;
	while (curr_label) {
		next_label = curr_label->next;
		cli();
		free(curr_label);
		sei();
		curr_label = next_label;
	}
}

static void free_dmi_menu(void)
{
	if (dmi_menu.actions != NULL) {
		for (int i = 0; i < dmi_menu.menu->num_elements -1; i++) {
			if (dmi_menu.actions[i].type == ACTION_TYPE_SHOW_FRAME) {
				if (dmi_menu.actions[i].frame != NULL) {
					free_dmi_frame(dmi_menu.actions[i].frame);
					cli();
					free(dmi_menu.actions[i].frame);
					sei();
				} else {
					break;
				}
			}
		}
	}
	cli();
	free(dmi_menu.menu );
	sei();
	is_dmi_set = false;
}

static void set_dmi_mono_menu(void)
{
	is_dmi_set = false;
	uint8_t count = 0;
	struct direct_string_item * direct_item = computer_data.details.direct_string;
	while (direct_item != 0){
		count++;
		direct_item = direct_item->next;
	}

	if (count > 0){
		dmi_menu.menu = malloc_locked(sizeof(struct gfx_mono_menu));
		if (dmi_menu.menu == NULL) {
			free_dmi_menu();
			return ;
		}
		dmi_menu.menu->num_elements = count;
		dmi_menu.menu->title = "DMI STRINGS";
		dmi_menu.menu->current_selection = 0;
		direct_item = computer_data.details.direct_string;
		for (uint8_t i = 0; i < count  && i < 5; i++){
			if (direct_item == 0)
				break;
			dmi_menu.menu->strings[i] = direct_item->type;
			direct_item = direct_item->next;
		}
		if (dmi_menu.menu->num_elements > 3){
			dmi_menu.menu->strings[4] = "Back To Main Menu";
			dmi_menu.menu->num_elements = 5;
		} else {
			dmi_menu.menu->strings[count] = "Back To Main Menu";
			dmi_menu.menu->num_elements++;
		}
		is_dmi_set = true;
	} else {
		cli();
		free(dmi_menu.menu);
		sei();
		dmi_menu.menu = 0;
	}
}

static void set_dmi_name_position(struct gfx_item* pos)
{
	pos->x = 0;
	pos->y = 12;
	pos->height = 8;
	pos->width = 0;
	pos->visible = true;
}

static void set_dmi_content_position(struct gfx_item* pos)
{
	pos->x = 0;
	pos->y = 32;
	pos->height = 8;
	pos->width = 0;
	pos->visible = true;
}

static void set_dmi_name_label(struct gfx_label_node **label_node, struct direct_string_item * direct_item)
{
	set_dmi_name_position(&(*label_node)->label.postion);
	(*label_node)->label.text.font = get_font_by_type(GLCD_FONT_SYSFONT_5X7);
	(*label_node)->label.text.is_progmem = false;
	(*label_node)->label.text.text = direct_item->content;
}

static void set_dmi_content_label(struct gfx_label_node *label_node, struct direct_string_item * direct_item)
{
	set_dmi_content_position(&label_node->label.postion);
	label_node->label.text.font = get_font_by_type(GLCD_FONT_SYSFONT_5X7);
	label_node->label.text.is_progmem = false;
	label_node->label.text.text = direct_item->type;
}

static int set_dmi_label(struct gfx_label_node *label_node, uint8_t index)
{
	struct direct_string_item * direct_item = computer_data.details.direct_string;
	while (direct_item != 0){
		index++;
		direct_item = direct_item->next;
	}
	label_node->next = malloc_locked(sizeof(struct gfx_label_node));
	if (label_node->next == NULL)
		return -1;
	set_dmi_content_label(label_node, direct_item);
	set_dmi_name_label(&(label_node->next), direct_item);
	return 0;
}

static void set_dmi_frame(struct gfx_frame *frame, uint8_t index)
{
	frame->information_head = 0;
	frame->image_head = 0;
	frame->label_head =  malloc_locked(sizeof(struct gfx_label_node));
	if (frame->label_head == NULL || set_dmi_label(frame->label_head, index) != 0) {
		free_dmi_menu();
		return ;
	}
}

static void set_dmi_label_text(void)
{
	struct direct_string_item *direct_item = computer_data.details.direct_string;
	struct gfx_label_node *label;
	for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
		label = dmi_menu.actions[i].frame->label_head;
		label->label.text.text = direct_item->content;
		label->label.text.textP = NULL;
		label = label->next;
		if (label != NULL) {
			label->label.text.text = direct_item->type;
			label->label.text.textP = NULL;
		}
		direct_item = direct_item->next;
	}
}

static void set_dmi_actions(void)
{
	if (is_dmi_set){
		dmi_menu.actions = malloc_locked(sizeof(struct gfx_item_action) * dmi_menu.menu->num_elements);
		if (dmi_menu.actions == NULL) {
			free_dmi_menu();
			return ;
		}
		for (int i = 0; i < dmi_menu.menu->num_elements -1; i++){
			dmi_menu.actions[i].type =  ACTION_TYPE_SHOW_FRAME;
			dmi_menu.actions[i].frame = malloc_locked(sizeof(struct gfx_frame));
			if (dmi_menu.actions[i].frame == NULL) {
				free_dmi_menu();
				return ;
			}
			set_dmi_frame(dmi_menu.actions[i].frame, i);
		}
		set_dmi_label_text();
		dmi_menu.actions[dmi_menu.menu->num_elements -1].type = ACTION_TYPE_SHOW_MENU;
		dmi_menu.actions[dmi_menu.menu->num_elements -1].menu = action_menus[0];
	}
}

void set_dmi_menu(void)
{
	set_dmi_mono_menu();
	set_dmi_actions();
}
