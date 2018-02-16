#include <linux/stddef.h>
#include <linux/slab.h>

#include "rlist.h"

#define STEP    4096

void
rlist_append(struct rlist *rlist, unsigned long addr)
{
        if (rlist->head == NULL) {
                rlist->head = kmalloc(sizeof(struct range), GFP_KERNEL);
                rlist->tail = rlist->head;
                rlist->tail->next = NULL;

                rlist->head->start = addr;
                rlist->head->end = addr + STEP;

                goto out;
        }

        if (rlist->tail->end == addr) {
                rlist->tail->end += STEP;
                goto out;
        } else {
                rlist->tail->next = kmalloc(sizeof(struct range), GFP_KERNEL);
                rlist->tail = rlist->tail->next;
                rlist->tail->next = NULL;
                rlist->tail->start = addr;
                rlist->tail->end = addr + STEP;
        }

out:
        ;
}

void
rlist_free(struct rlist *rlist)
{
        struct rang *tmp;

        while (rlist->head) {
                tmp = rlist->head;
                rlist->head = rlist->head->next;

                kfree(tmp);
        }
}
