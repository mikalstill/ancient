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

int usb_urb_header(char *file, long long *filep);
void usb_urb_controltransfer(char *file, long long *filep);
void usb_urb_hcd(char *file, long long *filep);
void usb_urb_listentry(char *file, long long *filep);
void usb_interface_info(char *file, long long *filep);
void usb_pipe_info(char *file, long long *filep);

int main(int argc, char *argv[]){
  int fd, npackets, otag, urbCount, function, psize, tsrelative, temp;
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

  // Details of the dump
  npackets = fileutil_displaynumber(file, "Number of packets: ", &filep); printf("\n");
  printf("%d URB pointers skipped\n", npackets);
  filep += npackets * 4;

  tsrelative = fileutil_getunumber(file, &filep);
  printf("Plugin timestamp: %d\n", tsrelative);
  
  fileutil_displayunumber(file, "Timestamp is relative (1 == true): ", &filep); printf("\n");
  printf("\n");

  // Details of the URB
  for(urbCount = 0; urbCount < npackets; urbCount++){
    printf("-----------------------------------------------\n");
    printf("URB %d starts at %d within file\n", urbCount, filep);

    // Get the object tag (a MFCism) -- it tells us if there is an object name coming up
    otag = fileutil_getushort(file, &filep); printf("\n");
    if(otag == 0xFFFF){
      fileutil_getushort(file, &filep);
      fileutil_displaystring(file, "SnoopyPro URB object name: ", &filep); printf("\n");
    }

    printf("Sequence: %u\n", fileutil_getuinteger(file, &filep));

    temp = fileutil_getushort(file, &filep);
    printf("Function: %s (0x%04x)\n", functname(temp), temp);
    printf("Time relative to start of dump: %d\n", fileutil_getuinteger(file, &filep) - tsrelative);

    fileutil_displaynumber(file, "Endpoint: ", &filep); printf("\n");

    printf("Pipe handle: 0x%08x\n", fileutil_getuinteger(file, &filep));

    fileutil_displayuinteger(file, "Flags: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Status: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Link: ", &filep); printf("\n");
    printf("\n");
    
    fileutil_displayuinteger(file, "Length: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Direction (0 to, 1 from): ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Sequence number: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Timestamp: ", &filep); printf("\n");
    printf("\n");

    function = usb_urb_header(file, &filep);
    
    // Whether or not it is expanded in the snoopypro view
    fileutil_getinteger(file, &filep);

    // Do something with the function
    switch(function){
    case 0:
      printf("Skipped usb configuration description pointer\n");
      filep += 4;
      printf("Skipped usb configuration handle pointer\n");
      filep += 4;
      usb_interface_info(file, &filep);
      
      printf("AT %d\n", filep);
      break;

    case 8:
      psize = fileutil_displayuinteger(file, "Transfer size: ", &filep); printf("\n");
      fileutil_displaynumber(file, "Some number I don't understand: ", &filep); printf("\n");
      fileutil_displaybyteblock(file, "Transfer: ", psize, &filep); printf("\n");
      printf("\n");

      // And now read the control transfer header
      usb_urb_header(file, &filep);
      usb_urb_controltransfer(file, &filep);
      break;

    case 11:
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

    case 8:
      return "CONTROL_TRANSFER";

    case 11:
      return "GET_DESCRIPTOR_FROM_DEVICE";
      
    default:
      return "UNKNOWN";
    }
}

int usb_urb_header(char *file, long long *filep)
{
  long long count = *filep;
  int function;

  printf("URB header:\n");
  fileutil_displayushort(file, "Length: ", &count); printf("\n");
  function = fileutil_displayushort(file, "Function: ", &count); printf("\n");
  fileutil_displayuinteger(file, "Status: ", &count); printf("\n");
  printf("Skipped device handle pointer\n");
  count += 4;
  fileutil_displayuinteger(file, "Flags: ", &count); printf("\n");
  printf("\n");

  *filep = count;
  return function;
}

void usb_urb_controltransfer(char *file, long long *filep)
{
  long long count = *filep;

  printf("URB control transfer:\n");
  printf("Skipped pipe handle pointer\n");
  count += 4;
  fileutil_displayinteger(file, "Transfer flags: ", &count); printf("\n");
  fileutil_displayinteger(file, "Transfer buffer length: ", &count); printf("\n");
  printf("Skipped transfer buffer pointer\n");
  count += 4;
  printf("Skipped transfer buffer MDL pointer\n");
  count += 4;
  printf("Skipped next URB pointer\n");
  count += 4;
  printf("\n");

  usb_urb_hcd(file, &count);
  fileutil_displaybyteblock(file, "Setup packet: ", 8, &count);
  printf("\n");

  *filep = count;
}

void usb_urb_hcd(char *file, long long *filep)
{
  long long count = *filep;

  printf("URB hcd:\n");
  printf("Skipped HCD endpoint pointer\n");
  count += 4;
  printf("Skipped HCD IRP pointer\n");
  count += 4;
  printf("\n");
  
  printf("HCD list entry 1:\n");
  usb_urb_listentry(file, &count);
  printf("HCD list entry 2:\n");
  usb_urb_listentry(file, &count);

  printf("Skipped HCD current IO flush pointer\n");
  count += 4;
  printf("Skipped HCD extension pointer\n");
  count += 4;
  printf("\n");

  *filep = count;
}

void usb_urb_listentry(char *file, long long *filep)
{
  long long count = *filep;

  printf("Skipped forward pointer\n");
  count += 4;
  printf("Skipped backward pointer\n");
  count += 4;
  printf("\n");

  *filep = count;
}

void usb_interface_info(char *file, long long *filep)
{
  long long count = *filep;

  fileutil_displaybyteblock(file, "Interface number: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Alternate setting: ", 1, &count); printf("\n");

  fileutil_displaybyteblock(file, "Class: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Subclass: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Protocol: ", 1, &count); printf("\n");
  fileutil_displaybyteblock(file, "Reserved: ", 1, &count); printf("\n");

  printf("Skipping usb interface handler pointer\n");
  count += 4;

  printf("\t\t\t\t\t\t\t\t\tAT %d\n", count);
  fileutil_displayuinteger(file, "Number of pipes: ", &count); printf("\n");

  // POssibly a second, depending on if the box was OSR2 compatible
  usb_pipe_info(file, &count);
  *filep = count;
}

void usb_pipe_info(char *input, long long *filep){
  // TODO
}
