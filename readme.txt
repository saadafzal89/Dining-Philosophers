Read Me

The dining philosopher problem has been implemented using semaphores and mmap in this assignment. 

The number of semaphores and their initial counter values are as follows:

Semaphore		Total Number			Initial count

Semap1			no of philosophers		0
Global_lock		1				1
Barrier			1				0
Counter			1				0
Status			no of philosophers		0

There are 3 programs and 1 header file:
1. ’smp.h’
2. ’smp.c’
3. ’host.c’
4. ‘philosopher.c’

smp.h: It contains all the function declarations required across different programs
smp.c: It contains all implemented semaphores functions and shared data structures
host.c It  initializes the stared data structure which was created in smp.c
philosopher.c: Contains the code for dining philosopher function

The makefile is included.

Input to be given: ./host N M
