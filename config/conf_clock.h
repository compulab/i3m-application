/**
 * \file
 *
 * \brief Chip-specific system clock manager configuration
 *
 * Copyright (c) 2011-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED

//sysclk sourced from the 2MHz internal oscilator, upscaled to 32MHz by a PLL
#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_RC32MHZ
#define CONFIG_PLL0_SOURCE          PLL_SRC_XOSC
#define CONFIG_PLL0_MUL             1
#define CONFIG_PLL0_DIV             1

//Do not apply prescaler on sysclk output. Thus, all peripherals, RAM, NVM, and
//CPU are clocked using unmodified sysclk
#define CONFIG_SYSCLK_PSADIV          SYSCLK_PSADIV_1
#define CONFIG_SYSCLK_PSBCDIV         SYSCLK_PSBCDIV_1_1

/* DFLL autocalibration */
#define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC OSC_ID_RC32KHZ

/* The following configures the USB controller clock
 * source to use the internal (nominally) 32MHz RC oscillator, up-calibrated to
 * run at 48MHz via the periodic 1ms USB Start Of Frame packets sent by the host.
 */
#define CONFIG_USBCLK_SOURCE                USBCLK_SRC_PLL
//#define CONFIG_OSC_RC32_CAL                 48000000UL
//#define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC  OSC_ID_USBSOF

/* RTC uses the internal 32.768KHz oscillator, divided by 32 */
#define CONFIG_RTC_SOURCE					SYSCLK_RTCSRC_RCOSC

#endif /* CONF_CLOCK_H_INCLUDED */
