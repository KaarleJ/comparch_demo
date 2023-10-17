#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
   pid_t pid = getpid();
   printf("Process ID: %d\n", pid);

   // Task 1: Get input
   char input[100];
   printf("Enter the test string: ");
   fgets(input, sizeof(input), stdin);

   // Here we create a pipe for communication between P0 and C0
   int pipe_fd[2];
   if (pipe(pipe_fd) == -1) {
      printf("Pipe creation failed");
      exit(1);
   }

   // Here we fork a child process
   pid_t child_pid = fork();

   // Here we execute code conditionally based on the process.
   if (child_pid < 0) {
      // Here forking fails and we exit the program
      printf("Fork failed \n");
      exit(1);
   } else if (child_pid == 0) {
      printf("Task 2 here\n");
   } else {
      // Here we read data from child process and set it to the input variable.
      printf("Task 1 here\n");
   }
   
   return 0;
}