#include "ASF/common/services/gfx_mono/gfx_mono.h"
#include "glcd.h"

void glcd_write(void)
{
	gfx_mono_ssd1306_put_framebuffer();
}

void glcd_init()
{
	glcd_select_screen(glcd_buffer, &glcd_bbox);
}
