/*
 *  $Id: intel_identify.c,v 1.2 2003-04-14 13:23:26 root Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 * Intel specific parts
 *
 * References:
 *	http://developer.intel.com/
 *	http://microcodes.sourceforge.net/CPUID.htm
 *
 */

#include <stdio.h>
#include <string.h>
#include "../x86info.h"
#include "Intel.h"

static char p4_423_datasheet[]="http://developer.intel.com/design/pentium4/datashts/24919805.pdf";
static char p4_478_datasheet[]="http://developer.intel.com/design/pentium4/datashts/24988703.pdf\n\thttp://developer.intel.com/design/pentium4/datashts/29864304.pdf";
static char p4_errata[]="http://developer.intel.com/design/pentium4/specupdt/24919928.pdf";

/* Decode Intel TLB and cache info descriptors */
//TODO : Errata workaround. http://www.sandpile.org/post/msgs/20002736.htm
void decode_Intel_TLB (int x, int family)
{
	switch (x & 0xff) {
	case 0:
		break;
	case 0x1:
		output (msg_insttlb, "Instruction TLB: 4KB pages, 4-way associative, 32 entries");
		break;
	case 0x2:
		output (msg_insttlb, "Instruction TLB: 4MB pages, fully associative, 2 entries");
		break;
	case 0x3:
		output (msg_datatlb, "Data TLB: 4KB pages, 4-way associative, 64 entries");
		break;
	case 0x4:
		output (msg_datatlb, "Data TLB: 4MB pages, 4-way associative, 8 entries");
		break;
	case 0x6:
		output (msg_l1instcache, "L1 Instruction cache:\n\tSize: 8KB\t4-way associative.\n\tline size=32 bytes.");
		break;
	case 0x8:
		output (msg_l1instcache, "L1 Instruction cache:\n\tSize: 16KB\t4-way associative.\n\tline size=32 bytes.");
		break;
	case 0xa:
		output (msg_l1datacache, "L1 Data cache:\n\tSize: 8KB\t2-way associative.\n\tline size=32 bytes.");
		break;
	case 0xc:
		output (msg_l1datacache, "L1 Data cache:\n\tSize: 16KB\t4-way associative.\n\tline size=32 bytes.");
		break;
	case 0x22:
		output (msg_l3cache, "L3 unified cache:\n\tSize: 512KB\t4-way associative.\n\tline size=64 bytes.");
		break;
	case 0x23:
		output (msg_l3cache, "L3 unified cache:\n\tSize: 1MB\t8-way associative.\n\tline size=64 bytes.");
		break;
	case 0x25:
		output (msg_l3cache, "L3 unified cache:\n\tSize: 2MB\t8-way associative.\n\tline size=64 bytes.");
		break;
	case 0x29:                                                                                                    
		output (msg_l3cache, "L3 unified cache:\n\tSize: 4MB\t8-way associative.\n\tline size=64 bytes.");               
		break;                                                                                                
	case 0x39:                                                                                                    
		output (msg_l2cache, "L2 unified cache:\n\tSize: 128KB\t4-way associative.\n\tline size=64 bytes.");             
		break;                                                                                                
	case 0x3c:                                                                                                    
		output (msg_l2cache, "L2 unified cache:\n\tSize: 256KB\t4-way associative.\n\tline size=64 bytes.");             
		break;
	case 0x40:
		if (family==15)
			output (msg_l3cache, "No L3 cache");	/* Pentium 4 */
		else
			output (msg_l3cache, "No L2 cache");
		break;
	case 0x41:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 128KB\t4-way associative.\n\tline size=32 bytes.");
		break;
	case 0x42:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 256KB\t4-way associative.\n\tline size=32 bytes.");
		break;
	case 0x43:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 512KB\t4-way associative.\n\tline size=32 bytes.");
		break;
	case 0x44:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 1MB\t4-way associative.\n\tline size=32 bytes.");
		break;
	case 0x45:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 2MB\t4-way associative.\n\tline size=32 bytes.");
		break;
	case 0x50:
		output (msg_insttlb, "Instruction TLB: 4K, 2MB or 4MB pages, fully associative, 64 entries.");
		break;
	case 0x51:
		output (msg_insttlb, "Instruction TLB: 4K, 2MB or 4MB pages, fully associative, 128 entries.");
		break;
	case 0x52:
		output (msg_insttlb, "Instruction TLB: 4K, 2MB or 4MB pages, fully associative, 256 entries.");
		break;
	case 0x5b:
		output (msg_datatlb, "Data TLB: 4KB or 4MB pages, fully associative, 64 entries.");
		break;
	case 0x5c:
		output (msg_datatlb, "Data TLB: 4KB or 4MB pages, fully associative, 128 entries.");
		break;
	case 0x5d:
		output (msg_datatlb, "Data TLB: 4KB or 4MB pages, fully associative, 256 entries.");
		break;
	case 0x66:
		output (msg_l1datacache, "L1 Data cache:\n\tSize: 8KB\tSectored, 4-way associative.\n\tline size=64 bytes.");
		break;
	case 0x67:
		output (msg_l1datacache, "L1 Data cache:\n\tSize: 16KB\tSectored, 4-way associative.\n\tline size=64 bytes.");
		break;
	case 0x68:
		output (msg_l1datacache, "L1 Data cache:\n\tSize: 32KB\tSectored, 4-way associative.\n\tline size=64 bytes.");
		break;
	case 0x70:
		output (msg_insttracecache, "Instruction trace cache:\n\tSize: 12K uOps\t8-way associative.");
		break;
	case 0x71:
		output (msg_insttracecache, "Instruction trace cache:\n\tSize: 16K uOps\t8-way associative.");
		break;
	case 0x72:
		output (msg_insttracecache, "Instruction trace cache:\n\tSize: 32K uOps\t8-way associative.");
		break;
	case 0x79:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 128KB\tSectored, 8-way associative.\n\tline size=64 bytes.");
		break;
	case 0x7a:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 256KB\tSectored, 8-way associative.\n\tline size=64 bytes.");
		break;
	case 0x7b:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 512KB\tSectored, 8-way associative.\n\tline size=64 bytes.");
		break;
	case 0x7c:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 1MB\tSectored, 8-way associative.\n\tline size=64 bytes.");
		break;
	case 0x82:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 256KB\t8-way associative.\n\tline size=32 bytes.");
		break;
	case 0x83:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 512KB\t8-way associative.\n\tline size=32 bytes.");
		break;
	case 0x84:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 1MB\t8-way associative.\n\tline size=32 bytes.");
		break;
	case 0x85:
		output (msg_l2cache, "L2 unified cache:\n\tSize: 2MB\t8-way associative.\n\tline size=32 bytes.");
		break;

	default:
		output (msg_warning, "unknown TLB/cache descriptor:\n\t0x%x", (x & 0xff));
		break;
	}
}


/* Intel-specific information */
void Identify_Intel (struct cpudata *cpu)
{
	char *nameptr;
	unsigned long eax, ebx, ecx, edx;
	int reserved;

	cpu->vendor = VENDOR_INTEL;
	nameptr = cpu->name;

	if (cpu->maxi < 1)
		return;

	/* Family/model/type etc */
	cpuid (cpu->number, 1, &eax, &ebx, &ecx, &edx);
	cpu->stepping = eax & 0xf;
	cpu->model = (eax >> 4) & 0xf;
	cpu->family = (eax >> 8) & 0xf;
	cpu->type = (eax >> 12) & 0x3;
	cpu->brand = (ebx & 0xf);
	reserved = eax >> 14;

	get_intel_cacheinfo (cpu);

	switch (cpu->family) {
	case 4:	nameptr += sprintf (cpu->name, "%s", "i486 ");
			break;
	case 5: nameptr += sprintf (cpu->name, "%s", "Pentium ");
			break;
	}

	switch (tuple(cpu) & 0xff0) {
	case 0x400:		/* Family 4 */
		nameptr+=sprintf (nameptr, "%s", "DX-25/33");
		break;
	case 0x410:
		nameptr+=sprintf (nameptr, "%s", "DX-50");
		break;
	case 0x420:
		nameptr+=sprintf (nameptr, "%s", "SX");
		break;
	case 0x430:
		nameptr+=sprintf (nameptr, "%s", "487/DX2");
		break;
	case 0x440:
		nameptr+=sprintf (nameptr, "%s", "SL");
		break;
	case 0x450:
		nameptr+=sprintf (nameptr, "%s", "SX2");
		break;
	case 0x470:
		nameptr+=sprintf (nameptr, "%s", "write-back enhanced DX2");
		break;
	case 0x480:
		nameptr+=sprintf (nameptr, "%s", "DX4");
		cpu->connector = CONN_SOCKET_3;
		break;
	case 0x490:
		nameptr+=sprintf (nameptr, "%s", "write-back enhanced DX4");
		cpu->connector = CONN_SOCKET_3;
		break;

	case 0x500:
		nameptr+=sprintf (nameptr, "%s", "A-step");
		cpu->connector = CONN_SOCKET_4;
		break;
	case 0x510:
		nameptr+=sprintf (nameptr, "%s", "60/66");
		cpu->connector = CONN_SOCKET_4;
		break;
	case 0x520:
		nameptr+=sprintf (nameptr, "%s", "75-200");
		cpu->connector = CONN_SOCKET_5_7;
		break;
	case 0x530:
		nameptr+=sprintf (nameptr, "%s", "Overdrive");
		cpu->connector = CONN_SOCKET_4;
		break;
	case 0x540:
		nameptr+=sprintf (nameptr, "%s", "MMX");
		cpu->connector = CONN_SOCKET_7;
		break;
	case 0x570:
		nameptr+=sprintf (nameptr, "%s", "Mobile");
		cpu->connector = CONN_SOCKET_7;
		break;
	case 0x580:
		nameptr+=sprintf (nameptr, "%s", "MMX Mobile");
		cpu->connector = CONN_SOCKET_7;
		break;
	case 0x600:
		nameptr+=sprintf (cpu->name, "%s", "Pentium Pro A-Step");
		cpu->connector = CONN_SOCKET_8;
		break;
	case 0x610:
		nameptr+=sprintf (cpu->name, "%s", "Pentium Pro");
		cpu->connector = CONN_SOCKET_8;
		switch (cpu->stepping) {
		case 1:
			nameptr+=sprintf (nameptr, "%s", "[B0]");
			break;
		case 2:
			nameptr+=sprintf (nameptr, "%s", "[C0]");
			break;
		case 6:
			nameptr+=sprintf (nameptr, "%s", "[sA0]");
			break;
		case 7:
			nameptr+=sprintf (nameptr, "%s", "[sA1]");
			break;
		case 9:
			nameptr+=sprintf (nameptr, "%s", "[sB1]");
			break;
		}
		break;
	case 0x630:
		nameptr+=sprintf (cpu->name, "%s", "Pentium II ");
		cpu->connector = CONN_SLOT_1;
		switch (cpu->stepping) {
		case 2:
			nameptr+=sprintf (nameptr, "%s", "Overdrive [tdB0]");
			break;
		case 3:
			nameptr+=sprintf (nameptr, "%s", "(Klamath) [C0]");
			break;
		case 4:
			nameptr+=sprintf (nameptr, "%s", "(Klamath) [C1]");
			break;
		}
		break;
	case 0x640:
		cpu->connector = CONN_SLOT_1;
		nameptr+=sprintf (cpu->name, "%s", "Pentium II (Deschutes?)");
		break;
	case 0x650:
		cpu->connector = CONN_SLOT_1;
		if (cpu->cachesize_L2 == 0) {
			nameptr+=sprintf (cpu->name, "%s", "Celeron (Covington)");
			break;
		}
		if (cpu->cachesize_L2 == 256) {
			nameptr+=sprintf (cpu->name, "%s", "Mobile Pentium II (Dixon)");
			break;
		}
		switch (cpu->stepping) {
		case 0:
			nameptr+=sprintf (cpu->name, "%s", "Pentium II [dA0]");
			break;
		case 1:
			nameptr+=sprintf (cpu->name, "%s", "Pentium II (Deschutes) [dA1]");
			break;
		case 2:
			nameptr+=sprintf (cpu->name, "%s", "Pentium II (Deschutes) [dB0]");
			break;
		case 3:
			nameptr+=sprintf (cpu->name, "%s", "Pentium II (Deschutes) [dB1]");
			break;
		default:
			nameptr+=sprintf (cpu->name, "%s", "Pentium II");
			break;
		}
		break;
	case 0x660:
		cpu->connector = CONN_SOCKET_370;
		if (cpu->cachesize_L2 == 128) {
			nameptr+=sprintf (cpu->name, "%s", "Celeron (Mendocino)");
			break;
		}
		switch (cpu->stepping) {
		case 0:
			nameptr+=sprintf (cpu->name, "%s", "Celeron-A [mA0]");
			break;
		case 5:
			nameptr+=sprintf (cpu->name, "%s", "Celeron-A [mB0]");
			break;
		case 0xA:
			nameptr+=sprintf (cpu->name, "%s", "Mobile Pentium II [mdA0]");
			break;
		default:
			nameptr+=sprintf (cpu->name, "%s", "Celeron / Mobile Pentium II");
			break;
		}
		break;
	case 0x670:
		cpu->connector = CONN_SLOT_1;
		switch (cpu->stepping) {
		case 2:
			nameptr+=sprintf (cpu->name, "%s", "Pentium III [kB0]");
			break;
		case 3:
			nameptr+=sprintf (cpu->name, "%s", "Pentium III (Katmai) [kC0]");
			break;
		default:
			nameptr+=sprintf (cpu->name, "%s", "Pentium III/Pentium III Xeon");
			break;
		}
		break;
	case 0x680:
		cpu->connector = CONN_SOCKET_370_FCPGA;
		if (cpu->cachesize_L2 == 128) {
			nameptr+=sprintf (cpu->name, "%s", "Celeron");
		} else {
			nameptr+=sprintf (cpu->name, "%s", "Pentium III");
		}
		nameptr+=sprintf (nameptr, "%s", " (Coppermine)");
		switch (cpu->stepping) {
		case 1:
			nameptr+=sprintf (nameptr, "%s", " [cA2]");
			break;
		case 3:
			nameptr+=sprintf (nameptr, "%s", " [cB0]");
			break;
		case 6:
			nameptr+=sprintf (nameptr, "%s", " [cC0]");
			break;
		case 0xA:
			nameptr+=sprintf (nameptr, "%s", " [cD0]");
			break;
		}
		break;
	case 0x6A0:
		cpu->connector = CONN_SLOT_1;
		switch (cpu->brand) {
		case 0:
			nameptr+=sprintf (cpu->name, "%s", "Pentium II (Deschutes)");
			break;
		case 1:
			nameptr+=sprintf (cpu->name, "%s", "Celeron");
			break;
		case 2:
			nameptr+=sprintf (cpu->name, "%s", "Pentium III");
			break;
		case 3:
			nameptr+=sprintf (cpu->name, "%s", "Pentium III Xeon");
			break;
		case 4:
			nameptr+=sprintf (cpu->name, "%s", "Pentium III (Cascades)");
			break;
		default:
			nameptr+=sprintf (cpu->name, "%s", "Unknown CPU");
			break;
		}
		break;
	case 0x6B0:
		cpu->connector = CONN_SLOT_1;
		switch (cpu->stepping) {
		case 1:
			nameptr+=sprintf (cpu->name, "%s", "Celeron / Pentium III (Tualatin) [tA1/cA2]");
			break;
		case 4:
			nameptr+=sprintf (cpu->name, "%s", "Celeron / Pentium III [B-1]");
			break;
		default:
			nameptr+=sprintf (cpu->name, "%s", "Unknown CPU");
			break;
		}
		break;

	case 0x700:		/* Family 7 */
		nameptr += sprintf (cpu->name, "%s", "Itanium");
		break;

	case 0xF00:	/* Family 15 */
		cpu->connector = CONN_SOCKET_423;
		cpu->datasheet_url = strdup (p4_423_datasheet);
		cpu->errata_url = strdup (p4_errata);
		nameptr += sprintf (cpu->name, "%s", "Pentium 4");
		switch (cpu->stepping) {
		case 7:
			nameptr+=sprintf (nameptr, "%s", " [B2]");
			break;
		case 0xA:
			nameptr+=sprintf (nameptr, "%s", " [C1]");
			break;
		}
		break;

	case 0xF10:
		cpu->connector = CONN_SOCKET_423;
		nameptr += sprintf (cpu->name, "%s", "Pentium 4 (Willamette)");
		cpu->datasheet_url = strdup (p4_423_datasheet);
		cpu->errata_url = strdup (p4_errata);
		switch (cpu->stepping) {
		case 1:
			nameptr+=sprintf (nameptr, "%s", " [C0]");
			break;
		case 2:
			nameptr+=sprintf (nameptr, "%s", " [D0]");
			break;
		case 3:
			nameptr+=sprintf (nameptr, "%s", " [E0]");
			break;
		}
		break;
	case 0xF20:
		cpu->connector = CONN_SOCKET_478;
		cpu->datasheet_url = strdup (p4_478_datasheet);
		cpu->errata_url = strdup (p4_errata);
		nameptr += sprintf (cpu->name, "%s", "Pentium 4 Xeon (Northwood)");
		switch (cpu->stepping) {
		case 4:
			nameptr+=sprintf (nameptr, "%s", " [B0]");
			break;
		case 7:
			nameptr+=sprintf (nameptr, "%s", " [C1]");
			break;
		}
		break;
	case 0xF40:
	case 0xF50:
		cpu->connector = CONN_SOCKET_478;
		cpu->datasheet_url = strdup (p4_478_datasheet);
		cpu->errata_url = strdup (p4_errata);
		nameptr+=sprintf (cpu->name, "%s", "P4 Xeon (Foster)");
		break;
	default:
		nameptr+=sprintf (cpu->name, "%s", "Unknown CPU");
		break;
	}

	switch (cpu->type) {
	case 0:
		sprintf (nameptr, "%s", " Original OEM");
		break;
	case 1:
		sprintf (nameptr, "%s", " Overdrive");
		break;
	case 2:
		sprintf (nameptr, "%s", " Dual-capable");
		break;
	case 3:
		sprintf (nameptr, "%s", " Reserved");
		break;
	}
}


void display_Intel_info (struct cpudata *cpu)
{
	int ntlb, i;
	unsigned long eax, ebx, ecx, edx;

	output (msg_cpuinfo, "Family: %d Model: %d Stepping: %d Type: %d",
		cpu->family, cpu->model, cpu->stepping, cpu->type);
	output (msg_cpuinfo, "CPU Model: %s", cpu->name);

	/* Pentium4 and above have cpu name. */
	if (cpu->family == 0xF)
		get_model_name (cpu);

	decode_feature_flags (cpu);

	if (cpu->maxi >= 2 && show_cacheinfo) {
		/* Decode TLB and cache info */
		ntlb = 255;
		for (i = 0; i < ntlb; i++) {
			cpuid (cpu->number, 2, &eax, &ebx, &ecx, &edx);
			ntlb = eax & 0xff;
			decode_Intel_TLB (eax >> 8, cpu->family);
			decode_Intel_TLB (eax >> 16, cpu->family);
			decode_Intel_TLB (eax >> 24, cpu->family);

			if ((ebx & 0x80000000) == 0) {
				decode_Intel_TLB (ebx, cpu->family);
				decode_Intel_TLB (ebx >> 8, cpu->family);
				decode_Intel_TLB (ebx >> 16, cpu->family);
				decode_Intel_TLB (ebx >> 24, cpu->family);
			}
			if ((ecx & 0x80000000) == 0) {
				decode_Intel_TLB (ecx, cpu->family);
				decode_Intel_TLB (ecx >> 8, cpu->family);
				decode_Intel_TLB (ecx >> 16, cpu->family);
				decode_Intel_TLB (ecx >> 24, cpu->family);
			}
			if ((edx & 0x80000000) == 0) {
				decode_Intel_TLB (edx, cpu->family);
				decode_Intel_TLB (edx >> 8, cpu->family);
				decode_Intel_TLB (edx >> 16, cpu->family);
				decode_Intel_TLB (edx >> 24, cpu->family);
			}
		}
		output (msg_format, "\n");
	}

	if (cpu->maxi >= 3) {
		/* Pentium III CPU serial number */
		unsigned long signature;
		cpuid (cpu->number, 1, &eax, NULL, NULL, NULL);
		signature = eax;

		cpuid (cpu->number, 3, &eax, &ebx, &ecx, &edx);
		output (msg_accumulate, "Processor serial: ");
		output (msg_accumulate, "%04lX", signature >> 16);
		output (msg_accumulate, "-%04lX", signature & 0xffff);
		output (msg_accumulate, "-%04lX", edx >> 16);
		output (msg_accumulate, "-%04lX", edx & 0xffff);
		output (msg_accumulate, "-%04lX", ecx >> 16);
		output (msg_accumulate, "-%04lX", ecx & 0xffff);
		output (msg_cpuinfo, "");
	}

	if (show_eblcr) {
		if (cpu->family == 6 && cpu->model >= 3) {
			unsigned long long eblcr;
			read_msr (cpu->number, 0x2A, &eblcr);
			interpret_eblcr(eblcr);
		}
	}

	/* FIXME: Bit test for MCA here!*/
	if (show_bluesmoke)
		decode_Intel_bluesmoke(cpu->number, cpu->family);

	/* Hyper-Threading Technology */
	if (cpu->flags & (1 << 28)) {
		int nr_ht = (cpu->eflags >> 16) & 0xFF;
		output (msg_cpuinfo, "Number of logical processors supported "
			"within the physical package: %d", nr_ht);
		output (msg_format, "\n");
	}
}
