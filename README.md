# DTEK2085-3001 Computer Architecture and Operating Systems

This repository is a project for the above mentioned course.
The code is supposed find all the missing alphabets (among a-z) in a given test string.
The point is to divide the task in to subtasks and complete them in a pipeline.

- Task 1: Get the input test string from user

- Task 2: Clean the test string by removing all number and special characters from the test string.

- Task 3: Find the missing alphabets from the cleaned string

## Implementation
Task 1 is the parent process P0. P0 Gets the input test string from the user.
P0 then passes the string to C0 through a pipe.

Task 2 is implemented as a child process C0 of P0 by invoking fork().
C0 gets the string from P0 through a pipe, cleans it and then writes the string to a shared memory segment.

Task 3 is implemented as a different process P1 by invoking execvp().
P1 reads the string from the shared memory segment and then prints out all the missing alphabets.

## Running the program
To run the program, first compile the source code files.
gcc main.c -o main
gcc find_missing.c -o find_missing

Then you can run the program.
./main
