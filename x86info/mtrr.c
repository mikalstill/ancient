/*
 *  $Id: mtrr.c,v 1.1.1.1 2003-04-24 09:37:34 root Exp $
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
		printf ("0x%016llx\n", val);
}

void dump_mtrrs (struct cpudata *cpu)
{
	int i;

	if (!(cpu->flags & (X86_FEATURE_MTRR)))
		return;

	printf ("MTRR registers:\n");

	printf ("MTRRcap (0xfe): ");
	dump_mtrr (cpu->number, 0xfe);

	for (i=0; i<16; i+=2) {
		printf ("MTRRphysBase%d (0x%x): ", i/2, 0x200+i);
		dump_mtrr(cpu->number, 0x200+i);
		printf ("MTRRphysMask%d (0x%x): ", i/2, 0x201+i);
		dump_mtrr(cpu->number, 0x201+i);
	}

	printf ("MTRRfix64K_00000 (0x250): ");
	dump_mtrr (cpu->number, 0x250);
	printf ("MTRRfix16K_80000 (0x258): ");
	dump_mtrr (cpu->number, 0x258);
	printf ("MTRRfix16K_A0000 (0x259): ");
	dump_mtrr (cpu->number, 0x259);

	printf ("MTRRfix4K_C8000 (0x269): ");
	dump_mtrr (cpu->number, 0x269);
	printf ("MTRRfix4K_D0000 0x26a: ");
	dump_mtrr (cpu->number, 0x26a);
	printf ("MTRRfix4K_D8000 0x26b: ");
	dump_mtrr (cpu->number, 0x26b);
	printf ("MTRRfix4K_E0000 0x26c: ");
	dump_mtrr (cpu->number, 0x26c);
	printf ("MTRRfix4K_E8000 0x26d: ");
	dump_mtrr (cpu->number, 0x26d);
	printf ("MTRRfix4K_F0000 0x26e: ");
	dump_mtrr (cpu->number, 0x26e);
	printf ("MTRRfix4K_F8000 0x26f: ");
	dump_mtrr (cpu->number, 0x26f);

	printf ("MTRRdefType (0x2ff): ");
	dump_mtrr (cpu->number, 0x2ff);

	printf ("\n\n");
}
