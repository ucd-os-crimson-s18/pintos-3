#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <list.h>
#include "threads/palloc.h"
#include "threads/synch.h"

struct list frame_list;

struct lock frame_lock;


struct frame {
  void *base; /* kernel virtual base address */
  struct page *page; /* mapped process page */
  lock lock 
};


#endif /* vm/frame.h */
