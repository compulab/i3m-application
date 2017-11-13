/*
 * usb.c
 *
 * Created: 13-Nov-17 3:34:31 PM
 *  Author: Nikita
 */

#include <stdbool.h>
#include "ASF/common/services/usb/udc/udc.h"
#include "ASF/common/utils/stdio/stdio_usb/stdio_usb.h"

bool my_callback_cdc_enable(void)
{
	return stdio_usb_enable();
}

void my_callback_cdc_disable(void)
{
	stdio_usb_disable();
}

void usb_init(void)
{
	udc_start();
}
