#include "usblogdump.h"
#include "fileutil.h"
#include "linux.h"
#include "decode.h"

extern int urbCount, do_informationaldumps, do_decoder, do_linux;
extern usb_allurbs *urbhead;

void do_control_transfer(char *file, long long *inputfilep)
{
  long long filep = *inputfilep;
  unsigned int psize;
  char *specver, *devrelno, *tempbinary;
  int class, subclass, protocol, packetsize, vend, prod,
    manusi, prodsi, serialsi, noofconfigs, temp;	      

  psize = fileutil_getuinteger (file, &filep);
  urb_printf ("Transfer size: %d\n", psize);
  
  // Test whether data is present
  if(psize != 0)
    {
      if(fileutil_getnumber (file, &filep) != 0)
	{
	  urb_printf ("Control Data: \n");
	  long long priorfilep = filep;
	  
	  // Skip the size byte (it's the same as the header)
	  filep++;
	  
	  temp = file[filep++];
	  switch(temp)
	    {
	    case 1:
	      // DEVICE DESCRIPTOR
	      specver = to_bcd(fileutil_getshort(file, &filep));
	      class = file[filep++];
	      subclass = file[filep++];
	      protocol = file[filep++];
	      packetsize = file[filep++];
	      vend = fileutil_getshort(file, &filep);
	      prod = fileutil_getshort(file, &filep);
	      
	      devrelno = to_bcd(fileutil_getshort(file, &filep));
	      
	      manusi = file[filep++];
	      prodsi = file[filep++];
	      serialsi = file[filep++];
	      noofconfigs = file[filep++];

	      if(do_informationaldumps == 1)
		{
		  urb_printf("\tDevice descriptor:\n");
		  urb_printf ("\t\tUSB specification version: %s\n", specver);
		  urb_printf("\t\tClass code: %d\n", class);
		  urb_printf("\t\tSubclass code: %d\n", subclass);
		  urb_printf("\t\tProtocol code: %d\n", protocol);
		  urb_printf("\t\tMaximum packet size for endpoint: %d\n", 
			     packetsize);
		  urb_printf("\t\tVendor id: %04x\n", vend);
		  urb_printf("\t\tProduct id: %04x\n", prod);
		  urb_printf ("\t\tDevice release number: %s\n", devrelno);
		  urb_printf("\t\tManufacturer string index: %d\n", manusi);
		  urb_printf("\t\tProduct string index: %d\n", prodsi);
		  urb_printf("\t\tSerial number string index: %d\n", serialsi);
		  urb_printf("\t\tNumber of configurations: %d\n", noofconfigs);
		}

	      free(specver);
	      free(devrelno);
	      break;
	      
	    case 2:
	      // CONFIGURATION DESCRIPTOR
	      urb_printf("\tConfiguration descriptor:\n");
	      
	      urb_printf("\t\tTotal length: %d\n",
			 fileutil_getshort(file, &filep));
	      urb_printf("\t\tNumber of interfaces: %d\n",
			 file[filep++]);
	      urb_printf("\t\tConfiguration value: %d\n",
			 file[filep++]);
	      urb_printf("\t\tConfiguration description string index: %d\n",
			 file[filep++]);
	      
	      tempbinary = to_binary(file[filep]);
	      urb_printf("\t\tPower and wakeup settings: %s\n",
			 tempbinary);
	      free(tempbinary);
	      if(file[filep] & 0x20) urb_printf("\t\t\tDevice is self powered\n");
	      if(file[filep] & 0x10) urb_printf("\t\t\tRemote wakeup supported\n");
	      filep++;
	      
	      urb_printf("\t\tBus power required: %d (millamperes / 2)\n",
			 file[filep++]);
	      
	      // We need to jump to the end, because there might be more here...
	      if(psize != 9)
		{
		  int dec = do_decoder;
		  
		  do_decoder = 0;
		  urb_printf("\n\t\tExtra data:\n");
		  dump_data(file, &filep, psize - 9, "DUMP");
		  urb_printf("\n");
		  do_decoder = dec;
		}
	      break;
	      
	    default:
	      urb_printf("\tUnknown control transfer (0x%02x).\n\n", 
			 (unsigned char) temp);
	      filep = priorfilep;
	      dump_data(file, &filep, psize, "CTRL");
	      break;
	    }
	}
      else
	{
	  dump_data(file, &filep, psize, "CTRL");
	}
    }
  urb_printf ("\n");
  
  // And now read the control transfer header
  // usb_urb_header(file, &filep);
  filep += USB_URB_HEADER_LENGTH;
  usb_urb_controltransfer (file, &filep);
  
  if(do_linux == 1)
    {
      urbhead[urbCount].abend = 0;
      output_probe_function();
    }
  
  *inputfilep = filep;
}
