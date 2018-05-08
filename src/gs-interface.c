/*
 * User interface for Gunslinger
 */

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "gs-interface.h"
#include "gs-log.h"
#include "gs-mm.h"

static char BUSY = 0;

static int gs_open(struct inode *inode, struct file *file);
static int gs_release(struct inode *inode, struct file *file);
static ssize_t gs_read(struct file *file, char __user *buffer, size_t length, loff_t *offset);
static ssize_t gs_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset);

struct file_operations fops = {
        .open           = gs_open,
        .release        = gs_release,
        .read           = gs_read,
        .write          = gs_write,
};

static int
gs_open(struct inode *inode, struct file *file)
{
        if (BUSY)
                return -EBUSY;
        BUSY++;

        if (!try_module_get(THIS_MODULE))
                return -EBUSY;

        return 0;
}

static int
gs_release(struct inode *inode, struct file *file)
{
        BUSY--;

        module_put(THIS_MODULE);

        return 0;
}

static ssize_t
gs_read(        struct file *file,
                char __user *buffer,
                size_t length,
                loff_t *offset)
{
        if (length == sizeof(int)) {
                copy_to_user(buffer, &addr_list_size, sizeof(int));
        }
        else if (length == addr_list_size * sizeof(unsigned long)) {
                copy_to_user(buffer, addr_list, addr_list_size * sizeof(unsigned long));
        }
        else {
                return -EINVAL;
        }

        return length;
}

static ssize_t
gs_write(       struct file *file,
                const char __user *buffer,
                size_t length,
                loff_t *offset)
{
        int pid;

        if (length != sizeof(int))
                return -EINVAL;

        if (copy_from_user(&pid, buffer, sizeof(int)))
                return -EFAULT;

        gs_log("Scanning pid %d", pid);

        clear_collect_ws(pid);

        return 0;
}
