## A Commodore LCD emulator using SDL2 library.
## Also included a very simple and inaccurate Commodore VIC-20 emulator
##
## Copyright (C)2016 LGB (Gábor Lénárt) <lgblgblgb@gmail.com>
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
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#OUR_ROMS	= clcd-u104-parasite.rom vic20-emulator-tool.rom
OUR_ROMS	= vic20-emulator-tool.rom


all: roms $(OUR_ROMS)

clean:
	rm -f *.o

distclean:
	rm -f *.o vic20-basic.rom vic20-kernal.rom vic20-chargen.rom clcd-u105.rom clcd-u102.rom clcd-u104.rom clcd-u103.rom c65-system.rom

roms:
	test -s vic20-basic.rom || wget -O vic20-basic.rom http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/vic20/basic.901486-01.bin
	test -s vic20-kernal.rom || wget -O vic20-kernal.rom http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/vic20/kernal.901486-07.bin
	test -s vic20-chargen.rom || wget -O vic20-chargen.rom http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/vic20/characters.901460-03.bin
	test -s clcd-u105.rom || wget -O clcd-u105.rom http://commodore-lcd.lgb.hu/files/SS-CALC-13apr-u105.bin
	test -s clcd-u102.rom || wget -O clcd-u102.rom http://commodore-lcd.lgb.hu/files/kizapr-u102.bin
	test -s clcd-u104.rom || wget -O clcd-u104.rom http://commodore-lcd.lgb.hu/files/sept-M-13apr-u104.bin
	test -s clcd-u103.rom || wget -O clcd-u103.rom http://commodore-lcd.lgb.hu/files/sizapr-u103.bin
	test -s c65-system.rom || wget -O c65-system.rom http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c65/910111.bin
	test -s geos-c64-chargen.rom || wget -O geos-c64-chargen.rom http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/characters.901225-01.bin
	test -s geos-c64-kernal.rom || wget -O geos-c64-kernal.rom http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/kernal.901227-03.bin
	test -s geos-c64-basic.rom || wget -O geos-c64-basic.rom http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/basic.901226-01.bin

%.rom: %.asm
	cl65 -t none --cpu 65c02 -o $@ $<

.PHONY: roms clean
