#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int num_pro;
int num_con;
int num_threads;
int num_places;

sem_t empty;
sem_t full;
sem_t mutex;

int last;
int *buffer;

void *Producer(void *threadid){
	for(int i = 0; i < 1000; i++){
        sem_wait(&full);
        sem_wait(&mutex);

        // Zeit zur Erzeugung simulieren
        sleep(1);
        buffer[last] = i;
        printf("Producer %ld puts %d into buffer at place %d.\n", (long)threadid, buffer[last], last);
        last ++;

        sem_post(&mutex);
        sem_post(&empty);
    }   
    pthread_exit(NULL);
}

void *Consumer(void *threadid){
    while(1){
        sem_wait(&empty);
        sem_wait(&mutex);

        // Zeit zur Abholung simulieren
        sleep(1);
        printf("Consumer %ld takes %d\n", (long)threadid, buffer[last - 1]);
        fflush(stdout);
        last --;

        sem_post(&mutex);
        sem_post(&full);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int rc;
    long t;
    
    // Anfrage zur Anzahl von Produzenten
    printf("Anzahl von Produzenten?\n");
    scanf("%d", &num_pro);

    // Anfrage zur Anzahl von Komsumenten
    printf("Anzahl von Komsumenten?\n");
    scanf("%d", &num_con);

    // Anfrage zur Anzahl von Plätzen
    printf("Anzahl von Plätzen?\n");
    scanf("%d", &num_places);

    if(num_pro < 0 || num_con < 0 || num_places < 0){
        printf("ERROR! INPUT NEGATIVE\n");
        exit(-1);
    }

    num_threads = num_con + num_pro;

    pthread_t threads[num_threads];

    buffer = (int *)malloc(sizeof(int) * num_places);

    // Initialisieren buffer und last 
    for(int i = 0; i < num_places; i ++){
        buffer[i] = 0;
    }
    last = 0;

    // Initialisieren Semaphoren
    sem_init(&empty, 0, 0);
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, num_places);
    
    //creating threads
    for(t = 0; t < num_threads; t++){
        printf("In main: creating thread %ld\n", t);
        
        if(t < num_pro){
            rc = pthread_create(&threads[t], NULL, Producer, (void *)t);
            if(rc){
                printf("Error; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }else{
            rc = pthread_create(&threads[t], NULL, Consumer, (void *)t);
            if(rc){
                printf("Error; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
    }

    // joining threads
    for(t = 0; t < num_threads; t++){
        pthread_join(threads[t], NULL);
    }

    // release semaphores
    sem_destroy(&full);
    sem_destroy(&mutex);
    sem_destroy(&empty);

    pthread_exit(NULL); 
    return 0;
}
