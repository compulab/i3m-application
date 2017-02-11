#include "scheduler.h"

static struct work requests_work = { .do_work = update_requests };

#define UPDATE_REQ_SEC			1
static double pending_req_get_recur_period(void)
{
	return UPDATE_REQ_SEC;
}

struct scheduler_task pending_req_tick_task = {
    .work = &requests_work,
    .get_recur_period = pending_req_get_recur_period,
};
