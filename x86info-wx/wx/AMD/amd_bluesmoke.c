/*
 *  $Id: amd_bluesmoke.c,v 1.2 2003-04-13 21:58:00 root Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  Dump MCA registers.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "../x86info.h"

#define MCG_CAP 0x0179
#define MCG_STATUS 0x17a
#define MCG_CTL 0x17b

#define MC_CTL 0x0400
#define MC_STATUS 0x0401
#define MC_ADDR 0x402
#define MC_MISC 0x403

void decode_athlon_bluesmoke(int cpunum)
{
	unsigned long long val, val2;
	int banks, i;

	if (!user_is_root)
		return;

	if (read_msr(cpunum, MCG_CAP, &val) != 1)
		return;

	if ((val & (1<<8)) == 0)
		output (msg_warning, "Erk, MCG_CTL not present! :%llx:\n", val);

	banks = val & 0xf;

	output (msg_reportingbanks, "Number of reporting banks : %d\n\n", banks);

	if (read_msr(cpunum, MCG_STATUS, &val) == 1) {
		if (val != 0) {
			printf("            31       23       15       7 \n");
			printf ("MCG_STATUS: ");
			dumpmsr_bin (cpunum, MCG_STATUS, 32);
		}
	}

	if (read_msr(cpunum, MCG_CTL, &val) == 1) {
		output (msg_format, "MCG_CTL:\n");

		output (msg_datacache, " Data cache check %sabled\n", val & (1<<0) ? "en" : "dis");
		if ((val & (1<<0)) == 1) {
			if (read_msr(cpunum, MC_CTL, &val2) == 1) {
				output ("  ECC 1 bit error reporting %sabled\n", val2 & (1<<0) ? "en" : "dis");
				output ("  ECC multi bit error reporting %sabled\n", val2 & (1<<1) ? "en" : "dis");
				output ("  Data cache data parity %sabled\n", val2 & (1<<2) ? "en" : "dis");
				output ("  Data cache main tag parity %sabled\n", val2 & (1<<3) ? "en" : "dis");
				output ("  Data cache snoop tag parity %sabled\n", val2 & (1<<4) ? "en" : "dis");
				output ("  L1 TLB parity %sabled\n", val2 & (1<<5) ? "en" : "dis");
				output ("  L2 TLB parity %sabled\n", val2 & (1<<6) ? "en" : "dis");
			}
		}

		output (" Instruction cache check %sabled\n", val & (1<<1) ? "en" : "dis");
		if (((val & (1<<1)) == 2) && (banks>1)) {
			if (read_msr(cpunum, MC_CTL+4, &val2) == 1) {
				output ("  ECC 1 bit error reporting %sabled\n", val2 & (1<<0) ? "en" : "dis");
				output ("  ECC multi bit error reporting %sabled\n", val2 & (1<<1) ? "en" : "dis");
				output ("  Instruction cache data parity %sabled\n", val2 & (1<<2) ? "en" : "dis");
				output ("  IC main tag parity %sabled\n", val2 & (1<<3) ? "en" : "dis");
				output ("  IC snoop tag parity %sabled\n", val2 & (1<<4) ? "en" : "dis");
				output ("  L1 TLB parity %sabled\n", val2 & (1<<5) ? "en" : "dis");
				output ("  L2 TLB parity %sabled\n", val2 & (1<<6) ? "en" : "dis");
				output ("  Predecode array parity %sabled\n", val2 & (1<<7) ? "en" : "dis");
				output ("  Target selector parity %sabled\n", val2 & (1<<8) ? "en" : "dis");
				output ("  Read data error %sabled\n", val2 & (1<<9) ? "en" : "dis");
			}
		}

		output (" Bus unit check %sabled\n", val & (1<<2) ? "en" : "dis");
		if ((val & (1<<2)) == 4 && (banks>2)) {
			if (read_msr(cpunum, MC_CTL+8, &val2) == 1) {
				output ("  External L2 tag parity error %sabled\n", val2 & (1<<0) ? "en" : "dis");
				output ("  L2 partial tag parity error %sabled\n", val2 & (1<<1) ? "en" : "dis");
				output ("  System ECC TLB reload error %sabled\n", val2 & (1<<2) ? "en" : "dis");
				output ("  L2 ECC TLB reload error %sabled\n", val2 & (1<<3) ? "en" : "dis");
				output ("  L2 ECC K7 deallocate %sabled\n", val2 & (1<<4) ? "en" : "dis");
				output ("  L2 ECC probe deallocate %sabled\n", val2 & (1<<5) ? "en" : "dis");
				output ("  System datareaderror reporting %sabled\n", val2 & (1<<6) ? "en" : "dis");
			}
		}

		output (" Load/Store unit check %sabled\n", val & (1<<3) ? "en" : "dis");
		if ((val & (1<<3)) == 8 && (banks>3)) {
			if (read_msr(cpunum, MC_CTL+12, &val2) == 1) {
				output ("  Read data error enable (loads) %sabled\n", val2 & (1<<0) ? "en" : "dis");
				output ("  Read data error enable (stores) %sabled\n", val2 & (1<<1) ? "en" : "dis");
			}
		}
	}
	output (msg_format, "\n");


	output(msg_format, "           31       23       15       7 \n");
	for (i=0; i<banks; i++) {
		output (msg_format, "Bank: %d (0x%x)\n", i, MC_CTL+i*4);
		output (msg_format, "MC%dCTL:    ", i);
		dumpmsr_bin (cpunum, MC_CTL+i*4, 32);
		output (msg_format, "MC%dSTATUS: ", i);
		dumpmsr_bin (cpunum, MC_STATUS+i*4, 32);
		output (msg_format, "MC%dADDR:   ", i);
		dumpmsr_bin (cpunum, MC_ADDR+i*4, 32);
		output (msg_format, "MC%dMISC:   ", i);
		dumpmsr_bin (cpunum, MC_MISC+i*4, 32);
		output (msg_format, "\n");
	}
}
