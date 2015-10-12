/*
 * menu.c
 *
 *  Created on: Oct 7, 2015
 *      Author: arkadi
 */
#include "menu.h"

struct menu *current_menu;


void show_menu(struct menu *menu)
{
	if (current_menu != 0)
		current_menu->visible = false;
	current_menu = menu;
	current_menu->visible = true;
	u8g_draw_menu(current_menu);
}

void show_action(struct item_action *action)
{
	current_menu->visible = false;
	switch (action->type){
	case ACTION_TYPE_SHOW_MENU:
		show_menu(action->menu);
		break;
	case ACTION_TYPE_SHOW_FRAME:
		u8g_draw_frame(action->frame);
		break;
	default:
		break;
	}
}

void handle_select_pressed()
{
	if (current_menu->visible)
		show_action(current_menu->curr_selected_item->action);
	else
		show_menu(current_menu);
}

void handle_down_pressed()
{
	if (current_menu->curr_selected_index == current_menu->size - 1)
		current_menu->curr_selected_index = 0;
	else
		current_menu->curr_selected_index++;
	show_menu(current_menu);
}

void handle_up_pressed()
{
	if (current_menu->curr_selected_index == 0)
		current_menu->curr_selected_index = current_menu->size - 1;
	else
		current_menu->curr_selected_index--;
	show_menu(current_menu);
}
