/*
 *  $Id: x86info.c,v 1.3 2003-04-16 12:09:04 root Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#ifndef linux

#ifdef __WIN32__
#include <sys/types.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#endif /* __WIN32__ */

#endif /* linux */

#include "x86info.h"

emitFunct emit = NULL;
char *accumString = NULL;

int show_msr=0;
int show_registers=0;
int show_flags=0;
int verbose=0;
int callback=0;
int show_eblcr=0;
int show_cacheinfo=0;
int show_bluesmoke=0;
int show_mtrr=0;
int show_connector=0;
int show_urls=0;

static int show_mptable=0;
static int show_MHz=0;

int silent = 0;
int used_UP = 0;
int user_is_root = 1;
int need_root = 0;

unsigned int nrCPUs=1, nrSMPCPUs;

void usage (char *programname)
{
	output (msg_usage, "Usage: %s [<switches>]\n\
-a,   --all\n\
-c,   --cacheinfo\n\
      --connector\n\
-f,   --flags\n\
-v,   --verbose\n\
-mhz, --mhz\n\
-mp,  --mptable\n\
-m,   --msr\n\
      --mult\n\
      --mtrr\n\
-r,   --registers\n\
-s,   --show-bluesmoke\n\
-u,   --urls\n", 
		programname);
	exit (0);
}

static void parse_command_line (int argc, char **argv)
{
	char **argp, *arg;

	for (argp = argv+1; argp <= argv + argc && (arg = *argp); argp++) {
		if ((!strcmp(arg, "-a") || !strcmp(arg, "--all"))) {
			show_bluesmoke =1;
			show_cacheinfo = 1;
			show_eblcr =1;
			show_flags = 1;
			show_mptable =1;
			show_msr = 1;
			show_MHz = 1;
			show_registers = 1;
			show_mtrr = 1;
			show_connector = 1;
			show_urls = 1;
			need_root = 1;
		}

		if ((!strcmp(arg, "-c") || !strcmp(arg, "--cache")))
			show_cacheinfo = 1;

		if (!strcmp(arg, "--connector"))
			show_connector = 1;

		if ((!strcmp(arg, "-f") || !strcmp(arg, "--flags")))
			show_flags = 1;

		if ((!strcmp(arg, "-m") || !strcmp(arg, "--msr"))) {
			need_root = 1;
			show_msr = 1;
		}

		if (!strcmp(arg, "--mult")) {
			need_root = 1;
			show_eblcr = 1;
		}

		if ((!strcmp(arg, "-mhz") || !strcmp(arg, "--mhz")))
			show_MHz = 1;

		if ((!strcmp(arg, "-mp") || !strcmp(arg, "--mptable"))) {
			need_root = 1;
			show_mptable = 1;
		}

		if (!strcmp(arg, "--mtrr")) {
			need_root = 1;
			show_mtrr = 1;
		}

		if ((!strcmp(arg, "-r") || !strcmp(arg, "--registers")))
			show_registers = 1;

		if ((!strcmp(arg, "-s") || !strcmp(arg, "--show-bluesmoke"))) {
			need_root = 1;
			show_bluesmoke = 1;
		}

		if ((!strcmp(arg, "-u") || !strcmp(arg, "--urls")))
			show_urls = 1;

		if ((!strcmp(arg, "-v") || !strcmp(arg, "--verbose")))
			verbose = 1;

		if ((!strcmp(arg, "?") || !strcmp(arg, "--help")))
			usage(argv[0]);	
	}
}


int x86info (int argc, char **argv)
{
	unsigned int i;
	struct cpudata cpu;

	memset (&cpu, 0, sizeof(struct cpudata));
	parse_command_line(argc, argv);
	if (!silent) {
		output (msg_author, "x86info v1.11.  Dave Jones 2001, 2002");
		output (msg_author, "Feedback to <davej@suse.de>.");
		output (msg_author, "wxWindows front end by Michael Still 2003");
		output (msg_author, "Feedback to <mikal@stillhq.com>.");
		output (msg_format, "\n");
	}

	if ((HaveCPUID())==0) {
		output (msg_warning, "No CPUID instruction available.");
		output (msg_warning, "No further information available for this CPU.");
		return(0);
	}

	if (getuid()!=0)
		user_is_root=0;

	if (need_root && !user_is_root)
		output (msg_warning, "Need to be root to use specified options.");

#if defined __WIN32__
	{
		SYSTEM_INFO sys_info;
		GetSystemInfo(&sys_info);
		nrCPUs = sys_info.dwNumberOfProcessors;
	}
#elif defined _SC_NPROCESSORS_CONF	/* Linux */
	nrCPUs = sysconf (_SC_NPROCESSORS_CONF);
#else /* BSD */
	{
		int mib[2] = { CTL_HW, HW_NCPU };
		size_t len;
		len = sizeof(nrCPUs);
		sysctl(mib, 2, &nrCPUs, &len, NULL, 0);
	}
#endif /* __WIN32__ */

	if (!silent) {
		output (msg_numcpu, "Found %u CPU%s.", nrCPUs, nrCPUs > 1 ? "s" : "");

		/* Check mptable if present. This way we get number of CPUs
		   on SMP systems that have booted UP kernels. */
		if (user_is_root) {
			issmp (&nrSMPCPUs, 0);
			if (nrSMPCPUs > nrCPUs) {
				output (msg_mptable, "Found %d CPUs in MPTable.", nrSMPCPUs);
			}
		}
	}

	/*
	 * can't have less than 1 CPU, or more than
	 * 65535 (some arbitrary large number)
	 */
	if ((nrCPUs < 1) || (nrCPUs > 65535)) {
		output (msg_warning, 
			"CPU count is bogus: defaulting to 1 CPU.");
		nrCPUs = 1;
	}

	if (show_mptable && user_is_root)
		issmp (&nrSMPCPUs, 1);

	for (i=0; i<nrCPUs; i++) {
		cpu.number = i;

		if (!silent && nrCPUs!=1)
			output (msg_begincpu, "CPU #%u", i+1);
		else
		  output (msg_begincpu, "");

		identify (&cpu);
		show_info (&cpu);

		/*
		 * Doing this per-cpu is a problem, as we can't
		 * schedule userspace code per-cpu.
		 * Although running nrCPUs * threads would probably work.
		 */
		if (show_MHz)
			estimate_MHz(i);
	}

	if (nrCPUs > 1 && used_UP==1 && (!silent)) {
		output (msg_smpup, "WARNING: Detected SMP, but unable to access cpuid driver.");
		output (msg_smpup, "Used single CPU routines. Results inaccurate.");
	}

	return (0);
}

// TODO Mikal: fix indentation
/* Controls all output from the detection routines, so that it can be redirected to the GUI */
void output(int level, char *format, ...)
{
  char *out;
  char ostr[1024];
  va_list argPtr;

  if(strcmp(format, "") == 0)
    {
      out = accumString;
    }
  else
    {
      // Deliberate truncation of string if it is too long
      va_start (argPtr, format);
      vsnprintf (ostr, 1024, format, argPtr);
      
      // If this is an accumulate, the put it into the global string
      if(level == msg_accumulate){
	int alen = 0;
	if(accumString != NULL)
	  alen = strlen(accumString);

	if((accumString = realloc(accumString, alen + strlen(ostr) + 1)) 
	   == NULL){
	  printf("Memory allocation error\n");
	  exit(1);
	}
	
	strcpy(accumString + alen, ostr);
	return;
      }

      out = ostr;
    }

  // Now decide how to output the string
  if(emit != NULL){
    if(level != msg_format)
      emit(level, out);
  }
  else if(strcmp(ostr, "") == 0)
    return;
  else if(level == msg_error)
    fprintf(stderr, "%sn", out);
  else if(level != msg_format)
    printf("%s\n", out);
  else
    printf("%s", out);

  if(strcmp(format, "") == 0){
    free(accumString);
    accumString = NULL;
  }
}

/* Setup emit as required */
void setEmit(emitFunct e)
{
  callback = 1;
  emit = e;
}
