/*
 *  $Id: features.c,v 1.2 2003-04-13 21:11:55 root Exp $
 *  This file is part of x86info
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  Feature flag decoding.
 */

#include <stdio.h>
#include "x86info.h"

void decode_feature_flags (struct cpudata *cpu)
{
	unsigned long eax, ebx, ecx, edx;
	int i;

	const char *generic_cap_flags[] = {
		"fpu", "vme", "de", "pse", "tsc", "msr", "pae", "mce",
		"cx8", "apic", NULL, "sep", "mtrr", "pge", "mca", "cmov",
		"pat", "pse36", "psn", "clflsh", NULL, "dtes", "acpi", "mmx",
		"fxsr", "sse", "sse2", "selfsnoop", "ht", "acc", "ia64", "pbe"
	};
	const char *generic_cap_flags_desc[] = {
		"Onboard FPU",
		"Virtual Mode Extensions",
		"Debugging Extensions",
		"Page Size Extensions",
		"Time Stamp Counter",
		"Model-Specific Registers",
		"Physical Address Extensions",
		"Machine Check Architecture",
		"CMPXCHG8 instruction",
		"Onboard APIC",
		NULL,
		"SYSENTER/SYSEXIT",
		"Memory Type Range Registers",
		"Page Global Enable",
		"Machine Check Architecture",
		"CMOV instruction",
		"Page Attribute Table",
		"36-bit PSEs",
		"Processor serial number",
		"CLFLUSH instruction",
		NULL,
		"Debug Trace Store",
		"ACPI via MSR",
		"MMX support",
		"FXSAVE and FXRESTORE instructions",
		"SSE support",
		"SSE2 support",
		"CPU self snoop",
		"Hyper-Threading",
		"Automatic clock Control",
		"IA-64 processor",
		"Pending Break Enable"
	};
	const char *amd_cap_flags[] = {
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, "syscall", NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, "mp", NULL, NULL, "mmxext", NULL,
		NULL, NULL, NULL, NULL, NULL, "lm", "3dnowext", "3dnow"
	};
	const char *centaur_cap_flags[] = {
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, "mmxext", NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, "3dnowext", "3dnow"
	};
	const char *transmeta_cap_flags[] = {
		"recovery", "longrun", NULL, "lrti", NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	};

	cpuid(cpu->number, 0x00000001, &eax, &ebx, &ecx, &edx);
	cpu->flags = edx;
	if (cpu->vendor==VENDOR_INTEL) {
		cpu->eflags = ebx;
	} else {
		if (cpu->maxei >= 0x80000001) {
			cpuid(cpu->number, 0x80000001, &eax, &ebx, &ecx, &edx);
			cpu->eflags = edx;
		}
	}

	if (show_flags == 0)
		return;

	output (msg_format, "Feature flags:\n");
	for (i = 0; i < 32; i++)
		if (cpu->flags & (1 << i)) {
			if (verbose || callback) {
				output (msg_feature, "\t%s", generic_cap_flags_desc[i]);
			} else {
				output (msg_format, " %s", generic_cap_flags[i]);
			}
		}

	output (msg_format, "\n");

	/* Vendor specific extensions. */
	switch (cpu->vendor) {

		case VENDOR_AMD:
			output (msg_format, "Extended feature flags:\n");
			for (i = 0; i < 32; i++) {
				if (cpu->eflags & (1 << i) && amd_cap_flags[i])
					output(msg_featureamd, "\t%s", amd_cap_flags[i]);
			}
			break;

		case VENDOR_CENTAUR:
			output (msg_format, "Extended feature flags:\n");
			for (i = 0; i < 32; i++) {
				if (cpu->eflags & (1 << i) && centaur_cap_flags[i])
					output (msg_featurecentaur, "\t%s", centaur_cap_flags[i]);
			}
			break;

		case VENDOR_TRANSMETA:
			output (msg_format, "Extended feature flags:\n");
			for (i = 0; i < 32; i++) {
				if (cpu->eflags & (1 << i) && transmeta_cap_flags[i])
					output (msg_featuretransmeta, "\t%s", transmeta_cap_flags[i]);
			}
			break;

		case VENDOR_CYRIX:
		        // msg_featurecyrix
			break;

		case VENDOR_INTEL:
	                // msg_featureintel
			break;

		default:
			/* Unknown CPU manufacturer or no special handling needed */
			break;
	}

	output (msg_format, "\n");
}
