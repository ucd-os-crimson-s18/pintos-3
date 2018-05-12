#ifndef VM_PAGE_H
#define VM_PAGE_H

#include "vm/frame.h"

#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "userprog/syscall.h"
#include "userprog/process.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/synch.h"

#define MAX_STACK_SIZE (1 << 23)
#define STACK_INDICATOR 0xbfff7f80

// page types
#define PAGE_CODE 1
#define PAGE_IN_SWAP 2
#define PAGE_MMAP 3
#define PAGE_STACK 4
#define PAGE_FILE 5

struct page 
  {
    /* Immutable members. */
    void *addr;                 /* User virtual address. */
    bool read_only;             /* Read-only page */
    struct thread *thread;      /* Owning thread. */

    /* Accessed only in owning process context. */
    struct hash_elem hash_elem; /* struct thread `pages' hash element. */

    /* Set only in owning process context with frame->lock held.
       Cleared only with scan_lock and frame->lock held. */
    struct frame *frame;        /* Page frame. */

    /* Swap information, protected by frame->lock. */
    block_sector_t sector;       /* Starting sector of swap area, or -1. */
    
    /* Memory-mapped file information, protected by frame->lock. */
    bool private;               /* False to write back to file,
                                   true to write back to swap. */
    struct file *file;          /* File. */
    off_t file_offset;          /* Offset in file. */
    off_t file_bytes;           /* Bytes to read/write, 1...PGSIZE. */
  };


/*
void s_page_table_clear(struct thread*t);


struct s_page_table_entry * get_s_pt_entry(void * );
bool load_page_in_s_page_table(struct s_page_table_entry * );
bool load_page_from_swap (struct s_page_table_entry * );
struct s_page_table_entry * frame_table_entry_to_s_pt_entry(struct frame_table_entry * ); 

void s_page_table_entry_clear_by_file(struct file * );
bool grow_stack_one_page(void *);

void validate_sys_read_buffer(void *, unsigned );
*/

#endif /* vm/page.h */
