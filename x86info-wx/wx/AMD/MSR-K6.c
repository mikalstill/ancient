/*
 *  $Id: MSR-K6.c,v 1.3 2003-04-13 22:12:33 root Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  AMD-K6 specific MSR information
 *  See 21329h1.pdf for more details.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "../x86info.h"
#include "AMD.h"

void dump_k6_MSR (struct cpudata *cpu)
{
	unsigned long long val=0;

	if (!user_is_root)
		return;

	output(msg_format, "\t\t\t\t31       23       15       7 \n");
	dumpmsr(cpu->number, 0xC0000082, 32);

	/* Original K6 or K6-2 (old core). */
	if ((cpu->model < 8) || ((cpu->model==8) && (cpu->stepping <8))) {
		if (read_msr (cpu->number, 0xC0000082, &val) == 1) {
			output (msg_waenable, "Write allocate enable limit: %dMbytes", 
				(int) ((val & 0x7e) >>1) * 4);
			output (msg_wambytes, "Write allocate 15-16M bytes: %s", 
				val & 1 ? "enabled" : "disabled");
		} else {
			output (msg_warning, "Couldn't read WHCR register.");
		}
	}

	/* K6-2 core (Stepping 8-F), K6-III or later. */
	if ((cpu->model > 8) || ((cpu->model==8) && (cpu->stepping>=8))) {
		if (read_msr (cpu->number, 0xC0000082, &val) == 1) {
			if (!(val & (0x3ff << 22)))
				output (msg_waenable, "Write allocate disabled");
			else {
				output (msg_waenable, "Write allocate enable limit: %dMbytes", 
					(int) ((val >> 22) & 0x3ff) * 4);
				output (msg_wambytes, "Write allocate 15-16M bytes: %s", 
					val & (1<<16) ? "enabled" : "disabled");
			}
		} else {
			output (msg_warning, "Couldn't read WHCR register.");
		}
	}

	/* Dump EWBE register on K6-2 & K6-3 */
	if ((cpu->family==5) && (cpu->model>=8)) {
		if (read_msr (cpu->number, 0xC0000080, &val) == 1) {
			if (val & (1<<0))
				output (msg_syscallextn, "System call extension present.");
			if (val & (1<<1))
				output (msg_dataprefetch, "Data prefetch enabled.");
			else
				output (msg_dataprefetch, "Data prefetch disabled.");
			output (msg_format, "EWBE mode: ");
			switch ((val & (1<<2|1<<3|1<<4))>>2) {
				case 0:	output(msg_ewbemode, "strong ordering (slowest performance)");
					break;
				case 1:	output(msg_ewbemode, "speculative disable (close to best performance)");
					break;
				case 2:	output(msg_ewbemode, "invalid");
					break;
				case 3:	output(msg_ewbemode, "global disable (best performance)");
					break;
			}
		} else {
			output (msg_warning, "Couldn't read EFER register.");
		}
	}

	output (msg_format, "\n");
}
