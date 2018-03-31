#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "userprog/process.h"

void syscall_init (void);
  /*------------------------------------------------------------ADDED BY CRIMSON*/ 
void syscall_halt (void);
void syscall_exit (int);
pid_t syscall_exec (const char *);
int syscall_wait (pid_t);
bool syscall_create (const char *, unsigned);
bool syscall_remove (const char *);
int syscall_open (const char *);
int syscall_filesize (int);
int syscall_read (int, void *, unsigned);
int syscall_write (int, void *, unsigned);
void syscall_seek (int, unsigned);
unsigned syscall_tell (int);
void syscall_close (int);

static bool check_ptr(void *, uint8_t);
static int get_user (const uint8_t *);
static bool put_user (uint8_t *udst, uint8_t byte);
  /*------------------------------------------------------------ADDED BY CRIMSON*/ 

#endif /* userprog/syscall.h */
