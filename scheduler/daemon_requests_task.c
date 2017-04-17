/*
 * daemon_requests_task.c
 *
 * Created: 3/23/2017 6:43:27 PM
 *  Author: Nikita
 */

#include "twi/i2c_buffer.h"
#include "gfx/gfx_components/gfx_information.h"
#include "gfx/action_menu/gfx_item_action.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include <stdbool.h>
#include <stddef.h>


static bool is_type_in_frame(enum information_type info_type, struct gfx_information_node *info_node)
{
	for (; info_node != NULL; info_node = info_node->next)
		if (info_node->information.info_type == info_type)
			return true;

	return false;
}

static bool need_to_update_req(enum information_type info_type)
{
//	if (frame_present)
//		return is_type_in_frame(info_type, frame_present->information_head);

	struct gfx_item_action *action;
	for (int i = 0; i < present_menu->menu->num_elements; i++) {
		action = &present_menu->actions[i];
		if (action->type == ACTION_TYPE_SHOW_FRAME && is_type_in_frame(info_type, action->frame->information_head))
			return true;
	}

	return false;
}

void update_requests(void *data)
{
	i2c_buffer.layout.hddtr = !i2c_buffer.layout.hddtr && need_to_update_req(SHOW_HDD_TEMPERTURE);
	i2c_buffer.layout.cpufr = !i2c_buffer.layout.cpufr && need_to_update_req(SHOW_CPU_FREQUENCY);
	i2c_buffer.layout.cputr = !i2c_buffer.layout.cputr && need_to_update_req(SHOW_CPU_TEMPERTURE);
	i2c_buffer.layout.gpu_temp_request = !i2c_buffer.layout.gpu_temp_request && need_to_update_req(SHOW_GPU_TEMPERTURE);
	i2c_buffer.layout.req = !i2c_buffer.layout.req && i2c_buffer.raw[PENDR0];
}

static struct work requests_work = { .do_work = update_requests };

static double pending_req_get_recur_period(void)
{
	return 1;
}

struct scheduler_task pending_req_tick_task = {
	.work = &requests_work,
	.get_recur_period = pending_req_get_recur_period,
};
