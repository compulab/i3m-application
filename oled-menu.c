#include "ASF/common/services/gfx_mono/sysfont.h"
#include "ASF/common/services/gfx_mono/gfx_mono_menu.h"
#include "config/conf_menu.h"
#include "oled-menu.h"
/**
 * \ingroup asfdoc_common2_gfx_mono_menu
 * @{
 */
void oled_menu_init(struct oled_menu *oledMenu)
{
	gfx_mono_menu_init(oledMenu->menu);
}

uint8_t oled_menu_process_key(struct oled_menu *oledMenu, uint8_t keycode)
{
	switch (keycode) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
	case GFX_MONO_MENU_KEYCODE_UP:
		return gfx_mono_menu_process_key(oledMenu->menu, keycode);
	case GFX_MONO_MENU_KEYCODE_ENTER:
			(oledMenu->operations[oledMenu->menu->current_selection])();
		break;
	case GFX_MONO_MENU_KEYCODE_BACK:
			(oledMenu->handleBack)();
		break;
	}
	return GFX_MONO_MENU_EVENT_IDLE;
}
