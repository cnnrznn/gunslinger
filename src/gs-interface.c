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

/*
 * TODO
 * The gs_read function should accept only a single value for length,
 * which is the number of physical page frames, divided by 8.
 * I should satisfy this request with a bitmap, one bit for every page
 * in physical memory to indicate whether that page has been touched since
 * last scan. Upon reading, this bit is also reset.
 */
static ssize_t
gs_read(        struct file *file,
                char __user *buffer,
                size_t length,
                loff_t *offset)
{
        if (length == 2*sizeof(int)) {
                copy_to_user(buffer, &ws_size, sizeof(int));
                copy_to_user(buffer + sizeof(int), &ds_size, sizeof(int));
        }
        else if (length == (ws_size + ds_size) * sizeof(unsigned long)) {
                copy_to_user(buffer, ws, ws_size * sizeof(unsigned long));
                copy_to_user(buffer + (ws_size * sizeof(unsigned long)),
                                ds, ds_size * sizeof(unsigned long));
        }
        else {
                return -EINVAL;
        }

        return length;
}

/*
 * TODO
 * This function's only job should be to specify the current PID to scan.
 * All 'heavy' actions performed by this module should be initiated by a call
 * to gs_read.
 */
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
