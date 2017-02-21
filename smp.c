#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include "smp.h"


/*semaphore create*/
semaphore_t *semaphore_create(char *semaphore_name, int count1)
{   //printf("Inside semaphore create for %s\n", semaphore_name);
    int fd;
    semaphore_t *semap;
    pthread_mutexattr_t psharedm;
    pthread_condattr_t psharedc;

    fd = open(semaphore_name, O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    return (NULL);
    
    (void) ftruncate(fd, sizeof(semaphore_t));
    (void) pthread_mutexattr_init(&psharedm);
    (void) pthread_mutexattr_setpshared(&psharedm, PTHREAD_PROCESS_SHARED);
    (void) pthread_condattr_init(&psharedc);
    (void) pthread_condattr_setpshared(&psharedc, PTHREAD_PROCESS_SHARED);
    
    semap = (semaphore_t *) mmap(NULL, sizeof(semaphore_t),PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);
    close (fd);
    
    (void) pthread_mutex_init(&semap->lock, &psharedm);
    (void) pthread_cond_init(&semap->nonzero, &psharedc);
    semap->count = count1;
    return (semap);
}


/*********************************************************************/
/*shared create for counter*/
counter_t *shared_create_counter(char *semaphore_name, int count1)
{   //printf("Inside counter semaphore create for %s\n", semaphore_name);
    int fd;
    counter_t *counterr;

    fd = open(semaphore_name, O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    return (NULL);
    
    (void) ftruncate(fd, sizeof(counter_t));
    
    counterr = (counter_t *) mmap(NULL, sizeof(counter_t),PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);
    close (fd);
    
    counterr->count = count1;
    return (counterr);
}


/*********************************************************************/
/*semaphore open*/
semaphore_t *semaphore_open(char *semaphore_name)
{   //printf("Inside semaphore open for %s\n",semaphore_name);
    int fd;
    semaphore_t *semap;
    
    fd = open(semaphore_name, O_RDWR, 0666);
    if (fd < 0)
    {
    	//printf("PROBLEM IN OPENING SEMAPHORE: %s\n", semaphore_name);
    	return (NULL);
    }
    semap = (semaphore_t *) mmap(NULL, sizeof(semaphore_t), PROT_READ | PROT_WRITE, MAP_SHARED,
    fd, 0);
        	//printf("SUCCESS OPENING: %s\n", semaphore_name);
    close (fd);
    return (semap);
}


/*********************************************************************/
/*counter open*/
counter_t *counter_open(char *semaphore_name)
{   //printf("Inside semaphore of counter open for %s\n",semaphore_name);
    int fd;
    counter_t *counterr;
    
    fd = open(semaphore_name, O_RDWR, 0666);
    if (fd < 0)
    return (NULL);
    counterr = (counter_t *) mmap(NULL, sizeof(counter_t), PROT_READ | PROT_WRITE, MAP_SHARED,
    fd, 0);
    close (fd);
    return (counterr);
}


/*********************************************************************/
/*semaphore post*/
void semaphore_post(semaphore_t *semap)
{   //printf("Inside semaphore post \n");
    pthread_mutex_lock(&semap->lock);
    if (semap->count == 0)
    pthread_cond_signal(&semap->nonzero);
    semap->count++;
    pthread_mutex_unlock(&semap->lock);
}


/*********************************************************************/
/*semaphore wait*/
void semaphore_wait(semaphore_t *semap)
{   //printf("Inside semaphore wait\n");
    pthread_mutex_lock(&semap->lock);
    //printf("semap->count b4 = %d\n", semap->count);
    while (semap->count == 0)
        pthread_cond_wait(&semap->nonzero, &semap->lock);
    semap->count--;
    //printf("semap->count aftr = %d\n", semap->count);
    pthread_mutex_unlock(&semap->lock);
}


/*********************************************************************/
/*semaphore close*/
void semaphore_close(semaphore_t *semap)
{   //printf("Inside semaphore closed\n");
    munmap((void *) semap, sizeof(semaphore_t));
}


/*********************************************************************/
/*counter close*/
void counter_close(counter_t *counterr)
{   //printf("Inside counter closed\n");
    munmap((void *) counterr, sizeof(counter_t));
}


/*********************************************************************/
