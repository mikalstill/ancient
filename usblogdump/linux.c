#include "linux.h"
#include "usblogdump.h"

// Attempt to generate a linux driver
void output_probe_function()
{
  code_printf(section_functions,
	      "static int led_probe(struct usb_interface *interface, const struct usb_device_id *id)\n"
	      "{\n"
	      "        struct usb_device *udev = interface_to_usbdev(interface);\n"
	      "        struct usb_led *dev = NULL;\n"
	      "        int retval = -ENOMEM;\n"
	      "\n"
	      "        dev = kmalloc(sizeof(struct usb_led), GFP_KERNEL);\n"
	      "        if (dev == NULL) {\n"
	      "                dev_err(&interface->dev, \"Out of memory\n\");\n"
	      "                goto error;\n"
	      "        }\n"
	      "        memset (dev, 0x00, sizeof (*dev));\n"
	      "\n"
	      "        dev->udev = usb_get_dev(udev);\n"
	      "\n"
	      "        usb_set_intfdata (interface, dev);\n"
	      "\n"
	      "        device_create_file(&interface->dev, &dev_attr_blue);\n"
	      "        device_create_file(&interface->dev, &dev_attr_red);\n"
	      "        device_create_file(&interface->dev, &dev_attr_green);\n"
	      "\n"
	      "        dev_info(&interface->dev, \"USB LED device now attached\n\");\n"
	      "        return 0;\n"
	      "\n"
	      "error:\n"
	      "        kfree(dev);\n"
	      "        return retval;\n"
	      "}\n");

}
