/*
 *  $Id: natsemi_identify.c,v 1.2 2003-04-14 13:23:27 root Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  NatSemi specific parts.
 */
#include <stdio.h>
#include "../x86info.h"

void Identify_NatSemi (struct cpudata *cpu)
{
	unsigned long eax, ebx, ecx, edx;
	cpu->vendor = VENDOR_NATSEMI;

	/* Do standard stuff */
	if (cpu->maxi < 1)
		return;

	cpuid (cpu->number, 1, &eax, &ebx, &ecx, &edx);
	cpu->stepping = eax & 0xf;
	cpu->model = (eax >> 4) & 0xf;
	cpu->family = (eax >> 8) & 0xf;

	switch (tuple(cpu) & 0xff0) {
		case 0x540:	sprintf (cpu->name, "%s", "Geode GX1");
					break;
		default:	sprintf (cpu->name, "%s", "Unknown CPU");
					break;
	}
}


void display_NatSemi_info(struct cpudata *cpu)
{
	output (msg_cpuinfo, "Family: %d Model: %d Stepping: %d",
		cpu->family, cpu->model, cpu->stepping);
	output (msg_cpuinfo, "CPU Model : %s", cpu->name);
	get_model_name (cpu);

	decode_feature_flags (cpu);
}
