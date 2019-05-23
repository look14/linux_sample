#include <linux/init.h>
#include <linux/module.h>
#include "test.h"

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
	printk("test1, hello world\n");
	test();
	return 0;
}

static void hello_exit(void)
{
	printk("test1, Good bye\n");
}

module_init(hello_init);
module_exit(hello_exit);
