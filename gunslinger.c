#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Connor Zanin <zanin@ccs.neu.edu>");
MODULE_DESCRIPTION("A module for enabling a distributed OS, complete with:\n \
                        \t(1) Process migration\n \
                        \t(2) Virtual files and virtual file-backed mappings\n \
                        \t(3) Fast working-set estimation" \
                        );

static int __init
gsm_init(void)
{
        return 1; // not implemented
}

static void __exit
gsm_exit(void)
{
        // not implemented
}

module_init(gsm_init);
module_exit(gsm_exit);
