/********************************************************************************

Modifications for XPLAINED XMEGA-A3BU by Donald R. Blake (donblake at att.net).

---------------------------------------------------------------------------------

Change Activity:

    Date       Description
   ------      -------------
  21 Jan 2014  Created from AVR8.h.

********************************************************************************/

/**
 * \file XPLAINED_XMEGA_A3BU.h
 * \brief Pinouts to Atmel XPLAINED XMEGA_A3BU 8-bit ATxmega256A3BU microcontroller.
 * \author Andy Gock
 */

/*
        Copyright (c) 2012, Andy Gock

        All rights reserved.

        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions are met:
                * Redistributions of source code must retain the above copyright
                  notice, this list of conditions and the following disclaimer.
                * Redistributions in binary form must reproduce the above copyright
                  notice, this list of conditions and the following disclaimer in the
                  documentation and/or other materials provided with the distribution.
                * Neither the name of Andy Gock nor the
                  names of its contributors may be used to endorse or promote products
                  derived from this software without specific prior written permission.

        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
        DISCLAIMED. IN NO EVENT SHALL ANDY GOCK BE LIABLE FOR ANY
        DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
        (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
        LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
        ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef GLCD_PINOUTS_XPLAINED_XMEGA_A3BU_H_
#define GLCD_PINOUTS_XPLAINED_XMEGA_A3BU_H_

#if defined(GLCD_DEVICE_XPLAINED_XMEGA_A3BU)

#include <avr/io.h>
#include <avr/pgmspace.h>

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define GLCD_CONTROLLER_ST7565R

#if defined (GLCD_CONTROLLER_ST7565R)
  /**
   * \name SPI port and pins
   * @{
   */
  #define CONTROLLER_CS1_PORT    PORTF  // DISPLAY_SS
  #define CONTROLLER_CS1_PIN   	 3
  #define CONTROLLER_MOSI_PORT 	 PORTD  // USARTD0_SPI_MOSI
  #define CONTROLLER_MOSI_PIN  	 3
  #define CONTROLLER_SCK_PORT  	 PORTD  // USARTD0_SPI_SCK
  #define CONTROLLER_SCK_PIN   	 1
  #define CONTROLLER_SCK_PINCTRL PIN1CTRL
  /**@}*/

  /**
   * \name Other pins needed for serial LCD controller
   * @{
   */
  #define CONTROLLER_A0_PORT   PORTD /**< Output port to GLCD A0 pin. */
  #define CONTROLLER_A0_PIN    0     /**< Output pin number to GLCD A0 pin. */  
  #define CONTROLLER_CS1_PORT  PORTF
  #define CONTROLLER_CS1_PIN   3
  #define CONTROLLER_RST_PORT  PORTA
  #define CONTROLLER_RST_PIN   3
  /**@}*/
#else
# error "Controller not supported by XPLAINED_XMEGA_A3BU"
#endif

/**
 * \name Macros for control lines
 * @{
 */

#define GLCD_SELECT( )     CONTROLLER_CS1_PORT.OUTCLR = 1 << CONTROLLER_CS1_PIN
#define GLCD_DESELECT( )   CONTROLLER_CS1_PORT.OUTSET = 1 << CONTROLLER_CS1_PIN

#define GLCD_RESET_LOW( )  CONTROLLER_RST_PORT.OUTCLR = 1 << CONTROLLER_RST_PIN
#define GLCD_RESET_HIGH( ) CONTROLLER_RST_PORT.OUTSET = 1 << CONTROLLER_RST_PIN

#define GLCD_A0_LOW( )     CONTROLLER_A0_PORT.OUTCLR = 1 << CONTROLLER_A0_PIN
#define GLCD_A0_HIGH( )    CONTROLLER_A0_PORT.OUTSET = 1 << CONTROLLER_A0_PIN

// LCD USART SPI port power reduction
#define CONTROLLER_USART_PRP    PRPD
#define CONTROLLER_USART_PRP_bm PR_USART0_bm

// LCD USART SPI port
#define CONTROLLER_USART_SPI USARTD0

/**@}*/

#endif

#endif /* GLCD_PINOUTS_XPLAINED_XMEGA_A3BU_H_ */
