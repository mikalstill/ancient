// Dump a Snoopy Pro logfile to stdout, including possible repeated URB 
// suppression. Copyright (C) Michael Still 2003, released under the terms 
// of the GNU GPL. If you find an URB marked "UNKNOWN", please email me 
// the Snoopy Pro log, and the URB number of the problem URB.

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "fileutil.h"
#include "md5-global.h"
#include "md5.h"

#define VERSION "(Version 0.3)"
void usage(char *name)
{
  printf (VERSION "\n\n");
  printf ("Try: %s [-i input] [-r] [-v]\n\n", name);
  printf ("\t-i <name>: The name of the input usblog file\n");
  printf ("\t-l:        Output Linux kernel equivalent descriptions\n");
  printf ("\t-m:        Show MD5 hashes of URB descriptions\n");
  printf ("\t             (Only useful if you're debugging -r)\n");
  printf ("\t-r:        Suppress repeated URB sequences\n");
  printf ("\t-R:        -r, and show the match debugging information\n");
  printf ("\t-v:        Verbose debugging output\n");
  printf ("\n");
}

char *functname (unsigned int function);

// URB things
void usb_urb_header (char *file, long long *filep);
#define USB_URB_HEADER_LENGTH 16
void usb_urb_controltransfer (char *file, long long *filep);
void usb_urb_hcd (char *file, long long *filep);
#define USB_URB_HCD_LENGTH 32
void usb_urb_listentry (char *file, long long *filep);
void usb_interface_info (char *file, long long *filep);
void usb_pipe_info (char *file, long long *filep);
void usb_ucd (char *file, long long *filep);
void usb_setup_packet (char *file, long long *filep);

// URB output
char *urb_buffer = NULL;
long urb_buffer_size = 0;
long urb_buffer_inset = 0;
int urb_reallocs = 0;
void urb_printf (char *format, ...);
char *urb_xsnprintf (char *format, va_list ap);
void urb_xfree(void *);
char *md5hash (char *);

// A structure to store all our URBs
typedef struct usb_internal_allurbs
{
  int number;
  long long filep;
  int sequence;
  unsigned int time;
  int reallocs;
  int deleted;

  int rptlen;
  int rpttimes;
  int abend;

  char *desc;
  char *md5;

  // Information about the URB
  int endpoint;
}
usb_allurbs;

// Used for correlation
int hashcmp (usb_allurbs * head, int inset, int testinset, int length);

int do_linux = 0, do_showhash = 0;

int
main (int argc, char *argv[])
{
  int fd, npackets, otag, urbCount, function, psize, tsrelative, temp, temp2,
    numifaces, seq, length, inset, testinset, corrstep, matchcount, optchar,
    do_suppress = 0, do_showmatch = 0, do_verbose = 0;
  char *file, *input_filename = NULL;
  long long filep;
  struct stat sb;
  usb_allurbs *urbhead;

  while ((optchar = getopt (argc, argv, "i:lmrRv")) != -1)
    {
      switch (optchar)
	{
	case 'l':
	  do_linux = 1;
	  break;

	case 'm':
	  do_showhash = 1;
	  break;

	case 'R':
	  do_showmatch = 1;
	case 'r':
	  do_suppress = 1;
	  break;

	case 'v':
	  do_verbose = 1;
	  break;

	case 'i':
	  input_filename = (char *) strdup (optarg);
	  break;

	default:
	case '?':
	  printf ("Unknown command line option...\n");
	  usage(argv[0]);
	  exit (0);
	  break;
	}
    }

  if (input_filename == NULL)
    {
      printf ("No input file specified...\n");
      usage(argv[0]);
      exit (0);
    }

  // Map the input file into memory
  if ((fd = open (input_filename, O_RDWR)) < 0)
    {
      fprintf (stderr, "Could not open the data file\n");
      exit (43);
    }

  if (fstat (fd, &sb) < 0)
    {
      fprintf (stderr, "File stat error\n");
      exit (43);
    }

  if ((file =
       (char *) mmap (NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,
		      fd, 0)) == -1)
    {
      fprintf (stderr, "Could not mmap data file\n");
      exit (43);
    }
  filep = 0;

  // Details of the dump, skip over the URB pointers
  npackets = fileutil_getnumber (file, &filep);
  printf ("Number of packets: %d\n", npackets);
  filep += npackets * 4;
  tsrelative = fileutil_getunumber (file, &filep);
  printf ("Plugin timestamp: %d\n", tsrelative);

  temp = fileutil_getunumber (file, &filep);
  if (temp == 1)
    {
      printf ("Timestamp is relative\n");
    }
  printf ("\n");

  if(do_showmatch == 1)
    printf ("Grabbing the URBs\n");

  // Setup the URB head
  if ((urbhead =
       (usb_allurbs *) malloc (sizeof (usb_allurbs) * npackets)) == NULL)
    {
      fprintf (stderr,
	       "Memory allocation problem whilst setting up URB list head\n");
      exit (1);
    }
  memset(urbhead, 0, sizeof (usb_allurbs) * npackets);

  // Details of the URB
  seq = -1;
  for (urbCount = 0; urbCount < npackets; urbCount++)
    {
      if(do_verbose == 1) printf("Grabbing URB %d\n", urbCount);
      urbhead[urbCount].abend = 0;

      // Get the object tag (a MFCism) -- it tells us if there is an object 
      // name coming up
      otag = fileutil_getushort (file, &filep);
      if (otag == 0xFFFF)
	{
	  fileutil_getushort (file, &filep);
	  fileutil_getstring (file, &filep);
	}

      temp = fileutil_getuinteger (file, &filep);
      urbhead[urbCount].number = urbCount;
      urbhead[urbCount].filep = filep;
      urbhead[urbCount].sequence = temp;

      function = fileutil_getushort (file, &filep);
      urb_printf ("Function: %s (0x%04x)\n", functname (function), 
		  function);

      // printf("Time relative to start of dump: %d\n", 
      // fileutil_getuinteger(file, &filep) - tsrelative);
      urbhead[urbCount].time =
	fileutil_getuinteger (file, &filep) - tsrelative;

      urbhead[urbCount].endpoint = fileutil_getnumber (file, &filep);
      urb_printf ("Endpoint: %d%s\n", urbhead[urbCount].endpoint,
		  urbhead[urbCount].endpoint == 0 ? " (default)" : "");
      urb_printf ("Pipe handle: 0x%08x\n",
		  fileutil_getuinteger (file, &filep));
      urb_printf ("Flags: %d\n", fileutil_getuinteger (file, &filep));
      urb_printf ("Status: %d\n", fileutil_getinteger (file, &filep));
      urb_printf ("Link: %d\n", fileutil_getuinteger (file, &filep));
      urb_printf ("\n");

      urb_printf ("Length: %d\n", fileutil_getuinteger (file, &filep));
      urb_printf ("Direction: %s\n",
		  0 == fileutil_getinteger (file, &filep) ? 
		  "host-to-device" : 
		  "device-to-host");

      // The sequence number and timestamp are repeated for some reason...
      fileutil_getuinteger (file, &filep);
      fileutil_getuinteger (file, &filep);
      urb_printf ("\n");

      usb_urb_header (file, &filep);

      // Whether or not it is expanded in the snoopypro view
      fileutil_getinteger (file, &filep);

      // Do something with the function
      switch (function)
	{
	case 0x0:
	  // SELECT_CONFIGURATION

	  // usb_urb_header(file, &filep);
	  filep += USB_URB_HEADER_LENGTH;
	  // Skipped usb configuration description pointer
	  filep += 4;
	  // Skipped usb configuration handle pointer
	  filep += 4;

	  usb_interface_info (file, &filep);
	  urb_printf ("\n");

	  urb_printf ("UCD present: %d\n",
		      fileutil_getinteger (file, &filep));
	  usb_ucd (file, &filep);

	  numifaces = fileutil_getuinteger (file, &filep);
	  urb_printf ("Number of interfaces ignored: %d\n", numifaces);
	  while (numifaces > 0)
	    {
	      filep += fileutil_getushort (file, &filep);
	      numifaces--;
	    }
	  break;

	case 0x1:
	  // SELECT_INTERFACE

	  // TODO mikal: should I decode this?
	  temp = fileutil_getnumber (file, &filep);
	  urb_printf ("Interface size: %d\n", temp);
	  filep += temp;
	  urb_printf ("\n");
	  urb_printf ("TODO: implement decoding\n");
	  break;

	case 0x2:
	  // ABORT_PIPE
	case 0x7:
	  // GET_CURRENT_FRAME_NUMBER
	case 0xB:
	  // GET_DESCRIPTOR_FROM_DEVICE
	  break;

	case 0x8:
	  // CONTROL_TRANSFER

	  psize = fileutil_getuinteger (file, &filep);
	  urb_printf ("Transfer size: %d\n", psize);

	  // Test whether data is present
	  if (fileutil_getnumber (file, &filep) != 0)
	    {
	      int i;

	      urb_printf ("Data: ");
	      for (i = 0; i < psize; i++)
		{
		  urb_printf ("0x%02x ", (unsigned char) file[filep++]);
		}
	      urb_printf ("\n");
	    }

	  // And now read the control transfer header
	  // usb_urb_header(file, &filep);
	  filep += USB_URB_HEADER_LENGTH;
	  usb_urb_controltransfer (file, &filep);
	  break;

	case 0xA:
	  // ISOCH_TRANSFER

	  temp = fileutil_getnumber (file, &filep);
	  urb_printf ("ISOCH transfer length: %d\n", temp);
	  urb_printf ("ISOCH transfer: ");
	  while (temp != 0)
	    {
	      urb_printf ("0x%02x ", (unsigned char) file[filep++]);
	      temp--;
	    }
	  urb_printf ("\n");

	  temp = fileutil_getnumber (file, &filep);
	  urb_printf ("ISOCH buffer count: %d\n", temp);
	  while (temp != 0)
	    {
	      urb_printf ("\n");
	      temp2 = fileutil_getnumber (file, &filep);
	      urb_printf ("ISOCH transfer buffer length: %d\n", temp2);
	      urb_printf ("ISOCH transfer buffer: ");
	      while (temp2 != 0)
		{
		  urb_printf ("0x%02x ", (unsigned char) file[filep++]);
		  temp2--;
		}
	      urb_printf ("\n");
	      temp--;
	    }
	  urb_printf ("\n");
	  break;

	case 0x17:
	  // VENDOR_DEVICE
	case 0x1B:
	  // CLASS_INTERFACE
	case 0x1C:
	  // CLASS_ENDPOINT
	case 0x20:
	  // VENDOR_OTHER
	case 0x22:
	  // CLEAR_FEATURE_TO_OTHER

	  if ((temp = fileutil_getinteger (file, &filep)) != 0)
	    {
	      urb_printf ("Transfer length: %d\n", temp);

	      // 0 indicates no data present
	      if (fileutil_getnumber (file, &filep) != 0)
		{
		  filep += temp;
		  urb_printf ("TODO: Implementing decoding\n");
		}
	    }
	  urb_printf ("\n");

	  // usb_urb_header(file, &filep);
	  filep += USB_URB_HEADER_LENGTH;
	  usb_urb_controltransfer (file, &filep);
	  break;

	case 0x1E:
	  // RESET_PIPE
	  break;

	default:
	  urb_printf ("\n*** Unknown function ***\n\n");
	  urb_printf ("Aborting further decoding. Please report this to\n");
	  urb_printf ("Michael Still (mikal@stillhq.com)\n");
	  urbhead[urbCount].abend = 1;
	}

      urbhead[urbCount].desc = urb_buffer;
      if (urb_buffer != NULL)
	{
	  urbhead[urbCount].md5 = md5hash (urb_buffer);
	}
      urbhead[urbCount].deleted = 0;
      urbhead[urbCount].rptlen = 0;
      urbhead[urbCount].rpttimes = 0;
      urbhead[urbCount].reallocs = urb_reallocs;

      // Finish up if we aborted this URB
      if (urbhead[urbCount].abend == 1)
	{
	  urbCount = npackets + 1;
	  do_suppress = 0;
	}

      // Reset the URB buffer
      urb_buffer = NULL;
      urb_buffer_inset = 0;
      urb_buffer_size = 0;
      urb_reallocs = 0;
    }

  // Correlate
  if (do_suppress == 1)
    {
      if(do_showmatch == 1)
	printf ("\nCorrelating the URBs:\n");
      corrstep = 0;
      for (length = (int) (npackets / 2); length != 0; length--)
	{
	  for (inset = 0; inset < npackets - length + 1; inset++)
	    {
	      matchcount = 1;
	      for (testinset = inset + length; testinset < npackets - length;
		   testinset += length)
		{
		  if (hashcmp (urbhead, inset, testinset, length) == 0)
		    {
		      matchcount++;
		    }
		  else
		    {
		      testinset = npackets + 100;
		    }
		}

	      if (matchcount > 1)
		{
		  if(do_showmatch == 1)
		    {
		      printf ("  Found a match at URB %d of length %d which "
			      "repeats %d times\n", inset, length, matchcount);
		    }
		  
		  urbhead[inset + length - 1].rptlen = length;
		  urbhead[inset + length - 1].rpttimes = matchcount;
		  for (temp = inset + length;
		       temp < matchcount * length + inset; temp++)
		    {
		      urbhead[temp].deleted = 1;
		    }
		  inset += matchcount * length;
		}
	    }
	}
    }

  // And now we should display the urbs
  for (urbCount = 0; urbCount < npackets; urbCount++)
    {
      printf ("\n"
	      "---------------------------------------------------------\n");
      printf ("URB %d, ", urbCount);
      printf ("number %d, ", urbhead[urbCount].number);
      printf ("offset %d, ", urbhead[urbCount].filep);
      printf ("sequence %d, ", urbhead[urbCount].sequence);
      printf ("time %d, ", urbhead[urbCount].time);
      printf ("allocs %d\n", urbhead[urbCount].reallocs);

      if (urbhead[urbCount].desc != NULL)
	{
	  if(do_showhash == 1)
	    {
	      printf ("MD5 hash: ");
	      for (temp = 0; temp < 16; temp++)
		{
		  printf ("%02x ", 
			  (unsigned char) urbhead[urbCount].md5[temp]);
		}
	      printf ("\n");
	    }
	  printf ("\n%s", urbhead[urbCount].desc);
	}

      if (urbhead[urbCount].rptlen != 0)
	{
	  printf ("\n(Sequence of %d URBs repeats %d times)\n\n",
		  urbhead[urbCount].rptlen, urbhead[urbCount].rpttimes);
	  urbCount +=
	    urbhead[urbCount].rptlen * (urbhead[urbCount].rpttimes - 1);
	}

      if (urbhead[urbCount].abend != 0)
	urbCount = npackets + 1;
    }

  // It's polite to cleanup
  munmap (file, sb.st_size);
  close (fd);
  return 0;
}

char *
functname (unsigned int function)
{
  switch (function)
    {
    case 0x0:
      return "SELECT_CONFIGURATION";

    case 0x1:
      return "SELECT_INTERFACE";

    case 0x2:
      return "ABORT_PIPE";

    case 0x7:
      return "GET_CURRENT_FRAME_NUMBER";

    case 0x8:
      return "CONTROL_TRANSFER";

    case 0xA:
      return "ISOCH_TRANSFER";

    case 0xB:
      return "GET_DESCRIPTOR_FROM_DEVICE";

    case 0x17:
      return "VENDOR_DEVICE";

    case 0x1B:
      return "CLASS_INTERFACE";

    case 0x1C:
      return "CLASS_ENDPOINT";

    case 0x1E:
      return "RESET_PIPE";

    case 0x20:
      return "VENDOR_OTHER";

    case 0x22:
      return "CLEAR_FEATURE_TO_OTHER";

    default:
      return "UNKNOWN";
    }
}

void
usb_urb_header (char *file, long long *filep)
{
  long long count = *filep;

  urb_printf ("URB header:\n");
  urb_printf ("Length: %d\n", fileutil_getushort (file, &count));

  // Function has already been got
  fileutil_getushort (file, &count);
  urb_printf ("Status: %d\n", fileutil_getuinteger (file, &count));
  // Skipped device handle pointer
  count += 4;
  urb_printf ("Flags: %d\n", fileutil_getuinteger (file, &count));

  *filep = count;
}

void
usb_urb_controltransfer (char *file, long long *filep)
{
  long long count = *filep;

  urb_printf ("URB control transfer:\n");
  // Skipped pipe handle pointer
  count += 4;
  urb_printf ("Transfer flags: %d\n", fileutil_getinteger (file, &count));
  urb_printf ("Transfer buffer length: %d\n",
	      fileutil_getinteger (file, &count));
  // Skipped transfer buffer pointer
  count += 4;
  // Skipped transfer buffer MDL pointer
  count += 4;
  // Skipped next URB pointer
  count += 4;
  urb_printf ("\n");

  // usb_urb_hcd(file, &count);
  count += USB_URB_HCD_LENGTH;

  // And now we have a setup packet
  usb_setup_packet(file, &count);
  *filep = count;
}

void
usb_urb_listentry (char *file, long long *filep)
{
  long long count = *filep;

  // Skipped forward pointer
  count += 4;
  // Skipped backward pointer
  count += 4;
  urb_printf ("\n");

  *filep = count;
}

void
usb_interface_info (char *file, long long *filep)
{
  long long count = *filep;

  urb_printf ("Length: %d\n", fileutil_getushort (file, &count));
  urb_printf ("Interface number: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Alternate setting: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Class: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Subclass: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Protocol: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Reserved: 0x%02x\n", (unsigned char) file[count++]);

  // Skipping usb interface handler pointer
  count += 4;

  urb_printf ("Number of pipes: %d\n", fileutil_getuinteger (file, &count));
  urb_printf ("\n");

  // POssibly a second, depending on if the box was OSR2 compatible
  usb_pipe_info (file, &count);
  *filep = count;
}

void
usb_pipe_info (char *file, long long *filep)
{
  long long count = *filep;

  urb_printf ("Maximum packet size: %d\n", fileutil_getushort (file, &count));
  urb_printf ("End point address: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Interval: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("USB pipe type: %d\n", fileutil_getinteger (file, &count));
  // Skipped pipe handle\n");
  count += 4;
  urb_printf ("Maximum transfer size: %d\n",
	      fileutil_getuinteger (file, &count));
  urb_printf ("Pipe flags: %d\n", fileutil_getuinteger (file, &count));

  *filep = count;
}

void
usb_ucd (char *file, long long *filep)
{
  long long count = *filep;

  urb_printf ("Length: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Descriptor type: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Total length: %d\n", fileutil_getushort (file, &count));
  urb_printf ("Number of interfaces: 0x%02x\n",
	      (unsigned char) file[count++]);
  urb_printf ("Configuration value: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Configuration: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Attributes: 0x%02x\n", (unsigned char) file[count++]);
  urb_printf ("Max power: 0x%02x\n", (unsigned char) file[count++]);

  *filep = count;
}

void
usb_setup_packet(char *file, long long *filep)
{
  long long count = *filep;
  int type, req, val, idx, len;
  int stdreq = 0, endval = 0, ifaceval = 0;
  char *lkdir = NULL, *lktype = NULL, *lkrecip = NULL, *lkreq = NULL;

  // Decode a setup packet
  // TODO: I have just written this, and need to desk check that it is correct
  urb_printf ("Setup packet:\n");
  type = (unsigned char) file[count++];
  req = (unsigned char) file[count++];
  val = fileutil_getshort(file, &count);
  idx = fileutil_getshort(file, &count);
  len = fileutil_getshort(file, &count);

  urb_printf ("  bmRequestType: 0x%02x (", type);
  if(((type & 0x80) >> 7) == 1)
    {
      urb_printf("device-to-host ");
      lkdir = strdup("USB_DIR_IN");
    }
  else
    {
      urb_printf("host-to-device ");
      lkdir = strdup("USB_DIR_OUT");
    }
  if(len == 0)
    urb_printf("[ignored] ");

  switch((type & 0x60) >> 5)
    {
    case 0:
      urb_printf("standard ");
      lktype = strdup("USB_TYPE_STANDARD");
      stdreq = 1;
      break;

    case 1:
      urb_printf("class ");
      lktype = strdup("USB_TYPE_CLASS");
      break;
      
    case 2:
      urb_printf("vendor ");
      lktype = strdup("USB_TYPE_VENDOR");
      break;

    case 3:
      urb_printf("reserved ");
      lktype = strdup("USB_TYPE_RESERVED");
      break;
    }
  switch(type & 0x0F)
    {
    case 0:
      urb_printf("device");
      lkrecip = strdup("USB_RECIP_DEVICE");
      break;

    case 1:
      urb_printf("interface");
      lkrecip = strdup("USB_RECIP_INTERFACE");
      ifaceval = 1;
      break;
      
    case 2:
      urb_printf("endpoint");
      lkrecip = strdup("USB_RECIP_ENDPOINT");
      endval = 1;
      break;

    case 3:
      urb_printf("other");
      lkrecip = strdup("USB_RECIP_OTHER");
      break;

    default:
      urb_printf("reserved");
      break;
    }
  urb_printf(")\n");
  if(do_linux == 1)
    urb_printf("  Linux kernel: %s | %s | %s\n\n", lkdir, lktype, lkrecip);
  urb_xfree(lkdir);
  urb_xfree(lktype);
  urb_xfree(lkrecip);

  urb_printf ("  bRequest: 0x%02x", req);
  if(stdreq == 1)
    {
      switch(req)
	{
        case 0x00:
          urb_printf(" GET_STATUS");
          lkreq = strdup("USB_REQ_GET_STATUS");
          break;

        case 0x01:
          urb_printf(" CLEAR_FEATURE");
          lkreq = strdup("USB_REQ_CLEAR_FEATURE");
          break;

        case 0x03:
          urb_printf(" SET_FEATURE");
          lkreq = strdup("USB_REQ_SET_FEATURE");
          break;

        case 0x05:
          urb_printf(" SET_ADDRESS");
          lkreq = strdup("USB_REQ_SET_ADDRESS");
          break;

        case 0x06:
          urb_printf(" GET_DESCRIPTOR");
          lkreq = strdup("USB_REQ_GET_DESCRIPTOR");
          break;

        case 0x07:
          urb_printf(" SET_DESCRIPTOR");
          lkreq = strdup("USB_REQ_SET_DESCRIPTOR");
          break;

        case 0x08:
          urb_printf(" GET_CONFIGURATION");
          lkreq = strdup("USB_REQ_GET_CONFIGURATION");
          break;

        case 0x09:
          urb_printf(" SET_CONFIGURATION");
          lkreq = strdup("USB_REQ_SET_CONFIGURATION");
          break;

        case 0x0A:
          urb_printf(" GET_INTERFACE");
          lkreq = strdup("USB_REQ_GET_INTERFACE");
          break;

        case 0x0B:
          urb_printf(" SET_INTERFACE");
          lkreq = strdup("USB_REQ_SET_INTERFACE");
          break;

        case 0x0C:
          urb_printf(" SYNCH_FRAME");
          lkreq = strdup("USB_REQ_SYNCH_FRAME");
          break;
	}

      urb_printf(" [standard]");
    }
  urb_printf("\n");
  if(do_linux == 1)
    urb_printf("  Linux kernel: %s\n\n", lkreq);
  urb_xfree(lkreq);

  urb_printf ("  wValue: 0x%04x (%d)\n", val, val);

  urb_printf ("  wIndex: 0x%04x (%d)", idx, idx);
  if(endval == 1)
    {
      urb_printf(" [%s endpoint %d]",
		 idx & 0x80 == 0x80 ? "in" : "out",
		 idx & 0x0F);
    }
  else if(ifaceval == 1)
    {
      urb_printf(" [interface %d]", (idx & 0xFF00) >> 8);
    }
  urb_printf("\n");

  urb_printf ("  wLength: 0x%04x (%d)\n", len, len);

  *filep = count;
}












// ----------------------------------------------------------------------
// Utility functions
// ----------------------------------------------------------------------
// This function is used to cache urb descriptions for duplicate checking
void
urb_printf (char *format, ...)
{
  va_list ap;
  char *temp;
  int templen;

  // Get the string which is just this output
  va_start (ap, format);
  temp = urb_xsnprintf (format, ap);
  templen = strlen (temp);
  va_end (ap);

  // Append it to the buffer which is this URB
  if (urb_buffer_inset + templen + 1 > urb_buffer_size)
    {
      urb_buffer =
	(char *) realloc (urb_buffer, urb_buffer_size + templen + 500);
      if (urb_buffer == NULL)
	{
	  fprintf (stderr, "Memory allocation error\n");
	  exit (1);
	}
      urb_buffer_size += templen + 500;
      urb_reallocs++;
    }
  strcpy (urb_buffer + urb_buffer_inset, temp);
  urb_buffer_inset += templen;

  // Cleanup
  free (temp);
}

char *
urb_xsnprintf (char *format, va_list ap)
{
  char *output = NULL;
  int size, result;

  /* We start with the size of the format string as a guess */
  size = strlen (format);
  while (1)
    {
      output = (char *) realloc (output, size);
      if (output == NULL)
	{
	  fprintf (stderr, "Memory allocation error 2\n");
	  exit (1);
	}
      result = vsnprintf (output, size, format, ap);

      if (result == -1)
	{
	  /* Up to glibc 2.0.6 and Microsoft's implementation */
	  size += 100;
	}
      else
	{
	  /* Check if we are done */
	  if (result < size)
	    break;

	  /* Glibc from now on */
	  size = result + 1;
	}
    }

  return output;
}

void
urb_xfree(void *m)
{
  if(m)
    free(m);
}

char *
md5hash (char *input)
{
  MD5_CTX context;
  unsigned char *digest;

  if ((digest = (char *) malloc (16)) == NULL)
    {
      fprintf (stderr, "Memory error 3\n");
      exit (1);
    }

  MD5Init (&context);
  MD5Update (&context, input, strlen (input));
  MD5Final (digest, &context);

  return digest;
}

int
hashcmp (usb_allurbs * head, int inset, int testinset, int length)
{
  int count, count2;

  for (count = 0; count < length; count++)
    {
      if (head[inset + count].deleted == 1)
	{
	  return -4;
	}
      if (head[testinset + count].deleted == 1)
	{
	  return -4;
	}

      for (count2 = 0; count2 < 16; count2++)
	{
	  if ((unsigned char) head[inset + count].md5[count2] !=
	      (unsigned char) head[testinset + count].md5[count2])
	    {
	      return -2;
	    }
	}
    }

  return 0;
}
