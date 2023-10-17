#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
  pid_t p1_pid = getpid();
  printf("P1 ID: %d\n", p1_pid);

  // We generate a shared memory segment
  printf("P1: Creating key for Shm\n");
  key_t shm_key = ftok("./keyfolder", 0);
  int shm_id = shmget(shm_key, 0, 0);

  // Handle failure of segment creation
  if (shm_id < 0)
  {
    perror("Segment failed\n");
    exit(1);
  }

  // We attach the segment to process address space
  char *shm_addr = shmat(shm_id, NULL, 0);

  // We check if the shmat fails
  if (shm_addr == (char *)-1)
  {
    perror("shmat failed\n");
    exit(1);
  }
  printf("P1: Shm attached\n");
  
  // Now we read the string from memory
  char cleaned[100];
  strcpy(cleaned, shm_addr);

  // Now we detach the shm
  shmdt(shm_addr);

  printf("P1: string read from memory: %s\n", cleaned);
}