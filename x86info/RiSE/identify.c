/*
 *  $Id: identify.c,v 1.1.1.1 2003-04-24 09:37:35 root Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  Rise specific parts.
 */
#include <stdio.h>
#include "../x86info.h"

void Identify_RiSE (struct cpudata *cpu)
{
	unsigned long eax, ebx, ecx, edx;
	cpu->vendor = VENDOR_RISE;

	/* Do standard stuff */
	if (cpu->maxi < 1)
		return;

	cpuid (cpu->number, 1, &eax, &ebx, &ecx, &edx);
	cpu->stepping = eax & 0xf;
	cpu->model = (eax >> 4) & 0xf;
	cpu->family = (eax >> 8) & 0xf;

	switch (tuple(cpu) & 0xff0) {
		case 0x500:	sprintf (cpu->name, "%s", "iDragon (0.25um)");
					break;
		case 0x520:	sprintf (cpu->name, "%s", "iDragon (0.18um)");
					break;
		case 0x580:	sprintf (cpu->name, "%s", "iDragon II (0.25um)");
					break;
		case 0x590:	sprintf (cpu->name, "%s", "iDragon II (0.18um)");
					break;
		default:	sprintf (cpu->name, "%s", "Unknown CPU");
					break;
	}
}


void display_RiSE_info(struct cpudata *cpu)
{
	printf ("Family: %d Model: %d Stepping: %d\n",
		cpu->family, cpu->model, cpu->stepping);
	printf ("CPU Model : %s\n", cpu->name);
	get_model_name (cpu);

	decode_feature_flags (cpu);
}
