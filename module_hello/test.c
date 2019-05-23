#include <linux/init.h>
#include <linux/module.h>

#include "test.h"

MODULE_LICENSE("Dual BSD/GPL");

void test(void)
{
	printk("test1, enter test\n");
}

EXPORT_SYMBOL(test);
