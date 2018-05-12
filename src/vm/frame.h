#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <list.h>
#include "threads/palloc.h"
#include "threads/synch.h"

struct list frame_list;

struct lock frame__lock;

struct lock evict_lock;

struct frame {
  void *base; /* kernel virtual base address */
  struct page *page; /* mapped process page */
  lock lock 
};


/*
void frame_table_init (void);
#void * frame_allocate (enum palloc_flags flags);
#bool evict ();
#struct frame_table_entry * frame_find_page(void * );
#void clear_frame_with_owner (struct thread * );
struct frame_table_entry * choose_a_victim ();
struct frame_table_entry * choose_a_victim_random ();
*/
#endif /* vm/frame.h */
