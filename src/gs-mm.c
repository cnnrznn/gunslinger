#include <linux/mm.h>
#include <linux/page_idle.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "gs-log.h"

#define WS_MAX 1024 * 4

unsigned long *addr_list = NULL;
unsigned long addr_list_size = 0;

struct gs_walk_params {
        char clear;
        unsigned long count;
};

static int
gs_walk_pte_entry(pte_t *ptep, unsigned long addr,
                unsigned long next, struct mm_walk *walk)
{
        pte_t pte = *ptep;
        struct page *page = pte_page(pte);
        struct gs_walk_params *gwp = walk->private;

        // check if PTE is more up-to-date
        if (gwp->clear && pte_young(pte)) {
                set_page_young(page);

                set_pte(ptep, pte_mkold(pte));
        }

        if (gwp->clear && (pte_young(pte) || !page_is_idle(page))) {
                if (addr_list_size < WS_MAX) {
                        addr_list[addr_list_size] = addr;
                        addr_list_size++;
                }
        }

        /* The pte bit may have been cleared elsewhere,
         * in which case we should use the page frame
         * descriptor maintained by the kernel.
         */
        gwp->count += (pte_young(pte) || !page_is_idle(page)) ? 1 : 0;

        // In any case, set idleness
        if (gwp->clear)
                set_page_idle(page);

        return 0;
}

static int
do_clear_collect_ws(struct mm_struct *mm, char clear)
{
        struct vm_area_struct *vma;
        struct gs_walk_params gwp;

        struct mm_walk walk = {
                .pte_entry = gs_walk_pte_entry,
                .private = &gwp,
        };

        walk.mm = mm;
        gwp.clear = clear;
        gwp.count = 0;

        // for each vm_area_struct
        for (vma=mm->mmap; vma; vma=vma->vm_next) {
                // 5. walk pte's in the range
                walk_page_vma(vma, &walk);

                gs_log("%lx - %lu", vma->vm_start, gwp.count);
        }

        return gwp.count;
}

int
clear_collect_ws(int nr)
{
        struct task_struct *task;
        struct mm_struct *mm;
        int count;

        task = pid_task(find_vpid(nr), PIDTYPE_PID);
        if (!task)
                return -1;

        addr_list_size = 0;
        if (!addr_list) {
                addr_list = kmalloc(WS_MAX * sizeof(unsigned long), GFP_KERNEL);
                if (!addr_list) {
                        gs_log("unable to allocate memory");
                        return 0;
                }
        }

        mm = task->mm;

        // 3. lock mm_struct for consistent view of page table
        down_write(&mm->mmap_sem);
        spin_lock(&mm->page_table_lock);

        count = do_clear_collect_ws(mm, 1);
        gs_log("number of working set pages: %d", count);

out:
        // 6. clean up (lock)
        spin_unlock(&mm->page_table_lock);
        up_write(&mm->mmap_sem);

        return count;
}
