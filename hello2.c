// SPDX-License-Identifier: GPL-2.0-only
/*
 * Lab 5: hello2 module
 * Group: IO-33
 * Student: Ishchyk Serhii
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ishchyk Serhii, IO-33");
MODULE_DESCRIPTION("hello2: calls hello1 print_hello() N times via parameter");
MODULE_VERSION("1.0");

static unsigned int count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of greetings to print");

static int __init hello2_init(void)
{
	unsigned int i;

	if (count > 10) {
		pr_err("hello2: invalid count=%u (>10)\n", count);
		return -EINVAL;
	}

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("hello2: count=%u (warning case), continue\n", count);

	pr_debug("hello2: loaded, count=%u\n", count);

	for (i = 0; i < count; i++)
		print_hello();

	return 0;
}
static void __exit hello2_exit(void)
{
	pr_debug("hello2: unloaded, count=%u\n", count);
}

module_init(hello2_init);
module_exit(hello2_exit);
