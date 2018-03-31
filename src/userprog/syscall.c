#include <stdio.h>
#include <stdlib.h>
#include <syscall-nr.h>
#include <user/syscall.h>
#include "filesys/filesys.h"
#include "userprog/process.h"
#include "userprog/syscall.h"
#include "userprog/pagedir.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"



static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  list_init (&open_files);
  lock_init (&filesys_lock);
}

static void
syscall_handler (struct intr_frame *f) 
{
  /*------------------------------------------------------------ADDED BY CRIMSON*/ 
  // Check if f->esp is a valid pointer
  if (check_ptr(f->esp, 4))

  switch(*(int*)f->esp)
  {
    /* Halt the operating system. */
    case SYS_HALT:
    {
      /* Run the syscall function */
      syscall_halt ();

      break;
    }                   
    case SYS_EXIT: /* Terminate this process. */                   
    {
      check_ptr(f->esp + 4, 4);
 
      /* Get the first argument, cast to int */
      int status = *((int*)f->esp + 1);

      /* Run the syscall function */
      syscall_exit (status);

      break;
    } 
    case SYS_EXEC: /* Start another process. */
    {
      check_ptr(f->esp + 4, 4);

      /* Get the first argument, cast to char* */
      const char *cmd_line = *((char*)f->esp + 1);

      /* Run the syscall function, store return into eax */
      f->eax = syscall_exec (cmd_line);

      break;
    }
    case SYS_WAIT: /* Wait for a child process to die. */
    {
      check_ptr(f->esp + 4, 4);

      /* Get the first argument, cast to pid_t */
      pid_t pid = *((pid_t*)f->esp + 1);

      /* Run the syscall function, store return into eax */
      f->eax = syscall_wait (pid);

      break;
    }
    case SYS_CREATE: /* Create a file. */
    {
      check_ptr(f->esp + 4, 8);
      /* Get the first argument, cast to char*  */
      const char *file = *((char**)(f->esp + 4));
      /* Get the second argument, cast to unsigned */
      unsigned initial_size = *((unsigned*)(f->esp + 8));
      /* Run the syscall function, store return into eax */
      f->eax = syscall_create (file, initial_size);

      break;
    }
    case SYS_REMOVE: /* Delete a file. */
    {
      check_ptr(f->esp + 4, 4);
      /* Get the first argument, cast to char*  */
      const char * file = *((char*)f->esp + 1);

      /* Run the syscall function, store return into eax */
      f->eax = syscall_remove (file);

      break;
    }
    case SYS_OPEN: /* Open a file. */
    {
      check_ptr(f->esp + 4, 4);
      /* Get the first argument, cast to char*  */
      const char * file = *((char**)(f->esp + 4));

      /* Run the syscall function, store return into eax */
      f->eax = syscall_open (file);

      break;
    }
    case SYS_FILESIZE: /* Obtain a file's size. */
    {
      check_ptr(f->esp + 4, 4);
      /* Get the first argument, cast to int */
      int fd = *((int*)f->esp + 1);

      /* Run the syscall function, store return into eax */
      f->eax = syscall_filesize (fd);

      break;
    }
    case SYS_READ: /* Read from a file. */
    {
      check_ptr(f->esp + 4, 12);
      /* Get the first argument, cast to int */
      int fd = *((int*)f->esp + 1);
      /* Get the second argument, cast to int* to void* */
      void* buffer = (void*) (*((int*)f->esp + 2));
      /* Get the third argument, cast to unsigned */
      unsigned size = *((unsigned*)f->esp + 3);

      /* Run the syscall function, store return into eax */
      f->eax = syscall_read(fd, buffer, size);

      break;
    }
    case SYS_WRITE: /* Write to a file. */
    {
      check_ptr(f->esp + 4, 12);
      /* Get the first argument, cast to int */
      int fd = *((int*)f->esp + 1);
      /* Get the second argument, cast to int* to void* */
      void* buffer = (void*) (*((int*)f->esp + 2));
      /* Get the third argument, cast to unsigned */
      unsigned size = *((unsigned*)f->esp + 3);

      /* Run the syscall function, store return into eax */
      f->eax = syscall_write(fd, buffer, size);

      break;
    }
    case SYS_SEEK: /* Change position in a file. */
    {
      check_ptr(f->esp + 4, 8);
      /* Get the first argument, cast to int  */
      int fd = *((int*)f->esp + 1);
      /* Get the second argument, cast to unsigned */
      unsigned position = *((unsigned*)f->esp + 2);;

      /* Run the syscall function */
      syscall_seek (fd, position);

      break;
    }
    case SYS_TELL: /* Report current position in a file. */
    {
      check_ptr(f->esp + 4, 4);
      /* Get the first argument, cast to int */
      int fd = *((int*)f->esp + 1);

      /* Run the syscall function, store return into esyscall_ax */
      f->eax = syscall_tell (fd);

      break;
    }
    case SYS_CLOSE: /* Close a file. */
    {
      check_ptr(f->esp + 4, 4);
      /* Get the first argument, cast to int */
      int fd = *((int*)f->esp + 1);

      /* Run the syscall function */
      syscall_close (fd);

      break;
    }
  }
}
/*
Terminates Pintos by calling shutdown_power_off() (declared in
‘devices/shutdown.h’). This should be seldom used, because you lose
some information about possible deadlock situations, etc.
*/
void 
syscall_halt (void)
{
  shutdown_power_off();
}

/*
Terminates the current user program, returning status to the kernel. If the process’s
parent waits for it (see below), this is the status that will be returned. Conventionally,
a status of 0 indicates success and nonzero values indicate errors.
*/
void 
syscall_exit (int status)
{
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit(status);
}

/*
Runs the executable whose name is given in cmd line, passing any given arguments,
and returns the new process’s program id (pid). Must return pid -1, which otherwise
should not be a valid pid, if the program cannot load or run for any reason. Thus,
the parent process cannot return from the exec until it knows whether the child
process successfully loaded its executable. You must use appropriate synchronization
to ensure this.
*/
pid_t 
syscall_exec (const char *cmd_line)
{
  process_execute(cmd_line);
}

/*
Waits for a child process pid and retrieves the child’s exit status.
If pid is still alive, waits until it terminates. Then, returns the status that pid passed
to exit. If pid did not call exit(), but was terminated by the kernel (e.g. killed due
to an exception), wait(pid) must return -1. It is perfectly legal for a parent process
to wait for child processes that have already terminated by the time the parent calls
wait, but the kernel must still allow the parent to retrieve its child’s exit status, or
learn that the child was terminated by the kernel.
wait must fail and return -1 immediately if any of the following conditions is true:
• pid does not refer to a direct child of the calling process. pid is a direct child
of the calling process if and only if the calling process received pid as a return
value from a successful call to exec.
Note that children are not inherited: if A spawns child B and B spawns child
process C, then A cannot wait for C, even if B is dead. A call to wait(C) by
process A must fail. Similarly, orphaned processes are not assigned to a new
parent if their parent process exits before they do.
• The process that calls wait has already called wait on pid. That is, a process
may wait for any given child at most once.
Processes may spawn any number of children, wait for them in any order, and may
even exit without having waited for some or all of their children. Your design should
consider all the ways in which waits can occur. All of a process’s resources, including
its struct thread, must be freed whether its parent ever waits for it or not, and
regardless of whether the child exits before or after its parent.
You must ensure that Pintos does not terminate until the initial process exits.
The supplied Pintos code tries to do this by calling process_wait() (in
‘userprog/process.c’) from main() (in ‘threads/init.c’). We suggest that you
implement process_wait() according to the comment at the top of the function
and then implement the wait system call in terms of process_wait().
Implementing this system call requires considerably more work than any of the rest.
*/
int 
syscall_wait (pid_t pid)
{

}

/*
Creates a new file called file initially initial size bytes in size. Returns true if successful,
false otherwise. Creating a new file does not open it: opening the new file is
a separate operation which would require a open system call.
*/
bool 
syscall_create (const char *file, unsigned initial_size)
{
  /* check to see if valid file pointer */
  check_ptr(file, initial_size);

  if(file == NULL)
  {
    syscall_exit(-1);
  }

  /* create the file */
  bool success = filesys_create(file, initial_size);

  return success;
}

/*
Deletes the file called file. Returns true if successful, false otherwise. A file may be
removed regardless of whether it is open or closed, and removing an open file does
not close it. See [Removing an Open File], page 35, for details.
*/
bool 
syscall_remove (const char *file)
{
  return filesys_remove(file);
}

/*
Opens the file called file. Returns a nonnegative integer handle called a “file descriptor”
(fd), or -1 if the file could not be opened.
File descriptors numbered 0 and 1 are reserved for the console: fd 0 (STDIN_FILENO) is
standard input, fd 1 (STDOUT_FILENO) is standard output. The open system call will
never return either of these file descriptors, which are valid as system call arguments
only as explicitly described below.
Each process has an independent set of file descriptors. File descriptors are not
inherited by child processes.
When a single file is opened more than once, whether by a single process or different
processes, each open returns a new file descriptor. Different file descriptors for a single
file are closed independently in separate calls to close and they do not share a file
position.
*/
int 
syscall_open (const char *file)
{
  /* check to see if valid file pointer */
  check_ptr(file, 4);

  int ret = -1;
  
  lock_acquire(&filesys_lock);
  /* open the file */
  struct file *f = filesys_open(file);
  struct file_desc *fd;
  struct thread *cur = thread_current();

 
  if(f != NULL)
  {
    fd = malloc(sizeof(struct file_desc));
    fd->fd = increment_fd();
    fd->tid = thread_current()->tid;
    fd->f = f;
    list_push_back(&open_files, &(fd->fd_elem));
    //list_push_back(&(cur->open_files), &(fd->thread_elem));

    ret = fd->fd;
  }

  lock_release(&filesys_lock);
  return ret;
}

/*
Returns the size, in bytes, of the file open as fd.
*/
int 
syscall_filesize (int fd)
{

}

/*
Reads size bytes from the file open as fd into buffer. Returns the number of bytes
actually read (0 at end of file), or -1 if the file could not be read (due to a condition
other than end of file). Fd 0 reads from the keyboard using input_getc().
*/
int 
syscall_read (int fd, void *buffer, unsigned size)
{

  // fd = 0 is stdin


  
}

/*
Writes size bytes from buffer to the open file fd. Returns the number of bytes actually
written, which may be less than size if some bytes could not be written.
Writing past end-of-file would normally extend the file, but file growth is not implemented
by the basic file system. The expected behavior is to write as many bytes as
possible up to end-of-file and return the actual number written, or 0 if no bytes could
be written at all.
Fd 1 writes to the console. Your code to write to the console should write all of buffer
in one call to putbuf(), at least as long as size is not bigger than a few hundred
bytes. (It is reasonable to break up larger buffers.) Otherwise, lines of text output
by different processes may end up interleaved on the console, confusing both human
readers and our grading scripts.
*/
int 
syscall_write (int fd, void *buffer, unsigned size)
{
  if(fd == 1)
  {
    putbuf(buffer, size);

    return size;
  }

  return 0;
}

/*
Changes the next byte to be read or written in open file fd to position, expressed in
bytes from the beginning of the file. (Thus, a position of 0 is the file’s start.)
A seek past the current end of a file is not an error. A later read obtains 0 bytes,
indicating end of file. A later write extends the file, filling any unwritten gap with
zeros. (However, in Pintos files have a fixed length until project 4 is complete, so
writes past end of file will return an error.) These semantics are implemented in the
file system and do not require any special effort in system call implementation.
*/
void 
syscall_seek (int fd, unsigned position)
{

}

/*
Returns the position of the next byte to be read or written in open file fd, expressed
in bytes from the beginning of the file.
*/
unsigned 
syscall_tell (int fd)
{

}

/*
Closes file descriptor fd. Exiting or terminating a process implicitly closes all its open
file descriptors, as if by calling this function for each one.
*/
void 
syscall_close (int fd)
{
  /* check to see if valid file pointer */
  check_ptr(fd, 4);

  struct thread *cur = thread_current();
  struct list_elem *e;
  struct file_desc *file_d = NULL;

  /* Find file pertaining to fd */
  for (e = list_begin (&cur->open_files); e != list_end (&cur->open_files); e = list_next (e))
  {
     file_d = list_entry (e, struct file_desc, thread_elem);
     if (file_d->fd == fd)
        break;
   }

  /* close the file */
  file_close(file_d->f);
  list_remove(&(file_d->fd_elem));
  // list_remove(&(file_d->thread_elem));
  free(file_d);
}


static bool
check_ptr(void *esp, uint8_t size)
{
  for(uint8_t i = 0; i <= size; i++)
  { 
    //printf("%p\n", esp + i);
    if(!(is_user_vaddr(esp + i))) 
    {
	    syscall_exit(-1);
    }
    if(pagedir_get_page(thread_current()->pagedir, esp + i) == NULL)
    {
      syscall_exit(-1);
    }
  }


  return true;
}

/* Reads a byte at user virtual address UADDR.
UADDR must be below PHYS_BASE.
Returns the byte value if successful, -1 if a segfault
occurred. */
static int
get_user (const uint8_t *uaddr)
{
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
  : "=&a" (result) : "m" (*uaddr));
  return result;
}
/* Writes BYTE to user address UDST.
UDST must be below PHYS_BASE.
Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
  : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}



/*Increases the fid by 1 each call, beginning at 2*/
static int
increment_fd (void)
{
  static int fid = 1;
  return ++fid;
}



  /*------------------------------------------------------------ADDED BY CRIMSON*/ 
