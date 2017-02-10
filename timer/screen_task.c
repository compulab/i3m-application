#include "timer.h"

static struct work update_screen_work = { .do_work = update_info };

#define UPDATE_SCREEN_TIME		1
static double screen_get_recur_period(void)
{
    return UPDATE_SCREEN_TIME;
}

struct scheduler_task screen_sec_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};
