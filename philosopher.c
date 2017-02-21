#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include "smp.h"

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int N;

void philosopher(int i);
void take_fork(int i);
void test(int i);
void put_fork(int i);

/*********************************************************************/
/*take_fork() function*/

void take_fork(int i)
{   printf("Philosopher[%d] is attempting to grab both forks\n",i);
    char buf[200];
    char buf1[200];
    char barrier_name[80];
    char global_name[80];
    char counter_name[80];
    char *status[N];   
    
    int left, right;
    left = (i+N-1)%N;														//defining Left & Right for each philosopher 
    right = (i+1)%N;
    
    strcpy(barrier_name,"barrier");											//naming the semaphores
    strcpy(global_name,"global_lock");
    strcpy(counter_name,"counter");
    
    semaphore_t *semap1[N];
    semaphore_t *m;
    semaphore_t *barrier;
    counter_t *count1;
    counter_t *countarr[N];
    //printf("Semaphores initialized\n");
    
    //printf("Opening semaphores after this line\n");						//opening all semaphores
    m = semaphore_open(global_name);
    //printf("Semaphore %s opened\n",global_name);
    barrier = semaphore_open(barrier_name);
    //printf("Semaphore %s opened\n",barrier_name);
    count1 = counter_open(counter_name);
    //printf("Semaphore %s opened\n",counter_name); 
    
    sprintf(buf,"sem_%d",i);
    semap1[i] = semaphore_open(buf);
    //printf("Semaphore %s opened\n", buf);
    
    sprintf(buf1,"status_%d",i);
    countarr[i] = counter_open(buf1);
    //printf("Semaphore %s opened\n", buf1);
    
    //printf("---Value of m counter in take_fork(): %d\n",m->count);
    
    semaphore_wait(m);
    countarr[i]->count=HUNGRY;												//setting status of philosopher[i] to Hungry
    printf("Philosopher[%d] is in hungry state\n",i);
    test(i);//condition to check if forks are available to eat
    semaphore_post(m);
    semaphore_wait(semap1[i]);
}

/*********************************************************************/
/*test() function*/

void test(int i)
{   //printf("philosopher[%d] is checking if other philosophers are not eating\n",i);
    char buf[200];
    char buf1[200];
    char buf2[200];
    char buf3[200];
    char barrier_name[80];
    char global_name[80];
    char counter_name[80];
    char *status[N]; 
    int left, right;  
    
    strcpy(barrier_name,"barrier");
    strcpy(global_name,"global_lock");
    strcpy(counter_name,"counter");
    
    left = (i+N-1)%N;
    right = (i+1)%N;
     
    semaphore_t *semap1[N];
    semaphore_t *m;
    semaphore_t *barrier;
    counter_t *count1;
    counter_t *countarr[N];
    
    m = semaphore_open(global_name);
    barrier = semaphore_open(barrier_name);
    count1 = counter_open(counter_name); 
    
    sprintf(buf,"sem_%d",i);
    semap1[i] = semaphore_open(buf);
    
    sprintf(buf1,"status_%d",i);
    countarr[i] = counter_open(buf1);
    
    sprintf(buf2,"status_%d",left);
    countarr[left] = counter_open(buf2);
    
    sprintf(buf3,"status_%d",right);
    countarr[right] = counter_open(buf3);
    
    if(countarr[i]->count== HUNGRY && countarr[left]->count!= EATING && countarr[right]->count!= EATING)
    {
    	printf("Philosopher[%d] successfully grabs both forks\n",i);		//if above condition is true, 
        countarr[i]->count= EATING;											//setting status of philosopher[i] to Eating
        printf("Philosopher[%d] is in eating state\n",i);
        printf("\n----Philosopher[%d] is eating now for 2 milliseconds----\n",i);
        printf("\n");
        usleep(2000);															//eating for 2 seconds
        semaphore_post(semap1[i]);
    }
}

/*********************************************************************/
/*put_fork() function*/

void put_fork(int i)
{   printf("philosopher[%d] is releasing the forks\n",i);
    char buf[200];
    char buf1[200];
    char barrier_name[80];
    char global_name[80];
    char counter_name[80];
    char *status[N];   
    int left, right;
    left = (i+N-1)%N;
    right = (i+1)%N;
    
    strcpy(barrier_name,"barrier");
    strcpy(global_name,"global_lock");
    strcpy(counter_name,"counter");
    
    semaphore_t *semap1[N];
    semaphore_t *m;
    semaphore_t *barrier;
    counter_t *count1;
    counter_t *countarr[N];
    
    m = semaphore_open(global_name);
    barrier = semaphore_open(barrier_name);
    count1 = counter_open(counter_name); 
    
    sprintf(buf,"sem_%d",i);
    semap1[i] = semaphore_open(buf);
    
    sprintf(buf1,"status_%d",i);
    countarr[i] = counter_open(buf1);
    
    semaphore_wait(m);
    countarr[i]->count=THINKING;									//After eating, setting state of Philosopher[i] to thinking
    printf("Philosopher[%d] is back to thinking state\n",i);
    test(left);														//checking if left philosopher needs to eat
    test(right);													//checking if right philosopher needs to eat
    semaphore_post(m);
}

/*********************************************************************/
/*Main Function*/

int main(int argc, char* argv[])
{
    int left, right,i,j,M;
    N=atoi(argv[1]);
    M=atoi(argv[2]);
    i=atoi(argv[3]);
    char buf[200];
    char buf1[200];
    char barrier_name[80];
    char global_name[80];
    char counter_name[80];
    char *status[N];   
    
    strcpy(barrier_name,"barrier");
    strcpy(global_name,"global_lock");
    strcpy(counter_name,"counter");
    
    left = (i+N-1)%N;
    right = (i+1)%N;
    
    semaphore_t *semap1[N];
    semaphore_t *m;
    semaphore_t *barrier;
    counter_t *count1;
    counter_t *countarr[N];
        
    m = semaphore_open(global_name);
    barrier = semaphore_open(barrier_name);
    count1 = counter_open(counter_name); 
    
    sprintf(buf,"sem_%d",i);
    semap1[i] = semaphore_open(buf);
    
    sprintf(buf1,"status_%d",i);
    countarr[i] = counter_open(buf1);
    
    /*****************Barrier code starts here*****************/
    semaphore_wait(m);
    count1->count++;
    semaphore_post(m);
    if(count1->count==N)
        //printf("In barrier, Counter value has reached value of N=%d\n",N);
        semaphore_post(barrier);
    semaphore_wait(barrier);
    semaphore_post(barrier);
    /*****************Barrier code ends here*****************/
    
    /*Philosopher starts here*/
    j=0;
    while(j<M)																// M iterations for each philosopher
    {
        status[i]=THINKING;
        printf("Philosopher[%d] is in thinking state\n",i);
        take_fork(i); 														//attempt to pick up forks on both sides
        /*philosopher is eating now*/
        put_fork(i); 														//releases both forks 
        j++;
    }
    
    //printf("Starting to close\n");   
    semaphore_close(semap1[i]);												//closing all semaphores
    semaphore_close(m);
    semaphore_close(barrier);
    counter_close(count1);
    
    return 0;
}
