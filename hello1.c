// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artem Hapon");
MODULE_DESCRIPTION("Hello1: printing and timing module");

struct hello_event {
    struct list_head list;
    ktime_t start;
    ktime_t end;
};

static LIST_HEAD(hello_events);

int print_hello(void)
{
    struct hello_event *event;

    event = kmalloc(sizeof(*event), GFP_KERNEL);
    if (!event) {
        pr_err("hello1: memory allocation failed\n");
        return -ENOMEM;
    }

    event->start = ktime_get();
    pr_info("Hello, world!\n");
    event->end = ktime_get();

    list_add_tail(&event->list, &hello_events);

    return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
    pr_info("hello1: module loaded\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct hello_event *event, *tmp;
    unsigned int i = 1;

    list_for_each_entry_safe(event, tmp, &hello_events, list) {
        pr_info("hello1: print %u took %lld ns\n",
                i++, (long long)ktime_to_ns(
                    ktime_sub(event->end, event->start)));

        list_del(&event->list);
        kfree(event);
    }

    pr_info("hello1: module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
