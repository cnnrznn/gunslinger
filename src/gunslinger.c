#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "gs-interface.h"
#include "gs-log.h"
#include "gs-mm.h"

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Connor Zanin <zanin@ccs.neu.edu>");
MODULE_DESCRIPTION("A module for enabling a distributed OS, complete with:\n \
                        \t(1) Process migration\n \
                        \t(2) Virtual files and virtual file-backed mappings\n \
                        \t(3) Fast working-set estimation" \
                        );

static int major;

static int __init
gsm_init(void)
{
        major = register_chrdev(0, DEVICENAME, &fops);
        if (major < 0) {
                gs_log("failed to load module");
                return major;
        }
        gs_log("successfully loaded module");
        gs_log("major number is %d", major);

        ws = kmalloc(MAX * sizeof(unsigned long), GFP_KERNEL);
        if (!ws) {
                gs_log("unable to allocate memory");
                return 1;
        }
        ds = kmalloc(MAX * sizeof(unsigned long), GFP_KERNEL);
        if (!ds) {
                gs_log("unable to allocate memory");
                return 1;
        }

        return 0;
}

static void __exit
gsm_exit(void)
{
        kfree(ws);
        kfree(ds);

        unregister_chrdev(major, DEVICENAME);
        gs_log("bye bye!");
}

module_init(gsm_init);
module_exit(gsm_exit);
