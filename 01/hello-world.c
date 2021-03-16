/*  
 *  hello-world.c - Hello World kernel module.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_AUTHOR("ndubouil");
MODULE_DESCRIPTION("Hello World! module");
MODULE_LICENSE("GPL");

int __init hello_init(void)
{
	printk(KERN_INFO "Hello world !\n");

	return 0;
}

void __exit hello_cleanup(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
