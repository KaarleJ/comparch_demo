#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
   pid_t pid = getpid();
   printf("Process ID: %d\n", pid);

   pid_t child_pid = fork();

   if (child_pid < 0) {
      printf("Fork failed");
      sleep(1);
   } else if (child_pid == 0) {
      printf("task2");
      sleep(1);
   } else {
      printf("parent process");
      sleep(1);
   }
   
   return 0;
}