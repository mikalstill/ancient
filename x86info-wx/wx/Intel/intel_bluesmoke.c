/*
 *  $Id: intel_bluesmoke.c,v 1.2 2003-04-14 13:23:26 root Exp $
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
#define MCG_CTL_PBIT	8
#define MCG_EXT_PBIT	9
#define MCG_STATUS 0x17a
#define MCG_CTL 0x17b
#define MCG_EXT 0x180

#define MC_CTL 0x0400
#define MC_STATUS 0x0401
#define MC_ADDR 0x402

#define PENTIUM4(family)	(family == 0xf)

void decode_Intel_bluesmoke(int cpunum, int family)
{
	unsigned long long val, val2;
	int banks, i, extcount = 0;

	if (!user_is_root)
		return;

	if (read_msr(cpunum, MCG_CAP, &val) != 1)
		return;

	banks = val & 0xff;

	output (msg_format, "\n");
	output (msg_reportingbanks, "Number of reporting banks : %d", banks);
	output (msg_format, "\n");

	if (PENTIUM4(family)) {
		if ((val & (1<<MCG_EXT_PBIT))) {
			extcount = (val >> 16) & 0xff;
			output (msg_mcreg, 
				"Number of extended MC registers : %d\n\n", 
				extcount);
		}
		else
			output (msg_warning, 
				"Erk, MCG_EXT not present! :%llx:\n", val);
	}
	else
	if ((val & (1<<MCG_CTL_PBIT)) == 0)
		output (msg_warning, "Erk, MCG_CTL not present! :%llx:\n", 
			val);

	if (read_msr(cpunum, MCG_CTL, &val) == 1) {
		output (msg_format, "MCG_CTL:\n");
		output (msg_datacache, 
			" Data cache check %sabled", 
			val & (1<<0) ? "en" : "dis");
		if ((val & (1<<0)) == 1) {
			if (read_msr(cpunum, MC_CTL, &val2) == 1) {
				output (msg_eccerror,
					"  ECC 1 bit error reporting %sabled",
					val2 & (1<<0) ? "en" : "dis");
				output (msg_eccerror, 
					"  ECC multi bit error reporting %sabled", 
					val2 & (1<<1) ? "en" : "dis");
				output (msg_dcparity,
					"  Data cache data parity %sabled", 
					val2 & (1<<2) ? "en" : "dis");
				output (msg_dcparity, 
					"  Data cache main tag parity %sabled", 
					val2 & (1<<3) ? "en" : "dis");
				output (msg_dcparity,
					"  Data cache snoop tag parity %sabled", 
					val2 & (1<<4) ? "en" : "dis");
				output (msg_tlbl1parity,
					"  L1 TLB parity %sabled", 
					val2 & (1<<5) ? "en" : "dis");
				output (msg_tlbl2parity,
					"  L2 TLB parity %sabled\n", 
					val2 & (1<<6) ? "en" : "dis");
			}
		}

		output (msg_instcache, " Instruction cache check %sabled", 
			val & (1<<1) ? "en" : "dis");
		if (((val & (1<<1)) == 2) && (banks>1)) {
			if (read_msr(cpunum, MC_CTL+4, &val2) == 1) {
				output (msg_eccerror,
					"  ECC 1 bit error reporting %sabled", 
					val2 & (1<<0) ? "en" : "dis");
				output (msg_eccerror, 
					"  ECC multi bit error reporting %sabled", 
					val2 & (1<<1) ? "en" : "dis");
				output (msg_instparity,
					"  Instruction cache data parity %sabled",
					val2 & (1<<2) ? "en" : "dis");
				output (msg_icparity,
					"  IC main tag parity %sabled", 
					val2 & (1<<3) ? "en" : "dis");
				output (msg_icparity,
					"  IC snoop tag parity %sabled", 
					val2 & (1<<4) ? "en" : "dis");
				output (msg_tlbl1parity,
					"  L1 TLB parity %sabled", 
					val2 & (1<<5) ? "en" : "dis");
				output (msg_tlbl2parity,
					"  L2 TLB parity %sabled", 
					val2 & (1<<6) ? "en" : "dis");
				output (msg_precodeparity,
					"  Predecode array parity %sabled", 
					val2 & (1<<7) ? "en" : "dis");
				output (msg_targselparity,
					"  Target selector parity %sabled", 
					val2 & (1<<8) ? "en" : "dis");
				output (msg_readerr,
					"  Read data error %sabled", 
					val2 & (1<<9) ? "en" : "dis");
			}
		}

		output (msg_busunit, " Bus unit check %sabled", 
			val & (1<<2) ? "en" : "dis");
		if ((val & (1<<2)) == 4 && (banks>2)) {
			if (read_msr(cpunum, MC_CTL+8, &val2) == 1) {
			  output (msg_extl2parity,
				  "  External L2 tag parity error %sabled", 
				  val2 & (1<<0) ? "en" : "dis");
				output (msg_l2err,
					"  L2 partial tag parity error %sabled", 
					val2 & (1<<1) ? "en" : "dis");
				output (msg_ecctlberror,
					"  System ECC TLB reload error %sabled", 
					val2 & (1<<2) ? "en" : "dis");
				output (msg_l2tlberror,
					"  L2 ECC TLB reload error %sabled", 
					val2 & (1<<3) ? "en" : "dis");
				output (msg_l2k7dealloc,
					"  L2 ECC K7 deallocate %sabled", 
					val2 & (1<<4) ? "en" : "dis");
				output (msg_l2eccprobe,
					"  L2 ECC probe deallocate %sabled", 
					val2 & (1<<5) ? "en" : "dis");
				output (msg_sysdatareaderr,
					"  System datareaderror reporting %sabled",
					val2 & (1<<6) ? "en" : "dis");
			}
		}

		output (msg_lsunit, " Load/Store unit check %sabled", 
			val & (1<<3) ? "en" : "dis");
		if ((val & (1<<3)) == 8 && (banks>3)) {
			if (read_msr(cpunum, MC_CTL+12, &val2) == 1) {
				output (msg_readerr, 
					"  Read data error enable (loads) %sabled",
					val2 & (1<<0) ? "en" : "dis");
				output (msg_readerr,
					"  Read data error enable (stores) %sabled",
					val2 & (1<<1) ? "en" : "dis");
			}
		}
	}
	output (msg_format, "\n");

	for (i=0; i<banks; i++) {
		output (msg_msrbank, "Bank: %d (0x%x)", i, MC_CTL+i*4);
		output (msg_accumulate, "MC%dCTL:    ", i);
		dumpmsr_bin (cpunum, MC_CTL+i*4, 64);
		output (msg_accumulate, "MC%dSTATUS: ", i);
		dumpmsr_bin (cpunum, MC_STATUS+i*4, 64);
		output (msg_accumulate, "MC%dADDR:   ", i);
		dumpmsr_bin (cpunum, MC_ADDR+i*4, 64);
		output (msg_format, "\n");
	}
}
