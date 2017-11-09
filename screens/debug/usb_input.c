/*
 * usb_input.c
 *
 * Created: 3/27/2017 8:33:27 PM
 *  Author: Nikita
 */

#include "ASF/xmega/drivers/wdt/wdt.h"
#include "gfx/gfx_components/gfx_information.h"
#include "ASF/common/services/usb/class/cdc/device/udi_cdc.h"
#include <string.h>

char buf[1024] = { 0 };
static void usb_show_frame()
{
	int i = 0;
	while (udi_cdc_is_rx_ready()) {
		wdt_reset();
		buf[i] = udi_cdc_getc();
		i++;
		i = i >= (1024) ? 0 : i;
	}
	struct gfx_mono_bitmap bitmap = { 128 , 64, GFX_MONO_BITMAP_RAM, NULL };
	bitmap.data.pixmap = buf;
	gfx_mono_generic_put_bitmap(&bitmap, 0, 0);
}

static void handle_usb_buttons(struct gfx_information *info, uint8_t key)
{
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		gfx_switch_to_current_menu();
		return;
	case GFX_MONO_MENU_KEYCODE_DOWN:
		while (!udi_cdc_is_tx_ready())
			;
		udi_cdc_putc('2');
		return;
	case GFX_MONO_MENU_KEYCODE_UP:
		while (!udi_cdc_is_tx_ready())
			;
		udi_cdc_putc('1');
	}

	gfx_redraw_current_frame();
}

int gfx_information_init_show_usb_serial(struct gfx_information *info)
{
	info->draw_data = usb_show_frame;
	info->handle_buttons = handle_usb_buttons;
	return 0;
}