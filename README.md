# Dining-Philosophers
Implemented the dining philosopher problem using pthreads, semaphores and mutex

In the Dining Philosophers problem there are N philosophers seated on a circular table and there are N forks, which are also
placed in a circular fashion so that each philosopher has one on its right and one on its left. 
A philosopher needs two forks to eat.

In this solution Pthreads library to implement a semaphore based and deadlock free solution to the Dining Philosophers.
The solution enables synchronization of processes, i.e., each philosopher will run as a separate process. 
To achieve this, I used Pthreads library’s PTHREAD_PROCESS_SHARED feature for condition 
and mutex variables and the mmap/munmap system calls.

The solution consists of two programs: 

1.  Host: Initializes the shared data structures based on the number of philosophers (N),
which is provided as a command line argument. After the initialization stage it creates N processes that 
execute the Philosopher program. It should pass the 2nd command line argument (M or the number of iterations) 
to these processes through exec system call.

2  Philosopher: Implements a philosopher that sits in a loop that executes M times. M is provided as a command line argument.
Inside the loop, each philosopher should print its state {THINKING, HUNGRY, and EATING} on the terminal as it goes through
various stages. The solution uses a barrier to make sure that all philosophers pass the barrier before they start their loops.
