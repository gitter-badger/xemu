/* This is an odd emulator, emulating a Commodore 64 like machine only for the
   level needed for a special version of GEOS to be able to run on it.
   You should have a really special one with own disk drive etc, since there
   is no hardware support for drive emulation etc, but it's built in the emulator
   with a kind of CPU trap! The purpose: know GEOS better and slowly replace
   more and more functions on C/emulator level, so at one point it's possible
   to write a very own version of GEOS without *any* previously used code in
   the real GEOS. Then it can be even ported to other architectures GEOS wasn't
   mean to run ever.
   ---------------------------------------------------------------------------------
   One interesting plan: write a GEOS emulator which does not use VIC-II bitmapped
   screen anymore, but the GEOS functions mean to be targeted a "modern UI toolkit",
   ie GTK, so a dozens years old (unmodified) GEOS app would be able to run on a PC
   with modern look and feel, ie anti-aliased fonts, whatever ...
   ---------------------------------------------------------------------------------
   Copyright (C)2016 LGB (Gábor Lénárt) <lgblgblgb@gmail.com>
   ---------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <SDL.h>

#include "commodore_geos.h"
#include "cpu65c02.h"
#include "geos.h"
#include "emutools.h"


#define GEOS_KERNAL_NAME "geos-kernal.bin"



void inject_screencoded_message ( int addr, const char *s )
{
	while (*s) {
		unsigned char c = (unsigned char)(*(s++));
		if (c >= 65 && c <= 90)
			c -= 64;
		else if (c >= 97 && c <= 122)
			c -= 96;
		colour_sram[addr] = 0xF1;
		memory[addr + 0x0400] = c;
		addr++;
	}
}



static int check_basic_stub ( Uint8 *p, int addr )
{
	int a = 10;
	if (*p != 0x9E)
		return -1;	// should be "SYS" token
	p++;
	while (*p == 0x20 && --a)
		p++;
	if (!a)
		return -1;
	printf("OK! %c%c%c%c\n", p[0], p[1], p[2], p[3]);
	if (
		!isdigit(p[0]) || !isdigit(p[1]) || !isdigit(p[2]) || !isdigit(p[3]) ||
		isdigit(p[4])
	)
		return -1;
	a = (p[0] - '0') * 1000 + (p[1] - '0') * 100 + (p[2] - '0') * 10 + (p[3] - '0');
	if (a < addr + 12 || a > addr + 100)
		return -1;
	printf("GEOS: basic stub SYS address is %d decimal." NL, a);
	return a;
}



int geos_load_kernal ( void )
{
	Uint8 buffer[0xB000];
	int len = emu_load_file(GEOS_KERNAL_NAME, buffer, sizeof buffer);
	int addr;
	if (len < 0) {
		INFO_WINDOW("Cannot open GEOS kernal (%s), or I/O error", GEOS_KERNAL_NAME);
		return 1;	// file not found, or I/O error
	}
	if (len < 0x1000) {
		INFO_WINDOW("Abnormally short GEOS kernal (%s)", GEOS_KERNAL_NAME);
		return 1;
	}
	if (len == sizeof buffer) {
		INFO_WINDOW("Abnormally large GEOS kernal (%s)", GEOS_KERNAL_NAME);
		return 1;
	}
	addr = buffer[0] | (buffer[1] << 8);	// load address
	if (addr == 0x5000) {
		cpu_pc = 0x5000;
	} else if (addr == 0x801) {
		int sys = check_basic_stub(buffer + 6, addr);
		if (sys < 0) {
			INFO_WINDOW("Invalid BASIC stub for the GEOS kernal (%s)", GEOS_KERNAL_NAME);
			return 1;
		}
		cpu_pc = sys;
		inject_screencoded_message(6 * 40, "*** Basic stub, you need to wait now ***");
	} else {
		INFO_WINDOW("Invalid GEOS kernal load address ($%04X) in (%s)", addr, GEOS_KERNAL_NAME);
		return 1;
	}
	// OK. Everything seems to be so okey ...
	memcpy(memory + addr, buffer + 2, len - 2);
	return 0;
}


void geos_cpu_trap ( Uint8 opcode )
{
	// Hopefully the trap itself is in RAM ..
	printf("GEOS: TRAP-SYM: \"%s\"" NL, memory + cpu_pc);
	cpu_pc += strlen((const char*)memory + cpu_pc) + 1;
}
