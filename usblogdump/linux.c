#include "usblogdump.h"
#include "linux.h"

extern char *module;

// Attempt to generate a linux driver
void output_preamble(int vid, int pid)
{
  code_printf(section_preamble,
	      "/*\n"
	      "* %s driver - Generated by usblogdump %s\n",
	      module, VERSION);
  code_printf(section_preamble,
	      "*\n"
	      "*	" COPYRIGHT "\n"
	      "*\n"
	      "*	This program is free software; you can redistribute it and/or\n"
	      "*	modify it under the terms of the GNU General Public License as\n"
	      "*	published by the Free Software Foundation, version 2.\n"
	      "*\n"
	      "*	Please credit usblogdump (http://www.stillhq.com) if you\n"
	      "*	edit this code... This code is based on the work of\n"
	      "*	Greg Kroah-Hartman\n"
	      "*/\n"
	      "\n"
	      "#include <linux/config.h>\n"
	      "#ifdef CONFIG_USB_DEBUG\n"
	      "	#define DEBUG	1\n"
	      "#endif\n"
	      "#include <linux/kernel.h>\n"
	      "#include <linux/errno.h>\n"
	      "#include <linux/init.h>\n"
	      "#include <linux/slab.h>\n"
	      "#include <linux/module.h>\n"
	      "#include <linux/usb.h>\n"
	      "\n"
	      "#define DRIVER_AUTHOR \"Michael Still, mikal@stillhq.com\"\n"
	      "#define DRIVER_DESC \"%s driver\"\n",
	      module);
  code_printf(section_preamble,
	      "\n"
	      "#define VENDOR_ID	0x%04x\n"
	      "#define PRODUCT_ID	0x%04x\n",
	      vid, pid);
  code_printf(section_preamble,
	      "\n"
	      "/* table of devices that work with this driver */\n"
	      "static struct usb_device_id id_table [] = {\n"
	      "	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },\n"
	      "	{ },\n"
	      "};\n"
	      "MODULE_DEVICE_TABLE (usb, id_table);\n"
	      "\n"
	      "struct usb_led {\n"
	      "	struct usb_device *	udev;\n"
	      "	/* TODO: Add device specific state management variables here */\n"
	      "};\n\n"
	      "static struct usb_driver led_driver = {\n"
	      "	.owner =	THIS_MODULE,\n"
	      "	.name =		\"%s\",\n",
	      module);
  code_printf(section_preamble,
	      "	.probe =	%s_probe,\n",
	      module);
  code_printf(section_preamble,
	      "	.disconnect =	%s_disconnect,\n",
	      module);
  code_printf(section_preamble,
	      "	.id_table =	%s_table,\n",
	      module);
  code_printf(section_preamble,
	      "};\n\n");
}

void output_probe_function()
{
  code_printf(section_functions,
	      "static int %s_probe(struct usb_interface *interface, const struct usb_device_id *id)\n",
	      module);
  code_printf(section_functions,
	      "{\n"
	      "        struct usb_device *udev = interface_to_usbdev(interface);\n"
	      "        struct usb_%s *dev = NULL;\n",
	      module);
  code_printf(section_functions,
	      "        int retval = -ENOMEM;\n"
	      "\n"
	      "        dev = kmalloc(sizeof(struct usb_%s), GFP_KERNEL);\n",
	      module);
  code_printf(section_functions,
	      "        if (dev == NULL) {\n"
	      "                dev_err(&interface->dev, \"Out of memory\\n\");\n"
	      "                goto error;\n"
	      "        }\n"
	      "        memset (dev, 0x00, sizeof (*dev));\n"
	      "\n"
	      "        dev->udev = usb_get_dev(udev);\n"
	      "\n"
	      "        usb_set_intfdata (interface, dev);\n"
	      "\n"
	      "        /* TODO: Add device specific dev files here */\n"
	      "\n"
	      "        dev_info(&interface->dev, \"usblogdump generated driver for %s attached\\n\");\n",
	      module);
  code_printf(section_functions,
	      "        return 0;\n"
	      "\n"
	      "error:\n"
	      "        kfree(dev);\n"
	      "        return retval;\n"
	      "}\n\n");

}

void output_disconnect_function()
{
  code_printf(section_functions,
	      "static void %s_disconnect(struct usb_interface *interface)\n",
	      module);
  code_printf(section_functions,
	      "{\n"
	      "	struct usb_%s *dev;\n",
	      module);
  code_printf(section_functions,
	      "\n"
	      "	dev = usb_get_intfdata (interface);\n"
	      "	usb_set_intfdata (interface, NULL);\n"
	      "\n"
	      "	/* TODO: Cleanup device specific dev entries */\n"
	      "\n"
	      "	usb_put_dev(dev->udev);\n"
	      "\n"
	      "	kfree(dev);\n"
	      "\n"
	      "	dev_info(&interface->dev, \"%s now disconnected\\n\");\n"
	      "}\n\n",
	      module);
}
