/*
 * screen_saver.c
 *
 * Created: 3/18/2017 12:10:19 AM
 *  Author: Nikita
 */

#include "Fp-utils.h"
#include "scheduler/scheduler.h"
#include "power/power.h"
#include "lib/syntax.h"

enum screen_saver_type {
	SCREEN_SAVER_SPLASH,
	SCREEN_SAVER_DASHBOARD,
	SCREEN_SAVER_CLOCK,
	SCREEN_SAVER_TYPE_SIZE,
};

const char *screen_saver_type_str[SCREEN_SAVER_TYPE_SIZE] = { "LOGO", "DASHBOARD", "CLOCK" };

static void handle_screen_saver_type_buttons(uint8_t key)
{
	if (!computer_data.details.screen_saver_visible)
		return;

	switch (key) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (computer_data.details.screen_saver_type == 0)
			return;
		computer_data.details.screen_saver_type--;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (computer_data.details.screen_saver_type == array_length(screen_saver_type_str) - 1)
			return;
		computer_data.details.screen_saver_type++;
		break;
	default:
		return;
	}

	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, computer_data.packed.screen_saver_config);
	gfx_frame_draw(frame_present, true);
}

static void handle_screen_saver_time_buttons(uint8_t key)
{
	if (!computer_data.details.screen_saver_visible)
		return;

	switch (key) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (computer_data.details.screen_saver_update_time <= SCREEN_SAVER_SECOND_MIN_VALUE)
			return;
		computer_data.details.screen_saver_update_time -= SCREEN_SAVER_SECOND_JUMP;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (computer_data.details.screen_saver_update_time >= SCREEN_SAVER_SECOND_MAX_VALUE)
			return;
		computer_data.details.screen_saver_update_time += SCREEN_SAVER_SECOND_JUMP;
		break;
	default:
		return;
	}

	eeprom_write_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS, computer_data.packed.screen_saver_update_time);
	gfx_frame_draw(frame_present, true);
}

static void handle_screen_saver_enable_buttons(uint8_t key)
{
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (!computer_data.details.screen_saver_visible)
			return;
		computer_data.details.screen_saver_visible = 0;
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		if (computer_data.details.screen_saver_visible)
			return;
		computer_data.details.screen_saver_visible = 1;
		break;
	}

	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, computer_data.packed.screen_saver_config);
	gfx_frame_draw(frame_present, true);
}

static void set_disabled(char *str)
{
	sprintf(str, "DISABLED");
}

void sprintf_screen_saver_type(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_type_buttons;
	if (computer_data.details.screen_saver_visible)
		sprintf(str, screen_saver_type_str[computer_data.details.screen_saver_type]);
	else
		set_disabled(str);

	present_menu->is_active_frame = true;
}

void sprintf_screen_saver_time(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_time_buttons;
	if (computer_data.details.screen_saver_visible)
		sprintf(str, "%d", computer_data.details.screen_saver_update_time);
	else
		set_disabled(str);

	present_menu->is_active_frame = true;
}

void sprintf_screen_saver_enable(char *str)
{
	frame_present->handle_buttons = handle_screen_saver_enable_buttons;
	sprintf(str, "DISABLE ENABLE");
	present_menu->is_active_frame = true;
}

void set_screen_saver_enable_draw_graphic_signs(void)
{
	draw_graphic_signs(computer_data.details.screen_saver_visible, 0, 1, false);
}

void set_screen_saver_time_draw_graphic_signs(void)
{
	if (computer_data.details.screen_saver_visible)
		draw_graphic_signs(computer_data.details.screen_saver_update_time, SCREEN_SAVER_SECOND_MIN_VALUE, SCREEN_SAVER_SECOND_MAX_VALUE, true);
}

void set_screen_saver_type_draw_graphic_signs(void)
{
	if (computer_data.details.screen_saver_visible)
		draw_graphic_signs(computer_data.details.screen_saver_type, 0, SCREEN_SAVER_TYPE_SIZE - 1, false);
}

bool screen_saver_mode_enabled;

void enable_screen_saver_mode(void)
{
	screen_saver_mode_enabled = true;
}

void disable_screen_saver_mode(void)
{
	screen_saver_mode_enabled = false;
}

bool reset_screen_saver_req;

static double screen_saver_get_recur_period(void)
{
	return computer_data.details.screen_saver_update_time;
}

static void update_screen_saver(void *data)
{
	if (!screen_saver_mode_enabled || computer_data.details.screen_saver_visible != 1)
		return;

	switch(computer_data.details.screen_saver_type) {
	case SCREEN_SAVER_SPLASH:
		show_logo();
		break;

	case SCREEN_SAVER_DASHBOARD:
		if (current_power_state != POWER_OFF && dashboard != NULL)
			show_frame(dashboard);
		break;

	case SCREEN_SAVER_CLOCK:
		if (clock != NULL && calendar_is_date_valid(&computer_date_time)) {
			show_frame(clock);
			display_state = DISPLAY_CLOCK;
		}
		break;
	}
}

static struct work screen_saver_work = { .do_work = update_screen_saver };

struct scheduler_task screen_saver_sec_task = {
	.work = &screen_saver_work,
	.get_recur_period = screen_saver_get_recur_period,
};