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

#define VERSION "(Version 0.2)"
void usage(char *name)
{
  printf (VERSION "\n\n");
  printf ("Try: %s [-i input] [-r] [-v]\n\n", name);
  printf ("\t-i <name>: The name of the input usblog file\n");
  printf ("\t-r:        Suppressed repeated URB sequences\n");
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

// URB output
char *urb_buffer = NULL;
long urb_buffer_size = 0;
long urb_buffer_inset = 0;
int urb_reallocs = 0;
void urb_printf (char *format, ...);
char *urb_xsnprintf (char *format, va_list ap);
char *md5hash (char *);

// A structure to store all our URBs
typedef struct usb_internal_allurbs
{
  int number;
  long long filep;
  int sequence;
  int time;
  int reallocs;
  int deleted;

  int rptlen;
  int rpttimes;
  int abend;

  char *desc;
  char *md5;
}
usb_allurbs;

// Used for correlation
int hashcmp (usb_allurbs * head, int inset, int testinset, int length);

int
main (int argc, char *argv[])
{
  int fd, npackets, otag, urbCount, function, psize, tsrelative, temp, temp2,
    numifaces, seq, length, inset, testinset, corrstep, matchcount, optchar,
    do_suppress = 0, do_verbose = 0;
  char *file, *input_filename = NULL;
  long long filep;
  struct stat sb;
  usb_allurbs *urbhead;

  while ((optchar = getopt (argc, argv, "i:rv")) != -1)
    {
      switch (optchar)
	{
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
  printf ("Grabbing the URBs\n");

  // Setup the URB head
  if ((urbhead =
       (usb_allurbs *) malloc (sizeof (usb_allurbs) * npackets)) == NULL)
    {
      fprintf (stderr,
	       "Memory allocation problem whilst setting up URB list head\n");
      exit (1);
    }

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
      //printf("URB %d starts at %d within file\n", urbCount, filep);
      //printf("Sequence: %u\n", temp);
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

      urb_printf ("Endpoint: %d\n", fileutil_getnumber (file, &filep));
      urb_printf ("Pipe handle: 0x%08x\n",
		  fileutil_getuinteger (file, &filep));
      urb_printf ("Flags: %d\n", fileutil_getuinteger (file, &filep));
      urb_printf ("Status: %d\n", fileutil_getinteger (file, &filep));
      urb_printf ("Link: %d\n", fileutil_getuinteger (file, &filep));
      urb_printf ("\n");

      urb_printf ("Length: %d\n", fileutil_getuinteger (file, &filep));
      urb_printf ("Direction: %s\n",
		  0 == fileutil_getinteger (file, &filep) ? "to" : "from");

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
	  // TODO mikal: should I decode this?
	  temp = fileutil_getnumber (file, &filep);
	  urb_printf ("Interface size: %d\n", temp);
	  filep += temp;
	  urb_printf ("\n");
	  break;

	case 0x2:
	case 0x7:
	case 0xB:
	  // Do nothing
	  break;

	case 0x8:
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
	case 0x1B:
	case 0x1C:
	case 0x20:
	case 0x22:
	  if ((temp = fileutil_getinteger (file, &filep)) != 0)
	    {
	      urb_printf ("Transfer length: %d\n", temp);

	      // 0 indicates no data present
	      if (fileutil_getnumber (file, &filep) != 0)
		filep += temp;
	    }
	  urb_printf ("\n");

	  // usb_urb_header(file, &filep);
	  filep += USB_URB_HEADER_LENGTH;
	  usb_urb_controltransfer (file, &filep);
	  break;

	case 0x1E:
	  // Do nothing?
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
	  urbhead[urbCount].reallocs = urb_reallocs;
	}
      urbhead[urbCount].deleted = 0;
      urbhead[urbCount].rptlen = 0;
      urbhead[urbCount].rpttimes = 0;

      // Finish up if we aborted this URB
      if (urbhead[urbCount].abend == 1)
	urbCount = npackets + 1;

      // Reset the URB buffer
      urb_buffer = NULL;
      urb_buffer_inset = 0;
      urb_buffer_size = 0;
      urb_reallocs = 0;
    }

  // Correlate
  if (do_suppress == 1)
    {
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
		  printf ("  Found a match at URB %d of length %d which "
			  "repeats %d times\n", inset, length, matchcount);
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
      printf ("\n-----------------\nURB %d\n", urbCount);
      printf ("Number: %d\n", urbhead[urbCount].number);
      printf ("Offset within dumpfile: %d\n", urbhead[urbCount].filep);
      printf ("Sequence: %d\n", urbhead[urbCount].sequence);
      printf ("Relative time: %d\n", urbhead[urbCount].time);
      printf ("Reallocs: %d\n", urbhead[urbCount].reallocs);
      if (urbhead[urbCount].desc != NULL)
	{
	  printf ("MD5 hash: ");
	  for (temp = 0; temp < 16; temp++)
	    {
	      printf ("%02x ", (unsigned char) urbhead[urbCount].md5[temp]);
	    }
	  printf ("\n\n");
	  printf ("%s", urbhead[urbCount].desc);
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
  int i;

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

  urb_printf ("Setup packet: ");
  for (i = 0; i < 8; i++)
    {
      urb_printf ("0x%02x ", (unsigned char) file[count++]);
    }
  urb_printf ("\n");

  *filep = count;
}

void
usb_urb_hcd (char *file, long long *filep)
{
  long long count = *filep;

  urb_printf ("URB hcd:\n");
  // Skipped HCD endpoint pointer
  count += 4;
  // Skipped HCD IRP pointer
  count += 4;
  urb_printf ("\n");

  urb_printf ("HCD list entry 1:\n");
  usb_urb_listentry (file, &count);
  urb_printf ("HCD list entry 2:\n");
  usb_urb_listentry (file, &count);

  // Skipped HCD current IO flush pointer
  count += 4;
  // Skipped HCD extension pointer
  count += 4;
  urb_printf ("\n");

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
