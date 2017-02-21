#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>


struct semaphore {
    pthread_mutex_t lock;
    pthread_mutex_t m;
    pthread_cond_t nonzero;
    unsigned count;
};

struct counter {
    int count;
};

typedef struct semaphore semaphore_t;
typedef struct counter counter_t;


semaphore_t *semaphore_create(char *semaphore_name, int count);
semaphore_t *semaphore_open(char *semaphore_name);
void semaphore_post(semaphore_t *semap);
void semaphore_wait(semaphore_t *semap);
void semaphore_close(semaphore_t *semap);

counter_t *shared_create_counter(char *semaphore_name, int count1);
counter_t *counter_open(char *semaphore_name);
void counter_close(counter_t *counterr);
