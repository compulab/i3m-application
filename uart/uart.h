/*
 * uart.h
 *
 *  Created on: Nov 15, 2015
 *      Author: arkadi
 */

#ifndef UART_UART_H_
#define UART_UART_H_
#include "../asf.h"

#define BSEL 	207
#define UART_BASE	USARTD0

void uart_init(void);

void uart_send_char(char c);

void uart_send_string(char *text);

#endif /* UART_UART_H_ */
