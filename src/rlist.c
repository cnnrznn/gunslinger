#include <linux/stddef.h>
#include <linux/slab.h>

#include "rlist.h"

#define STEP    4096

void
rlist_append(struct rlist *rlist, unsigned long addr)
{
        if (!rlist)
                goto out;

        if (!rlist->head) {
                rlist->head = kmalloc(sizeof(struct rang), GFP_KERNEL);
                if (!rlist->head)
                        goto out;

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
                rlist->tail->next = kmalloc(sizeof(struct rang), GFP_KERNEL);
                if (!rlist->tail->next)
                        goto out;

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

        if (!rlist)
                goto out;

        while (rlist->head) {
                tmp = rlist->head;
                rlist->head = rlist->head->next;

                kfree(tmp);
        }

out:
        ;
}
