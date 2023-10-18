#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void findMissing(char *cleaned)
{
  printf("P1: Cleaned string: %s\n", cleaned);
  printf("P1: Missing alphabets: ");

  // Implement here...

  printf("\n");
}

int main()
{
  pid_t p1_pid = getpid();
  printf("P1 ID: %d\n", p1_pid);

  // Generate key for memory segment the same way as in C0 in main.c ftok()
  // Get the id for the segment shmget()
  // Attach the segment to address space shmat()
  // Read the string from shm and set it to a variable strcpy()
  // Detach the shm

  // Here we print out the missing alphabetics
  // Call findMissing for cleaned string
}