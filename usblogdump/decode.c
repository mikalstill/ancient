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
  int temp;
  char *tempbinary;

  psize = fileutil_getuinteger (file, &filep);
  urb_printf ("Transfer size: %d\n", psize);
  
  // Test whether data is present
  if(psize != 0)
    {
      if(fileutil_getnumber (file, &filep) != 0)
	{
	  urb_printf ("Control Data: \n");
	  if(do_informationaldumps == 1)
	    {
	      long long priorfilep = filep;
	      char *tempbcd;
	      
	      // Skip the size byte (it's the same as the header)
	      filep++;
	      
	      temp = file[filep++];
	      switch(temp)
		{
		case 1:
		  // DEVICE DESCRIPTOR
		  urb_printf("\tDevice descriptor:\n");
		  
		  tempbcd = to_bcd(fileutil_getshort(file, &filep));
		  urb_printf ("\t\tUSB specification version: %s\n", tempbcd);
		  free(tempbcd);
		  
		  urb_printf("\t\tClass code: %d\n", file[filep++]);
		  urb_printf("\t\tSubclass code: %d\n", file[filep++]);
		  urb_printf("\t\tProtocol code: %d\n", file[filep++]);
		  urb_printf("\t\tMaximum packet size for endpoint: %d\n", 
			     file[filep++]);
		  urb_printf("\t\tVendor id: %04x\n",
			     fileutil_getshort(file, &filep));
		  urb_printf("\t\tProduct id: %04x\n",
			     fileutil_getshort(file, &filep));
		  
		  tempbcd = to_bcd(fileutil_getshort(file, &filep));
		  urb_printf ("\t\tDevice release number: %s\n", tempbcd);
		  free(tempbcd);
		  
		  urb_printf("\t\tManufacturer string index: %d\n", file[filep++]);
		  urb_printf("\t\tProduct string index: %d\n", file[filep++]);
		  urb_printf("\t\tSerial number string index: %d\n", file[filep++]);
		  urb_printf("\t\tNumber of configurations: %d\n", file[filep++]);
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
    }
  
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
