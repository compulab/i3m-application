///********************************************************************************
//
//Modifications for XPLAINED XMEGA-A3BU by Donald R. Blake (donblake at att.net).
//
//---------------------------------------------------------------------------------
//
//Change Activity:
//
//    Date       Description
//   ------      -------------
//  21 Jan 2014  Created from AVR8.c.
//
//********************************************************************************/
//
///**
//   \file XPLAINED_XMEGA_A3BU.c
//   \brief Functions relating to Atmel AVR 8-bit AVRs.
//          For use with GNU toolchain
//   \author Andy Gock
//
// */
//
///*
//        Copyright (c) 2012, Andy Gock
//
//        All rights reserved.
//
//        Redistribution and use in source and binary forms, with or without
//        modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright
//                  notice, this list of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright
//                  notice, this list of conditions and the following disclaimer in the
//                  documentation and/or other materials provided with the distribution.
//                * Neither the name of Andy Gock nor the
//                  names of its contributors may be used to endorse or promote products
//                  derived from this software without specific prior written permission.
//
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//        DISCLAIMED. IN NO EVENT SHALL ANDY GOCK BE LIABLE FOR ANY
//        DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//        (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//        LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//        ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*/
//
//
//
//#include "../glcd.h"
//
//
//
//// XMEAGA header file fix for USART.CTRLC bit positions
//
//#ifndef USART_UDORD_bp
//#  define USART_UDORD_bp 2
//#endif
//#ifndef USART_UCPHA_bp
//#  define USART_UCPHA_bp 1
//#endif
//
//
//
//
//// LCD clock speed - minimum clock period is 50 ns (20 MHz)
//#define NHD_LCD_CLOCK_SPEED 1000000UL
//#if ( 2 * NHD_LCD_CLOCK_SPEED ) > F_CPU
//#  error Bad NHD_LCD_CLOCK_SPEED
//#endif
//#define BAUDRATE_SEL ( ( ( F_CPU ) / ( 2 * ( NHD_LCD_CLOCK_SPEED ) ) ) - 1 )
//
//
//
//#if defined( GLCD_DEVICE_XPLAINED_XMEGA_A3BU )
//
//static
//void
//glcd_usart_spi_init(
//  void
//)
//{
//
//  // disable Rx
//  CONTROLLER_USART_SPI.CTRLB &= ~( USART_RXEN_bm );
//
//  // configure SCK pin as output
//  CONTROLLER_SCK_PORT.DIRSET = 1 << CONTROLLER_SCK_PIN;
//
//  // set SCK pin mode
//  CONTROLLER_SCK_PORT.CONTROLLER_SCK_PINCTRL = PORT_OPC_TOTEM_gc | PORT_INVEN_bm;
//
//  // set SCK pin high
//  CONTROLLER_SCK_PORT.OUTSET = 1 << CONTROLLER_SCK_PIN;
//
//  // configture MOSI pin as output and set high
//  CONTROLLER_MOSI_PORT.DIRSET = 1 << CONTROLLER_MOSI_PIN;
//  CONTROLLER_MOSI_PORT.OUTSET = 1 << CONTROLLER_MOSI_PIN;
//
//  // set Master SPI mode, data order (MSB first) and clock phase (sample
//  // on rising edge)
//  CONTROLLER_USART_SPI.CTRLC =
//     USART_CMODE_MSPI_gc |
//     ( 0 << USART_UDORD_bp ) |
//     ( 1 << USART_UCPHA_bp );
//
//  // set Baud rate
//  CONTROLLER_USART_SPI.BAUDCTRLB =
//    (uint8_t) ( ( ~USART_BSCALE_gm ) & ( BAUDRATE_SEL >> 8 ) );
//  CONTROLLER_USART_SPI.BAUDCTRLA = (uint8_t) BAUDRATE_SEL;
//
//  // enable Tx & Rx
//  CONTROLLER_USART_SPI.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
//
//} // end glcd_usart_spi_init
//
//
//
//void
//glcd_init(
//  void
//)
//{
//
//#if defined( GLCD_CONTROLLER_ST7565R )
//
//  /* Set up GPIO directions */
//
//  // set LCD CS1 pin as output
//  CONTROLLER_CS1_PORT.DIRSET = 1 << CONTROLLER_CS1_PIN;
//
//  /* Set LCD A0 pin as output */
//  CONTROLLER_A0_PORT.DIRSET = 1 << CONTROLLER_A0_PIN;
//
//  // deselect LCD
//  GLCD_DESELECT( );
//
//  // ensure LCD USART port clock is on
//  PR.CONTROLLER_USART_PRP &= ~( CONTROLLER_USART_PRP_bm );
//
//  // initialize the LCD interface (USART SPI)
////  glcd_usart_spi_init( );
//
//  delay_ms( 30 ); /* Example in datasheet does this (20ms) */
//
////  glcd_ST7565R_init( );
//
////  glcd_all_on( );
//
//  delay_ms( 500 );
////  glcd_normal( );
//
////  glcd_set_start_line( 0 );
////  glcd_clear_now( );
//
////  glcd_select_screen( glcd_buffer,&glcd_bbox );
//
////  glcd_clear( );
//
//#else
//# error "Controller not supported"
//#endif /* GLCD_CONTROLLER_* */
//
//} // end glcd_init
//
//
//
//void
//glcd_spi_write(
//  uint8_t c
//)
//{
//
//  // set CS pin low (select device)
//  GLCD_SELECT( );
//
//  // wait for data register empty
//  while ( ( CONTROLLER_USART_SPI.STATUS & USART_DREIF_bm ) == 0 );
//
//  // transmit byte
//  CONTROLLER_USART_SPI.DATA = c;
//
//  // wait for transmit complete
//  while ( ( CONTROLLER_USART_SPI.STATUS & USART_TXCIF_bm ) == 0 );
//
//  // clear transmit complete
//  CONTROLLER_USART_SPI.STATUS = USART_TXCIF_bm;
//
//
//  // set CS pin high (deselect device)
//  GLCD_DESELECT( );
//
//} // end glcd_spi_write
//
//
//
//void glcd_reset(void)
//{
//  /* Toggle RST low to reset. Minimum pulse 100ns on datasheet. */
//  GLCD_SELECT( );
//  GLCD_RESET_LOW( );
//  delay_ms( GLCD_RESET_TIME );
//  GLCD_RESET_HIGH( );
//  GLCD_DESELECT( );
//}
//
//
//
//#endif // if defined( GLCD_DEVICE_XPLAINED_XMEGA_A3BU )
