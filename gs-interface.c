/*
 * User interface for Gunslinger
 */

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>

#include "gs-interface.h"
#include "gs-log.h"

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
        return 0; // not implemented
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

        gs_log("User wants to monitor pid %d\n", pid);

        // TODO do the mm walk
        //

        return 0;
}
