#
# Copyright (C) 2016 CompuLab, Ltd.
# Authors: Arkadi Gurkov <arkadi.gurkov@compulab.co.il>
#	   Igor Grinberg <grinberg@compulab.co.il>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

VERSION = 0
PATCHLEVEL = 83
FRONT_PANEL_APP_VERSION = $(VERSION).$(PATCHLEVEL)

AUTO_GENERATED_FILE = auto_generated.h

auto: $(AUTO_GENERATED_FILE)

CROSS_COMPILE ?=

$(AUTO_GENERATED_FILE): 
	@( printf '#define VERSION "%s%s"\n' "$(FRONT_PANEL_APP_VERSION)" \
	'$(shell ./setversion)' ) > $@
	@date +'#define BUILD_DATE "%d %b %C%y"' >> $@
	@date +'#define BUILD_TIME "%T"' >> $@


