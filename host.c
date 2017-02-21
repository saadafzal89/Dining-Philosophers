#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "smp.h"

int i, N, M, left, right;


int main(int argc, char*argv[])
{
    int N,i,status;
    char name[20];
    char name_count[20];
    char barrier_name[80];
    char global_name[80];
    char counter_name[80];
    pid_t pid;
    N = atoi(argv[1]);														//Number of philosophers = N
    M = atoi(argv[2]);														//Number of times each philosopher eats = M

    strcpy(barrier_name,"barrier");
    strcpy(global_name,"global_lock");
    strcpy(counter_name,"counter");
    
    char value_of_i[5];
    
    semaphore_t *semap1[N];
    semaphore_t *m;
    semaphore_t *barrier;
    counter_t *count1;
    counter_t *countarr[N];
    
    m = semaphore_create(global_name,1);									//creating system wide single semaphore
    semaphore_close(m);
    
    barrier=semaphore_create(barrier_name,0);								//creating barrier semaphore
    semaphore_close(barrier);
    
    count1=shared_create_counter(counter_name,0);							//creating semaphore for shared counter
    counter_close(count1);
    
   
    
    for(i=0;i<N;i++)
    {
        sprintf(name,"sem_%d",i);
        semap1[i] = semaphore_create(name,0);								//creating indivdual semaphore for each philosopher
        if (semap1[i] == NULL)
        {
            printf("Semaphore_%d exited\n",i);
            exit(1);
        }
        semaphore_close(semap1[i]);
        sprintf(name_count,"status_%d",i);									//creating status for each philosopher
        countarr[i] = shared_create_counter(name_count,0);
        if (countarr[i] == NULL)
        {
            printf("Semaphore status_%d exited\n",i);
            exit(1);
        }
        counter_close(countarr[i]);
        
        pid=fork();															//creating N child processes
        if(pid==0)
        {
            sprintf(value_of_i, "%d",i);
            execlp("./philosopher", "philosopher",argv[1],argv[2],value_of_i, NULL);	//calling philospher program for each child
            printf("EXEC error");
            exit(1);
        }
        else
        {
        	//printf("\n%d philosophers have eaten %d number of times.\n",N,M);
        }
    }
    
    for(i=0;i<N;i++)
    {
        wait(&status);
    }
    
    
    return (0);
}

