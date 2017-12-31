/*
 * app_version.c
 *
 * Created: 3/29/2017 9:00:26 PM
 *  Author: Nikita
 */

#include "gfx/gfx_components/gfx_information.h"
#include "information.h"
#include <stdio.h>
#include <string.h>

#ifndef DEBUG_BUILD
#define DEBUG_BUILD ""
#endif

#ifndef BUILD_DATE
#define BUILD_DATE __DATE__
#endif

#ifndef BUILD_TIME
#define BUILD_TIME __TIME__
#endif

#define VERSION_BUILD_TIME_INDEX	0
#define VERSION_BUILD_DATE_INDEX	1
#define VERSION_APP_VERSION_INDEX	2

static void sprintf_app_version(struct gfx_information *info, char *output_str)
{
	switch (info->metadata) {
	case VERSION_BUILD_TIME_INDEX:
		strcpy(output_str, BUILD_TIME);
		break;

	case VERSION_BUILD_DATE_INDEX:
		strcpy(output_str, BUILD_DATE);
		break;

	case VERSION_APP_VERSION_INDEX:
		strcpy(output_str, "VER: " VERSION DEBUG_BUILD);
		break;

	default:
		strcpy(output_str, "-");
		break;
	}
}

int gfx_information_init_show_app_version(struct gfx_information *info)
{
	info->to_string = sprintf_app_version;
	return 0;
}
