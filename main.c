#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>

void cleanInput(char *input, char *cleaned)
{
   // We initialize variables i and j for iteration
   int i = 0;
   int j = 0;

   // Here we iterate and clean out the string
   while (input[i] != '\0')
   {
      if (isalpha(input[i]))
      {
         cleaned[j] = input[i];
         j++;
      }
      i++;
   }
   // Here we end the string
   cleaned[j] = '\0';
}

int main()
{
   pid_t p0_pid = getpid();
   printf("Process ID: %d\n", p0_pid);

   // Here we declare the input variable
   char input[100];

   // Here we create a pipe for communication between P0 and C0
   int pipe_fd[2];
   if (pipe(pipe_fd) == -1)
   {
      printf("Pipe creation failed");
      exit(1);
   }

   // Here we fork a child process C0
   pid_t child_pid = fork();

   // Here we execute code conditionally based on the process.
   if (child_pid < 0)
   {
      // Here forking fails and we exit the program.
      printf("Fork failed \n");
      exit(1);
   }
   else if (child_pid == 0)
   {
      // Task 2: Here we first read the string from the parent process.
      read(pipe_fd[0], input, sizeof(input));
      printf("C0: Got string %s\n", input);

      // Printing the process id for debugging
      pid_t C0 = getpid();
      printf("C0 ID: %d\n", C0);

      // Here we clean the string
      char cleaned[100];
      cleanInput(input, cleaned);
      printf("C0: Cleaned string: %s\n", cleaned);

      // We generate a shared memory segment. We create a new one with all permissions.
      key_t shm_key = ftok("./keyfolder", 0);
      int shm_id = shmget(shm_key, sizeof(cleaned), IPC_CREAT | 0666);

      // Handle error in segment creation
      if (shm_id < 0)
      {
         perror("shmget failed\n");
         exit(1);
      }

      // We attach the segment to this process's address-space
      char *shm_addr = shmat(shm_id, NULL, 0);
      if (shm_addr == (char *)-1)
      {
         perror("shmat failed\n");
         exit(1);
      }

      // We copy the string into the memory segment
      printf("C0: Writing into shm\n");
      strcpy(shm_addr, cleaned);
      printf("C0: Written\n");
      // Then we detach the segment and close the read end of the pipe
      shmdt(shm_addr);
      close(pipe_fd[0]);
      printf("C0: Shm detached\n");

      exit(0);
   }
   else
   {
      // Task 1: Get input
      printf("P0: Enter the test string: ");
      fgets(input, sizeof(input), stdin);

      // Printing the process id for debugging
      pid_t P0 = getpid();
      printf("P0 ID: %d\n", P0);

      // Here we pass the input string to the child process through the pipe.
      printf("P0: passing string to C0 \n");
      close(pipe_fd[0]);
      write(pipe_fd[1], input, strlen(input) + 1);
      close(pipe_fd[1]);
   }

   // Task 3: Here we fork a new process and use exec to create a different process
   pid_t p1_pid = fork();

   if (p1_pid < 0)
   {
      // Here forking fails and we exit the program.
      printf("Fork failed \n");
      exit(1);
   }
   else if (p1_pid == 0)
   {
      printf("P1: Calling execvp()\n");
      char *const exec_args[] = {"./find_missing", NULL};
      execvp("./find_missing", exec_args);
      perror("execvp failed\n");
      exit(1);
   }
   else
   {
      printf("P0: waiting\n");
      // Here we wait for the child process to finish
      wait(NULL);
      wait(NULL);
      printf("P0: waiting ended\n");
   }

   return 0;
}