/*
 * sampleMenu.c
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */
#include "sampleMenu.h"


void sampleDataFunc(char ** data){
	*data = "day!";
}


void sampleMenu(gfx_action_menu * menu){
	item_action *actionArr = malloc(sizeof(item_action)*menu->menu->num_elements);
	item_splash_action_init(&actionArr[0],&compulab_logo);
	item_none_action_init(&actionArr[1]);
	item_data_function_action_init(&actionArr[2],"Good",sampleDataFunc);
	item_none_action_init(&actionArr[3]);
	item_none_action_init(&actionArr[4]);
	item_data_action_init(&actionArr[5],"Voltage: ","3.6 V");

	menu->menu = &testMenu;
	menu->parent = NULL;
	menu->actions = actionArr;
}

