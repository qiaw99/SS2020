#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct timespec max_timeout;

int last = 0;
int *buffer;
int num_con, num_pro, num_place;

// default initialize mutex and conditional varibales
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t is_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t is_full = PTHREAD_COND_INITIALIZER;

void put(void *threadid, int i){
    pthread_mutex_lock(&mutex);
    
    while (last == num_place){
        pthread_cond_wait(&is_full, &mutex);
    }
    
    buffer[last] = i;
    printf("Producer %ld puts %d at %d \n", (long)threadid, buffer[last], last);
    last++;
    
    // release the signal
    pthread_cond_broadcast(&is_empty);
    
    pthread_mutex_unlock(&mutex);
}

int take(void *threadid){
    pthread_mutex_lock(&mutex);

    while (last == 0){
        if (pthread_cond_timedwait(&is_empty, &mutex, &max_timeout) != 0){
            printf("Consumer %ld Timedout\n", (long)threadid);
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }

    printf("Consumer %ld takes %d from %d\n", (long)threadid, buffer[(last - 1)], last - 1);
    last--;

    pthread_cond_broadcast(&is_full);
    
    pthread_mutex_unlock(&mutex);
    return 0;
}

void *producer(void *threadid){
    for (int i = 0; i < 1000; i++){
        put(threadid, i);
    }
    pthread_exit(NULL);
}

void *consumer(void *threadid){
    while (1){
        if (take(threadid) != 0){
            break;
        }
    }
    pthread_exit(NULL);
}

int main(void){
    int rc;
    long t;

    clock_gettime(CLOCK_REALTIME, &max_timeout);
    max_timeout.tv_sec += 5;

    printf("How many consumers?\n");
    scanf("%d", &num_con);

    printf("How many producers?\n");
    scanf("%d", &num_pro);

    printf("How many places?\n");
    scanf("%d", &num_place);

    // check inputs
    if (num_con < 0 || num_pro < 0){
        printf("Input can't be negative!\n");
        exit(0);
    }

    // allocate the buffer
    buffer = (int *)malloc(sizeof(int) * num_place);

    // initialize threads 
    pthread_t consumers[num_con];
    pthread_t producers[num_pro];

    // initialize the buffer with 0 
    for (t = 0; t < num_place; t++){
        buffer[t] = 0;
    }

    last = 0;

    // create threads
    for (t = 0; t < num_con; t ++){
        rc = pthread_create(&consumers[t], NULL, consumer, (void *)t);
        if (rc){
            printf("ERROR: return code from pthread_create () is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < num_pro; t ++){
        rc = pthread_create(&producers[t], NULL, producer, (void *)t);
        if (rc){
            printf("ERROR: return code from pthread_create () is %d\n", rc);
            exit(-1);
        }
    }

    // joining consumer threads
    for (t = 0; t < num_con; t ++){
        pthread_join(consumers[t], NULL);
    }

    // joining producer threads
    for (t = 0; t < num_pro; t ++){
        pthread_join(producers[t], NULL);
    }

    // release condition variables
    pthread_cond_destroy(&is_empty);
    pthread_cond_destroy(&is_full);

    // release the mutex
    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
}
