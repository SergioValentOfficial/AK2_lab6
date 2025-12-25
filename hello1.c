// SPDX-License-Identifier: GPL-2.0-only
/*
 * Lab 5: hello1 module
 * Group: IO-33
 * Student: Ishchyk Serhii
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ishchyk Serhii, IO-33");
MODULE_DESCRIPTION("hello1: exports print_hello() and stores timestamps in list");
MODULE_VERSION("1.0");

struct hello_event {
	struct list_head list;
	ktime_t before;
	ktime_t after;
};

static LIST_HEAD(hello_list);
static unsigned int hello_idx;
void print_hello(void)
{
	struct hello_event *ev;

	ev = kmalloc(sizeof(*ev), GFP_KERNEL);
	if (!ev) {
		pr_warn("hello1: kmalloc failed, skip\n");
		return;
	}

	ev->before = ktime_get();

	hello_idx++;
	pr_debug("Hello, IO-33 Ishchyk! (%u)\n", hello_idx);

	ev->after = ktime_get();

	list_add_tail(&ev->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	hello_idx = 0;
	pr_debug("hello1: loaded\n");
	return 0;
}
static void __exit hello1_exit(void)
{
	struct hello_event *ev, *tmp;
	u64 delta_ns;
	unsigned int n = 0;

	list_for_each_entry_safe(ev, tmp, &hello_list, list) {
		n++;
		delta_ns = ktime_to_ns(ktime_sub(ev->after, ev->before));
		pr_debug("hello1: event %u print_time=%llu ns\n", n, delta_ns);
		list_del(&ev->list);
		kfree(ev);
	}

	pr_debug("hello1: unloaded, total_events=%u\n", n);
}
module_init(hello1_init);
module_exit(hello1_exit);
