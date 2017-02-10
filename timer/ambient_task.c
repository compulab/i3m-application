#include "timer.h"

#define MAX_AMBIENT_UPDATE_FAIL	2

void update_ambient_temp()
{
	static uint8_t ambient_update_fail_count = 0;

    if (current_power_state != POWER_ON)
        return;

    if (TWI_read_reg(AMBIENT_TWI_ADDRESS, AMBIENT_TEMPERATURE_ADDRESS, &layout.l.ambt, 2)) {
		ambient_update_fail_count = 0;
		layout.l.ambs = 1;
	} else if (ambient_update_fail_count == MAX_AMBIENT_UPDATE_FAIL) {
		layout.l.ambs = 0;
	} else  {
		ambient_update_fail_count++;
		update_ambient_temp();
	}

	computer_data.details.ambs = layout.l.ambs;
	computer_data.details.ambt = layout.l.ambt;
}

static struct work ambient_work = { .do_work = update_ambient_temp };

#define UPDATE_AMBIENT_SEC		4
static double ambient_get_recur_period(void)
{
	return UPDATE_AMBIENT_SEC;
}

struct scheduler_task ambient_tick_task = {
    .work = &ambient_work,
    .get_recur_period = ambient_get_recur_period,
};
