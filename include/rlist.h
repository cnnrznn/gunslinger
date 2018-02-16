/*
 * Simple linked list implementation
 */

#ifndef _GS_RLIST_H
#define _GS_RLIST_H

struct rang {
        unsigned long start;
        unsigned long end;
        struct rang *next;
};

struct rlist {
        struct rang *head;
        struct rang *tail;
};

extern void rlist_append(struct rlist *rlist, unsigned long addr);

extern void rlist_free(struct rlist *rlist);

#endif /* _GS_RLIST_H */
