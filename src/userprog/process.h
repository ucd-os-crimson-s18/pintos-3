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

struct child
    {
        pid_t pid;                          /* Process identifier */
        enum process_status status;         /* Process state */
        bool success;                       /* Child created */
        int exit_code;                      /* How child exited */
        struct lock rw_lock;                /* Protect read/write */
        struct semaphore child_load;        /* Synch loading of child */
        struct semaphore child_dead;        /* Synch dying of child (wait) */
        struct list_elem child_elem;        /* Parent uses to add to its child list */
    };





struct process_control_block
    {
      pid_t pid;                          /*pid of child */
      struct semaphore child_load;        /* Synch loading of child */
      struct semaphore child_dead;        /* Synch dying of child (wait) */
      char *args;                         /* Args passed to thread_create*/
      struct list_elem child_elem;        /* Parent uses to add to its child list */
      struct thread *parent;              /*Parent of new child */
      int32_t exit_code;                  /*Exist code passed from exit()*/

     };

/*------------------------------------------------------------ADDED BY CRIMSON*/ 

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (int);
void process_activate (void);
 


#endif /* userprog/process.h */
