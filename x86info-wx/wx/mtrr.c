/*
 *  $Id: mtrr.c,v 1.2 2003-04-13 21:11:55 root Exp $
 *  This file is part of x86info
 *  (C) 2002 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  MTRR register dumping.
 *  TODO : decode the registers too.
 */

#include <stdio.h>
#include "x86info.h"

static void dump_mtrr (int cpu, int msr)
{
	unsigned long long val=0;

	if (read_msr(cpu, msr, &val) == 1)
		output (msg_accumulate, "0x%016llx", val);
}

void dump_mtrrs (struct cpudata *cpu)
{
	int i;

	if (!(cpu->flags & (X86_FEATURE_MTRR)))
		return;

	output (msg_format, "MTRR registers:\n");
	output(msg_accumulate, "MTRRcap (0xfe): ");
	dump_mtrr (cpu->number, 0xfe);
	output (msg_mtrr, "");

	for (i=0; i<16; i+=2) {
		output(msg_accumulate, "MTRRphysBase%d (0x%x): ", i/2, 0x200+i);
		dump_mtrr(cpu->number, 0x200+i);
		output (msg_mtrr, "");

		output(msg_accumulate, "MTRRphysMask%d (0x%x): ", i/2, 0x201+i);
		dump_mtrr(cpu->number, 0x201+i);
		output (msg_mtrr, "");
	}

	output(msg_accumulate, "MTRRfix64K_00000 (0x250): ");
	dump_mtrr (cpu->number, 0x250);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix16K_80000 (0x258): ");
	dump_mtrr (cpu->number, 0x258);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix16K_A0000 (0x259): ");
	dump_mtrr (cpu->number, 0x259);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix4K_C8000 (0x269): ");
	dump_mtrr (cpu->number, 0x269);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix4K_D0000 0x26a: ");
	dump_mtrr (cpu->number, 0x26a);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix4K_D8000 0x26b: ");
	dump_mtrr (cpu->number, 0x26b);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix4K_E0000 0x26c: ");
	dump_mtrr (cpu->number, 0x26c);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix4K_E8000 0x26d: ");
	dump_mtrr (cpu->number, 0x26d);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix4K_F0000 0x26e: ");
	dump_mtrr (cpu->number, 0x26e);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRfix4K_F8000 0x26f: ");
	dump_mtrr (cpu->number, 0x26f);
	output (msg_mtrr, "");

	output(msg_accumulate, "MTRRdefType (0x2ff): ");
	dump_mtrr (cpu->number, 0x2ff);
	output (msg_mtrr, "");

	output (msg_format, "\n");
}
