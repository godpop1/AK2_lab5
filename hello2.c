// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artem Hapon");
MODULE_DESCRIPTION("Hello2: calls print_hello from hello1");

static unsigned int repeat_count = 1;
module_param(repeat_count, uint, 0444);
MODULE_PARM_DESC(repeat_count, "Number of Hello, world! messages");

static int __init hello2_init(void)
{
    unsigned int i;
    int ret;

    if (repeat_count > 10) {
        pr_err("hello2: invalid repeat_count = %u\n",
               repeat_count);
        return -EINVAL;
    }

    if (repeat_count == 0 ||
        (repeat_count >= 5 && repeat_count <= 10))
        pr_warn("hello2: warning, repeat_count = %u\n",
                repeat_count);

    for (i = 0; i < repeat_count; i++) {
        ret = print_hello();
        if (ret)
            return ret;
    }

    pr_info("hello2: module loaded, repeat_count = %u\n",
            repeat_count);

    return 0;
}

static void __exit hello2_exit(void)
{
    pr_info("hello2: module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
