// A hello world module

#define __KERNEL__
#define MODULE
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/config.h>
#include <linux/proc_fs.h>

static int __init hello_init(void)
{
  printk(KERN_INFO "Hello world from hellomodule\n");
  return 0;
}

static void __exit hello_exit(void)
{
  printk(KERN_INFO "Goodbye from hellomodule\n");
}

module_init(hello_init);
module_exit(hello_exit);

// Now required by insmod
MODULE_AUTHOR("Michael Still");
MODULE_DESCRIPTION("Simple hello world module");
MODULE_LICENSE("GPL");
