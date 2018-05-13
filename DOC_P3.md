        +---------------------------+
		    | CSCI 3453                    |
		    | PROJECT 3: VIRTUAL MEMORY	|
		    |	DESIGN DOCUMENT           |
		    +---------------------------+

---- GROUP ----

>> Fill in the names and email addresses of your group members.

Jeff McMillan <jeff.mcmillan@ucdenver.edu>
Eric Holguin <eric.holguin@ucdenver.edu>

---- PRELIMINARIES ----


We were happy with just the 79 tests that pass from the start so we kept it that way given that we didn't find it worth it to spend time breaking those tests to fix them in a different way. We ended up with 83 tests passing. 
 
			PAGE TABLE MANAGEMENT
			=====================

---- DATA STRUCTURES ----

>> A1: Copy here the declaration of each new or changed `struct' or
>> `struct' member, global or static variable, `typedef', or
>> enumeration.  Identify the purpose of each in 25 words or less.

struct frame {
  void *base; /* kernel virtual base address */
  struct page *page; /* mapped process page */
  lock lock 
};

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


We added the page and frame structs as they were defined in the Operating Systems Running Syllabus. 