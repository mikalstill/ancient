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
  int fd, npackets, otag, urbCount, function, psize, tsrelative, temp, temp2, numifaces, seq;
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
  seq = -1;
  for(urbCount = 0; urbCount < npackets; urbCount++){
    // Get the object tag (a MFCism) -- it tells us if there is an object name coming up
    otag = fileutil_getushort(file, &filep); printf("\n");
    if(otag == 0xFFFF){
      fileutil_getushort(file, &filep);
      fileutil_getstring(file, &filep);
    }

    temp = fileutil_getuinteger(file, &filep);
    if(seq != temp){
      printf("===============================================\n");
      seq = temp;
    }
    else{
      printf("---------\n");
    }
    printf("URB %d starts at %d within file\n", urbCount, filep);
    printf("Sequence: %u\n", temp);

    function = fileutil_getushort(file, &filep);
    printf("Function: %s (0x%04x)\n", functname(function), function);
    printf("Time relative to start of dump: %d\n", fileutil_getuinteger(file, &filep) - tsrelative);
    printf("Endpoint: %d\n", fileutil_getnumber(file, &filep));
    printf("Pipe handle: 0x%08x\n", fileutil_getuinteger(file, &filep));
    printf("Flags: %d\n", fileutil_getuinteger(file, &filep));
    printf("Status: %d\n", fileutil_getinteger(file, &filep));
    printf("Link: %d\n", fileutil_getuinteger(file, &filep));
    printf("\n");
    
    printf("Length: %d\n", fileutil_getuinteger(file, &filep));
    printf("Direction: %s\n", 0 == fileutil_getinteger(file, &filep) ? "to": "from");

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

      printf("UCD present: %d\n", fileutil_getinteger(file, &filep));
      usb_ucd(file, &filep);

      numifaces = fileutil_getuinteger(file, &filep);
      printf("Number of interfaces ignored: %d\n", numifaces);
      while(numifaces > 0){
	filep += fileutil_getushort(file, &filep);
	numifaces--;
      }
      break;

    case 1:
      // TODO mikal: should I decode this?
      temp = fileutil_getnumber(file, &filep);
      printf("Interface size: %d\n", temp);
      filep += temp;
      printf("\n");
      break;

    case 7:
      // Do nothing
      break;

    case 8:
      psize = fileutil_getuinteger(file, &filep);
      printf("Transfer size: %d\n", psize);

      // Test whether data is present
      if(fileutil_getnumber(file, &filep) != 0){
	int i;

	printf("Data: ");
	for(i = 0; i < psize; i++){
	  printf("0x%02x ", (unsigned char) file[filep++]);
	}
	printf("\n");
      }

      // And now read the control transfer header
      // usb_urb_header(file, &filep);
      filep += USB_URB_HEADER_LENGTH;
      usb_urb_controltransfer(file, &filep);
      break;

    case 10:
      temp = fileutil_getnumber(file, &filep);
      printf("ISOCH transfer length: %d\n", temp);
      printf("ISOCH transfer: ");
      while(temp != 0){
	printf("0x%02x ", (unsigned char) file[filep++]);
	temp--;
      }
      printf("\n");

      temp = fileutil_getnumber(file, &filep);
      printf("ISOCH buffer count: %d\n", temp);
      while(temp != 0){
	printf("\n");
	temp2 = fileutil_getnumber(file, &filep);
	printf("ISOCH transfer buffer length: %d\n", temp2);
	printf("ISOCH transfer buffer: ");
	while(temp2 != 0){
	  printf("0x%02x ", (unsigned char) file[filep++]);
	  temp2--;
	}
	printf("\n");
	temp--;
      }
      printf("\n");
      break;

    case 11:
      break;

    case 23:
      if((temp = fileutil_getinteger(file, &filep)) != 0){
	printf("Transfer length: %d\n", temp);

	// 0 indicates no data present
	if(fileutil_getnumber(file, &filep) != 0)
	  filep += temp;
      }
      printf("\n");
      
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
  printf("Length: %d\n", fileutil_getushort(file, &count));

  // Function has already been got
  fileutil_getushort(file, &count);
  printf("Status: %d\n", fileutil_getuinteger(file, &count));
  // Skipped device handle pointer
  count += 4;
  printf("Flags: %d\n", fileutil_getuinteger(file, &count));

  *filep = count;
}

void usb_urb_controltransfer(char *file, long long *filep)
{
  long long count = *filep;
  int i;

  printf("URB control transfer:\n");
  // Skipped pipe handle pointer
  count += 4;
  printf("Transfer flags: %d\n", fileutil_getinteger(file, &count));
  printf("Transfer buffer length: %d\n", fileutil_getinteger(file, &count));
  // Skipped transfer buffer pointer\n");
  count += 4;
  // Skipped transfer buffer MDL pointer\n");
  count += 4;
  // Skipped next URB pointer\n");
  count += 4;
  printf("\n");

  // usb_urb_hcd(file, &count);
  count += USB_URB_HCD_LENGTH;

  printf("Setup packet: ");
  for(i = 0; i < 8; i++){
    printf("0x%02x ", (unsigned char) file[count++]);
  }
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

  printf("Length: %d\n", fileutil_getushort(file, &count));
  printf("Interface number: 0x%02x\n", (unsigned char) file[count++]);
  printf("Alternate setting: 0x%02x\n", (unsigned char) file[count++]);
  printf("Class: 0x%02x\n", (unsigned char) file[count++]);
  printf("Subclass: 0x%02x\n", (unsigned char) file[count++]);
  printf("Protocol: 0x%02x\n", (unsigned char) file[count++]);
  printf("Reserved: 0x%02x\n", (unsigned char) file[count++]);
  
  // Skipping usb interface handler pointer
  count += 4;

  printf("Number of pipes: %d\n", fileutil_getuinteger(file, &count));
  printf("\n");

  // POssibly a second, depending on if the box was OSR2 compatible
  usb_pipe_info(file, &count);
  *filep = count;
}

void usb_pipe_info(char *file, long long *filep){
  long long count = *filep;

  printf("Maximum packet size: %d\n", fileutil_getushort(file, &count));
  printf("End point address: 0x%02x\n", (unsigned char) file[count++]);
  printf("Interval: 0x%02x\n", (unsigned char) file[count++]);
  printf("USB pipe type: %d\n", fileutil_getinteger(file, &count));
  // Skipped pipe handle\n");
  count += 4;
  printf("Maximum transfer size: %d\n", fileutil_getuinteger(file, &count));
  printf("Pipe flags: %d\n", fileutil_getuinteger(file, &count));

  *filep = count;
}

void usb_ucd(char *file, long long *filep){
  long long count = *filep;

  printf("Length: 0x%02x\n", (unsigned char) file[count++]);
  printf("Descriptor type: 0x%02x\n", (unsigned char) file[count++]);
  printf("Total length: %d\n", fileutil_getushort(file, &count));
  printf("Number of interfaces: 0x%02x\n", (unsigned char) file[count++]);
  printf("Configuration value: 0x%02x\n", (unsigned char) file[count++]);
  printf("Configuration: 0x%02x\n", (unsigned char) file[count++]);
  printf("Attributes: 0x%02x\n", (unsigned char) file[count++]);
  printf("Max power: 0x%02x\n", (unsigned char) file[count++]);

  *filep = count;
}
