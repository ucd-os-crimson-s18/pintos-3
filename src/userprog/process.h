#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "threads/synch.h"

/*------------------------------------------------------------ADDED BY CRIMSON*/ 
enum process_status
    {
        DEAD,
        ONE_ALIVE,
        ALIVE
    };

/* Process identifier type. */
typedef int pid_t;
typedef int tid_t;

struct child_process
    {
      pid_t pid;                          /* pid of child */
      enum process_status status;         /* Process state */
      int exit_status;                    /* Exit code passed from exit()*/
      char *args;                         /* Args passed to thread_create*/
      struct semaphore child_dead;        /* Synch dying of child (wait) */
      struct list_elem child_elem;        /* Parent uses to add to its child list */
      struct thread *parent;              /* Parent of new child */
     };



/*------------------------------------------------------------ADDED BY CRIMSON*/ 

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (int);
void process_activate (void);
 


#endif /* userprog/process.h */
