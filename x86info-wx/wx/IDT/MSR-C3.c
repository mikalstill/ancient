/*
 *  $Id: MSR-C3.c,v 1.2 2003-04-14 13:23:25 root Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  IDT/Centaur specific parts.
 */
#include <stdio.h>
#include "../x86info.h"

void dump_C3_MSR (struct cpudata *cpu)
{
	unsigned long long val=0;
	int longhaul=0;

	if (!user_is_root)
		return;

	output (msg_format, "FCR: ");
	dumpmsr (cpu->number, 0x1107, 32);

	output (msg_accumulate, "Longhaul v");
	if (cpu->model==6 || (cpu->model==7 && cpu->stepping==0))
		output (msg_accumulate, "1.0");
	if (cpu->model==7 && cpu->stepping>0) {
		output (msg_accumulate, "2.0");
		longhaul=2;
	}
	if (cpu->model==8)
		output (msg_accumulate, "3.0");
	output (msg_accumulate, " present");
	output (msg_longhaul, "");

	if (longhaul==2 && read_msr(cpu->number, 0x110A, &val) == 1) {
		dumpmsr (cpu->number, 0x110A, 64);
		if (val & 1)
			output (msg_softvid, "\tSoftVID support");
		if (val & 2)
			output (msg_softvid, "\tSoftBSEL support");
		if (val==0)
			output (msg_softvid, "\tSoftware clock multiplier only: No Softvid");

		output (msg_softvid, "\tRevision key: %llu", (val & (1<<7|1<<6|1<<5|1<<4)) >> 4);
		if (val & (1<<8))
			output (msg_softvid, "\tEnableSoftBusRatio=Enabled");
		if (val & (1<<9))
			output (msg_softvid, "\tEnableSoftVid=Enabled");
		if (val & (1<<10))
			output (msg_softvid, "\tEnableSoftBSEL=Enabled");

		output (msg_softvid, "\tSoftBusRatio=%llu", val & 1<<14);
		if (val & 0xf)
			output (msg_softvid, "\tVRM Rev=%s",
				((val & 1<<15)) ? "Mobile VRM" : "VRM 8.5");

		val = val>>32;
		binary32 (val);
		output (msg_softvid, "\tMaxMHzBR: %s%s%s%s",
			(((val & (1<<3)) >>3) ? "1" : "0"),
			(((val & (1<<2)) >>2) ? "1" : "0"),
			(((val & (1<<1)) >>1) ? "1" : "0"),
			(((val & (1<<0)) >>0) ? "1" : "0"));
		output (msg_softvid, "\tMaximumVID: %s%s%s%s%s",
			(((val & (1<<8)) >>8) ? "1" : "0"),
			(((val & (1<<7)) >>7) ? "1" : "0"),
			(((val & (1<<6)) >>6) ? "1" : "0"),
			(((val & (1<<5)) >>5) ? "1" : "0"),
			(((val & (1<<4)) >>4) ? "1" : "0"));
		output (msg_softvid, "\tMaxMHzFSB: %s%s",
			(((val & (1<<10)) >>10) ? "1" : "0"),
			(((val & (1<<9))  >>9) ? "1" : "0"));
		output (msg_softvid, "\tMinMHzBR: %s%s%s%s",
			(((val & (1<<19)) >>19) ? "1" : "0"),
			(((val & (1<<18)) >>18) ? "1" : "0"),
			(((val & (1<<17)) >>17) ? "1" : "0"),
			(((val & (1<<16)) >>16) ? "1" : "0"));
		output (msg_softvid, "\tMinimumVID: %s%s%s%s",
			(((val & (1<<23)) >>23) ? "1" : "0"),
			(((val & (1<<22)) >>22) ? "1" : "0"),
			(((val & (1<<21)) >>21) ? "1" : "0"),
			(((val & (1<<20)) >>20) ? "1" : "0"));
		output (msg_softvid, "\tMinMHzFSB: %s%s",
			(((val & (1<<26)) >>26) ? "1" : "0"),
			(((val & (1<<25)) >>25) ? "1" : "0"));
	}
}
