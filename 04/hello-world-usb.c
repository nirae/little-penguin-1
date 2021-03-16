#include <linux/module.h>
#include <linux/kernel.h>

MODULE_AUTHOR("ndubouil");
MODULE_DESCRIPTION("Hello World USB! module");
MODULE_LICENSE("GPL");

int init_module(void)
{
	printk(KERN_INFO "Hello world USB !\n");

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up hello usb module.\n");
}
