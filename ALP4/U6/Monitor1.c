#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct timespec max_timeout;

int num_con, num_pro, num_place;

struct monitors{
	pthread_mutex_t mutex;
	pthread_cond_t is_empty;
	pthread_cond_t is_full;
	int last;
	int *buffer;
};

struct monitors *monitor;

void put(void *threadid, int i){
    pthread_mutex_lock(& monitor -> mutex);
    
    while (monitor -> last == num_place){
        pthread_cond_wait(&monitor -> is_full, &monitor -> mutex);
    }
    
    monitor -> buffer[monitor -> last] = i;
    printf("Producer %ld puts %d at %d \n", (long)threadid, monitor -> buffer[monitor -> last], monitor -> last);
    monitor -> last++;
    
    // release the signal
    pthread_cond_broadcast(&monitor -> is_empty);
    
    pthread_mutex_unlock(&monitor -> mutex);
}

int take(void *threadid){
    pthread_mutex_lock(&monitor -> mutex);

    while (monitor -> last == 0){
        if (pthread_cond_timedwait(&monitor -> is_empty, &monitor -> mutex, &max_timeout) != 0){
            printf("Consumer %ld Timedout\n", (long)threadid);
            pthread_mutex_unlock(&monitor -> mutex);
            return -1;
        }
    }

    printf("Consumer %ld takes %d from %d\n", (long)threadid, monitor -> buffer[(monitor -> last - 1)], monitor -> last - 1);
    monitor -> last--;

    pthread_cond_broadcast(&monitor -> is_full);
    
    pthread_mutex_unlock(&monitor -> mutex);
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

    // initialize threads 
    pthread_t consumers[num_con];
    pthread_t producers[num_pro];

    monitor = malloc(sizeof(struct monitors));
    monitor -> buffer = calloc(num_place, sizeof(int));
    monitor -> last = 0;
    pthread_mutex_init(&monitor -> mutex, NULL);
    pthread_cond_init(& monitor -> is_full, NULL);
    pthread_cond_init(& monitor -> is_empty, NULL);

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
    pthread_cond_destroy(& monitor -> is_empty);
    pthread_cond_destroy(&monitor -> is_full);

    // release the mutex
    pthread_mutex_destroy(&monitor -> mutex);
    free(monitor -> buffer);
    free(monitor);

    pthread_exit(NULL);
}
