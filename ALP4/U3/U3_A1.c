#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

char on_bridge;
int num_threads;
int accident = 0;

pthread_mutex_t lock;

void *cross_bridge(void *threadid){
    long tid = (long)threadid;
    
    for(int i = 0; i < 10000; i++){

        pthread_mutex_lock(&lock);

        if(on_bridge == 1){
            printf("Unfall!\n");
            accident++;
        }else{
            printf("Kein Problem!\n");
        }
        on_bridge = 1;
        
        // Zeit auf der Brücke simulieren   
        sleep(1/1000000000000);

        on_bridge = 0;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int rc;
    long t;
    
    // Anfrage zur Anzahl von Threads
    printf("Anzahl von Threads?\n");
    scanf ("%d",&num_threads);

    pthread_t threads[num_threads];
    
    // Lock initialisieren
    pthread_mutex_init(&lock, NULL);
    
    // 0 bedeutet, dass kein Auto auf der Brücke ist.
    on_bridge = 0;

    //creating threads
    for(t = 0; t < num_threads; t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, cross_bridge, (void*)t);
        if(rc){
            printf("Error; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // joining threads
    for(t = 0; t < num_threads; t++){
        pthread_join(threads[t], NULL);
    }
    printf("accident %d\n", accident);

    pthread_exit(NULL); 
}
