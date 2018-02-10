#include <linux/mm.h>
#include <linux/pid.h>
#include <linux/sched.h>

#include "gs-log.h"

static unsigned long count;

static int
gs_walk_pte_entry(pte_t *pte, unsigned long addr,
                unsigned long next, struct mm_walk *walk)
{
        // 1. if pte_young
        // 2. count++;
        // 3. pte_mkold(), set_page_young
        return 0;
}

void
clear_notify_young(int nr)
{
        struct task_struct *task;
        struct mm_struct *mm;
        struct vm_area_struct *vma;

        struct mm_walk walk = {
                .pte_entry = gs_walk_pte_entry,
        };

        task = pid_task(find_vpid(nr), PIDTYPE_PID);
        if (!task)
                goto out;

        mm = task->mm;

        count = 0;

        // 3. lock mm_struct for consistent view of page table
        down_write(&mm->mmap_sem);
        spin_lock(&mm->page_table_lock);

        // 4. for each vm_area_struct
        for (vma=mm->mmap; vma; vma=vma->vm_next) {
                // 5. walk pte's in the range
                walk_page_vma(vma, &walk);
        }

        // 6. clean up (lock)
        spin_unlock(&mm->page_table_lock);
        up_write(&mm->mmap_sem);
out:
        ;
}
