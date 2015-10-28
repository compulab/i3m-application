/*
 * debug.h
 *
 *  Created on: Aug 4, 2015
 *      Author: arkadi
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include <stdlib.h>
#include "asf.h"

//#define DEBUG_MODE

#define DELAY_TIME 0.02

char debug[7];


#ifdef DEBUG_MODE

#endif

#define CLEAR ssd1306_clear();

#define MSG(str, y)	gfx_mono_draw_string(str, 0, y, &sysfont); \
					gfx_mono_put_framebuffer(); \
					delay_s(DELAY_TIME);
//					CLEAR

#define MSG_hex(num)   ssd1306_set_page_address(4); \
					ssd1306_set_column_address(3); \
					itoa(num,debug,16); \
					ssd1306_write_text(debug);
//					delay_s(DELAY_TIME);
//					CLEAR

#define MSG_dec(num)   ssd1306_set_page_address(4); \
					ssd1306_set_column_address(3); \
					itoa(num,debug,10); \
					ssd1306_write_text(debug);
//					delay_s(DELAY_TIME);
//					CLEAR

#define MSG_T_N(str,num,y)	gfx_mono_draw_string(str, 0, y, &sysfont); \
					itoa(num,debug,16); \
					gfx_mono_draw_string(debug, 10, y, &sysfont); \
					delay_s(DELAY_TIME);
//					CLEAR

#define MSG_T_T(title,str)	ssd1306_set_page_address(3); \
					ssd1306_set_column_address(3); \
					ssd1306_write_text(title); \
					ssd1306_set_page_address(5); \
					ssd1306_set_column_address(3); \
					ssd1306_write_text(str);
//					delay_s(DELAY_TIME);
//					CLEAR

#endif /* DEBUG_H_ */
