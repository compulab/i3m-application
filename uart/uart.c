/*
 * uart.c
 *
 *  Created on: Nov 15, 2015
 *      Author: arkadi
 */

#include "uart.h"

#ifdef UART_DEBUG

#define BSEL 	16
#define UART_BASE	USARTD0

void uart_init(void)
{
	PORTD.OUTSET = PIN3_bm;
	PORTD.DIRCLR = PIN2_bm;

	PORTD.DIRSET = PIN3_bm;

	UART_BASE.BAUDCTRLA = BSEL;
	UART_BASE.BAUDCTRLB = (0 << USART_BSCALE0_bp) | (BSEL >> 8);

	UART_BASE.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
	UART_BASE.CTRLB = USART_TXEN_bm | USART_RXEN_bm ;
}

char num_str[10];

void uart_send_char(char c)
{
	if (c == '\0')
		return;

	while( !(UART_BASE.STATUS & USART_DREIF_bm) ); //Wait until DATA buffer is empty
	UART_BASE.DATA = c;
}

void uart_send_string(char *text)
{
    while(*text)
    	uart_send_char(*text++);
}

void uart_send_num(uint32_t num, uint8_t radix)
{
	ltoa(num, num_str, radix);
	uart_send_string(num_str);
}

#else

void uart_init(void) { return; }

void uart_send_char(char c) { return; }

void uart_send_string(char *text) { return; }

void uart_send_num(uint32_t num, uint8_t radix) { return; }

#endif
