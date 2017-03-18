/*
 * screen_saver.h
 *
 * Created: 3/18/2017 12:21:13 AM
 *  Author: Nikita
 */


#ifndef SCREEN_SAVER_H_
#define SCREEN_SAVER_H_

void set_screen_saver_type(char *str);
void set_screen_saver_time(char *str);
void set_screen_saver_enable(char *str);
void enable_screen_saver_mode(void);
void disable_screen_saver_mode(void);
void set_screen_saver_enable_draw_graphic_signs(void);
void set_screen_saver_time_draw_graphic_signs(void);
void set_screen_saver_type_draw_graphic_signs(void);

#endif /* SCREEN_SAVER_H_ */