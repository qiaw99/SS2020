#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <math.h>

int *count;

int num_threads;

sem_t mutex;

void *philosopher(void *threadid){
	long tid = (long)threadid;
	for(int i = 0; i < 1000; i++){
		printf("%ld is thinking...\n", tid);
		sleep(2);

		sem_wait(&mutex);
		count[tid] ++;
		printf("%ld is dinning\n", tid);
		sleep(1);
		
		printf("%ld finished.\n", tid);
		sleep(1);
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
    
    // Initializieren die Anzahl von Stäbchen
    if(num_threads == 1){
    	sem_init(&mutex, 0, 1);
    }else if(num_threads < 0){
    	exit(-1);
    }else{
 	   sem_init(&mutex, 0, floor(num_threads / 2));
 	}

 	// Initialisieren count
    count = (int *)malloc(sizeof(int) * num_threads);
    for(int i = 0; i < num_threads; i++){
    	count[i] = 0;
    }
    
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

    for(int i = 0; i < num_threads; i++){
    	printf("philosopher %d has eaten %d times\n", i, count[i]);
    }
    sem_destroy(&mutex);

    pthread_exit(NULL); 
}
