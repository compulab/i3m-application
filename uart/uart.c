/*
 * uart.c
 *
 *  Created on: Nov 15, 2015
 *      Author: arkadi
 */

#include "uart.h"

#ifdef UART_DEBUG


void uart_init(void)
{
	PORTD.OUTSET = PIN3_bm;
	PORTD.DIRCLR = PIN2_bm;

	PORTD.DIRSET = PIN3_bm;

	UART_BASE.BAUDCTRLA = BSEL;
	UART_BASE.BAUDCTRLB = (0 << USART_BSCALE0_bp)|(BSEL >> 8);

	UART_BASE.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
	UART_BASE.CTRLB = USART_TXEN_bm | USART_RXEN_bm ;
}

char num_str[5];

void uart_send_char(char c)
{
	if (c == '\0')
		return ;
    while( !(UART_BASE.STATUS & USART_DREIF_bm) ); //Wait until DATA buffer is empty

    UART_BASE.DATA = c;

}

void uart_send_string(char *text)
{
    while(*text)
    {
    	uart_send_char(*text++);
    }
}

void uart_send_num(uint32_t num, uint8_t radix)
{
	itoa(num, num_str, radix);
	uart_send_string(num_str);
}
//
//void uart_system_reset_msg()
//{
//	uint16_t count = TCC0.CNT;
//	char *msg;
//	if ((RST.STATUS & RST_SRF_bm) != 0)
//		msg = "software reset\n\r";
//	else if ((RST.STATUS & RST_PDIRF_bm) != 0)
//		msg = "PDI reset\n\r";
//	else if ((RST.STATUS & RST_WDRF_bm) != 0)
//		msg = "WDT reset\n\r";
//	else if ((RST.STATUS & RST_BORF_bm) != 0)
//		msg = "Brownout reset\n\r";
//	else if ((RST.STATUS & RST_PORF_bm) != 0)
//		msg = "power on reset\n\r";
//	else
//		msg = "unknown reset\n\r";
//	uart_send_string(msg);
//	uart_send_num(count, 16);
//	uart_send_string("- ticks from start\n\r");
//}

//void uart_send_num(uint32_t num, uint8_t radix) {return ;}


#else

void uart_init(void) {return ;}

void uart_send_char(char c) {return ;}

void uart_send_string(char *text) {return ;}

void uart_send_num(uint32_t num, uint8_t radix) {return ;}

#endif
