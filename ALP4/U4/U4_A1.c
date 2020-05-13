#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <math.h>

//#define THINKING 0
//#define HUNGRY 1
//#define EATING 2

//int *chop;
int num_threads;

pthread_mutex_t lock;

sem_t mutex;

void *philosopher(void *threadid){
	long tid = (long)threadid;

	for(int i = 0; i < 1000; i++){
		printf("%ld is thinking...\n", tid);
		sleep(1);

		sem_wait(&mutex);
		printf("%ld is dinning\n", tid);
		sleep(1);
		printf("%ld finished.\n", tid);
		sem_post(&mutex);
	}
}

int main(int argc, char *argv[]){
    int rc;
    long t;
    
    // Anfrage zur Anzahl von Threads
    printf("Anzahl von Threads?\n");
    scanf ("%d",&num_threads);

    pthread_t threads[num_threads];
    
    sem_init(&mutex, 0, floor(num_threads / 2));

    // Lock initialisieren
    pthread_mutex_init(&lock, NULL);

    //chop = (int *)malloc(sizeof(int) * num_threads);
    
    //creating threads
    for(t = 0; t < num_threads; t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, philosopher, (void*)t);
        if(rc){
            printf("Error; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // joining threads
    for(t = 0; t < num_threads; t++){
        pthread_join(threads[t], NULL);
    }

    free(chop);

    sem_destroy(&mutex);

    pthread_exit(NULL); 
}
