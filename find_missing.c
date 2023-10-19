#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <ctype.h>

void findMissing(char *cleaned)
{
  printf("P1: Missing alphabets: ");

  // We initialize an array to keep track of all the found alphabets
  bool found[26] = {false};

  for (int i = 0; cleaned[i] != '\0'; i++)
  {
    // we convert the char to lowercase
    char c = tolower(cleaned[i]);
    if (c >= 'a' && c <= 'z')
    {
      // if found then we set the char as found
      found[c - 'a'] = true;
    }
  }
  for (int i = 0; i < 26; i++)
  {
    if (!found[i])
    {
      // Here we print the missing alphabets
      printf("%c", 'a' + i);
    }
  }
  printf("\n");
}

int main()
{
  pid_t p1_pid = getpid();
  printf("P1 ID: %d\n", p1_pid);

  // Generate a key for the shared memory segment and the id
  key_t shm_key = ftok("./keyfolder", 0);
  int shm_id = shmget(shm_key, 0, 0);
  // Handle errors in segment creation
  if (shm_id < 0)
  {
    perror("P1: shmget failed\n");
    exit(1);
  }

  // Attach the shared memory segment to this process's address-space
  char *shm_addr = shmat(shm_id, NULL, 0);
  if (shm_addr == (char)-1)
  {
    perror("P1: shmat failed\n");
    exit(1);
  }

  // Attach to the same shared memory flag as in C0
  key_t flag_key = ftok("/keydirectory", 2);
  int flag_shm_id = shmget(flag_key, sizeof(int), 0);
  if (flag_shm_id == -1)
  {
    perror("shmget (flag) failed\n");
    exit(1);
  }
  int *flag_ptr = (int *)shmat(flag_shm_id, NULL, 0);

  // Read the string from shared memory and set it to a variable
  char received_string[100];
  strcpy(received_string, shm_addr);
  printf("P1: Received String: %s\n", received_string);

  // Detach the shared memory segment
  shmdt(shm_addr);

  // Here we print out the missing alphabetics
  findMissing(received_string);

  printf("P1: Terminating P1 and sending signal to C0 to terminate\n");
  *flag_ptr = 1;
  exit(0);
}