#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

char *functname (unsigned int function);

// URB things
void usb_urb_header (char *file, long long *filep);
#define USB_URB_HEADER_LENGTH 16
void usb_urb_controltransfer (char *file, long long *filep);
void usb_urb_bulktransfer (char *file, long long *filep);
void usb_urb_hcd (char *file, long long *filep);
#define USB_URB_HCD_LENGTH 32
void usb_urb_listentry (char *file, long long *filep);
void usb_interface_info (char *file, long long *filep);
void usb_pipe_info (char *file, long long *filep);
void usb_ucd (char *file, long long *filep);
void usb_setup_packet (char *file, long long *filep);

// Output
void dump_data(char *file, long long *filep, int psize, char *type);
char *to_binary(unsigned char number);
char *to_bcd(int twobcdbytes);
int decodein, decodeout;

// URB output
void urb_printf (char *format, ...);
char *urb_xsnprintf (char *format, va_list ap);
void urb_printf_flags (unsigned int number);
void urb_xfree (void *);
char *md5hash (char *);

// Code output
typedef enum 
  {
    section_preamble = 0,
    section_functions,
    section_other,
    section_max
  } section;
char code_buffers[section_max];
void code_printf (section section, char *format, ...);

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

