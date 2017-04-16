/*
 * app_version.c
 *
 * Created: 3/29/2017 9:00:26 PM
 *  Author: Nikita
 */

#include "gfx/gfx_components/gfx_information.h"
#include "auto_generated.h"
#include <stdio.h>
#include <string.h>

#ifndef VERSION
#define VERSION "Version not set"
#endif

#ifndef BUILD_DATE
#define BUILD_DATE "Some day"
#endif

#ifndef BUILD_TIME
#define BUILD_TIME "hh:mm:ss"
#endif

#define VERSION_BUILD_TIME_INDEX	0
#define VERSION_BUILD_DATE_INDEX	1
#define VERSION_APP_VERSION_INDEX	2

static void sprintf_app_version(struct gfx_information *info, char *output_str)
{
	switch (info->info_data) {
	case VERSION_BUILD_TIME_INDEX:
		strcpy(output_str, BUILD_TIME);
		break;

	case VERSION_BUILD_DATE_INDEX:
		strcpy(output_str, BUILD_DATE);
		break;

	case VERSION_APP_VERSION_INDEX:
		strcpy(output_str, "VER: " VERSION);
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
