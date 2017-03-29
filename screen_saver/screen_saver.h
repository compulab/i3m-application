/*
 * screen_saver.h
 *
 * Created: 3/18/2017 12:21:13 AM
 *  Author: Nikita
 */


#ifndef SCREEN_SAVER_H_
#define SCREEN_SAVER_H_

enum screen_saver_type {
	SCREEN_SAVER_SPLASH,
	SCREEN_SAVER_DASHBOARD,
	SCREEN_SAVER_CLOCK,
	SCREEN_SAVER_TYPE_SIZE,
};

static inline void sprintf_disabled(char *output_string)
{
	sprintf(output_string, "DISABLED");
}

void sprintf_screen_saver_type(struct gfx_information *info, char *output_str);
void sprintf_screen_saver_time(struct gfx_information *info, char *output_str);
void sprintf_screen_saver_enable(struct gfx_information *info, char *output_str);
void enable_screen_saver_mode(void);
void disable_screen_saver_mode(void);
void set_screen_saver_enable_draw_graphic_signs(void);
void set_screen_saver_time_draw_graphic_signs(void);
void set_screen_saver_type_draw_graphic_signs(void);

#endif /* SCREEN_SAVER_H_ */