#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "fileutil.h"

char *functname(unsigned int function);

void usb_urb_header(char *file, long long *filep);
#define USB_URB_HEADER_LENGTH 16

void usb_urb_controltransfer(char *file, long long *filep);

void usb_urb_hcd(char *file, long long *filep);
#define USB_URB_HCD_LENGTH 32

void usb_urb_listentry(char *file, long long *filep);
void usb_interface_info(char *file, long long *filep);
void usb_pipe_info(char *file, long long *filep);
void usb_ucd(char *file, long long *filep);

int main(int argc, char *argv[]){
  int fd, npackets, otag, urbCount, function, psize, tsrelative, temp, temp2, numifaces;
  char *file;
  long long filep;
  struct stat sb;

  if(argc < 2){
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    exit(43);
  }

  // Map the input file into memory
  if ((fd = open (argv[1], O_RDWR)) < 0)
    {
      fprintf (stderr, "Could not open the data file\n");
      exit (43);
    }
  
  if(fstat(fd, &sb) < 0){
    fprintf(stderr, "File stat error\n");
    exit(43);
  }

  if ((file =
       (char *) mmap (NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == -1)
    {
      fprintf (stderr, "Could not mmap data file\n");
      exit (43);
    }
  filep = 0;

  // Details of the dump, skip over the URB pointers
  npackets = fileutil_getnumber(file, &filep);
  printf("Number of packets: %d\n", npackets);
  filep += npackets * 4;
  tsrelative = fileutil_getunumber(file, &filep);
  printf("Plugin timestamp: %d\n", tsrelative);
  
  temp = fileutil_getunumber(file, &filep);
  if(temp == 1){
    printf("Timestamp is relative\n");
  }

  // Details of the URB
  for(urbCount = 0; urbCount < npackets; urbCount++){
    printf("-----------------------------------------------\n");
    printf("URB %d starts at %d within file\n", urbCount, filep);

    // Get the object tag (a MFCism) -- it tells us if there is an object name coming up
    otag = fileutil_getushort(file, &filep); printf("\n");
    if(otag == 0xFFFF){
      fileutil_getushort(file, &filep);
      fileutil_getstring(file, &filep);
    }

    printf("Sequence: %u\n", fileutil_getuinteger(file, &filep));

    function = fileutil_getushort(file, &filep);
    printf("Function: %s (0x%04x)\n", functname(function), function);
    printf("Time relative to start of dump: %d\n", fileutil_getuinteger(file, &filep) - tsrelative);
    printf("Endpoint: %d\n", fileutil_getnumber(file, &filep));
    printf("Pipe handle: 0x%08x\n", fileutil_getuinteger(file, &filep));

    fileutil_displayuinteger(file, "Flags: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Status: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Link: ", &filep); printf("\n");
    printf("\n");
    
    fileutil_displayuinteger(file, "Length: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Direction (0 to, 1 from): ", &filep); printf("\n");

    // The sequence number and timestamp are repeated for some reason...
    fileutil_getuinteger(file, &filep);
    fileutil_getuinteger(file, &filep);
    printf("\n");

    usb_urb_header(file, &filep);
    
    // Whether or not it is expanded in the snoopypro view
    fileutil_getinteger(file, &filep);

    // Do something with the function
    switch(function){
    case 0:
      // usb_urb_header(file, &filep);
      filep += USB_URB_HEADER_LENGTH;
      // Skipped usb configuration description pointer
      filep += 4;
      // Skipped usb configuration handle pointer
      filep += 4;

      usb_interface_info(file, &filep);
      printf("\n");

      fileutil_displayinteger(file, "UCD present: ", &filep); printf("\n");
      usb_ucd(file, &filep);

      numifaces = fileutil_displayuinteger(file, "Number of interfaces: ", &filep); printf("\n");
      while(numifaces > 0){
	printf("\n");
	filep += fileutil_displayushort(file, "Interface length: ", &filep); printf("\n");
	numifaces--;
      }
      break;

    case 1:
      // TODO mikal: should I decode this?
      filep += fileutil_displaynumber(file, "Interface size: ", &filep);
      printf("\n");
      break;

    case 7:
      // Do nothing
      break;

    case 8:
      psize = fileutil_displayuinteger(file, "Transfer size: ", &filep); printf("\n");
      if(fileutil_displaynumber(file, "Data length: ", &filep) != 0){
	printf("\n");
	fileutil_displaybyteblock(file, "Transfer: ", psize, &filep); printf("\n");
      }
      printf("\n");

      // And now read the control transfer header
      // usb_urb_header(file, &filep);
      filep += USB_URB_HEADER_LENGTH;
      usb_urb_controltransfer(file, &filep);
      break;

    case 10:
      temp = fileutil_displaynumber(file, "ISOCH transfer length: ", &filep); printf("\n");
      fileutil_displaybyteblock(file, "ISOCH transfer: ", temp, &filep); printf("\n");
      temp = fileutil_displaynumber(file, "ISOCH buffer count: ", &filep); printf("\n");
      while(temp != 0){
	printf("\n");
	temp2 = fileutil_displaynumber(file, "ISOCH transfer buffer length: ", &filep);
	printf("\n");
	fileutil_displaybyteblock(file, "ISOCH transfer buffer: ", temp2, &filep); printf("\n");
				  
	temp--;
      }
      printf("\n");
      break;

    case 11:
      break;

    case 23:
      if((temp = fileutil_displayinteger(file, "Transfer length: ", &filep)) != 0){
	printf("\n");
	if(fileutil_displaynumber(file, "Data present: ", &filep) != 0)
	  filep += temp;
      }
      printf("\n\n");
      
      // usb_urb_header(file, &filep);
      filep += USB_URB_HEADER_LENGTH;
      usb_urb_controltransfer(file, &filep);
      break;

    case 30:
      // Do nothing?
      break;

    default:
      printf("***Unknown function***\n");
    }
  }

  // It's polite to cleanup
  munmap(file, sb.st_size);
  close(fd);
  return 0;
}

char *functname(unsigned int function)
{
  switch(function)
    {
    case 0:
      return "SELECT_CONFIGURATION";

    case 1:
      return "SELECT_INTERFACE";
      
    case 7:
      return "GET_CURRENT_FRAME_NUMBER";

    case 8:
      return "CONTROL_TRANSFER";

    case 10:
      return "ISOCH_TRANSFER";

    case 11:
      return "GET_DESCRIPTOR_FROM_DEVICE";
      
    case 23:
      return "VENDOR_DEVICE";

    case 30:
      return "RESET_PIPE";

    default:
      return "UNKNOWN";
    }
}

void usb_urb_header(char *file, long long *filep)
{
  long long count = *filep;

  printf("URB header:\n");
  fileutil_displayushort(file, "Length: ", &count); printf("\n");

  // Function has already been displayed
  fileutil_getushort(file, &count);

  fileutil_displayuinteger(file, "Status: ", &count); printf("\n");
  // Skipped device handle pointer
  count += 4;
  fileutil_displayuinteger(file, "Flags: ", &count); printf("\n");
  printf("\n");

  *filep = count;
}

void usb_urb_controltransfer(char *file, long long *filep)
{
  long long count = *filep;

  printf("URB control transfer:\n");
  // Skipped pipe handle pointer
  count += 4;
  fileutil_displayinteger(file, "Transfer flags: ", &count); printf("\n");
  fileutil_displayinteger(file, "Transfer buffer length: ", &count); printf("\n");
  // Skipped transfer buffer pointer\n");
  count += 4;
  // Skipped transfer buffer MDL pointer\n");
  count += 4;
  // Skipped next URB pointer\n");
  count += 4;
  printf("\n");

  // usb_urb_hcd(file, &count);
  count += USB_URB_HCD_LENGTH;

  fileutil_displaybyteblock(file, "Setup packet: ", 8, &count);
  printf("\n");

  *filep = count;
}

void usb_urb_hcd(char *file, long long *filep)
{
  long long count = *filep;

  printf("URB hcd:\n");
  // Skipped HCD endpoint pointer\n");
  count += 4;
  // Skipped HCD IRP pointer\n");
  count += 4;
  printf("\n");
  
  printf("HCD list entry 1:\n");
  usb_urb_listentry(file, &count);
  printf("HCD list entry 2:\n");
  usb_urb_listentry(file, &count);

  // Skipped HCD current IO flush pointer\n");
  count += 4;
  // Skipped HCD extension pointer\n");
  count += 4;
  printf("\n");

  *filep = count;
}

void usb_urb_listentry(char *file, long long *filep)
{
  long long count = *filep;

  // Skipped forward pointer\n");
  count += 4;
  // Skipped backward pointer\n");
  count += 4;
  printf("\n");

  *filep = count;
}

void usb_interface_info(char *file, long long *filep)
{
  long long count = *filep;

  fileutil_displayushort(file, "Length: ", &count); printf("\n");
  fileutil_displaybyteblock(file, "Interface number: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Alternate setting: ", 1, &count); printf("\n");

  fileutil_displaybyteblock(file, "Class: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Subclass: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Protocol: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Reserved: ", 1, &count); printf("\n");

  printf("Skipping usb interface handler pointer\n");
  count += 4;

  fileutil_displayuinteger(file, "Number of pipes: ", &count); printf("\n");
  printf("\n");

  // POssibly a second, depending on if the box was OSR2 compatible
  usb_pipe_info(file, &count);
  *filep = count;
}

void usb_pipe_info(char *file, long long *filep){
  long long count = *filep;

  fileutil_displayushort(file, "Maximum packet size: ", &count); printf("\n");
  fileutil_displaybyteblock(file, "End point address: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Interval: ", 1, &count); printf("\n");
  fileutil_displayinteger(file, "USB pipe type: ", &count); printf("\n");
  // Skipped pipe handle\n");
  count += 4;
  fileutil_displayuinteger(file, "Maximum transfer size: ", &count); printf("\n");
  fileutil_displayuinteger(file, "Pipe flags: ", &count); printf("\n");

  *filep = count;
}

void usb_ucd(char *file, long long *filep){
  long long count = *filep;

  fileutil_displaybyteblock(file, "Length: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Decriptor type: ", 1, &count); printf("\n");
  fileutil_displayushort(file, "Total length: ", &count); printf("\n");
  fileutil_displaybyteblock(file, "Number of interfaces: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Configuration value: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Configuration: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Attributes: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Max power: ", 1, &count); printf("\n");

  *filep = count;
}
