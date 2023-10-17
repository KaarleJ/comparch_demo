#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>



int main() {
   pid_t pid = getpid();
   printf("Process ID: %d\n", pid);

   // Here we declare the input variable
   char input[100];

   // Here we create a pipe for communication between P0 and C0
   int pipe_fd[2];
   if (pipe(pipe_fd) == -1) {
      printf("Pipe creation failed");
      exit(1);
   }

   // Here we fork a child process C0
   pid_t child_pid = fork();

   // Here we execute code conditionally based on the process.
   if (child_pid < 0) {
      // Here forking fails and we exit the program.
      printf("Fork failed \n");
      exit(1);
   } else if (child_pid == 0) {
      // Here we first read the string from the parent process.
      read(pipe_fd[0], input, sizeof(input));

      // Printing the process id for debugging
      pid_t C0 = getpid();
      printf("C0 ID: %d\n", C0);

      // Here we print out the read string
      printf("C0: reading from P0 %s\n", input);

      // Implement task 2 here...

      // Result should be sent to P0 through shared memory

   } else {
      // Printing the process id for debugging
      pid_t P0 = getpid();
      printf("P0 ID: %d\n", P0);

      // Task 1: Get input
      printf("Enter the test string: ");
      fgets(input, sizeof(input), stdin);

      // Here we pass the input string to the child process through the pipe.
      printf("P0: passing string to C0 \n");
      close(pipe_fd[0]);
      write(pipe_fd[1], input, strlen(input) + 1);
      close(pipe_fd[1]);
   }

   
   
   return 0;
}